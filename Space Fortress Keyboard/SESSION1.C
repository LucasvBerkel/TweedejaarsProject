#include <stdarg.h>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <bios.h>
#include <alloc.h>
#include <dos.h>
#include <math.h>

#include "myconst.h"
#include "myvars.h"


extern char Mine_Indicator;
extern mine_type Mine_Type;
extern char Foe_Menu[3][1];
extern char Friend_Menu[3][1];


		/* Functions  */

int Get_Random_Index(int vec[]);
int Select_Mine_Menus();
void Mydelay(unsigned Time);
char Keyboard1();
int gprintf( int *xloc, int *yloc, char *fmt, ... );
Set_Graphics_Eraser();
Show_Score(int val, int x, int y);
Update_Points();
Update_Control();
Update_Velocity();
Update_Vulner();
Update_Interval();
Update_Speed();
Update_Shots();
void Open_Graphics(void);
void Close_Graphics(void);
Init_Session();
Init_Game();
float Fcos(int Headings_Degs);
float Fsin(int Headings_Degs);
int Draw_Ship (int x, int y, int Headings, int size);
int Draw_Hexagone(int X_Center,int Y_Center,int Hex_Size);
void Draw_Frame();
int Draw_Fort (int x, int y, int Headings, int size );
int Draw_Missile (int x, int y, int Headings, int size);
int Draw_Shell(int x, int y, int Headings, int size);
int Draw_Mine (int x, int y, int size);
void Reset_Screen();
void Initialize_Graphics(void);
int task_elements(int level_indicator);
void Get_User_Input();
Check_Bonus_Input();
int Generate_Non_Bonus_Char();
Generate_Resource_Character();



extern Set_Kbd_Rate(unsigned char Rate);
extern void Capture_Kbd(void interrupt far (*func) () );
extern void Capture_Tik(void interrupt far (*func) () );
extern void interrupt far Get_Tik();
extern Set_Timer();
extern void interrupt far Get_Key();
extern Restore_Tik();
extern Restore_Kbd();
extern Reset_Timer();

extern Move_Ship();
extern Handle_MIssile();

extern int Handle_MIne();
extern Find_Interval();

extern Handle_Bonus();
extern Set_Bonus_Message();
extern Set_Bonus_Chars();

char Char_Set[10][1]={"Y","M","P","B","Q","K","C","W","R","Z"};



/* Routine om meerdere verschillende random waarden te
   genereren, wordt gebruikt in de Select_Mine_Menus() voor
   het bepalen van drie vetrschillende letters voor FOE en FRIEND
   mines.
*/

int Get_Random_Index(int vec[])
{
  int k;

  do
     { k=random(10); }
  while (vec[k]==-1);

  vec[k]=-1;
  return(k);
}


int Select_Mine_Menus()
{
  int vec[10];
  int i,ri;

  for(i=0;i<10;i++) vec[i]=0;
  for (i=0;i<3;i++)
      {
	ri=Get_Random_Index(vec);
	Friend_Menu[i][0]= Char_Set[ri][0];
	ri=Get_Random_Index(vec);
	Foe_Menu[i][0]= Char_Set[ri][0];
      }
  return(0);
}



void Mydelay(unsigned Time)
{
  unsigned long end;

  end=Time_Counter+Time;
  while(Time_Counter<end);

}

char Keyboard1() /* handles escape key press only */
{
  union u_type{int a; char b[3];} keystroke;
  char inkey=0;

  while(bioskey(1)==0);  	/* key relieved, no input */
  keystroke.a=bioskey(0);	/* fetch ascii code */
  inkey=keystroke.b[0];		/* ..and load code into variable */
  return(inkey);

}

int gprintf( int *xloc, int *yloc, char *fmt, ... )
{
  va_list  argptr;			/* Argument list pointer	*/
  char str[140];			/* Buffer to build sting into	*/
  int cnt;				/* Result of SPRINTF for return */

  va_start( argptr, fmt );		/* Initialize va_ functions	*/
  cnt = vsprintf( str, fmt, argptr );	/* prints string to buffer	*/
  outtextxy( *xloc, *yloc, str );	/* Send string in graphics mode */
  va_end( argptr );			/* Close va_ functions		*/
  return( cnt );			/* Return the conversion count	*/
}

