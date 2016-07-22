// OS X compilation:
/* gcc -Wall -g  myvars.c TCOL.c DE.c HM.c RS.c -I/usr/local/include/cairo -L/usr/local/lib/ -lcairo `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`  -o RS -Wno-dangling-else -Wno-switch; */
// To run without GTK warnings: (actually running without any error logging to the terminal)
// ./RS 2>/dev/null 

/* test graphics 21.2.90 18:00
            definitions */
//#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
//#include <graphics.h>
//#include <process.h>
//#include <bios.h>
//#include <alloc.h>
//#include <dos.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//#include "myconst.h"
//#include "myvars.h"

#include "DE.h"
#include "HM.h"
#include "TCOL.h"
#include "RS.h"

#include <gdk/gdkkeysyms.h>

// Linux does not have M_PI in math.h for some reason
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


// I don't think we do anything with this menu stuff
extern char Friend_Menu[3][1];
extern char Foe_Menu[3][1];
extern char Mine_Indicator;
extern mine_type Mine_Type;

int jitter_switch = 1;

        /* Functions  */ // Done with the header
//extern void Open_Graphics(void);
//extern void Close_Graphics(void);
//extern float Fcos(int Headings_Degs);
//extern float Fsin(int Headings_Degs);
//extern Draw_Ship (int x, int y, int Headings, int size);
//extern int Draw_Hexagone(int X_Center,int Y_Center,int Hex_Size);
//extern void Draw_Frame();
//extern Draw_Fort (int x, int y, int Headings, int size );
//extern Draw_Missile (int x, int y, int Headings, int size);
//extern Draw_Shell(int x, int y, int Headings, int size);
//extern Draw_Mine (int x, int y, int size);
//extern int Find_Headings(int x1,int y1,int x2,int y2);
//extern Update_Ship_Dynamics();
//extern Update_Ship_Display();
//extern Move_Ship(cr);
//extern Fire_Shell();
//extern Handle_Fortress();
//extern Test_Collisions();
//extern Generate_Mine();
//extern Move_Mine();
//extern Handle_Mine();
//extern Handle_Shell();
//extern Handle_Missile(cairo_t *cr);
//extern Accumulate_Data();
//extern Push_Buttons();
//extern Joystick();
//extern Setup_Mouse_Handler();
//extern Reset_Mouse_Handler();
//extern Initialize_Graphics();

// Something to do with double presses or something
// t2 - t1 should give some value in milliseconds, if that is above some threshold some
// flag is set. 
void handle_F3()
{
	if((Key==GDK_KEY_F3)&&(Lastkey!=GDK_KEY_F3)&&(!(Timing_Flag))) { /* first F3 keypress */
       t1=clock();
       Timing_Flag=ON;
       Check_Mine_Flag=ON; /* is used by Get_User_Input(cr) */
   }

   if((Key==GDK_KEY_F3)&&(Lastkey==GDK_KEY_F3)&&(Timing_Flag)) {   /* second F3 keypress */
       t2=clock();
       Timing_Flag=OFF;
       Key=0;   /* to enable consecutive double_press */
       /* where with next keypress Lastkey=0 */
       Display_Interval_Flag=ON;  /* is used in main */
   }

}

void Check_Bonus_Input(cairo_t *cr) {
//    if((Bonus_Display_Flag==NOT_PRESENT)||(Bonus_Display_Flag==NON_BONUS)) {
//    } else if(Bonus_Display_Flag==FIRST_BONUS) {
//        Bonus_Wasted_Flag=ON;
//    } else if(Bonus_Display_Flag==SECOND_BONUS) {
//        if(!Bonus_Wasted_Flag) {
//            if(Key==GDK_KEY_F1) {
//                No_Of_Points_Bonus_Taken++;
//                Points=Points+100;
//                Update_Points(cr);
//            } else {
//                No_Of_Missiles_Bonus_Taken++;
//                Missile_Stock=Missile_Stock+50;
//                if(Missile_Stock>=100) Missile_Stock=100;
//                Update_Shots(cr);
//            }
//        Bonus_Display_Flag=NOT_PRESENT;
//        Bonus_Granted=ON;
//        Xor_Bonus_Char(rn);    /* erase present $ char */
//        Write_Bonus_Message(cr); /*  Announce_Bonus  */
//        }
//    }
}


