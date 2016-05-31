/* test collision and generate effects   */

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

extern Get_Ship_Input();
extern Update_Ship_Dynamics();
extern Update_Ship_Display();
extern Move_Ship();
extern Fire_Missile();
extern Handle_Missile();
extern Generate_Mine();
extern Move_Mine();
extern Handle_Mine();
extern Fire_Shell();
extern Handle_Shell();
extern Handle_Fortress();
extern Update_Vulner();
extern Update_Velocity();
extern Update_Control();
extern Update_Points();
extern Update_Score();
extern Update_Mines();
extern Reset_Screen();
extern Mydelay(unsigned Timedelay);
extern int give_feedback(int points, int slice);

extern int Wrap_Around_Flag;

char *Small_Expl_Buffer;
int Data_Update_Counter=20;
int Last_Center_Dist;


/* int Last_Missile_Hit=0;  to measure interval between two consecutive
			    hits of the fortress */
/*int Ship_Killings_Counter=0; */


Gen_Explosion(int X_Pos,int Y_Pos,int Radius)
{
  int i,j;
  int iarc;
  int X_dot,Y_dot;
  int svcolor;
  int Last_Pitch;

  Effect_Flag=ON;
  svcolor=getcolor();
  j=0;
  for(i=10;i<Radius;i=i+10)
     {
       setcolor(LIGHTRED);
       sound(200+10*i);
       for(iarc=i/5;iarc<360+i/5;iarc=iarc+20)
	  {
	    arc(X_Pos,Y_Pos,iarc,iarc+2,i);
	   }
	Mydelay(250/i);  /* 100/i*5 */
	sound(200+15*i);
	setcolor(YELLOW);
	if (j>0)
	 for(iarc=j/5;iarc<360+j/5;iarc=iarc+20)
	   {
	     arc(X_Pos,Y_Pos,iarc,iarc+2,j);
	   }
       j=i;  /* erase in de_fasage */
       }

  Last_Pitch=200+10*i;
  for (i=0;i<150;i++)  /* final audio effect */
      {
	Mydelay(5);
	Last_Pitch=Last_Pitch+80;
	sound(Last_Pitch);
      }
  setcolor(svcolor);
  nosound();
  return(0);
}

Gen_Snap_Effect()
{
  int Last_Pitch,i;

  Effect_Flag=ON;
  Last_Pitch=3000;
  for (i=0;i<30;i++)  /* final audio effect */
      {
	Mydelay(5);
	Last_Pitch=Last_Pitch-100;
	sound(Last_Pitch);
      }
  Mydelay(100);
  nosound();
  return(0);
}

Zero_Vulner_Sound()
{
  sound(600);
  Sound_Flag=4;
  return(0);
}

Jitter_Ship()
{
  int Jitter_Headings;
  int Jitter_X_Pos,Jitter_Y_Pos;
  int i;

  Effect_Flag=ON;
  Draw_Ship(Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX); /*erase ship */
  for (i=8;i>0;i--)
  {
    Jitter_Headings=Ship_Headings+2*i;
    Jitter_X_Pos=Ship_X_Pos+i*Fcos(Jitter_Headings);
    Jitter_Y_Pos=Ship_Y_Pos+i*Fsin(Jitter_Headings);
    Draw_Ship(Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings,
				   SHIP_SIZE_FACTOR*MaxX);  /* draw ship */
    Mydelay(i*5);
    Draw_Ship(Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings,
				SHIP_SIZE_FACTOR*MaxX);  /* erase ship */
    Jitter_Headings=Ship_Headings-2*i;
    Jitter_X_Pos=Ship_X_Pos+i*Fsin(Jitter_Headings);
    Jitter_Y_Pos=Ship_Y_Pos+i*Fcos(Jitter_Headings);
    Draw_Ship(Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings,
				SHIP_SIZE_FACTOR*MaxX);  /* draw ship */
    Mydelay(i*5);
    Draw_Ship(Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings,
				SHIP_SIZE_FACTOR*MaxX);  /* erase ship */
  }
 Draw_Ship(Ship_X_Pos,Ship_Y_Pos,Ship_Headings,
	   SHIP_SIZE_FACTOR*MaxX); /* restore ship to its previous position */
return(0);
}

void Reset_All_Missiles()
{
  int i;

  for (i=1;i<6;i++)
      if(Missile_Flag[i]==ALIVE)  Missile_Flag[i]=KILL;
  Handle_Missile();
}

int Check_Collision(int First_X,int First_Y,int Second_X,
		    int Second_Y,int Crash_Distance)
{
  int dist;

  dist =abs(sqrt(pow(First_X-Second_X,2)+
		 pow(First_Y-Second_Y,2)  ));
  if(dist<Crash_Distance) return(1);
		     else
		     return(0);
}