Set_Graphics_Eraser()
{
  int size;

  size=imagesize(0,0,40,9);        /*length of 5 characters*/
  buffer1=malloc(size);
  getimage(100,100,140,109,buffer1);
  return(0);
}

Show_Score(int val, int x, int y) /* anywhere within data panel */
{
  int svcolor;

  svcolor=getcolor();
  setcolor(TEXT_COLOR);
  setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);

	/* data panel in screen global coordinates */

  putimage(x,y,buffer1,COPY_PUT); /* erase garbage */
  gprintf(&x,&y,"%d",val);

  setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);   /* restore gaming area */
  setcolor(svcolor); /* restore previous color */
  return(0);
}

Update_Points()
{
  Show_Score(Points,Points_X-8,Data_Line);
  return(0);
}

Update_Control()
{
  Show_Score(Control,Control_X-8,Data_Line);
  return(0);
}

Update_Velocity()
{
  Show_Score(Velocity,Velocity_X,Data_Line);
  return(0);
}

Update_Vulner()  /* for vulner only */
{
  Show_Score(Vulner_Counter,Vulner_X,Data_Line);
  return(0);
}

/* IFF is missing here */

Update_Interval()
{
  Show_Score(Double_Press_Interval,Interval_X,Data_Line);
  return(0);
}

Update_Speed()
{
  Show_Score(Speed,Speed_X-8,Data_Line);
  return(0);
}

Update_Shots()
{
  Show_Score(Missile_Stock,Shots_X,Data_Line);
  return(0);
}

void Reset_Screen()
{
  int i;
		/*  reset variables */
  Ship_X_Pos=0.25*MaxX; /* on a 640 x 480 screen VGA-HI */
  Ship_Y_Pos=0.5*MaxY; /* same as above */
  Ship_X_Speed=0.0;
  Ship_Y_Speed=0.0;
  Ship_Headings=0;
  Mine_Flag=DEAD;
  for(i=1;i<6;i++) Missile_Flag[i]=DEAD;
  Missile_Type=VS_FRIEND;
  Missile_Vs_Mine_Only=OFF;
  Missiles_Counter=0;
  Shell_Flag=DEAD;
  Rotate_Input=0; /* joystick left/right */
  Accel_Input=0; /* joystick forward */
  End_Flag=OFF;
  Fort_Headings=270;
  Vulner_Counter=0;
  Timing_Flag=OFF; /* if screen reset between consecutive presses */
  Resource_Flag=OFF;
  Resource_Off_Counter=0;
  Bonus_Display_Flag=NOT_PRESENT;   /* in case bonus is pressed after
  Bonus_Granted=OFF;
  Fort_Lock_Counter=0;

		/* reset screen */
  Draw_Frame();
  if(AspectRatio==1.0)
  {
    Draw_Hexagone(MaxX/2,MaxY/2,BIG_HEXAGONE_SIZE_FACTOR*MaxX);
    Draw_Hexagone(MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX);
  }
  else
  {
    Draw_Hexagone(MaxX/2,MaxY/2,BIG_HEXAGONE_SIZE_FACTOR*MaxX/GraphSqrFact);
    Draw_Hexagone(MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX/GraphSqrFact);
  }
  Draw_Fort(MaxX/2,MaxY/2,Fort_Headings,FORT_SIZE_FACTOR*MaxX);
  Draw_Ship(Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX);

			/* reset panel */
  Update_Points();
  Update_Vulner();
  Update_Interval();
  Update_Shots();
  Update_Control();
  Update_Velocity();
  Update_Speed();

}  /* end reset screen */