void Get_User_Input(cairo_t *cr)
{
    if (New_Input_Flag) /* new input occured */
    {
        New_Input_Flag=OFF; /* make sure no repetitions on same input */
        if (Key==GDK_KEY_Up) Accel_Input=1;        /*   UP    */
        if (Key==GDK_KEY_Left)  Rotate_Input=-1;      /*   LEFT  */
        if (Key==GDK_KEY_Right) Rotate_Input=1;       /*   RIGHT */ 
        if (Key==GDK_KEY_space)  New_Missile_Flag=ON;  /*   DOWN  */ // Used to be down
        if (Key==GDK_KEY_F1)    Check_Bonus_Input(cr);        /*   P(oints) */
        if (Key==GDK_KEY_F2)    Check_Bonus_Input(cr);        /*   M(issiles) */
				// probably not done right
    		if (Key==GDK_KEY_F3)   handle_F3(); // was handled by kbd interrupt handler */ // hmm
				// enter pauses the game 
        if (Key==GDK_KEY_Return) Freeze_Flag=Freeze_Flag^1; /* toggle freeze flag */ 
        if (Key==GDK_KEY_Escape)   End_Flag=ON;
    }
    if(Check_Mine_Flag) /* after first press of F3 */
        {
            Check_Mine_Flag=OFF;
            if((Mine_Flag==ALIVE) && (Mine_Type==FRIEND))
    Missile_Type=WASTED;
        Show_Mine_Type(cr, Mine_Indicator);
        }
}


void ms_sleep(unsigned long miliseconds)
{
	struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = miliseconds * 1000000L;
	nanosleep(&tim , &tim2);

}


// Not sure what this does
// I've assumed that this erases the text in the panel but idk why anymore
void Set_Graphics_Eraser(cairo_t *cr)
{

//  size=imagesize(0,0,40,9);        /*length of 5 characters*/
//  buffer1=malloc(size);
//  getimage(100,100,140,109,buffer1);

//	cairo_save(cr);
//	cairo_set_source_rgb(cr, 0, 0, 0);
//	cairo_rectangle(cr, 0, 0, TEXT_WIDTH, TEXT_HEIGHT);
//	cairo_fill(cr);
//	cairo_restore(cr);
}


// What does this do?
void Clear_Interval()   /* clear double-press interval */
{
//        int svcolor;
//        int x,y;
//
//        svcolor=getcolor();
//        setcolor(TEXT_COLOR);
//        setviewport( Xmargin, Panel_Y_Start, Xmargin+MaxX, Panel_Y_End, 1);
//        x=Interval_X; y=Data_Line;
//        putimage(x,y,buffer1,COPY_PUT); /* erase garbage */
//        setviewport( Xmargin, 0, Xmargin+MaxX, MaxY, 1);   /* restore gaming area */
//        setcolor(svcolor); /* restore previous color */
}

void Find_Interval(cairo_t *cr)   /* display double-press interval */
{
//    int svcolor;
//    int x,y; // Unused
    int interval;

    interval=Double_Press_Interval=((t2-t1)/CLOCKS_PER_SEC)*1000; /* in milliseconds */
    if((interval<SF_DELAY*20)&&(interval>SF_DELAY)) /* only when interval makes sense */
    {
        if((interval>=Interval_Lower_Limit)&&(interval<=Interval_Upper_Limit)
             &&(Mine_Flag==ALIVE)&&(Mine_Type==FOE))
    Missile_Type=VS_FOE;   /* rearm missile */
        Show_Mine_Type(cr, Mine_Indicator);
//        Update_Interval(cr);
    }
}


void Init_Session() {
    One_Game_Loops=One_Game_Duration*60*20;
    Game_Type=SPACE_FORTRESS;
}

void Init_Game()
{
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

		// We don't do anything with menus? 
//    Select_Mine_Menus();
    /*
    clrscr(); // From graphics.h or something
    gotoxy(30,5);
    printf("SPACE  FORTRESS ");
    gotoxy(20,15);
    Select_Mine_Menus();
    printf("Your foe mines are:");
    for(int i=0;i<3;i++) printf("    %c",Foe_Menu[i][0]);
    gotoxy(1,24);
    printf("Type any  key to continue ..\n");
    getch();*/
}

void Display_Bonus_Char(cairo_t *cr, char Bonus_Char)
{
//    int svcolor;
//    int x,y;
//    svcolor=getcolor();
//    setcolor(TEXT_COLOR);
//    settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
//    x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//    y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//    gprintf(&x,&y,"%c",Bonus_Char);
//    settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
//    setcolor(svcolor); /* restore previous color */
}

