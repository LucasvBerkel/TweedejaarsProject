/* test collision and generate effects   */

#include <stdio.h>
#include <stdlib.h>
//#include <graphics.h>
//#include <process.h>
//#include <process.h>
//#include <bios.h>
//#include <alloc.h>
//#include <dos.h>
#include <time.h>
#include <math.h>
#include <cairo.h>
#include <unistd.h>

//#include "DE.h"
#include "HM.h"
#include "RS.h"
#include "TCOL.h"


//#include "myconst.h"
//#include "myext.h"

// Addded
//#include "HM.h"


char *Small_Expl_Buffer;
int Data_Update_Counter=20;
int Last_Center_Dist;
int Wrap_Around_Flag=OFF;

//extern Get_Ship_Input(); // not used or something
/* Uncomment (for HM.c)
extern void Update_Ship_Dynamics();
extern void Update_Ship_Display();
extern void Move_Ship();
extern void Fire_Missile();
extern void Handle_Missile(cairo_t *cr);
extern void Generate_Mine();
extern void Move_Mine();
extern void Handle_Mine(cairo_t *cr);
extern void Fire_Shell();
extern void Handle_Shell(cairo_t *cr);
extern void Handle_Fortress(); */

// ------- Uncomment (RE) ------------ //
//extern Update_Vulner();
//extern Update_Velocity();
//extern Update_Control();
//extern Update_Points();
//extern Reset_Screen();
//extern Mydelay(unsigned Timedelay);
// --------------------------------- //


// Added in header
//char *Small_Expl_Buffer;
//int Data_Update_Counter=20;
//int Last_Center_Dist;
//int Wrap_Around_Flag=OFF;
/* int Last_Missile_Hit=0;  to measure interval between two consecutive
			    hits of the fortress */
/*int Ship_Killings_Counter=0; */


// Okay to drop animation?
void Gen_Explosion(int X_Pos,int Y_Pos,int Radius)
{
//  int i,j;
//  int iarc;
//	g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
//	cairo_reset_clip(cr);
//	cairo_rectangle(cr, 0, 0, 320, 240);
//	cairo_paint(cr);
/* -- unused --
  int X_dot,Y_dot;
  int svcolor;
  int Last_Pitch; */
  Effect_Flag=ON;
	Explosion_Flag=1;
//  svccd olor=getcolor();

	ExpRadius = Radius;
	ExpX = X_Pos;
	ExpY = Y_Pos;

//  j=0;
//  for(i=10;i<Radius;i=i+10)
//  {
////       setcolor(LIGHTRED);
//		cairo_set_source_rgb(cr, 1.0, 102.0/255.0, 102.0/255.0);
////       sound(200+10*i);
//		for(iarc=i/5;iarc<360+i/5;iarc=iarc+20)
//	  {
////		 	void arc(int x, int y, int stangle, int endangle, int radius);
//// 			arc function is used to draw an arc with center (x,y) and stangle specifies starting
////			angle, endangle specifies the end angle and last parameter specifies the radius of the arc
////			void cairo_arc (cairo_t *cr, double xc, double yc, double radius, double angle1, double angle2);
//			cairo_new_sub_path(cr);
//			cairo_arc(cr, X_Pos,Y_Pos, i, deg2rad(iarc), deg2rad(iarc+2));
//
////	    arc(X_Pos,Y_Pos,iarc,iarc+2,i);
//		}
////		printf("hey wow\n");
////		while(gtk_events_pending())
////		{
////		printf("hm \n");
////		}
//
//		cairo_stroke(cr);
//		// -- DELAY HERE --
//		ms_sleep(250.0/(double long)i);/* 100/i*5 */ // I guess a delay only makes sense when drawing on a window
////		ms_sleep(100000);
////	sound(200+15*i);
////	setcolor(YELLOW);
//		// This most surely does not do anything
//		if (j>0)
//		{
//			cairo_set_source_rgb(cr,1,1,52/255);
//	 		for(iarc=j/5;iarc<360+j/5;iarc=iarc+20)
//			{
//				cairo_new_sub_path(cr);
//				cairo_arc(cr,X_Pos,Y_Pos,j,deg2rad(iarc),deg2rad(iarc+2));
//			}
//			cairo_stroke(cr);
//		}
//    j=i;  /* erase in de_fasage */
//	}
//	cairo_stroke(cr);

//  Last_Pitch=200+10*i;
//  for (i=0;i<150;i++)  /* final audio effect */
//  {
//		ms_sleep(5);
//		Last_Pitch=Last_Pitch+80;
//		sound(Last_Pitch);
//  }
//  setcolor(svcolor);
//  nosound();
}

void Gen_Snap_Effect()
{

// ---- Don't forget to add the delays back in if needed  -----

//  int Last_Pitch,i;
//	int i;

  Effect_Flag=ON;
//  Last_Pitch=3000;
//  for (i=0;i<30;i++)  /* final audio effect */
//  {
//		Mydelay(5);
//		Last_Pitch=Last_Pitch-100;
//		sound(Last_Pitch);
//  }
//  Mydelay(100);
//  nosound();
}

void Zero_Vulner_Sound()
{
//  sound(600);
//  Sound_Flag=4;
//  return(0);
}