Init_Session()
{
  int i;
  char a;

	/* get game parameters parameters */

  clrscr();
  gotoxy(1,5);
  printf("The default # of games is 3, would you like to change it? (Y/N):");
  a=Keyboard1();
  if((a==121)||(a==89))  /* y=121 and Y=89 */
    {
     printf("\n\nPlease type new number of games (1-9):");
     do { a=Keyboard1();
	  if((a>=49)&&(a<=57)) { No_Of_Games=a-48;
				 printf(" %d\n",No_Of_Games);
				 delay(500);
			       }
	  else a=0;
	} while(a==0);
    }
  gotoxy(1,10);
  printf("The default game duration is 3 minutes, would you like to change it? (Y/N):");
  a=Keyboard1();
  if((a==121)||(a==89))  /* y=121 and Y=89 */
    {
     printf("\n\nPlease type new game duration (1-6 minutes):");
     do { a=Keyboard1();
	  if((a>=49)&&(a<=54)) { One_Game_Duration=a-48;
				 printf(" %d\n",One_Game_Duration);
				 delay(500);
			       }
	  else a=0;
	} while(a==0);
    }
  One_Game_Loops=One_Game_Duration*60*20;
  Game_Type=SPACE_FORTRESS;
  Mine_Live_Loops=200;
  Mine_Wait_Loops=80;
  return(0);
}

Init_Game()
{
  int i;

  Score=0;
  Points=0;
  Velocity=0;
  Control=0;
  Speed=0;
  No_Of_Bonus_Intervals=6;
  No_Of_Points_Bonus_Taken=0;
  No_Of_Missiles_Bonus_Taken=0;
  Ship_Damaged_By_Fortress=0;
  Ship_Damaged_By_Mines=0;
  Fortress_Destroyed=0;
  Normal_Game_Termination=0;
  Vulner_Counter=0;
  Last_Missile_Hit=0; /* to measure interval between two consecutive
			    hits of the fortress */
  Ship_Killings_Counter=0;
  Resource_Flag=OFF;
  Resource_Off_Counter=0;
  Bonus_Display_Flag=NOT_PRESENT;   /* in case bonus is pressed
					after game ends */
  No_Of_Bonus_Windows=0;
  Missile_Stock=100;
  clrscr();
  gotoxy(30,5);
  printf("SPACE  FORTRESS ");
  gotoxy(20,15);
  Select_Mine_Menus();
  printf("Your foe mines are:");
  for(i=0;i<3;i++) printf("    %c",Foe_Menu[i][0]);
  gotoxy(1,24);
  printf("Type any  key to continue ..\n");
  getch();
  return(0);
}
/*									*/
/*	OPEN GRAPHICS: Initializes the graphics system and reports 	*/
/*	any errors which occured.					*/
/*									*/


void Open_Graphics(void)
{
  int xasp,yasp;


  GraphDriver = DETECT;
		/* Request auto-detection	*/
  /*GraphMode=EGAHI;*/
  initgraph( &GraphDriver, &GraphMode, "" );
  ErrorCode = graphresult();		/* Read result of initialization*/
  if( ErrorCode != grOk ){		/* Error occured during init	*/
    printf(" Graphics System Error: %s\n", grapherrormsg( ErrorCode ) );
    exit( 1 );
  }

  getpalette( &palette );		/* Read the palette from board	*/
  MaxColors = getmaxcolor() + 1;	/* Read maximum number of colors*/

  MaxX = getmaxx();
  MaxY = getmaxy();			/* Read size of screen		*/

  getaspectratio( &xasp, &yasp );	/* read the hardware aspect	*/
  AspectRatio = (double)xasp / (double)yasp; /* Get correction factor	*/
  GraphSqrFact=MaxX*AspectRatio/MaxY;       /* for EGA cases */
  setwritemode(XOR_PUT);
}

void Initialize_Graphics(void)
{
  int Height,OldmaxX;
  int t,t1;
  int x,dx;

  cleardevice();
  Height=textheight("H");		/* Get basic text height */
  OldmaxX=MaxX;
  t1=4*Height;

  Panel_Y_End=MaxY;
  Panel_Y_Start=MaxY-t1+2;
  MaxY_Panel=Panel_Y_End-Panel_Y_Start;

  MaxY=MaxY-t1;
  if(AspectRatio==1.0) /* VGA HI */
    MaxX=MaxY;
    else  /* for all others */
    {
      MaxX=MaxX*AspectRatio;    /********* MaxX and MaxY give a square */
      MaxX=MaxX-t1/AspectRatio;  /******** less two panel lines */
    }
  Xmargin=OldmaxX/2-MaxX/2;
  setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);

  if(Game_Type==SPACE_FORTRESS)
    {          	/* set data value locations for space fortress */
      dx=MaxX/8;
      Points_X=x=2*textwidth("Z");
      x=x+dx; Control_X=x;
      x=x+dx; Velocity_X=x;
      x=x+dx; Vulner_X=x;
      x=x+dx; IFF_X=x;
      x=x+dx; Interval_X=x;
      x=x+dx; Speed_X=x;
      x=x+dx; Shots_X=x;
    }
  else /* set data value locations for aiming test */
  {
    dx=MaxX/3;
    Mines_X=x=MaxX/6-2*textwidth( "H" );
    x=x+dx;   Speed_X=x;
    x=x+dx-textwidth("H"); Score_X=x;
  }
	/* set graphics eraser is done in main */
}