Test_Collisions()
{
  int breakflag;
  int i;
  int Handle_Missile_Flag;
  int goodshot;

  Handle_Missile_Flag=OFF;
  breakflag=OFF;

  if(Game_Type==AIMING_TEST)
    {
      for(i=1;i<6;i++)      /* for all  possible missiles */
       {                    /* check against mine only */
	 if(Mine_Flag==ALIVE)
	   if(Missile_Flag[i]==ALIVE)
	     if(Check_Collision(Missile_X_Pos[i],Missile_Y_Pos[i],
			    Mine_X_Pos,Mine_Y_Pos,COLLISION_DIST) )
	       {
		  Missile_Flag[i]=KILL;
		  Handle_Missile_Flag=ON;
		  Gen_Snap_Effect();
		  Mine_Flag=KILL;
		  Handle_Mine();
		  Mines=Mines+20;
		  Update_Mines();
	       } /* end missile vs. mine for aiming test */
       }

    }
    else  /* space_fortress  section */
    {
	/******* mine vs. ship collision ***********/

  if((Mine_Flag==ALIVE)&&(train.level_ind > 1))
    if(Check_Collision(Ship_X_Pos,Ship_Y_Pos,Mine_X_Pos,Mine_Y_Pos,
					       COLLISION_DIST) )
      {
	Ship_Killings_Counter++;
	Ship_Damaged_By_Mines++;
	breakflag=ON; 	/* no use to check others */
	if(Ship_Killings_Counter>=4)
	   {
	     Points -= 100;
	     Mine_Handle_Points -= 100;
	     Gen_Explosion(Ship_X_Pos,Ship_Y_Pos,80);
	     Ship_Killings_Counter=0;
	     Reset_Screen();
	   }
	 else
	   {
	     Points -= 50;
	     Mine_Handle_Points -= 50;
	     if (train.level_ind == 2) give_feedback(Points, 1);
	     Mine_Flag=KILL;
	     Handle_Mine();       	/* kill mine */
	     if(Shell_Flag==ALIVE)
	       {
		  Shell_Flag=KILL;      /* kill shell */
		  Handle_Shell();
	       }
	     Reset_All_Missiles();    	/* kill all missiles */
	     Gen_Snap_Effect();
	     Jitter_Ship();		/* leaves ship on screen */
	   }
      }  /* end ship vs. mine collision */


		/******** shell vs. ship collision *********/

  if((Shell_Flag==ALIVE) && (!breakflag) && (train.level_ind > 0) )
	 if(Check_Collision(Ship_X_Pos,Ship_Y_Pos,
			    Shell_X_Pos,Shell_Y_Pos,COLLISION_DIST) )
	   {
	     Ship_Killings_Counter++;
	     Ship_Damaged_By_Fortress++;
	     breakflag=ON;
	     if(Ship_Killings_Counter>=4)
	       {
		 Points=Points-100;
		 Sh_Ctrl_Points -= 100;
		 Gen_Explosion(Ship_X_Pos,Ship_Y_Pos,80);
		 Ship_Killings_Counter=0;
		 Reset_Screen();
	       }
	     else
	       {
		 Points=Points-50;
		 Sh_Ctrl_Points -= 50;
		 if (train.level_ind < 3) give_feedback(Points, 1);
		 Shell_Flag=KILL;        /* kill shell */
		 Handle_Shell();
		 if(Mine_Flag==ALIVE)    /* kill  mine  */
		   {
		     Mine_Flag=KILL;
		     Handle_Mine(); /* erase mine and reset counters */
		   }
		 Reset_All_Missiles();
		 Gen_Snap_Effect();
		 Jitter_Ship();     	/* leaves ship on screen */
	       }
	   }

  if(!breakflag)
  for(i=1;i<6;i++)   /* for all  possible missiles */
  {                  /* check against mine and fortress */
    if(Mine_Flag==ALIVE)
      if(Missile_Flag[i]==ALIVE)

		/***** check missile vs. mine ********/

	 if((Check_Collision(Missile_X_Pos[i],Missile_Y_Pos[i],
			    Mine_X_Pos,Mine_Y_Pos,COLLISION_DIST) &&
			    (train.level_ind > 1)) )

	   {
	     Missile_Flag[i]=KILL;
	     Handle_Missile_Flag=ON;
	     goodshot=OFF;
	     if((Missile_Type==VS_FRIEND)&&(Mine_Type==FRIEND))
	       {
		 goodshot=ON;
		 Points=Points+20;
		 Mine_Handle_Points += 20;
		 Mines_Destroyed++;
		 Shot_Hits++;
		 if (train.level_ind == 2)
		      give_feedback(Points, 1);
		 Vulner_Counter++;
		 Update_Vulner();
	       }
	     else
	     if((Missile_Type==VS_FOE)&&(Mine_Type==FOE))
	       {
		 goodshot=ON;
		 Points=Points+30;
		 Mine_Handle_Points += 30;
		 Mines_Destroyed++;
		 Shot_Hits++;
		 if (train.level_ind == 2)
		     give_feedback(Points, 1);
	       }
	     if(goodshot)
	       {
		 goodshot=OFF; /* redundant */
		 Gen_Snap_Effect();
		 Mine_Flag=KILL;
		 Handle_Mine();
	       }
	  } /* end missile vs. mine */

		/******** misile vs. fortress *********/

  if((!Missile_Vs_Mine_Only)&&(train.level_ind > 0))
    if(Missile_Flag[i]==ALIVE)
	 if(Check_Collision(Missile_X_Pos[i],Missile_Y_Pos[i],
			    MaxX/2,MaxY/2,COLLISION_DIST) )
	 {
	   Missile_Flag[i]=KILL;
	   Handle_Missile_Flag=ON;
	   if(Missile_Type==VS_FRIEND)
	    if(Vulner_Counter>=11) /* fortress destruction */
	     if(Loop_Counter-Last_Missile_Hit<6)  /* 6 loops ...*/
	      {
		Fortress_Destroyed++;
		Points=Points+104; /* including the last missile */
		Sh_Ctrl_Points += 104;
		Shot_Hits += 2;
		train.fort_destruction_times[train.trial] = Loop_Counter - Last_Fort;
		train.trial++;
		Last_Fort = Loop_Counter;
		Vulner_Counter=0;
		  if(Bonus_Granted)
		     {
			Write_Bonus_Message();     /* erase bonus message */
			Bonus_Granted=OFF;
		      }
		Gen_Explosion(Missile_X_Pos[i],Missile_Y_Pos[i],120);
		Reset_Screen();
		Handle_Missile_Flag=OFF;
		Last_Missile_Hit=Loop_Counter;
		break;  /* no more missiles checks */
	      }
	     else /*  >=6 you're too slow my friend.. */
	      {
		Points=Points+4; /* is this correct */
		Sh_Ctrl_Points += 4;
		Vulner_Counter++;
		Shot_Hits++;
		Update_Vulner();
		if (train.level_ind < 3)
			 give_feedback(Points, 1);
		Last_Missile_Hit=Loop_Counter;
	      }
	    else /* Vulner_Counter<11 */
	     if(Loop_Counter-Last_Missile_Hit>=6)
	     {
	       Vulner_Counter++;
	       Update_Vulner();
	       Points=Points+4;
	       Sh_Ctrl_Points += 4;
	       Shot_Hits++;
	       if (train.level_ind < 3)
		      give_feedback(Points, 1);
	       Last_Missile_Hit=Loop_Counter;
	     }
	     else /* double strike before it's OK */
	     {
	       Vulner_Counter=0; /* for speeeding, ha ha ha .... */
	       Shot_Hits+=2;
	       Update_Vulner();
	       Last_Missile_Hit=Loop_Counter;
	       Zero_Vulner_Sound();
	     }
	 } /* missile vs. fortress */
  } /* end for missile do-loop */
  } /* end else space fortress case */
  if(Handle_Missile_Flag) Handle_Missile(); /* KILL them all */
return(0);
}


