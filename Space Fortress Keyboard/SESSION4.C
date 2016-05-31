#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <process.h>
#include <bios.h>
#include <alloc.h>
#include <dos.h>
#include <time.h>
#include <math.h>

#include "myext.h"

extern char Char_Set[10][1];
extern int Interval_Upper_Limit;
extern int Interval_Lower_Limit;

int Handle_Speed_Score();
int Clear_Mine_Type();
int Show_Mine_Type(char Minetype);
Reset_Mine_Headings();
int Generate_Mine();
int Move_Mine();
int Handle_Mine();
Update_Score();
Update_Mines();
Clear_Interval();
Find_Interval();



int Mine_Alive_Counter=0;
int Mine_Dead_Counter=0;


char Tmp_Char_Set[10][1];

char Foe_Menu[3][1];
char Friend_Menu[3][1];
char Mine_Indicator;
mine_type Mine_Type;


int Handle_Speed_Score()
{
  int interval;
  int dts;

  if(Game_Type==SPACE_FORTRESS)
  {
  dts=0;
  if(Mine_Type==FOE)
    if(Missile_Type==VS_FOE) /* successful double press */
      {
	interval=t0-t2; /* time interval from missile */
			/* appearance to end of successful */
			/* double press interval      */
	if(interval<=1000) dts=40;
	else
	if(interval<=2000) dts=30;
	else
	if(interval<=3000) dts=20;
	else
	if(interval<=4000) dts=10;
	else
	if(interval<=5000) dts=0;
	else
	if(interval<=6000) dts=-10;
	else
	if(interval<=7000) dts=-20;
	else
	if(interval<=8000) dts=-30;
	else
	if(interval<=9000) dts=-40;
	else
	dts=-50;
     }
     else /* no successful double-press */
	dts=-50; /* this is "unfair" */
   Speed=Speed+dts;
  } /* end if(SPACE_FORTRESS) */

	/* mine bonus for any type */

   dts=0;
   if(Mine_Alive_Counter>=Mine_Live_Loops) dts=-100;
   else
   if(Mine_Alive_Counter<=20) dts=80;
   else
   if(Mine_Alive_Counter<=40) dts=60;
   else
   if(Mine_Alive_Counter<=60) dts=40;
   else
   if(Mine_Alive_Counter<=80) dts=20;
   else
   if(Mine_Alive_Counter<=100) dts=0;
   else
   if(Mine_Alive_Counter<=120) dts=-10;
   else
   if(Mine_Alive_Counter<=140) dts=-40;
   else
   if(Mine_Alive_Counter<=160) dts=-60;
   else
   if(Mine_Alive_Counter<=180) dts=-80;
   else
   if(Mine_Alive_Counter<=200) dts=-100;

  Speed=Speed+dts;
  Update_Speed();
  if(Game_Type==AIMING_TEST)
    {
      Score=Mines+Speed;
      Update_Score();
    }
  return(0);
}


int Clear_Mine_Type()
{
  int x,y;

  setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);
  x=IFF_X; y=Data_Line;
  putimage(x,y,buffer1,COPY_PUT); /* erase garbage */
  setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);
  return(0);
}

int Show_Mine_Type(char Minetype)
{
  int svcolor;
  int x,y;

  svcolor=getcolor();
  setcolor(TEXT_COLOR);
  setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);
  x=IFF_X; y=Data_Line;
  putimage(x,y,buffer1,COPY_PUT); /* erase garbage */
  gprintf(&x,&y,"%c",Minetype);
  setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);
  setcolor(svcolor); /* restore previous color */
  return(0);
}

Reset_Mine_Headings()
{
   Mine_Headings=Find_Headings(Mine_X_Pos,Mine_Y_Pos,Ship_X_Pos,
					     Ship_Y_Pos);
   Mine_Course_Count=MINE_COURSE_INTERVAL;
   Mine_X_Speed=Mine_Speed*Fsin(Mine_Headings);
   Mine_Y_Speed=-Mine_Speed*Fcos(Mine_Headings);
   return(0);
}