void Close_Graphics(void)
{
  cleardevice();
  restorecrtmode();
}

float Fcos(int Headings_Degs) /* compute cos of 0 - 359 degrees */
  {
    float arc;
    arc=Headings_Degs*ARC_CONV;
    return(cos(arc));
  }

float Fsin(int Headings_Degs) /* compute sin of 0 - 359 degrees */
  {
    float arc;
    arc=Headings_Degs*ARC_CONV; /* convert degrees to radians */
    return(sin(arc));
  }

void Draw_Frame()
{
  int Height;
  int t,t1,svcolor;
  int x,y,dx;

  Height=textheight("H");		/* Get basic text height */

  svcolor=getcolor();                   /* save present color */
  setbkcolor(BACKGROUND_COLOR);
  cleardevice();
  setcolor(FRAME_COLOR);
	/* handle panel */
  setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);
	/* data panel in screen global coordinates */
  rectangle(0,0,MaxX,MaxY_Panel);
  line(0,2*Height,MaxX,2*Height);

		  /* write panel headers */
  if(Game_Type==SPACE_FORTRESS)
    {
      x=2;
      y=4;
      dx=MaxX/8; /* step between two headers */
      Data_Line=2*Height+4;
      gprintf ( &x, &y,"  PNTS");
      x=x+dx; gprintf ( &x, &y," CNTRL");
      x=x+dx; gprintf ( &x, &y," VLCTY");
      x=x+dx; gprintf ( &x, &y," VLNER");
      x=x+dx; gprintf ( &x, &y,"  IFF ");
      x=x+dx; gprintf ( &x, &y,"INTRVL");
      x=x+dx; gprintf ( &x, &y," SPEED");
      x=x+dx; gprintf ( &x, &y," SHOTS");

	  /* draw vertical lines between columns */

	      line(x,0,x,MaxY_Panel);
      x=x-dx; line(x,0,x,MaxY_Panel);
      x=x-dx; line(x,0,x,MaxY_Panel);
      x=x-dx; line(x,0,x,MaxY_Panel);
      x=x-dx; line(x,0,x,MaxY_Panel);
      x=x-dx; line(x,0,x,MaxY_Panel);
      x=x-dx; line(x,0,x,MaxY_Panel);
   }
   else /* frame for aiming test */
   {
     x=MaxX/6-32;
     y=4;
     dx=MaxX/3;		 /* step between two headers */
     Data_Line=2*Height+4;
	  gprintf ( &x, &y,"  MINES");
     x=x+dx; gprintf ( &x, &y," SPEED");
     x=x+dx; gprintf ( &x, &y," SCORE");

	  /* draw vertical lines between columns */
     x=dx;   line(x,0,x,MaxY_Panel);
     x=x+dx; line(x,0,x,MaxY_Panel);
   }

  setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1); /* in screen global coordinates */
  rectangle(0,0,MaxX,MaxY); 		/* main frame of the viewport */

  /* set graphics eraser is done in main */

  setcolor(svcolor); /* restore previous color */
}

int Draw_Hexagone(int X_Center,int Y_Center,int Hex_Outer_Radius)
{
  int Abs_Y;
  int svcolor;

  svcolor=getcolor(); /* save present color */
  setcolor(HEX_COLOR);

  Abs_Y=Hex_Outer_Radius*0.866;  /* sin(60)=0.866 */
  moveto(X_Center+Hex_Outer_Radius,Y_Center); /* right-hand tip */
  lineto(X_Center+Hex_Outer_Radius/2,Y_Center-Abs_Y);
  lineto(X_Center-Hex_Outer_Radius/2,Y_Center-Abs_Y);

  lineto(X_Center-Hex_Outer_Radius,Y_Center);
  lineto(X_Center-Hex_Outer_Radius/2,Y_Center+Abs_Y);

  lineto(X_Center+Hex_Outer_Radius/2,Y_Center+Abs_Y);
  lineto(X_Center+Hex_Outer_Radius,Y_Center);

  setcolor(svcolor); /* restore previous color */
  return(0);
}

