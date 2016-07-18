/* ******************   Handle_Mine and all others elements   ***************** */
#include <stdio.h>
#include <stdlib.h>
//#include <graphics.h>
//#include <process.h>
//#include <bios.h>
//#include <alloc.h>
//#include <dos.h>
#include <time.h>
#include <math.h>
//#include <cairo.h>

//#include "myconst.h"
//#include "myext.h"
//#include "myvars.h"

// Added
#include "DE.h"
#include "HM.h"
#include "RS.h"

mine_type Mine_Type;
int Mine_Alive_Counter=0;
int Mine_Dead_Counter=0;
int Missile_Delay_Counter=0;

char Char_Set[10][1]={"Y","M","P","B","Q","K","C","W","R","Z"};
char Tmp_Char_Set[10][1];

char Foe_Menu[3][1];
char Friend_Menu[3][1];
char Mine_Indicator;


//int Get_Random_Index(int vec[])
//{
//  int k;
//
//  do
//     { k=random(10); }
//  while (vec[k]==-1);
//
//  vec[k]=-1;
//  return(k);
//}

// Not needed ? 
//void Select_Mine_Menus()
//{
//  int vec[10];
//  int i,ri;
//
//  for(i=0;i<10;i++) vec[i]=0;
//  for (i=0;i<3;i++)
//      {
//	ri=Get_Random_Index(vec);
//	Friend_Menu[i][0]= Char_Set[ri][0];
//	ri=Get_Random_Index(vec);
//	Foe_Menu[i][0]= Char_Set[ri][0];
//      }
//}

void Update_Ship_Dynamics()
{
  Ship_Old_Headings=Ship_Headings;
  Ship_X_Old_Pos=Ship_X_Pos;
  Ship_Y_Old_Pos=Ship_Y_Pos;
  Ship_Display_Update=0; /* do not refresh if no movement */

  if (Rotate_Input!=0)      /* if ship rotates */
     {
       Ship_Display_Update=1;  /* at least rotates */
       Ship_Headings=Ship_Headings + Rotate_Input*Ship_Angular_Step;
       if (Ship_Headings<0) Ship_Headings= 359+Ship_Headings-1;
       if (Ship_Headings>359) Ship_Headings= Ship_Headings-359-1;
       Rotate_Input=0;        /* reset input */
     }
  if(Game_Type==SPACE_FORTRESS)
  {
  if (Accel_Input!=0)
     {
       Ship_X_Speed=Ship_X_Speed+0.65*Ship_Accel*Fsin(Ship_Headings);
       Ship_Y_Speed=Ship_Y_Speed-0.65*Ship_Accel*Fcos(Ship_Headings);
       Accel_Input=0; 	/* reset input */

       /* assure it does not exceed MAXspeed */

       if(fabs(Ship_X_Speed)>Ship_Max_Speed)
		 if(Ship_X_Speed<0) Ship_X_Speed=-Ship_Max_Speed;
				    else
				    Ship_X_Speed=Ship_Max_Speed;
       if(fabs(Ship_Y_Speed)>Ship_Max_Speed)
		 if(Ship_Y_Speed<0) Ship_Y_Speed=-Ship_Max_Speed;
				    else
				    Ship_Y_Speed=Ship_Max_Speed;

     }  /* end accel_input */
		/* now update ship position */

    if ((Ship_X_Speed!=0.0)||(Ship_Y_Speed!=0.0))
     {
       Ship_Display_Update=1; /* ship moves */

       Ship_X_Pos=Ship_X_Pos+Ship_X_Speed;
       Ship_Y_Pos=Ship_Y_Pos+Ship_Y_Speed;
			/* check if crossed screen boundary */
       if(Ship_X_Pos<0) { Ship_X_Pos=MaxX;
			  Wrap_Around_Flag=ON; }
       if(Ship_X_Pos>MaxX) { Ship_X_Pos=0;
				     Wrap_Around_Flag=ON; }
       if(Ship_Y_Pos<0) { Ship_Y_Pos=MaxY;
			  Wrap_Around_Flag=ON; }
       if(Ship_Y_Pos>MaxY) { Ship_Y_Pos=0;
				     Wrap_Around_Flag=ON; }
			/* check if bumped into the fortress */
       if(sqrt(pow(Ship_X_Pos-MaxX/2,2)+
	       pow(Ship_Y_Pos-MaxY/2,2) ) < (COLLISION_DIST) )
	 {
	   Ship_X_Speed=-Ship_X_Speed;		/* reverse direction */
	   Ship_Y_Speed=-Ship_Y_Speed;
	   Ship_X_Pos=Ship_X_Pos+Ship_X_Speed; /* move ship out of range */
	   Ship_Y_Pos=Ship_Y_Pos+Ship_Y_Speed;
	 }
     } /* end ship is moving */
  } /* end SPACE_FORTRESS */
}