// Sort of draws all the bonus characters to a location of the screen so to save them? 
void Set_Bonus_Chars(cairo_t *cr)
{
//    int size,i,j;
//    int x,y;
//
//    /* set character Size */
//    size=imagesize(0,0,16,16);
//    /* get right location */
//    x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//    y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//
// for (i=0;i<10;i++) // Save all the images with a 16x16 size
//         {
//        bc[i]=malloc(size);
//        Display_Bonus_Char(cr, Bonus_Char_Vector[i][0]);
//        getimage(x,y,x+16,y+16,bc[i]);
//        putimage(x,y,bc[i],XOR_PUT);
//         }
}

// Erease the currently displayed image set by getimage with an XOR operation (i.e. erase 
// everything that previous image drawing operation has drawed)
// The cairo interpretation of this function sucks
void Xor_Bonus_Char(cairo_t *cr, int n)   /* write and erase bonus character */
{
//	int x,y;
//	
//	/* get right location */
//	x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//	y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//	cairo_set_source_rgb(
//	cairo_rectangle(cr, x, y, TEXT_WIDTH, TEXT_HEIGHT);
//	cairo_fill(cr);

//    putimage(x,y,bc[n],XOR_PUT);

}

// What does this even do in the game [2]
// Tells the player about the bonuses before the game starts
//void Set_Bonus_Message()
//{
////    int size;
////    int svcolor;
////    int x,y;
////
////    svcolor=getcolor();
////    setcolor(TEXT_COLOR);
////    x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
////    y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
////    gprintf(&x,&y,"Bonus");
////    setcolor(svcolor); /* restore previous color */
////
////    size=imagesize(0,0,40,9);        /*length of 5 characters*/
////    buffer2=malloc(size);
////    getimage(x,y,x+40,y+9,buffer2);
////    putimage(x,y,buffer2,XOR_PUT);
////    setcolor(svcolor);
//}

// Write bonus messsage to the graphics bitmap memory
void Write_Bonus_Message()
{
// int x,y;
//
// x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
// y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
// putimage(x,y,buffer2,XOR_PUT);
}


int Generate_Non_Bonus_Char()
{
   int rn;

   do { rn=randrange(0,10); }
   while(rn==Bonus_Indication_Index); // I think the only reason for this being here 
//	 is the the original random function always returned a number between 0 and n,
//	 while other random fucntions can return a number in between m and n. 
   return(rn);
}


void Generate_Resource_Character()
{
    int lastrn;
    static bonus_character lastchar=NON_BONUS; // This is a struct

    if((lastchar==NON_BONUS) && (No_Of_Bonus_Windows<MAX_BONUS_WINDOWS))
		{
   		if(randrange(0,10)<7) /* display first bonus */
     	{
				No_Of_Bonus_Windows++;
				// An index for an array with bonus characters (like '$) of chartype 
				rn=Bonus_Indication_Index; 
//				Xor_Bonus_Char(rn); // Put the character/image currently passed to graphics
				lastchar=Bonus_Display_Flag=FIRST_BONUS;
				Bonus_Char_Should_Update = 1;
				Bonus_Wasted_Flag=OFF;
			}
			else /* display non_bonus character */
     	{
				lastrn=rn;
				do { rn=Generate_Non_Bonus_Char(); }
				while(rn==lastrn); /* new char is different from last one */
//				Xor_Bonus_Char(rn); // put the image to game
				Bonus_Char_Should_Update = 1;
				lastchar=Bonus_Display_Flag=NON_BONUS;
			}
		}
    else
		{
    	if(lastchar==FIRST_BONUS)
      {
//				Xor_Bonus_Char(rn);
					Bonus_Char_Should_Update = 1;
					lastchar=Bonus_Display_Flag=SECOND_BONUS;
			}
    	else
			{
    		if(lastchar==SECOND_BONUS)
        {
	        rn=Generate_Non_Bonus_Char();
//	        Xor_Bonus_Char(rn);// put the image to gam
					Bonus_Char_Should_Update = 1;
	        lastchar=Bonus_Display_Flag=NON_BONUS;
				}
			}
		}
}