int Generate_Mine()
{
  int a;
  do
  {
    Mine_X_Pos=random(MaxX);
    Mine_Y_Pos=random(MaxY);
    a=sqrt(pow(Mine_X_Pos-Ship_X_Pos,2)+pow(Mine_Y_Pos-Ship_Y_Pos,2) );
  } while(a < 0.5*MaxX );  /* repeat until distance exceeds min. */

  Reset_Mine_Headings();
  Draw_Mine(Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);  /* draw mine first time */

  if(random(2)) Mine_Type=FRIEND;
  else
    {
      Mine_Type=FOE;
      t0=Time_Counter; /* when "a mine is born .."? */
    }

  if (Mine_Type==FRIEND) Mine_Indicator=Friend_Menu[random(3)][0];
  else                   Mine_Indicator=Foe_Menu[random(3)][0];
  Show_Mine_Type(Mine_Indicator);
  return(0);

}

int Move_Mine()
{
    Draw_Mine(Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX); /* erase mine */

    Mine_X_Pos=Mine_X_Pos+Mine_X_Speed;      /* update position */
    Mine_Y_Pos=Mine_Y_Pos+Mine_Y_Speed;

    Draw_Mine(Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);  /* redraw mine */

    if(--Mine_Course_Count<=0)  Reset_Mine_Headings();
    if(   (Mine_X_Pos<0) || (Mine_X_Pos>MaxX)
     || (Mine_Y_Pos<0) || (Mine_Y_Pos>MaxY) )
      Reset_Mine_Headings();
   return(0);
}

int Handle_Mine()
{
 switch(Mine_Flag)
 {
  case KILL  : {
		  Handle_Speed_Score();
		  Draw_Mine(Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);
							/* erase mine */
		  Mine_Flag=DEAD;
		  Mine_Dead_Counter=0;
		  Missile_Type=VS_FRIEND;
		  Missile_Vs_Mine_Only=OFF;
		  Timing_Flag=OFF;
		  Clear_Mine_Type(); /* clear mine type display */
		  Clear_Interval();
		  break;
		}
  case DEAD   : {
		  if(Mine_Dead_Counter++ >= Mine_Wait_Loops)
		    {
		      Generate_Mine();
		      Mine_Flag=ALIVE;
		      Mine_Alive_Counter=0;
		    }
		   break;
		}
  case ALIVE  : {
		  Move_Mine();
		  if(Mine_Alive_Counter++ >= Mine_Live_Loops)
		  Mine_Flag=KILL;
		  if(Mine_Alive_Counter>MISSILE_FORT_TIME_LIMIT)
		    Missile_Vs_Mine_Only=ON;

		 }

 } /* end switch */
 return(0);

}

Update_Score()
{
  Show_Score(Score,Score_X,Data_Line);
  return(0);
}

Update_Mines()
{
  Show_Score(Mines,Mines_X,Data_Line);
  return(0);
}


Clear_Interval()   /* clear double-press interval */
{
    int svcolor;
    int x,y;

    svcolor=getcolor();
    setcolor(TEXT_COLOR);
    setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);
    x=Interval_X; y=Data_Line;
    putimage(x,y,buffer1,COPY_PUT); /* erase garbage */
    setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);   /* restore gaming area */
    setcolor(svcolor); /* restore previous color */
    return(0);
}

Find_Interval()   /* display double-press interval */
{
  int svcolor;
  int x,y;
  int interval;

  interval=Double_Press_Interval=t2-t1; /* in milliseconds */
  if((interval<990)&&(interval>90)) /* only when interval makes sense */
  {
    if((interval>=Interval_Lower_Limit)&&(interval<=Interval_Upper_Limit)
       &&(Mine_Flag==ALIVE)&&(Mine_Type==FOE))
	Missile_Type=VS_FOE;   /* rearm missile */

    Update_Interval();
  }
  return(0);
}
