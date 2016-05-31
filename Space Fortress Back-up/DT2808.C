//  Routines voor aansturing van de DT2808 A/D converter
//  Herschreven voor C++   oorspronkelijk in assembler
//  P.Spoelstra  nov'91 (rev.juli '92)

#define _disable()   disable()
#define _enable()    enable()
#define BOOLEAN int
#define TRUE 1
#define FALSE 0

#define ERR    -1
#define ERR_TMO    -3
#define OK    0
#define ADDA    0x02EC      // base address
#define ADDA_COM    1
#define ADDA_COM_TRIG    0x80
#define ADDA_COM_CLOK    0x40
#define ADDA_COM_CONT    0x20
#define ADDA_COM_DMA     0x10
#define ADDA_COM_OPCD    0x0F
#define OPC_RSET    0x00
#define OPC_CLER    0x01
#define OPC_RDER    0x02
#define OPC_SCLK    0x03
#define OPC_SPIN    0x04
#define OPC_SPOU    0x05
#define OPC_RDII    0x06
#define OPC_WDOI    0x07
#define OPC_WDAI    0x08
#define OPC_SDAP    0x09
#define OPC_WDA     0x0A
#define OPC_TEST    0x0B
#define OPC_RADI    0x0C
#define OPC_SADP    0x0D
#define OPC_RAD     0x0E
#define OPC_STOP    0x0F
#define ADDA_STAT    1
#define STAT_ERR    0x80
#define STAT_COM    0x08
#define STAT_RDY    0x04
#define STAT_DIF    0x02
#define STAT_DOR    0x01
#define ADDA_DATA   0
#define addainit_ssize    2

#include <dos.h>


unsigned int  addaerr=0; // error code


/* Prototypes */
	 /* addawcom - routine to write a command to DT2808 */
int addawcom(int command);  /* returns:  OK if OK ,   ERR_TMO if timed out */
	 /* addawdat - routine to write data to DT2808 */
int addawdat(char *buffer,  /* pointer to data buffer */
	     int size);  /* number of bytes to write in DATA IN */
				/* returns:  OK if OK, ERR_TMO    if timed out */
	 /* addardat - routine to read data from DT2808 */
int addardat(char *buffer,  /* pointer to data buffer */
	     int size);   /* number of bytes to read from DATA OUT */
			  /* returns:  OK  if OK, ERR_TMO    if timed out */
	 /* addachek - routine to check status of DT2808 */
int addachek(void);  // returns:  OK if OK,  ERR if error see _addaerr
			  //        ERR_TMO    if timed out
	 /* addainit - routine to reset the DT2808 */
int addainit(void);  /* returns:  OK  if OK ,   ERR_TMO  if timeout on reset */



	 /* addawcom - routine to write a command to DT2808 */
int addawcom(int command)  // returns:  OK if OK ,   ERR_TMO if timed out
{
  unsigned int h,t=0;
  unsigned char a;
  BOOLEAN klaar,err;

  h=ADDA+ADDA_STAT;
  do
  {
     t++;
     a=(unsigned char)(inp(h));
     a=(a & (unsigned char)STAT_RDY);
     klaar=(a!=0);
     err=(t>0xFFFD);
  }
  while (! (klaar || err));
  if (err) return(ERR_TMO);
  h=ADDA+ADDA_COM;
  outp(h,command);
  return(OK);
}


         /* addawdat - routine to write data to DT2808 */
int addawdat(char *buffer,  /* pointer to data buffer */
             int size)      /* number of bytes to write in DATA IN */
                            /* returns:  OK if OK, ERR_TMO    if timed out */
{
  int i;
  unsigned int h,t;
  unsigned char a;
  BOOLEAN klaar,err;

  for (i=0; i<size; i++)
  {
    t=0;
    h=ADDA+ADDA_STAT;
    do
    {
       t++;
       a=(unsigned char)(inp(h));
       a=(a & (unsigned char)STAT_DIF);
       klaar=(a==0);
       err=(t>0xFFFD);
    }
    while (! (klaar || err));
    if (err) return(ERR_TMO);
    h=ADDA+ADDA_DATA;
    outp(h,buffer[i]);
  }
  return(OK);
}


         /* addardat - routine to read data from DT2808 */
int addardat(char *buffer,  /* pointer to data buffer */
             int size)      /* number of bytes to read from DATA OUT */
                            /* returns:  OK  if OK, ERR_TMO    if timed out */
{
  int i;
  unsigned int h,t;
  unsigned char a;
  BOOLEAN klaar,err;

  for (i=0; i<size; i++)
  {
    h=ADDA+ADDA_STAT; t=0;
    do
    {
       t++;
       a=(unsigned char)(inp(h));
       a=(a & (unsigned char) (STAT_RDY | STAT_DOR));
       klaar=(a!=0);
       err=(t>0xFFFD);
    }
    while (! (klaar || err));
    if (err) return(ERR_TMO);
    h=ADDA+ADDA_DATA;
    buffer[i]=(unsigned char)(inp(h));
  }
  return(OK);
}


         /* addachek - routine to check status of DT2808 */

int addachek(void)   // returns:  OK if OK,  ERR if error see _addaerr
                     //           ERR_TMO    if timed out 
{
  unsigned int h,t;
  unsigned char a,a0;
  BOOLEAN klaar,err;
  int hs;

  h=ADDA+ADDA_STAT; t=0;
  do
  {
     t++;
     a=(unsigned char)(inp(h));
     a0=a;
     a=(a & (unsigned char)(STAT_RDY));
     klaar=(a!=0);
     err=(t>0xFFFD);
  }
  while (! (klaar || err));
  if (err) return(ERR_TMO);
  a0=(a0 & (unsigned char)(STAT_ERR));
  if (a0==0) return(OK);
  else
  {
    hs=addawcom(OPC_RDER);
    if (hs<0) return(hs);
    hs=addardat((char *)&addaerr,2);
    if (hs<0) return(hs);
    else
      return(ERR);
  }
}

         /* addainit - routine to reset the DT2808 */
int addainit(void)  /* returns:  OK  if OK ,   ERR_TMO  if timeout on reset */
{
  BOOLEAN nogeens=TRUE;
  int i=0;
  unsigned int h,t;
  unsigned char a;
  BOOLEAN err;

  while (nogeens)
  {
    i++;
    err= (addawcom(OPC_STOP)!=OK) ;
    if (err && i<4) nogeens=TRUE;
    else nogeens=FALSE;
  }
  h=ADDA+ADDA_COM;
  if (err) outp(h,OPC_STOP);
  h=ADDA+ADDA_DATA;
  inp(h); // dummy read
  // send reset signal
  err= (addawcom(OPC_RSET)!=OK) ;
  if (err) return(ERR_TMO);
  h=ADDA+ADDA_STAT;

  t=0;
  do
  {
    a=(unsigned char)inp(h);
    a=a & (unsigned char)STAT_DOR; t++;
  }
  while (a==0 && t<0xFFFD);
  h=ADDA+ADDA_DATA;
  inp(h);
  return(OK);
}

