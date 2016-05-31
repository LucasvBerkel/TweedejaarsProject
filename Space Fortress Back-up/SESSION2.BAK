#include <dos.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <mem.h>

#include "myext.h"
#include "dt2808.h"
#include "readch.h"
#include "wrchan.h"


#define TRUE 1
#define FALSE 0
#define BOOLEAN int

#define RT_MAX 5000
#define NR_KEYS 16

long rtimes[NR_KEYS];
long resptime;

#include "rkeyrt.c"
#include "timrrt.c"


extern int Freeze_Flag;


Set_Kbd_Rate(unsigned char Rate);
int keyboard (void);
void interrupt far Get_Key();
void Capture_Kbd(void interrupt far (*func) () );
void Capture_Respo(void interrupt far (*func) () );
void Capture_Tik(void interrupt far (*func) () );
void interrupt far Get_Tik();
void interrupt far newkey(void);
Set_Timer();
Restore_Tik();
Restore_Kbd();
Reset_Timer();
int Find_Headings(int x1,int y1,int x2,int y2);
void  Translate_Stick(void);
int   Read_Stick(void);
void Read_Respo(void);
void Init_Stick(void);
void Init_Respo(void);
void Close_Respo(void);
int Jstk_Calibration(void);

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

  if(inkey==25) { train.termination_ind = 1; return(ESC); }
                                  /* CTRL_Y terminates program */
  if(inkey==10) return(ENTER); /* CTRL_ENTER pauses program */
  if(inkey!=0) return(REGULAR_CRAP);   /* the rest is crap */

  /* which leaves inkey==0 */
  xtnd=keystroke.b[1];
    switch (xtnd)
     {
       case 80 /* DOWN*/ : return(DOWN);  //!!
       case 72 /*UP*/    : return(UP);    //!!
       case 75 /*LEFT*/  : return(LEFT);  //!!
       case 77 /*RIGHT*/ : return(RIGHT); //!!
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


int respoboard (void)
{

 if (hit==TRUE)
 {
   switch((int)lastrespokey)
   {
   case 179: return(179);
   case 176: New_Missile_Flag=ON; hit=FALSE; return(176);
   case 177: Check_Bonus_Input(); hit=FALSE; return(177);
   case 178: Check_Bonus_Input(); hit=FALSE; return(178);
   default : return(NO_INPUT);
   }
 }

return(-1);
}


void interrupt far newkey( void )  /* geen video IO! */
{
   int tmp;
   unsigned int h;

   lastrespokey=(unsigned char)inp(RKEY_REG); /* moet altijd gelezen worden*/
   hit=TRUE;
   h=2*((unsigned int)(lastrespokey));
   if ((h/256)>0)  /* hoogste bit gezet? */
   {
     h=h*2;
     h=h & 0x3C;
     outp(0x20,0x20); /* EOI naar 8259 */
     h=h/4; /* zie toets-schema begin file */
     rktimes[h]=timrread();  /* Let op! timrinit() in locale prg */
   }

  tmp=Respoboard();

  if(tmp)        /* throw away zeros, they indicate key release! */
    {
      New_Input_Flag=ON;
      Lastkey=Sponskey;
      Sponskey=tmp;

	/***** now handle double press time interval measurement  ******/

      if(Sponskey==179)
	{
	   if((Sponskey==179)&&(Lastkey!=179)&&(!(Timing_Flag))) /* first "F3" keypress */
	   {
	     t1=Time_Counter;
	     Timing_Flag=ON;
	     Check_Mine_Flag=ON; /* is used by Get_User_Input() */
	     hit = FALSE;
	   }
	   if((Sponskey==179)&&(Lastkey==179)&&(Timing_Flag))    /* second "F3" keypress */
	   {
	     t2=Time_Counter;
	     Timing_Flag=OFF;
	     Key=0;  	/* to enable consecutive double_press */
			/* where with next keypress Lastkey=0 */
	     Display_Interval_Flag=ON; 	/* is used in main */
	     hit = FALSE;
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

void Capture_Respo(void interrupt far (*func) () )
{
  /* save old interrupt */
  oldrespo=getvect(11);
  /* install our new interrupt handler */
  disable();
  setvect(11,func);
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

Restore_Respo()
{
  disable();
  setvect(11,oldrespo);   /* restore old interrupt handler */
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



void Get_User_Input()
{
//!!  Read_Stick(); //!!
//!!  if (Stick_Input != NIL) Translate_Stick(); //!!

  if (New_Input_Flag) /* new input occured */
  {
    New_Input_Flag=OFF;	/* make sure no repetitions on same input */
    if (Key==UP)    Accel_Input=1;    	 	/*   UP    */
    if (Key==LEFT)  Rotate_Input=-1;  		/*   LEFT  */
    if (Key==RIGHT) Rotate_Input=1;   		/*   RIGHT */
    if (Key==DOWN)  New_Missile_Flag=ON; 	/*   DOWN  */
    if (Key==F1)    Check_Bonus_Input();        /*   P(oints) */
    if (Key==F2)    Check_Bonus_Input();        /*   M(issiles) */
/*    if (Key==F3)    is handled by kbd interrupt handler */
    if (Key==ENTER) Freeze_Flag=Freeze_Flag^1; /* toggle freeze flag */
    if (Key==ESC)   End_Flag=ON;
  }
  if(Check_Mine_Flag) /* after first press of F3 */
    {
      Check_Mine_Flag=OFF;
      if((Mine_Flag==ALIVE) && (Mine_Type==FRIEND))
	Missile_Type=WASTED;
    }
}

void Init_Stick(void)
{

int 	i;

  for (i=0; i<5 ; i++)
  {
   if (addainit() == 0) break;
  }
  if (i >= 5)
  {
    printf("fout in ADO");
    delay(1000);
    exit(-1);
  }
   writchan(0,0xFF);
   if (Jstk_Calibration() != 0)
     {
     Show_Error("Joystick Defect, Roep de Instructeur !");
     exit(-1);
     }

}

int Read_Stick()
{
int h, x, y;

if (Game_Type == SPACE_FORTRESS) {x=3; y=3;}
else {x=10; y=10;} /* setting the responsivity of joystick */

h = (int)readchan(15);


    if( (int)readchan(7)  < (jstk_clb.y_center - (int)(jstk_clb.y_range/y)))
      {
	  Stick_Input = THRUST;
       }
    if( h > (jstk_clb.x_center + (int)((jstk_clb.x_leftrange)/x)))
      {
	 Stick_Input = ROTATE_LEFT;
	 Ship_Angular_Step = (int)( ((h - jstk_clb.x_center)*20) /jstk_clb.x_leftrange);
       }
    if( h < (jstk_clb.x_center - (int)((jstk_clb.x_rightrange)/x)))
      {
	 Stick_Input = ROTATE_RIGHT;
	 Ship_Angular_Step = (int)( ((jstk_clb.x_center - h)*20) /jstk_clb.x_rightrange);
      }

    if (Ship_Angular_Step == 0) Ship_Angular_Step = 1;

    if (Game_Type == SPACE_FORTRESS) Ship_Angular_Step = 10;

return(0);
}


void Translate_Stick()
{
  switch(Stick_Input)
   {
   case THRUST       : Accel_Input  =  1;  break;  /*   UP    */
   case ROTATE_LEFT  : Rotate_Input = -1;  break;  /*   LEFT  */
   case ROTATE_RIGHT : Rotate_Input =  1;  break;  /*   RIGHT */
   }
  Stick_Input=NIL;
  New_Input_Flag=OFF;
}

void Init_Respo(void)
{
  int i;

  timrinit();   // NIET VERGETEN!!
  rkeyinit();
}

void Close_Respo(void)
{
rkeyrset();
}

int Jstk_Calibration(void)
{

    int Flag=1;
  for(;;)
  {
    clrscr();
    cprintf("\r\n%-80s","Plaats joystick aan de LINKER kant\r\n");
    cprintf("\r%-80s","Druk op de VUURKNOP als dat gebeurd is\r\n");
    while(Flag==1)
     {
      if((int)lastrespokey == 176) Flag=0;
      jstk_clb.x_left = readchan(15);
      cprintf("\rx_left=%4d",jstk_clb.x_left);
     }
    Flag=1;
    lastrespokey = 0;
    jstk_clb.x_left = readchan(15);    /* read final sample */
    if (jstk_clb.x_left < 0) return(jstk_clb.x_left);  /* error in AD/DA */
    clrscr();
    cprintf("\r\n%80s","Plaats joystick aan de RECHTER kant\r\n");
    cprintf("\r%80s","Druk op de VUURKNOP als dat gebeurd is\r\n");
    while(Flag==1)
    {
      if ((int)lastrespokey == 176) Flag=0;
      jstk_clb.x_right = readchan(15);
      cprintf("\rx_right=%4d",jstk_clb.x_right);
    }
    Flag=1;
    lastrespokey = 0;
    jstk_clb.x_right = readchan(15);
    if (jstk_clb.x_right < 0) return(jstk_clb.x_right); /* error in AD/DA */
    clrscr();
    cprintf("\r\n");
    cprintf("\r\n%-50s","Plaats joystick in het CENTRUM\r\n");
    cprintf("\r%-50s","Druk op de VUURKNOP als dat gebeurd is\r\n");
    while(Flag==1)
     {
      if((int)lastrespokey == 176) Flag=0;
      jstk_clb.x_center = readchan(15);
      jstk_clb.y_center = readchan(7);
      cprintf("\rx_center=%4d",jstk_clb.x_center);
      cprintf("\ry_center=%4d",jstk_clb.y_center);
     }
    Flag=1;
    lastrespokey = 0;
    jstk_clb.y_center = readchan(7);  /* read final sample */
    if (jstk_clb.x_left < 0) return(jstk_clb.x_left);  /* error in AD/DA */
    if (jstk_clb.x_right > jstk_clb.x_left)
    {
      Show_Error("Probeer nog eens. Druk ENTER om verder te gaan");
    }
    else
      if ((jstk_clb.x_left - jstk_clb.x_center < 150) ||
	  (jstk_clb.x_center - jstk_clb.x_right < 150))
      {
	cprintf("x_left=%d x_right=%d\r\n",jstk_clb.x_left,jstk_clb.x_right);
	Show_Error("Plaats de joystick goed in de hoeken. Druk ENTER om verder te gaan");
	cprintf("\r\n");
      }
      else break;
  }

  for(;;)
  {
    clrscr();
    cprintf("\r\n%80s","Druk nu de joystick naar VOREN\r\n");
    cprintf("\r%80s","Druk op de VUURKNOP als dat gebeurd is\r\n");
    while(Flag==1)
    {
      if ((int)lastrespokey == 176) Flag=0;
      jstk_clb.y_up = readchan(7);
      cprintf("\ry_up=%4d",jstk_clb.y_up);
    }
    Flag=1;
    lastrespokey = 0;
    jstk_clb.y_up = readchan(7);
    if (jstk_clb.y_up < 0) return(jstk_clb.y_up); /* error in AD/DA */
    if (jstk_clb.y_center < jstk_clb.y_up) return(jstk_clb.y_center);
    else
      if (jstk_clb.y_center - jstk_clb.y_up < 150)
      {
	cprintf("x_left=%d x_right=%d\r\n",jstk_clb.x_left,jstk_clb.x_right);
	Show_Error("probeer nog eens! Druk ENTER om verder te gaan");
	cprintf("\r\n");
      }
      else break;
  }

  jstk_clb.x_leftrange = jstk_clb.x_left - jstk_clb.x_center;
  jstk_clb.x_rightrange = jstk_clb.x_center - jstk_clb.x_right;
  jstk_clb.y_range = jstk_clb.y_center -jstk_clb.y_up;
  clrscr();
  lastrespokey=0;
  return(0);

}


