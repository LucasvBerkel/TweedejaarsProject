#include <dos.h>
#include <graphics.h>
#include <math.h>

#include "myext.h"

Set_Kbd_Rate(unsigned char Rate);
int keyboard (void);
void interrupt far Get_Key();
void Capture_Kbd(void interrupt far (*func) () );
void Capture_Tik(void interrupt far (*func) () );
void interrupt far Get_Tik();
Set_Timer();
Restore_Tik();
Restore_Kbd();
Reset_Timer();
int Find_Headings(int x1,int y1,int x2,int y2);

extern int Freeze_Flag;

extern Update_Points();
extern Update_Shots();


Set_Kbd_Rate(unsigned char Rate)
{
  _AH=0x3;
  _AL=0x5;
  _BH=0;
  _BL=Rate; 	/* repeat rate of 20 Hz */
  geninterrupt(0x16);
  return(0);
}

int keyboard (void)
{
  union u_type{int a; char b[3];} keystroke;
  char inkey=0;
  char xtnd=0;

  if(bioskey(1)==0) return(NO_INPUT);  /* key relieved, no input */
  keystroke.a=bioskey(0);		/* fetch ascii code */
  inkey=keystroke.b[0];			/* ..and load code into variable */

  if(inkey==27) return(ESC); /* ESCcape terminates program */
  if(inkey==13) return(ENTER); /* ENTER pauses program */
  if(inkey!=0) return(REGULAR_CRAP);   /* the rest is crap */

  /* which leaves inkey==0 */
  xtnd=keystroke.b[1];
    switch (xtnd)
     {
       case 80 /* DOWN*/ : return(DOWN);
       case 72 /*UP*/    : return(UP);
       case 75 /*LEFT*/  : return(LEFT);
       case 77 /*RIGHT*/ : return(RIGHT);
       case 59 /*F1*/    : return(F1);
       case 60 /*F2*/    : return(F2);
       case 61 /*F3*/    : return(F3);
       default           : return(EXTENDED_CRAP);/* all rest irrelevant */
  }
}


void interrupt far Get_Key()
{
  int tmp;
  oldfunc(); /* now perform the old BIOS handler to keep things clean */

  tmp=keyboard();
  if(tmp)        /* throw away zeros, they indicate key release! */
    {
      New_Input_Flag=ON;
      Lastkey=Key;
      Key=tmp;

	/***** now handle double press time interval measurement  ******/

      if(Key==F3)
	{
	   if((Key==F3)&&(Lastkey!=F3)&&(!(Timing_Flag))) /* first F3 keypress */
	   {
	     t1=Time_Counter;
	     Timing_Flag=ON;
	     Check_Mine_Flag=ON; /* is used by Get_User_Input() */
	   }
	   if((Key==F3)&&(Lastkey==F3)&&(Timing_Flag))    /* second F3 keypress */
	   {
	     t2=Time_Counter;
	     Timing_Flag=OFF;
	     Key=0;  	/* to enable consecutive double_press */
			/* where with next keypress Lastkey=0 */
	     Display_Interval_Flag=ON; 	/* is used in main */
	   }
	   New_Input_Flag=OFF;   /* input was handled here */
	} /* end double press */
    } /* end if(tmp) */
}


	/****** capture any keyboard input via indicated routine **********/

void Capture_Kbd(void interrupt far (*func) () )
{
  /* save old interrupt */
  oldfunc=getvect(9);
  /* install our new interrupt handler */
  disable();
  setvect(9,func);
  enable();
}


void Capture_Tik(void interrupt far (*func) () )
{
  /* save old interrupt */
  oldtik=getvect(8);
  /* install our new interrupt handler */
  disable();
  setvect(8,func);
  enable();
}

void interrupt far Get_Tik()
{
  Time_Counter++;
  oldtik(); /*  now perform the old BIOS handler to keep things clean */
}

Set_Timer()
{
  outportb(0x43,0x36);
  outportb(0x40,1193&0xFF); /* 100 Hz */
  outportb(0x40,1193>>8);
  return(0);
}


Restore_Tik()
{
  disable();
  setvect(8,oldtik);   /* restore old interrupt handler */
  enable();
  return(0);
}


Restore_Kbd()
{
  disable();
  setvect(9,oldfunc);   /* restore old interrupt handler */
  enable();
  return(0);
}


Reset_Timer()
{
  outportb(0x43,0x36);  /* 36 talk to control register */
  outportb(0x40,0xFFFF);
  outportb(0x40,0xFFFF>>8);
  return(0);
}

int Find_Headings(int x1,int y1,int x2,int y2)
{
  int quadrant;
  double arcsinalfa;
  double b;
  double a;
  arcsinalfa=abs(x1-x2);
  a=pow(x1-x2,2)+pow(y1-y2,2);
  b=sqrt(a);
  arcsinalfa=asin(arcsinalfa/b);
  if (x1<x2)
     if (y1>y2) /* quadrant=1 */ return(arcsinalfa*57.3+0.5);
	else
		/* quadrant=2 */ return(180-arcsinalfa*57.3+0.5);
     else
     if (y1>y2) /* quadrant=4 */ return(360-arcsinalfa*57.3+0.5);
	else
		/* quadrant=3 */ return(180+arcsinalfa*57.3+0.5);

}