void Handle_Bonus()
{
	if(!Resource_Flag)   /* resource is off */ // What is a resource
  {
		Resource_Off_Counter++;
		// After a counter reaches a threshold, display a resource
    if(Resource_Off_Counter>=No_Resource_Display_Interval)
    {
	    Resource_Flag=ON;
	    Resource_On_Counter=0;
	    Generate_Resource_Character();
    }
  }
	else   /* Resource_Flag=ON; */
	{
		Bonus_Char_Should_Clean = 1;
		Resource_On_Counter++;
		if(Resource_On_Counter>=Resource_Display_Interval)
		{
			Resource_Flag=OFF;
			Resource_Off_Counter=0;
			Bonus_Display_Flag=NOT_PRESENT; /* in case bonus is pressed after  */
			if (Bonus_Granted) // If the player did the interval right
			{
//				Write_Bonus_Message();     /* erase bonus message */ // previous message?
				Bonus_Granted=OFF;
			}
		}
	}
}


void SF_iteration(cairo_t *cr)
{


//        Set_Kbd_Rate(0x8); /* to slow repeat rate 15Hz */
//        Capture_Kbd(Get_Key); /* redirect KBD interrupts to Get_Key() */ // Uncomment
// I think we don't need this initialization with clock()
//        Time_Counter=0;

//        Capture_Tik(Get_Tik);
//        Set_Timer();

// Do all the drawing in one go here, first clear all (using prev_paths) in need of 
// an update, then optionally draw the hexagon (maybe only if it has been crossed)
// and then draw all in need of an update
	Loop_Counter++;
	// This was done by processor interupts, but is allowed automatically by GTK
	Get_User_Input(cr);
	// Pauses the game (when the flag is set, continues this loop) 
	while(Freeze_Flag) Get_User_Input(cr); 
	Move_Ship(cr);
	Handle_Missile(cr);
	//            if(Sound_Flag>1) Sound_Flag--;
	//            if(Sound_Flag==1) {Sound_Flag--; nosound();}
	Handle_Mine(cr);
	Test_Collisions(cr);
	Handle_Shell(cr);
	Handle_Fortress(cr);
	if(Display_Interval_Flag) {   /* of double press */
	    if(Mine_Type==FOE) Find_Interval(cr);
	    Display_Interval_Flag=OFF;
	}
	Accumulate_Data(cr);
	Handle_Bonus();
	if(Effect_Flag) {
	// This only says something like
	// The game should always wait 50ms between frames, so sleep until the loop
	// body takes 50ms
	// is this valid c?
		Effect_Flag=OFF;  /* no delay necessary */
	}
	Score=Points+Velocity+Control+Speed;
}


// Misses some syntax
//int Run_SF(cairo_t *cr)
//{
//    clock_t elapsed_time;
//    clock_t loop_start_time;
    // SCORE SAVE FILE
		// TODO: instead of this file, make a python function that returns the score as an int
		// Probably just some kind of wrapper function like get_score that you can call after
		// update_frame() that just returns the Score global
//    FILE *f = fopen("SAVE\\SCORE.TXT", "w");
//    if (f == NULL) {
//        printf("A state file is not present.\n");
//        exit(1);
//    }
//    fclose(f); 
//		Init_Game();
//		Open_Graphics();
//		Initialize_Graphics(cr);  Probably not needed (or depends on GTK/versus array render)
//		Reset_Screen(cr); 
//		 Draw_Frame(cr here?)
//		Loop_Counter=0;
//
//    Init_Session();
//    Game_Counter=0;
//    do {   /* loop on number of games here */
//        SF_iteration
//        } while(!Restart_Flag&&!End_Flag&&(Loop_Counter < One_Game_Loops));
//				 This says quit the loop if the time defined by One_Game_Loops (default is 3 min)
//				 has passed (this can be measured with clock I guess?) Because the loop always takes
//				 exactly 50ms we can just increment loop counter until it reaches a threshold 
//        /* ESC or three minutes */
//
//         RUNNING FILE
//        f = fopen("SAVE\\SCORE.TXT", "w"); // Open this earlier for performace
//        fprintf(f, "FALSE");
//        fclose(f);
//
//        Restore_Tik();
//        Reset_Timer();
//        Restore_Kbd();
//        Set_Kbd_Rate(0x4); /* to repeat rate 20Hz */
//
//        nosound();   /* just in case */
//        sound(400);
//        delay(500); // Not intersting cus only sound
//        nosound();
//        Game_Counter++;
//        
//         final_display();
//        Close_Graphics(cr);
//				 Pretty sure we can skip this
//        printf("Episode %d score: %d\n", Game_Counter, Score);
//        if(!Restart_Flag && !End_Flag) {
//            while(1) {
//								char ex = 0;
//                char ex = getch(); // getch reads one keyboard input char from the user
//                if(ex==9) {
//                    break;
//                } else if(ex==27) {
//                    return(0);
//                }
//            }
//        }
//
//        
//        clrscr();  // From graphics.h or something
//        /* end one game here */
//    } while(!Restart_Flag && !End_Flag);
//    } while((Game_Counter< No_Of_Games)&&(!End_Flag));
//
//    nosound();   /* just in case */
//    sound(400);
//    delay(1000);
//    nosound();
//    if(Restart_Flag) {
//        return(1);
//    }
//    return(0);
//}

