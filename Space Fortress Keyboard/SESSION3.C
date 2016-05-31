#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <process.h>
#include <bios.h>
#include <alloc.h>
#include <dos.h>
#include <time.h>
#include <math.h>

#include "myconst.h"
#include "myext.h"

extern int Find_Headings(int x1,int y1,int x2,int y2);

int Move_Ship();
int Update_Ship_Display();
int Update_Ship_Dynamics();
int Handle_Missile();
int Fire_Missile(int Index);
int Handle_Fortress();
int Handle_Shell();
int Fire_Shell();


int Missile_Delay_Counter=0;
int Wrap_Around_Flag = OFF;


int Update_Ship_Dynamics()
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
  return(0);
}

int Update_Ship_Display()
{
		/* erase ship in old location */
  Draw_Ship(Ship_X_Old_Pos,Ship_Y_Old_Pos,Ship_Old_Headings,SHIP_SIZE_FACTOR
							     *MaxX);
		/* draw ship in new location */
  Draw_Ship(Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX);
  return(0);
}


int Move_Ship()
{
  Update_Ship_Dynamics();
  if(Ship_Display_Update)  Update_Ship_Display();
  return(0);
}

int Handle_Missile()
{
 int i;

		/* update all existing missiles */
 for(i=1;i<6;i++)
    if(Missile_Flag[i] != DEAD)
      switch(Missile_Flag[i])
      {
	 case KILL  : {
			Draw_Missile(Missile_X_Pos[i],Missile_Y_Pos[i],
			     Missile_Headings[i],MISSILE_SIZE_FACTOR*MaxX);  /* erase missile */
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
			    Draw_Missile(Missile_X_Pos[i],Missile_Y_Pos[i],
			      Missile_Headings[i],MISSILE_SIZE_FACTOR*MaxX);
							/* erase old */
			    Missile_X_Pos[i]=Missile_X_Pos[i]+Missile_X_Speed[i];
			    Missile_Y_Pos[i]=Missile_Y_Pos[i]+Missile_Y_Speed[i];
			    Draw_Missile(Missile_X_Pos[i],Missile_Y_Pos[i],
			     Missile_Headings[i],MISSILE_SIZE_FACTOR*MaxX);
							  /* draw new */
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
      for(i=1;i<6;i++)
	 if(Missile_Flag[i]==DEAD) break; /* from for-loop */
      Missile_Flag[i]=ALIVE;
      Fire_Missile(i);
      if(Game_Type==SPACE_FORTRESS)
	{
	  Missile_Stock--;
	  Update_Shots();
	}
   } while(OFF); /* to enable the break command */
 return(0);
}

int Fire_Missile(int Index)
{
 Missile_Headings[Index]=Ship_Headings;
 Missile_X_Pos[Index]=Ship_X_Pos;
 Missile_Y_Pos[Index]=Ship_Y_Pos;
 Missile_X_Speed[Index]= Missile_Speed*Fsin(Ship_Headings);
 Missile_Y_Speed[Index]=-Missile_Speed*Fcos(Ship_Headings);
 Draw_Missile(Missile_X_Pos[Index],Missile_Y_Pos[Index],
	      Missile_Headings[Index],MISSILE_SIZE_FACTOR*MaxX);
							/* first time */
 sound(1000);
 Sound_Flag=4;
 return(0);
}

int Handle_Fortress()
{
  int dif,nh;

  if( (++Fort_Lock_Counter>FORT_LOCK_INTERVAL)&&(Shell_Flag==DEAD) )
    {
      Fire_Shell();
      Shell_Flag=ALIVE;
      Fort_Lock_Counter=0;
    }

  nh=Find_Headings(MaxX/2,MaxY/2,Ship_X_Pos,Ship_Y_Pos);
  if (abs(Fort_Headings-nh)>10)
     {
       Draw_Fort(MaxX/2,MaxY/2,Fort_Headings,FORT_SIZE_FACTOR*MaxX);
						/* erase old position */
       Fort_Headings=nh;
       Draw_Fort(MaxX/2,MaxY/2,Fort_Headings,FORT_SIZE_FACTOR*MaxX);
						/* draw new position */
       Fort_Lock_Counter=0;  /* reset firing counter */
     }
  return(0);
}

int Fire_Shell()
{
  Shell_X_Pos=MaxX/2+0.5*SMALL_HEXAGONE_SIZE_FACTOR*MaxX*Fsin(Fort_Headings);
  Shell_Y_Pos=MaxY/2-0.5*SMALL_HEXAGONE_SIZE_FACTOR*Fcos(Fort_Headings);
  Shell_Headings=Fort_Headings;
  Shell_X_Speed=SHELL_SPEED*Fsin(Shell_Headings);
  Shell_Y_Speed=-SHELL_SPEED*Fcos(Shell_Headings);
  Draw_Shell(Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
				SHELL_SIZE_FACTOR*MaxX);  /* first time */
  sound(800);
  Sound_Flag=6;
  return(0);
}

int Handle_Shell()
{
 switch(Shell_Flag)
 {
  case KILL  : {
		  Draw_Shell(Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
			    SHELL_SIZE_FACTOR*MaxX); /* erase shell */
		  Shell_Flag=DEAD;
		  break;
		}

  case ALIVE  : {
		  Draw_Shell(Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
				       SHELL_SIZE_FACTOR*MaxX);
						/* erase shell */
		  Shell_X_Pos=Shell_X_Pos+Shell_X_Speed;
		  Shell_Y_Pos=Shell_Y_Pos+Shell_Y_Speed;
		  if( (Shell_X_Pos<0) || (Shell_X_Pos>MaxX)
		      || (Shell_Y_Pos<0) || (Shell_Y_Pos>MaxY) )
		    Shell_Flag=KILL;  /* kill shell */
		  else
		    Draw_Shell(Shell_X_Pos,Shell_Y_Pos,Shell_Headings,
					SHELL_SIZE_FACTOR*MaxX);
		}
 } /* end switch */
 return(0);
}