int  Draw_Ship (int x, int y, int Headings, int size)
{
  /* size - is the entire length of the ship */
  int x1,y1;  /* ship's aft location */
  int x2,y2;  /* ship's nose location */
  int xl,yl;     /* ship's left wing tip location */
  int xr,yr;     /* ship's right wing tip location */
  int xc,yc;  /* fuselage and wings connecting point */
  int Right_Wing_Headings;
  int Left_Wing_Headings;
  int svcolor;
  float tmp;

  svcolor=getcolor(); /* save present color */
  setcolor(SHIP_COLOR);
  xc=x;
  yc=y;
  x1=xc-0.5*size*Fsin(Headings);
  y1=yc+0.5*size*Fcos(Headings);
  x2=xc+0.5*size*Fsin(Headings);
  y2=yc-0.5*size*Fcos(Headings);
  line(x1,y1,x2,y2);
  Right_Wing_Headings=Headings+135;
  if(Right_Wing_Headings>359) Right_Wing_Headings=Right_Wing_Headings-360;
  Left_Wing_Headings=Headings+225;
  if(Left_Wing_Headings>359) Left_Wing_Headings=Left_Wing_Headings-360;
  xr=xc+0.707*size*Fsin(Right_Wing_Headings);
  yr=yc-0.707*size*Fcos(Right_Wing_Headings);
  line(xc,yc,xr,yr);
  xl=xc+0.707*size*Fsin(Left_Wing_Headings);
  yl=yc-0.707*size*Fcos(Left_Wing_Headings);
  line(xc,yc,xl,yl);
  setcolor(svcolor); /* restore previous color */
  return(0);
}

int Draw_Fort (int x, int y, int Headings, int size )
{
  int x1,y1;     /* fort's aft location */
  int x2,y2;     /* fort's nose location */
  int xl,yl;     /* ship's left wing tip location */
  int xr,yr;     /* ship's right wing tip location */
  int xc,yc;     /* fuselage and wings connecting point */
  int xrt,yrt;   /* tip of right wing */
  int xlt,ylt;   /* tip of left wing */
  int Right_Wing_Headings;
  int Left_Wing_Headings;
  int Right_Wing_Tip_Headings;
  int Left_Wing_Tip_Headings;
  int svcolor;

  svcolor=getcolor(); /* save present color */
  setcolor(FORT_COLOR);
  x1=x;
  y1=y;
  x2=x1+size*Fsin(Headings);
  y2=y1-size*Fcos(Headings);
  line(x1,y1,x2,y2);
  xc=x1+(x2-x1)*0.5;
  yc=y1+(y2-y1)*0.5;
  Right_Wing_Headings=Headings+90;
  if(Right_Wing_Headings>359) Right_Wing_Headings=Right_Wing_Headings-360;
  Left_Wing_Headings=Headings+270;
  if(Left_Wing_Headings>359) Left_Wing_Headings=Left_Wing_Headings-360;
  xr=xc+0.4*size*Fsin(Right_Wing_Headings)+0.5;
  yr=yc-0.4*size*Fcos(Right_Wing_Headings)+0.5;
  line(xc,yc,xr,yr);
  xl=xc+0.4*size*Fsin(Left_Wing_Headings)+0.5;
  yl=yc-0.4*size*Fcos(Left_Wing_Headings)+0.5;
  line(xc,yc,xl,yl);
  Right_Wing_Tip_Headings=Right_Wing_Headings+90;
  if(Right_Wing_Tip_Headings>359) Right_Wing_Tip_Headings=
				       Right_Wing_Tip_Headings-360;
  xrt=xr+0.5*size*Fsin(Right_Wing_Tip_Headings)+0.5;
  yrt=yr-0.5*size*Fcos(Right_Wing_Tip_Headings)+0.5;
  line(xr,yr,xrt,yrt);
  Left_Wing_Tip_Headings=Right_Wing_Tip_Headings;
  xlt=xl+0.5*size*Fsin(Left_Wing_Tip_Headings)+0.5;
  ylt=yl-0.5*size*Fcos(Left_Wing_Tip_Headings)+0.5;
  line(xl,yl,xlt,ylt);
  setcolor(svcolor); /* restore previous color */
  return(0);
}