void Update_Ship_Display(cairo_t *cr)
{
//		/* erase ship in old location */
//	clear_prev_path(cr, PrevShip);
	Ship_Should_Clean = 1; 	
//  Draw_Ship(cr,Ship_X_Old_Pos,Ship_Y_Old_Pos,Ship_Old_Headings,SHIP_SIZE_FACTOR
//							     *MaxX);
		/* draw ship in new location */

//  Draw_Ship(cr,Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX);
//	stroke_in_clip(cr);
	Ship_Should_Update = 1;
	
}

void Move_Ship(cairo_t *cr)
{
  Update_Ship_Dynamics();
  if(Ship_Display_Update)
	{
		Update_Ship_Display(cr);
	}
	else
	{
		Ship_Should_Clean = 0;
		Ship_Should_Update = 0;
	}
}

void Fire_Shell(cairo_t *cr)
{
  Shell_X_Pos=MaxX/2+0.5*SMALL_HEXAGONE_SIZE_FACTOR*MaxX*Fsin(Fort_Headings);
  Shell_Y_Pos=MaxY/2-0.5*SMALL_HEXAGONE_SIZE_FACTOR*Fcos(Fort_Headings);
  Shell_Headings=Fort_Headings;
  Shell_X_Speed=SHELL_SPEED*Fsin(Shell_Headings);
  Shell_Y_Speed=-SHELL_SPEED*Fcos(Shell_Headings);
//  Draw_Shell(cr, Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
//				SHELL_SIZE_FACTOR*MaxX);  /* first time */ // First time??
//	stroke_in_clip(cr);
	Ship_Should_Update = 1; // first time apperantly 
//  sound(800);
//  Sound_Flag=6;
}

void Handle_Fortress(cairo_t *cr)
{
//  int dif,nh;
	int nh;

  if( (++Fort_Lock_Counter>FORT_LOCK_INTERVAL)&&(Shell_Flag==DEAD) )
    {
      Fire_Shell(cr);
      Shell_Flag=ALIVE;
      Fort_Lock_Counter=0;
    }

  nh=Find_Headings(MaxX/2,MaxY/2,Ship_X_Pos,Ship_Y_Pos);
  if (abs(Fort_Headings-nh)>10)
  {
//		clear_prev_path(cr, PrevShip);
		//       Draw_Fort(cr, MaxX/2,MaxY/2,Fort_Headings,FORT_SIZE_FACTOR*MaxX);
		Fort_Should_Clean = 1;
		//						/* erase old position */
		Fort_Headings=nh;
//		Draw_Fort(cr, MaxX/2,MaxY/2,Fort_Headings,FORT_SIZE_FACTOR*MaxX);
//		stroke_in_clip(cr);
		Fort_Should_Update = 1;
		/* draw new position */
		Fort_Lock_Counter=0;  /* reset firing counter */
  }
}