/*************************************************************************/

        /* Run_Aiming Module  4.4.90 */

/**************************************************************************/


static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
//	printf("Ship pos on draw: %d %d\n", Ship_X_Pos, Ship_Y_Pos);

	// Oddly enough, clipping seems to work different accros surfaces. Therefore it is 
	// sometimes wise to set things to always update here. (a clip within a quartz 	
	// surface erases everything outside of the clipping region)
	Initialize_Graphics(cr);  // Why is this needed again

	if(Initialized_Graphics == 0)
	{
	  set_initial_vals(cr);
		Initialized_Graphics = 1;
	}
	// Main sf stuff

	Fort_Should_Update = 1;
	if(!Explosion_Flag && !Jitter_Flag)
	{
		unsigned long elapsed_time;
	  clock_t loop_start_time;
		struct timespec tim, tim2;
	  tim.tv_sec = 0;
		loop_start_time=clock();

		SF_iteration(cr);
		Ship_Should_Update = 1;
		Draw_Frame(cr);

		Update_Points(cr);
		Update_Control(cr);
		Update_Velocity(cr);
		Update_Vulner(cr);
		Update_Interval(cr);
		Update_Speed(cr);
		Update_Shots(cr);
		clean(cr);
		Draw_Hexagone(cr, MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX);
		stroke_in_clip(cr);
		update_drawing(cr);

		elapsed_time=((clock()-loop_start_time)/(double)CLOCKS_PER_SEC)*1000.0;
	  tim.tv_nsec = (SF_DELAY-elapsed_time) * 1000000L;
    if(elapsed_time < SF_DELAY)
		{
//				printf("Sleeping for %Lf \n", SF_DELAY-elapsed_time);
//        ms_sleep(SF_DELAY-elapsed_time);  /* wait up to 50 milliseconds */
				nanosleep(&tim , &tim2);
		}
	}
	else  // Animating
	{
		Draw_Frame(cr);
		// we don't want our object disappearing!
		if (Mine_Should_Update) // Only update when already on
		{
			Mine_Should_Update = 1; 
		}
		if (Shell_Should_Update)
		{
			Shell_Should_Update = 1;
		}
		if(Explosion_Flag)
		{
			unsigned long exp_sleep;
			// Handle drawing here, as otherwhise the ship will move to it's new location
			Ship_Should_Update = 0;
			Ship_Should_Clean = 0;
			cairo_new_path(cr);
			cairo_set_source_rgb(cr, SF_YELLOW);
			cairo_append_path(cr, PrevShip);
			cairo_stroke(cr);

//			cairo_fill_preserve(cr);
		
			// This actually does nothing the first time around
			// explosion_step2 is sort of the inner, yellow circle, one step behind step1
			for(int i = 0; i < Explosion_Step+1; i++)
			{
				explosion_step2(cr, ExpX, ExpY, i);
			}
			explosion_step1(cr, ExpX, ExpY, Explosion_Step);
			

			exp_sleep = (250.0/ ((double) Explosion_Step)) + ANIMATION_DELAY_EXP;
			ms_sleep(exp_sleep);
			Explosion_Step++;
			if((Explosion_Step * 10) >= ExpRadius)
			{
				Explosion_Step = 0;
				Explosion_Flag = 0;
				Ship_Should_Update = 1;
				Ship_Should_Clean = 1;
				Reset_Screen(cr);
			}
		}
		else if(Jitter_Flag)
		{
			Ship_Should_Update = 0;
			Ship_Should_Clean = 0;

			for(int m = 0; m < MAX_NO_OF_MISSILES; m++)
			{ 
				if (Missile_Flag[m]==ALIVE)
				{
					Missile_Should_Update[m] = 1;
				}
			}
			if(jitter_switch)
			{
				jitter_step1(cr, Jitter_Step);
				jitter_switch = 0;
			}
			else
			{
				jitter_step2(cr, Jitter_Step);
				Jitter_Step--;
				jitter_switch = 1;
			}
		  ms_sleep((((unsigned long)Jitter_Step)*5L) + ANIMATION_DELAY_JITTER);

			if(Jitter_Step < 1)
			{
				Jitter_Step = 8;
				Jitter_Flag = 0;

				// Restore Ship to it's previous position
				clear_prev_path(cr, PrevShip);
				Draw_Ship(cr,Ship_X_Pos,Ship_Y_Pos,Ship_Headings, SHIP_SIZE_FACTOR*MaxX);
				stroke_in_clip(cr);
			}
		}
		Update_Points(cr);
		Update_Control(cr);
		Update_Velocity(cr);
		Update_Vulner(cr);
		Update_Interval(cr);
		Update_Speed(cr);
		Update_Shots(cr);
		clean(cr);
		Draw_Hexagone(cr, MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX);
		stroke_in_clip(cr);
		update_drawing(cr);
	}


	


	return FALSE; // Not sure why this should return false
}

