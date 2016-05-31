// Timer is nog niet 100% betrouwbaar uit te lezen

#define ITIMER       0x254    /* base address of PIT */
#define TIMER2_VAL   1000  /*   to get 1kc clock */

/* Definitions for 8254 programmable interval timer */

/*  PORT DISPLACEMENTS */

#define PITC0    0x000    /* COUNTER 0 */
#define PITC1    0x001    /* COUNTER 1 */
#define PITC2    0x002    /* COUNTER 2 */
#define PITCW    0x003    /* CONTROL WORD */

/*  CONTROL WORD */

#define PITSC     192 /* SELECT COUNTER */
#define PITSC0    0x000
#define PITSC1    0x040
#define PITSC2    0x080
#define PITSCRB   0x0C0    /* Read back command */

#define PITRL      48  /*READ/LOAD*/
#define PITRLCL    0x000    /* COUNTER 0xLATCING OPERATION */
#define PITRLMB    0x010    /* MSB */
#define PITRLLB    0x020    /* LSB */
#define PITRLW     0x030    /* WORD */

#define PITM       14  /*MODE*/
#define PITMITC    0x000    /* 0 - INTERRUPT ON TERMINAL COUNT */
#define PITMP1S    0x002    /* 1 - PROGRAMMABLE ONE SHOT */
#define PITMRG     0x004    /* 2 - RATE GENERATOR */
#define PITMSW     0x006    /* 3 - SQUARE WAVE RATE GENERATOR */
#define PITMSTS    0x008    /* 4 - SOFTWARE TRIGGERED STROBE */
#define PITMHTS    0x00A    /* 5 - HARDWARE TRIGGERED STROBE */

#define PITBCD   1  /* 0 = 16 BIT BINARY COUNTER */


#define _dos_getvect getvect
#define _dos_setvect setvect
#define _disable()   disable()
#define _enable()    enable()

void timrinit(void);
volatile long timrread(void); /* read timers 0 and 1 as 32 bit number */
	   /*  timer 1 goes to next value on FFFF of timer0 */
void waits(int time); /* wacht [time] ms */
void timrrset(void);

volatile BOOLEAN timr_geset=FALSE;
static unsigned long oude_waarde=0;
BOOLEAN tfout=FALSE;

unsigned char lo(int w)
{
  return((unsigned char)(w % 256));
}


unsigned char hi(int w)
{
  return((unsigned char)(w / 256));
}

void timrinit(void)
{
  unsigned int a,a2;
  unsigned char h;
  BOOLEAN klaar;

  a=ITIMER+PITC0;
  h=(unsigned char)inp(a); h=(unsigned char)inp(a);
  a++;
  h=(unsigned char)inp(a); h=(unsigned char)inp(a);
  a++;
  h=(unsigned char)inp(a); h=(unsigned char)inp(a);
  a++;
  h=PITSC0+PITRLW+PITMRG;
  outp(a,h);
  h=PITSC1+PITRLW+PITMRG;
  outp(a,h);
  h=PITSC2+PITRLW+PITMRG;
  outp(a,h);
  /* set count value of timer 1 */
  a=ITIMER+PITC1;
  outp(a,0); outp(a,0);
  /* timer 0, fast, init timer1 */
  a=ITIMER+PITC0;
  outp(a,2); outp(a,0);  /* timer 0 -> fast counter */
  /* set timer 2*/
  a=ITIMER+PITC2;
  a2=TIMER2_VAL;
  outp(a,lo(a2));
  outp(a,hi(a2));
  /* Wait for timer 1 to start */
  do
  {
    a=ITIMER+PITCW;
    h=PITSCRB+0x24;
    outp(a,h);
    a=ITIMER+PITC1;
    h=(unsigned char)inp(a);
    h=(h & (unsigned char)0x40);
    klaar= ( h==0x00);
  }
  while (!klaar);
  /* again init timer 0 */
  a=ITIMER+PITCW;
  h=PITSC0+PITRLW+PITMRG; /* timer 0 16 bits counter */
  a=ITIMER+PITC0;
  outp(a,0); outp(a,0);
  timr_geset=TRUE;
  timrread(); timrread(); /* correctie evt. foute startwaarde */
}


volatile long timrread(void) /* read timers 0 and 1 as 32 bit number */
	   /*  timer 1 goes to next value on FFFF of timer0 */
{
  unsigned long l;
  unsigned int l0,l1,l2,l3;
  unsigned int a;
  unsigned char h;

  if (!timr_geset) timrinit();
  a=ITIMER+PITCW;
  h=PITSCRB+0x16;
  _disable();
  outp(a,h);
  a=ITIMER+PITC0;
  l0=(unsigned int)inp(a);
  l1=(unsigned int)inp(a);
  /* get timer 1 */
  a++;
  l2=(unsigned int)inp(a);
  l3=(unsigned int)inp(a);
  _enable();
  l1 =l1 << 8;
  l1+=l0;
  l1=0xFFFF-l1;
  l3=l3 << 8;
  l3+=l2;
  if (l3>0) {l3=0xFFFF-l3; l3++;}
  l= (unsigned long)l3 << 16;
  l+=(unsigned long)l1;
  if (l<oude_waarde) {l=oude_waarde; tfout=TRUE;}
  return((long)l);
}


void timrrset(void)
{
   /* dummy */
}


void waits(int time) /* wacht [time] ms */
{
  long time1;

  time1 = timrread() + 1000L*(long)time;
  while (timrread() < time1) ;
}