void Handle_Speed_Score(cairo_t *cr)
{
  int interval;
  int dts;

  if(Game_Type==SPACE_FORTRESS)
  {
  dts=0;
  if(Mine_Type==FOE)
    if(Missile_Type==VS_FOE) /* successful double press */
      {
	interval=((t0-t2)/CLOCKS_PER_SEC)*1000; /* time interval from missile */
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
  Update_Speed(cr);
  if(Game_Type==AIMING_TEST)
    {
      Score=Mines+Speed;
      Update_Score(cr);
    }
}

// It seems that this function might draw over the place where a dead mine has been found to
// erase it. 
void Clear_Mine_Type(cairo_t *cr)
{
  int x,y;

//  setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);
	cairo_translate(cr, 0, Panel_Y_Start);
  x=IFF_X; y=Data_Line;
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_rectangle(cr, x, y, TEXT_WIDTH, TEXT_HEIGHT); // 
	cairo_clip_preserve(cr);
	cairo_fill(cr);
	cairo_reset_clip(cr);
	cairo_translate(cr, 0, -Panel_Y_Start);
//  putimage(x,y,buffer1,COPY_PUT); /* erase garbage */ // Function to draw over  the mine?
//  setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);
}

void Show_Mine_Type(cairo_t *cr, char Minetype)
{
//  int svcolor;
  int x,y;

//  svcolor=getcolor();
  if((Mine_Type==FRIEND && Missile_Type==VS_FRIEND) || (Mine_Type==FOE && Missile_Type==VS_FOE)) {
    cairo_set_source_rgb(cr, 0, 1, 0);
  } else if(Missile_Type==WASTED) {
    cairo_set_source_rgb(cr, 1, 0, 0);
  } else {
			cairo_set_source_rgb(cr, 1.0, 102.0/255.0, 102.0/255.0); // Light red
//    setcolor(LIGHTRED);
  }
	// What does this viewport do in context?
	cairo_translate(cr, 0, Panel_Y_Start);
//  setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1); // ?
  x=IFF_X; y=Data_Line;
//  putimage(x,y,buffer1,COPY_PUT); /* erase garbage */
	char Minetype_str[15];
	sprintf(Minetype_str, "%c", Minetype);
	cairo_text_at(cr, x, y, Minetype_str); // Originally was "%c" Minetype
//  gprintf(&x,&y,"%c",Minetype);
	cairo_translate(cr, 0, -Panel_Y_Start);
//  setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);
//  setcolor(svcolor); /* restore previous color */
}

void Reset_Mine_Headings()
{
   Mine_Headings=Find_Headings(Mine_X_Pos,Mine_Y_Pos,Ship_X_Pos,
					     Ship_Y_Pos);
   Mine_Course_Count=MINE_COURSE_INTERVAL;
   Mine_X_Speed=Mine_Speed*Fsin(Mine_Headings);
   Mine_Y_Speed=-Mine_Speed*Fcos(Mine_Headings);
}

int randrange(int min, int max)
{
	return (rand() % (max + 1 - min)) + min;
}

void Generate_Mine(cairo_t *cr)
{
  int a;
  do
  {
//    Mine_X_Pos=random(MaxX); // Maybe not available, what does it do? 
//    Mine_Y_Pos=random(MaxY);
		Mine_X_Pos=randrange(0, MaxX);
		Mine_Y_Pos=randrange(0, MaxY);
    a=sqrt(pow(Mine_X_Pos-Ship_X_Pos,2)+pow(Mine_Y_Pos-Ship_Y_Pos,2) );
  } while(a < 0.5*MaxX );  /* repeat until distance exceeds min. */

  Reset_Mine_Headings();

//  Draw_Mine(cr,Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);  /* draw mine first time */
	Mine_Should_Update = 1;


  if(randrange(0,2)) Mine_Type=FRIEND; // was random(2)
  else
    {
      Mine_Type=FOE;
			// I'm not sure if this line does anything
      t0=clock(); /* when "a mine is born .."? */ 
    }

  if (Mine_Type==FRIEND) Mine_Indicator=Friend_Menu[randrange(0,3)][0];
  else                   Mine_Indicator=Foe_Menu[randrange(0,3)][0];
  Show_Mine_Type(cr, Mine_Indicator);
}

void Move_Mine(cairo_t *cr)
{
//		stroke_in_clip(cr);
		// Do something with cairo_save() if approiate 
//    Draw_Mine(cr, Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX); /* erase mine */
		Mine_Should_Clean = 1;

    Mine_X_Pos=Mine_X_Pos+Mine_X_Speed;      /* update position */
    Mine_Y_Pos=Mine_Y_Pos+Mine_Y_Speed;

//    Draw_Mine(cr, Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);  /* redraw mine */
		Mine_Should_Update = 1;
//		stroke_in_clip(cr);

    if(--Mine_Course_Count<=0)  Reset_Mine_Headings();
    if(   (Mine_X_Pos<0) || (Mine_X_Pos>MaxX)
     || (Mine_Y_Pos<0) || (Mine_Y_Pos>MaxY) )
      Reset_Mine_Headings();
}

void Handle_Mine(cairo_t *cr)
{
 switch(Mine_Flag)
 {
  case KILL  : {
		  Handle_Speed_Score(cr);
//		  Draw_Mine(cr, Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);
//			clear_prev_path(cr, PrevMine);
				Mine_Should_Clean = 1;
							/* erase mine */
		  Mine_Flag=DEAD;
		  Mine_Dead_Counter=0;
		  Missile_Type=VS_FRIEND;
		  Missile_Vs_Mine_Only=OFF;
		  Timing_Flag=OFF;
		  Clear_Mine_Type(cr); /* clear mine type display */
//		  Clear_Interval(); // Double press checking  
		  break;
		}
  case DEAD   : {
		  if(Mine_Dead_Counter++ >= Mine_Wait_Loops)
		    {
		      Generate_Mine(cr);
		      Mine_Flag=ALIVE;
		      Mine_Alive_Counter=0;
		    }
		   break;
		}
  case ALIVE  : {
		  Move_Mine(cr);
		  if(Mine_Alive_Counter++ >= Mine_Live_Loops)
		  Mine_Flag=KILL;
		  if(Mine_Alive_Counter>MISSILE_FORT_TIME_LIMIT)
		    Missile_Vs_Mine_Only=ON;
		 }

 } /* end switch */
}


void Generate_Aim_Mine(cairo_t *cr)
{
    float radius;
    float mine_distance;
    float mine_angle;

    radius=MaxX/2.2;
    mine_angle=randrange(0,16)*22.5;
    mine_distance=radius/2+randrange(0,2)*radius/2;
    if(mine_angle>338.0) 
		{
			mine_angle=0.0;
    	Mine_X_Pos=MaxX/2 + mine_distance*Fsin(mine_angle);
      Mine_Y_Pos=MaxY/2 - mine_distance*Fcos(mine_angle);
		}
    else 
		{
			Mine_Y_Pos=MaxY/2 - mine_distance*Fcos(mine_angle)/GraphSqrFact;
		}
		     /* Y/X square ratio */
		Mine_Should_Update = 1;
//    Draw_Mine(cr, Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);
//	     /* draw mine */
//		stroke_in_clip(cr);
}

void Handle_Aim_Mine(cairo_t *cr)
{
 switch(Mine_Flag)
 {
  case KILL  : {
		  Handle_Speed_Score(cr);
//		  Draw_Mine(cr, Mine_X_Pos,Mine_Y_Pos,MINE_SIZE_FACTOR*MaxX);
//							/* erase mine */
//			clear_prev_path(cr, PrevMine);
			Mine_Should_Clean = 1;
		  Mine_Flag=DEAD;
		  Mine_Dead_Counter=0;
		  break;
		}
  case DEAD   : {
		  if(Mine_Dead_Counter++ >= Mine_Wait_Loops)
		    {
		      Generate_Aim_Mine(cr);
		      Mine_Flag=ALIVE;
		      Mine_Alive_Counter=0;
		    }
		   break;
		}
  case ALIVE  : {
		  if(Mine_Alive_Counter++ >= Mine_Live_Loops)
		  Mine_Flag=KILL;
		}

 } /* end switch */
}


void Handle_Shell(cairo_t *cr)
{
 switch(Shell_Flag)
 {
  case KILL  : {
//		  Draw_Shell(cr, Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
//			    SHELL_SIZE_FACTOR*MaxX); /* erase shell */
//			clear_prev_path(cr, PrevMine);
			Shell_Should_Update = 1;
		  Shell_Flag=DEAD;
		  break;
		}

  case ALIVE  : {
//		  Draw_Shell(cr, Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
//				       SHELL_SIZE_FACTOR*MaxX);
//			clear_prev_path(cr, PrevMine);
			Shell_Should_Clean = 1;
		  Shell_X_Pos=Shell_X_Pos+Shell_X_Speed;
		  Shell_Y_Pos=Shell_Y_Pos+Shell_Y_Speed;
		  if( (Shell_X_Pos<0) || (Shell_X_Pos>MaxX)
		      || (Shell_Y_Pos<0) || (Shell_Y_Pos>MaxY) )
		    Shell_Flag=KILL;  /* kill shell */
		  else
//		    Draw_Shell(cr, Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
//					SHELL_SIZE_FACTOR*MaxX);
//				stroke_in_clip(cr);
				Shell_Should_Update = 1;
		}
 } /* end switch */
}


void Fire_Missile(cairo_t *cr, int Index)
{
	Missile_Headings[Index]=Ship_Headings;
	Missile_X_Pos[Index]=Ship_X_Pos;
	Missile_Y_Pos[Index]=Ship_Y_Pos;
	Missile_X_Speed[Index]= Missile_Speed*Fsin(Ship_Headings);
	Missile_Y_Speed[Index]=-Missile_Speed*Fcos(Ship_Headings);
	Missile_Should_Update[Index] = 1;
// Draw_Missile(cr,Missile_X_Pos[Index],Missile_Y_Pos[Index],
//	      Missile_Headings[Index],MISSILE_SIZE_FACTOR*MaxX);
//	stroke_in_clip(cr);

							/* first time */
// sound(1000);
// Sound_Flag=4;
}

// Use PrevMine etc. in these kinds of functions (the moves/handles and alike)
void Handle_Missile(cairo_t *cr)
{
 int i;
		/* update all existing missiles */
 for(i=0;i<MAX_NO_OF_MISSILES;i++)
    if(Missile_Flag[i] != DEAD)
      switch(Missile_Flag[i])
      {
	 case KILL  : {
//			clear_prev_path(cr,PrevMissile);
			Missile_Should_Clean[i] = 1;
//			Draw_Missile(cr, Missile_X_Pos[i],Missile_Y_Pos[i],
//			     Missile_Headings[i],MISSILE_SIZE_FACTOR*MaxX);  /* erase missile */
			Missile_Flag[i]=DEAD;
			Missiles_Counter--;
			break;
		      }

	 case ALIVE  : {
			 if((Missile_X_Pos[i]<0) || (Missile_X_Pos[i]>MaxX)
			 || (Missile_Y_Pos[i]<0) || (Missile_Y_Pos[i]>MaxY))
			    Missile_Flag[i]=KILL;
			 else
			  {
//			    Draw_Missile(cr, Missile_X_Pos[i],Missile_Y_Pos[i],
//			      Missile_Headings[i],MISSILE_SIZE_FACTOR*MaxX);
//							/* erase old */
//					clear_prev_path(cr,PrevMissile);
					Missile_Should_Clean[i] = 1;
			    Missile_X_Pos[i]=Missile_X_Pos[i]+Missile_X_Speed[i];
			    Missile_Y_Pos[i]=Missile_Y_Pos[i]+Missile_Y_Speed[i];
//			    Draw_Missile(cr, Missile_X_Pos[i],Missile_Y_Pos[i],
//			     Missile_Headings[i],MISSILE_SIZE_FACTOR*MaxX);
					Missile_Should_Update[i] = 1;
							  /* draw new */
					stroke_in_clip(cr);

			   }
			 }
      } /* end switch */

		/******** handle new missile **************/

 if(New_Missile_Flag && (Missiles_Counter<5))
  do {
      New_Missile_Flag=OFF;

      if(Game_Type==SPACE_FORTRESS)
	if(Missile_Stock<=0)    /* stock control */
	  if(Missile_Limit_Flag) break;  /* overdraft not allowed */
	  else           Points=Points-3;   /* our low-low interest rates.. */

      Missiles_Counter++;
      for(i=0;i<MAX_NO_OF_MISSILES;i++)
	 			if(Missile_Flag[i]==DEAD) break; /* from for-loop */
      	Missile_Flag[i]=ALIVE;
      	Fire_Missile(cr,i);
      	if(Game_Type==SPACE_FORTRESS)
				{
	 	 			Missile_Stock--;
	  			Update_Shots(cr);
				}
   } while(OFF); /* to enable the break command */
}


//
//int main()
//{
//	printf("Yo man! \n");
//}
//