int Draw_Mine (int x, int y, int size)  /* x,y is on screen center location
					size is half of horizontal diagonal */
{
  int svcolor;

  svcolor=getcolor(); /* save present color */
  setcolor(MINE_COLOR);
  moveto(x-size,y);
  lineto(x,y-1.18*size);   /* 1.3/1.1=1.18 */
  lineto(x+size,y);
  lineto(x,y+1.18*size);
  lineto(x-size,y);
  setcolor(svcolor); /* restore previous color */
  return(0);
}

int Draw_Missile (int x, int y, int Headings, int size)
{
  int x1,y1;  /* ship's aft location */
  int x2,y2;  /* ship's nose location */
  int xl,yl;     /* ship's left wing tip location */
  int xr,yr;     /* ship's right wing tip location */
  int xc,yc;  /* fuselage and wings connecting point */
  int Right_Wing_Headings;
  int Left_Wing_Headings;
  int svcolor;

  svcolor=getcolor(); /* save present color */
  setcolor(MISSILE_COLOR);
  x1=x;
  y1=y;
  x2=x1+size*Fsin(Headings);
  y2=y1-size*Fcos(Headings);
  line(x1,y1,x2,y2);
  xc=x2;
  yc=y2;
  Right_Wing_Headings=Headings+135;
  if(Right_Wing_Headings>359) Right_Wing_Headings=Right_Wing_Headings-360;
  Left_Wing_Headings=Headings+225;
  if(Left_Wing_Headings>359) Left_Wing_Headings=Left_Wing_Headings-360;
  xr=xc+0.25*size*Fsin(Right_Wing_Headings);
  yr=yc-0.25*size*Fcos(Right_Wing_Headings);
  line(xc,yc,xr,yr);
  xl=xc+0.25*size*Fsin(Left_Wing_Headings);
  yl=yc-0.25*size*Fcos(Left_Wing_Headings);
  line(xc,yc,xl,yl);
  setcolor(svcolor); /* restore previous color */
  return(0);
}

int Draw_Shell(int x, int y, int Headings, int size)
{
  int x1,y1;  /* shell's aft location */
  int x2,y2;  /* shell's nose location */
  int xl,yl;     /* shell's left tip location */
  int xr,yr;     /* shell's right tip location */
  int Right_Tip_Headings;
  int Left_Tip_Headings;
  int svcolor;

  svcolor=getcolor(); /* save present color */
  setcolor(SHELL_COLOR);
  x1=x;
  y1=y;
  x2=x1+size*Fsin(Headings);
  y2=y1-size*Fcos(Headings);
  Right_Tip_Headings=Headings+30;
  if(Right_Tip_Headings>359) Right_Tip_Headings=Right_Tip_Headings-360;
  Left_Tip_Headings=Headings+330;
  if(Left_Tip_Headings>359) Left_Tip_Headings=Left_Tip_Headings-360;
  xr=x1+0.4*size*Fsin(Right_Tip_Headings);
  yr=y1-0.4*size*Fcos(Right_Tip_Headings);
  xl=x1+0.4*size*Fsin(Left_Tip_Headings);
  yl=y1-0.4*size*Fcos(Left_Tip_Headings);
  moveto(x1,y1);
  lineto(xl,yl);
  lineto(x2,y2);
  lineto(xr,yr);
  lineto(x1,y1);
  setcolor(svcolor); /* restore previous color */
  return(0);

}


