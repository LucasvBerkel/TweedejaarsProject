/* Routines voor IZF timer response kaart
   nieuwe versie geheel in C geschreven
   P.Spoelstra okt 1990
*/


/*   TASKOMAT (8 toetsen worden gebruikt (max. 16))

	-- toetsnummering --

     onderste contact (proefleider)
	   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
	   ³  0              3    ³
	   ³    1           2     ³       toetsen 4 t/m 7 en 12 t/m 15
	   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ       in huidige taskomat ongebruikt
     bovenste contact (proefpersoon)
	   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
	   ³ 8                11  ³
	   ³   9            10    ³
	   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

*/

#include "timrrt.h"

#define _dos_getvect getvect
#define _dos_setvect setvect
#define _disable()   disable()
#define _enable()    enable()


#define RKEY_REG  0x250    /* I/O address for IZF timerrespo card */
#define I8259A    0x20
#define I8259AI   8
#define RKEY_INTNR  (I8259AI+3) /* 3! */
#define RKEY_INT_MASK  8   /* 1 shl 3 =8 */
#define NOT_RKEY_INT_MASK 247 /* 11110111 */
#define KEY_MAX   16    /* assume max 16 keys */

extern void interrupt far newkey();

void (interrupt far *oldkey)( void );
void rkeyinit(void);
void rkeyrset(void);
int iodelay(void);
void rkeyread(long rtimes[KEY_MAX]);
BOOLEAN rkeyhit(BOOLEAN welkekeys[KEY_MAX]);

volatile BOOLEAN rkey_geset=FALSE;
volatile BOOLEAN hit=FALSE;
static unsigned char oldmask; /* 8259 masker */
volatile unsigned char lastrespokey=0;  /* globale variabele */
volatile long rktimes[KEY_MAX]= /* response tijden; index is keynummer (zie schema) */
{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };




int iodelay(void)
{
  volatile long oudtijd;
  volatile int tel=0; /* voorkom eruit optimalisatie */

  oudtijd=12345670L;
  while ( (oudtijd=oudtijd-1234567L) >1 ) tel++;
  return(tel);
}


void rkeyinit(void)
{
   unsigned char newmask;

   _disable();
   oldkey = _dos_getvect( RKEY_INTNR );
   _dos_setvect( RKEY_INTNR, newkey );
   oldmask=(unsigned char)inp(I8259A+1);
   iodelay();
   newmask=oldmask & (unsigned char)NOT_RKEY_INT_MASK;   /* enable hard int. naar 8259 */
   outp(I8259A+1,newmask);
   iodelay();
   inp( RKEY_REG ); /* dummy read */
   iodelay();
   rkey_geset=TRUE;
   _enable();
   atexit(rkeyrset);
}



void rkeyrset(void)
{
  if (rkey_geset)
  {
    _disable();
    _dos_setvect( RKEY_INTNR, oldkey );
    outp(I8259A+1,oldmask);
    iodelay();
    _enable();
    rkey_geset=FALSE;
  }
}



void rkeyread(long rtimes[KEY_MAX])
{
  int i;

  for (i=0; i<KEY_MAX; i++) rtimes[i]=rktimes[i];
  for (i=0; i<KEY_MAX; i++) rktimes[i]=-1;
}


BOOLEAN rkeyhit(BOOLEAN welkekeys[KEY_MAX])
{
  int i;

  for (i=0; i<KEY_MAX; i++)
  {
    if (welkekeys[i])
    {
      if (rktimes[i]!=-1L) return(TRUE);
    }
  }
  return(FALSE);
}