// -- These kind of animation functions maybe should get some sort of special treatment
// within the step function as they redraw the ship with a delay multiple times --
void Jitter_Ship()
{
//  int Jitter_Headings;
//  int Jitter_X_Pos,Jitter_Y_Pos;
//  int i;

  Effect_Flag=ON;
	Jitter_Flag=1;
	// Call clean() with only ship update on?
//  Draw_Ship(cr,Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX); /*erase ship */
//	stroke_in_clip(cr);

//  for (i=8;i>0;i--)
//  {
//    Jitter_Headings=Ship_Headings+2*i;
//    Jitter_X_Pos=Ship_X_Pos+i*Fcos(Jitter_Headings);
//    Jitter_Y_Pos=Ship_Y_Pos+i*Fsin(Jitter_Headings);
////		Call update with only ship update on?
////		clear_prev_path(cr, PrevShip);
////    Draw_Ship(cr,Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings, SHIP_SIZE_FACTOR*MaxX);
////		stroke_in_clip(cr);
//    ms_sleep(((double long)i)*5L);
////		clear_prev_path(cr, PrevShip);
////    Draw_Ship(cr,Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings, SHIP_SIZE_FACTOR*MaxX);  /* erase ship */
////		stroke_in_clip(cr);
//    Jitter_Headings=Ship_Headings-2*i;
//    Jitter_X_Pos=Ship_X_Pos+i*Fsin(Jitter_Headings);
//    Jitter_Y_Pos=Ship_Y_Pos+i*Fcos(Jitter_Headings);
////		clear_prev_path(cr, PrevShip);
////    Draw_Ship(cr,Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings, SHIP_SIZE_FACTOR*MaxX);  /* draw ship */
////		stroke_in_clip(cr);
//    ms_sleep(((double long)i)*5L);
////		clear_prev_path(cr, PrevShip);
////    Draw_Ship(cr,Jitter_X_Pos,Jitter_Y_Pos,Jitter_Headings, SHIP_SIZE_FACTOR*MaxX);  /* erase ship */
////		stroke_in_clip(cr);
//  }
//	clear_prev_path(cr, PrevShip);
//	Draw_Ship(cr,Ship_X_Pos,Ship_Y_Pos,Ship_Headings, SHIP_SIZE_FACTOR*MaxX); /* restore ship to its previous position */
//	Draw_Ship(cr,Ship_X_Pos,Ship_Y_Pos,Ship_Headings, SHIP_SIZE_FACTOR*MaxX); /* restore ship to its previous position */
//	stroke_in_clip(cr);
}

void Reset_All_Missiles()
{
  int i;

  for (i=0;i<MAX_NO_OF_MISSILES;i++)
      if(Missile_Flag[i]==ALIVE)  Missile_Flag[i]=KILL;
  Handle_Missile();
}

int Check_Collision(float First_X,float First_Y,float Second_X,
		    float Second_Y, int Crash_Distance)
{
  int dist;

  dist=fabs(sqrt(pow(First_X-Second_X,2)+
		 pow(First_Y-Second_Y,2)  ));
  if(dist<Crash_Distance) return(1);
		     else
		     return(0);
}

void Test_Collisions()
{

  int breakflag;
  int i;
  int Handle_Missile_Flag;

  Handle_Missile_Flag=OFF;
  breakflag=OFF;

  for(i=0;i<6;i++)   /* for all  possible missiles */
    {                  /* check against mine only */
  if(Mine_Flag==ALIVE)
    if(Missile_Flag[i]==ALIVE)
      if(Check_Collision(Missile_X_Pos[i],Missile_Y_Pos[i],
  	    Mine_X_Pos,Mine_Y_Pos,COLLISION_DIST) )
        {
    Missile_Flag[i]=KILL;
    Handle_Missile_Flag=ON;
    Mine_Flag=KILL;
    Score=1.0;
    Handle_Mine();
//    Update_Mines();
//    Update_Score();
      } /* end missile vs. mine for aiming test */
    }
  if(Handle_Missile_Flag) Handle_Missile(); /* KILL them all */


}

void Accumulate_Data()
{
  float shipvel;
  int shipcenterdist;

  if(--Data_Update_Counter<=0)
   {
     Data_Update_Counter=20;

	/* update Velocity */
     shipvel=sqrt(pow(Ship_X_Speed,2)+pow(Ship_Y_Speed,2));
		if(shipvel<SHIP_GOOD_VELOCITY)
		{
 			Velocity=Velocity+7;
//			Update_Velocity(cr); // --- UNCOMMENT --- //
		}

	/* update Control */

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

		/* if Wrap_Around  */
//     Update_Control(cr);
//     Update_Points(cr);

   } /* if data-update-counter */
}

//int main()
//{
//	start_drawing();
//	Ship_Y_Pos +=50;
//	Gen_Explosion(SF_canvas, Ship_X_Pos, Ship_Y_Pos, 80);
//	Draw_Ship(SF_canvas, Ship_X_Pos, Ship_Y_Pos, 90,SHIP_SIZE_FACTOR*MaxX);
//	double x1;
//	double y1;
//	double x2;
//	double y2;
//	cairo_path_extents(SF_canvas,&x1,&y1,&x2,&y2);
//	cairo_stroke(SF_canvas);
//
//	cairo_set_source_rgb(SF_canvas,1,0,0);
//	cairo_rectangle(SF_canvas, x1, y1, x2-x1, y2-y1);
//	cairo_stroke(SF_canvas);
//	cairo_surface_write_to_png(surface, "exp.png");
//	stop_drawing();
////	start_drawing(); ////	Gen_Explosion(SF_canvas, MaxX/2, MaxY/2, 120);
////	stop_drawing();
//}