Accumulate_Data()
{

  if(--Data_Update_Counter<=0)
   {
     Data_Update_Counter=20;
     Update_Points();
		/* if Wrap_Around  */

     if(Wrap_Around_Flag)
       {
	 No_Of_Wraps++;
	 Wrap_Around_Flag=OFF;
       }

   }
     return(0); // de rest heb ik niet nodig, maar kan wel nuttig zijn
}

	/* update Velocity */

/*   shipvel=sqrt(pow(Ship_X_Speed,2)+pow(Ship_Y_Speed,2));
     if(shipvel<SHIP_GOOD_VELOCITY)
       {
	 Velocity=Velocity+7;
	 Update_Velocity();
       }
*/

	/* update Control */
/*
     shipcenterdist=sqrt(pow(Ship_X_Pos-MaxX/2,2)+
			 pow(Ship_Y_Pos-MaxY/2,2));

     if((shipcenterdist<SMALL_HEXAGONE_SIZE_FACTOR*MaxX)&&
	(Last_Center_Dist>SMALL_HEXAGONE_SIZE_FACTOR*MaxX))
	 Control=Control-5;
     else
     if(shipcenterdist<BIG_HEXAGONE_SIZE_FACTOR*MaxX) Control=Control+7;
     else
				     Control=Control+3;
     Last_Center_Dist=shipcenterdist;
*/