int task_elements(int level_indicator)
{
  {
  unsigned elapsed_time;
  unsigned long loop_start_time;


  Open_Graphics();
  Set_Bonus_Message();
  Set_Bonus_Chars();
  Set_Graphics_Eraser();
  Close_Graphics();
  randomize();
  delay(0);
  clrscr();
/* Init_Session(); */
  Game_Counter=0;
  do
  {
  Init_Game();
  Open_Graphics();
  Initialize_Graphics();
  Reset_Screen();
  Loop_Counter=0;
  Set_Kbd_Rate(0x8);
  Capture_Kbd(Get_Key);
  Time_Counter=0;
  Capture_Tik(Get_Tik);
  Set_Timer();

  do
    {
     loop_start_time=Time_Counter;
     Loop_Counter++;
     Get_User_Input();
      while(Freeze_Flag) Get_User_Input();
     Move_Ship();
     Handle_Missile();
     if(Sound_Flag>1) Sound_Flag--;
     if(Sound_Flag==1) {Sound_Flag--; nosound();}
     if (level_indicator > 1) Handle_Mine();
     Test_Collisions(level_indicator);
     Handle_Shell();
     Handle_Fortress();
     if(Display_Interval_Flag)
       {
	 if(Mine_Type==FOE) Find_Interval();
	 Display_Interval_Flag=OFF;
       }
     Accumulate_Data();
     if (level_indicator > 2) Handle_Bonus();
     if(!Effect_Flag)
       {
	 if ( (elapsed_time=Time_Counter-loop_start_time) < 50)
	    Mydelay(50-elapsed_time);
       }
     else Effect_Flag=OFF;
    } while((!End_Flag)&&(Loop_Counter < One_Game_Loops));


  Restore_Tik();
  Reset_Timer();
  Restore_Kbd();
  Set_Kbd_Rate(0x4);

  nosound();
  sound(400);
  delay(500);
  nosound();
  Game_Counter++;
  Close_Graphics();
  /* Save_Game_Results(Training_Session); */
  getch();getch();
  clrscr();
   } while((Game_Counter< No_Of_Games)&&(!End_Flag));

  nosound();
  sound(400);
  delay(1000);
  nosound();
  return(0);
 }
}


void Get_User_Input()
{
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

Check_Bonus_Input()
{
  if((Bonus_Display_Flag==NOT_PRESENT)||
     (Bonus_Display_Flag==NON_BONUS)); /* useless button press */
  else
  if(Bonus_Display_Flag==FIRST_BONUS) Bonus_Wasted_Flag=ON;
  else
  if(Bonus_Display_Flag==SECOND_BONUS)
    if(!Bonus_Wasted_Flag)
      {
	if(Key==F1)
	   {
	     No_Of_Points_Bonus_Taken++;
	     Points=Points+100;
	     Update_Points();
	   }
	else /* only one other alternative */
	  {
	    No_Of_Missiles_Bonus_Taken++;
	    Missile_Stock=Missile_Stock+50;
	    if(Missile_Stock>=100) Missile_Stock=100;
	    Update_Shots();
	  }
	Bonus_Display_Flag=NOT_PRESENT;
	Bonus_Granted=ON;
	Xor_Bonus_Char(rn);    /* erase present $ char */
	Write_Bonus_Message(); /*  Announce_Bonus  */
       }
return(0);
}

int Generate_Non_Bonus_Char()
{
   int rn;

   do { rn=random(10); }
   while(rn==Bonus_Indication_Index);
   return(rn);
}

Generate_Resource_Character()
{
  int lastrn;
  static bonus_character lastchar=NON_BONUS;

  if((lastchar==NON_BONUS)&&
     (No_Of_Bonus_Windows<MAX_BONUS_WINDOWS))

       if(random(10)<7) /* display first bonus */
	 {
	   No_Of_Bonus_Windows++;
	   rn=Bonus_Indication_Index;
	   Xor_Bonus_Char(rn);
	   lastchar=Bonus_Display_Flag=FIRST_BONUS;
	   Bonus_Wasted_Flag=OFF;
	 }
       else /* display non_bonus character */
	 {
	   lastrn=rn;
	   do { rn=Generate_Non_Bonus_Char(); }
	   while(rn==lastrn); /* new char is different from last one */
	   Xor_Bonus_Char(rn);
	   lastchar=Bonus_Display_Flag=NON_BONUS;
	 }
  else
  if(lastchar==FIRST_BONUS)
    {
      Xor_Bonus_Char(rn);
      lastchar=Bonus_Display_Flag=SECOND_BONUS;
    }
  else
  if(lastchar==SECOND_BONUS)
    {
       rn=Generate_Non_Bonus_Char();
       Xor_Bonus_Char(rn);
       lastchar=Bonus_Display_Flag=NON_BONUS;
    }
return(0);
}