// This is so that the python interface can set the key (maybe move to DE? (nah, runloop 
// is also here))
void set_key(int key_value)
{
	Lastkey = Key;
	Key = key_value;
//  A list of GTK hex key values as decimals
//	GDK_KEY_F1 0xffbe 65470 
//	GDK_KEY_F2 0xffbf 65471
//	GDK_KEY_F3 0xffc0 65472
//	GDK_KEY_Left 0xff51 65361
//	GDK_KEY_Up 0xff52 65362
//	GDK_KEY_space 0x020 32 
}


gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
		Lastkey = Key;
		Key = event->keyval;
		New_Input_Flag=ON;
	
//  switch (event->keyval)
//  {
//    case GDK_KEY_Left:
////			Ship_Headings = (Ship_Headings - 5) % 360;
////      Ship_Should_Update = 1;
////			Ship_Should_Clean = 1;
//
//			break;
//    case GDK_KEY_Up:
////			Ship_X_Pos = (Ship_X_Pos + 5) % MaxX;
////      Ship_Should_Update = 1;
////			Ship_Should_Clean = 1;
//			break;
//    case GDK_KEY_Right:
////			Ship_Headings = (Ship_Headings + 5) % 360;
////      Ship_Should_Update = 1;
////			Ship_Should_Clean = 1;
//			break;
//		case GDK_KEY_space:
//			
//			break;
//		case GDK_KEY_F3:
//
//			break;
//		case GDK_KEY_F2:
//
//		case GDK_KEY_Return:
//			
//			break;
//		case GDK_KEY_Escape:
//
//			break;
//    default: // Not sure if this always reached
//      return FALSE;
//  }

  return FALSE; 
}

void animation_loop(GtkWidget *darea)
{
	// Initilization of SF here

    // SCORE SAVE FILE
		// TODO: instead of this file, make a python function that returns the score as an int
		// Probably just some kind of wrapper function like get_score that you can call after
		// update_frame() that just returns the Score global
//    FILE *f = fopen("SAVE\\SCORE.TXT", "w");
//    if (f == NULL) {
//        printf("A state file is not present.\n");
//        exit(1);
//    }
//    fclose(f); 
		

	Init_Session();
	Game_Counter=0;

	do 
	{   /* loop on number of games here */
		Init_Game();
//		Reset_Screen(cr); // Moved to set initial vals
		Loop_Counter=0;
		do 
		{
			gtk_widget_queue_draw(darea);
			while(gtk_events_pending())
			{
    		gtk_main_iteration_do(TRUE); // Redraw the frame
			}
		}
		while(!Restart_Flag&&!End_Flag&&(Loop_Counter < One_Game_Loops));
		Initialized_Graphics = 0;
		Game_Counter++;
		// Close_Graphics(cr); // Not sure if closing is appropiate (it's impossible to close 
		// in this part of the program because we don't have acces to GTK cairo context) 
	} 
	while(!Restart_Flag && !End_Flag);

	// And the clean up here (close graphics) 
}

int main(int argc, char *argv[])
{
	Initialized_Graphics = 0;

// Basic GTK initialization
 	GtkWidget *window;
  GtkWidget *darea;
  gtk_init(&argc, &argv);
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
  g_signal_connect(window, "destroy", G_CALLBACK(exit), NULL);
  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

  gtk_window_set_title(GTK_WINDOW(window), "Space Fortress");
//	gtk_print_context_get_cairo_context();

  gtk_widget_show_all(window);
	animation_loop(darea);

//	stop_drawing(); // GTK handles this I guess

  return 0;
}




