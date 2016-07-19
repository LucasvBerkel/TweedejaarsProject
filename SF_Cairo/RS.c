// OS X compilation:
/* gcc -Wall -g  myvars.c TCOL.c DE.c HM.c RS.c -I/usr/local/include/cairo -L/usr/local/lib/ -lcairo `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`  -o RS -Wno-dangling-else -Wno-switch;

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

//#include "myconst.h"
//#include "myvars.h"

#include "DE.h"
#include "HM.h"
#include "TCOL.h"
#include "RS.h"

#include <gdk/gdkkeysyms.h>

// I don't think we do anything with this menu stuff
extern char Friend_Menu[3][1];
extern char Foe_Menu[3][1];
extern char Mine_Indicator;
extern mine_type Mine_Type;

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


char Keyboard1() /* handles escape key press only */ // wtf why
{
//    union u_type{int a; char b[3];} keystroke;
//    char inkey=0;
//
//    while(bioskey(1)==0);   /* key relieved, no input */
//    keystroke.a=bioskey(0); /* fetch ascii code */
//    inkey=keystroke.b[0];   /* ..and load code into variable */
//    return(inkey);
	return 0;  // Make the compiler happy
}

//int gprintf( int *xloc, int *yloc, char *fmt, ... )
//{
//    va_list  argptr;      /* Argument list pointer  */
//    char str[140];      /* Buffer to build sting into */
//    int cnt;        /* Result of SPRINTF for return */
//
//    va_start( argptr, fmt );    /* Initialize va_ functions */
//    cnt = vsprintf( str, fmt, argptr ); /* prints string to buffer  */
//    outtextxy( *xloc, *yloc, str ); /* Send string in graphics mode */
//    va_end( argptr );     /* Close va_ functions    */
//    return( cnt );      /* Return the conversion count  */
//}

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
        Update_Interval(cr);
    }
}


void Init_Session() {
    One_Game_Loops=One_Game_Duration*60*20;
    Game_Type=SPACE_FORTRESS;
    Mine_Live_Loops=200;
    Mine_Wait_Loops=80;
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
// for (i=0;i<10;i++)
//         {
//        bc[i]=malloc(size);
//        Display_Bonus_Char(cr, Bonus_Char_Vector[i][0]);
//        getimage(x,y,x+16,y+16,bc[i]);
//        putimage(x,y,bc[i],XOR_PUT);
//         }
}

// What does this even do in the game [3]
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
void Set_Bonus_Message()
{
//    int size;
//    int svcolor;
//    int x,y;
//
//    svcolor=getcolor();
//    setcolor(TEXT_COLOR);
//    x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//    y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
//    gprintf(&x,&y,"Bonus");
//    setcolor(svcolor); /* restore previous color */
//
//    size=imagesize(0,0,40,9);        /*length of 5 characters*/
//    buffer2=malloc(size);
//    getimage(x,y,x+40,y+9,buffer2);
//    putimage(x,y,buffer2,XOR_PUT);
//    setcolor(svcolor);
}

// What does this even do in the game
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
//     int rn;
//
//     do { rn=randrange(0,10); }
//     while(rn==Bonus_Indication_Index);
//     return(rn);
	return 0; // Make the compiler happy
}

void Generate_Resource_Character()
{
//    int lastrn;
//    static bonus_character lastchar=NON_BONUS;
//
//    if((lastchar==NON_BONUS)&&
//         (No_Of_Bonus_Windows<MAX_BONUS_WINDOWS))
//
//             if(randrange(0,10)<7) /* display first bonus */
//     {
//         No_Of_Bonus_Windows++;
//         rn=Bonus_Indication_Index;
//         Xor_Bonus_Char(rn);
//         lastchar=Bonus_Display_Flag=FIRST_BONUS;
//         Bonus_Wasted_Flag=OFF;
//     }
//             else /* display non_bonus character */
//     {
//         lastrn=rn;
//         do { rn=Generate_Non_Bonus_Char(); }
//         while(rn==lastrn); /* new char is different from last one */
//         Xor_Bonus_Char(rn);
//         lastchar=Bonus_Display_Flag=NON_BONUS;
//     }
//    else
//    if(lastchar==FIRST_BONUS)
//        {
//            Xor_Bonus_Char(rn);
//            lastchar=Bonus_Display_Flag=SECOND_BONUS;
//        }
//    else
//    if(lastchar==SECOND_BONUS)
//        {
//             rn=Generate_Non_Bonus_Char();
//             Xor_Bonus_Char(rn);
//             lastchar=Bonus_Display_Flag=NON_BONUS;
//        }
}

void Handle_Bonus()
{
//
//    if(!Resource_Flag)   /* resource is off */
//        {
//            Resource_Off_Counter++;
//            if(Resource_Off_Counter>=No_Resource_Display_Interval)
//    {
//        Resource_Flag=ON;
//        Resource_On_Counter=0;
//        Generate_Resource_Character();
//    }
//     }
// else   /* Resource_Flag=ON; */
//     {
//         Resource_On_Counter++;
//         if(Resource_On_Counter>=Resource_Display_Interval)
//             {
//     Resource_Flag=OFF;
//     Resource_Off_Counter=0;
//     Bonus_Display_Flag=NOT_PRESENT; /* in case bonus is pressed after
//                            $ disappears */
//     if (Bonus_Granted)
//            {
//                Write_Bonus_Message();     /* erase bonus message */
//                Bonus_Granted=OFF;
//            }
//     else
//     Xor_Bonus_Char(rn);  /* Erase_Resource_Char */
//             }
//     }
}

// For the python interface
int get_score()
{
	return Score;
}


void SF_iteration(cairo_t *cr)
{
	long double elapsed_time;
  clock_t loop_start_time;

//        Set_Kbd_Rate(0x8); /* to slow repeat rate 15Hz */
//        Capture_Kbd(Get_Key); /* redirect KBD interrupts to Get_Key() */ // Uncomment
// I think we don't need this initialization with clock()
//        Time_Counter=0;

//        Capture_Tik(Get_Tik);
//        Set_Timer();

// Do all the drawing in one go here, first clear all (using prev_paths) in need of 
// an update, then optionally draw the hexagon (maybe only if it has been crossed)
// and then draw all in need of an update
	loop_start_time=clock();
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
	Test_Collisions(cr); // Animations are done here
	Handle_Shell(cr);
	Handle_Fortress(cr);
	if(Display_Interval_Flag) {   /* of double press */
	    if(Mine_Type==FOE) Find_Interval(cr);
	    Display_Interval_Flag=OFF;
	}
	Accumulate_Data(cr);
	Handle_Bonus();
	if(!Effect_Flag) {
	// This only says something like
	// The game should always wait 50ms between frames, so sleep until the loop
	// body takes 50ms
	// is this valid c?
		elapsed_time=((clock()-loop_start_time)/(double)CLOCKS_PER_SEC)*1000.0;
    if(elapsed_time < SF_DELAY)
		{
//				printf("Slaap kindje slaap %Lf \n", elapsed_time);
        usleep(SF_DELAY-elapsed_time);  /* wait up to 50 milliseconds */
		}
	} else Effect_Flag=OFF;  /* no delay necessary */
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

// Not needed
void Announce_Game_End()
{
//    int svcolor;
//    int x,y;
//
//    svcolor=getcolor();
//    setcolor(TEXT_COLOR);
//    x=0.35*MaxX; y=0.7*MaxY;
//    gprintf(&x,&y,"GAME IS OVER !");
//    x=0.25*MaxX; y=0.8*MaxY;
//    gprintf(&x,&y,"<press any key to continue>");
//    setcolor(svcolor); /* restore previous color */
}

// Skip this?
void Announce_Session_End()
{
//    int svcolor;
//    int x,y;
//
//    svcolor=getcolor();
//    setcolor(TEXT_COLOR);
//    x=0.35*MaxX; y=0.7*MaxY;
//    gprintf(&x,&y,"SESSION IS OVER !");
//    x=0.25*MaxX; y=0.8*MaxY;
//    gprintf(&x,&y,"<press any key to continue>");
//    setcolor(svcolor); /* restore previous color */
}

void Update_Mines(cairo_t *cr)
{
    Show_Score(cr, Mines,Mines_X,Data_Line);
}

void Update_Score(cairo_t *cr)
{
    Show_Score(cr, Score,Score_X,Data_Line);
}

void Reset_Aim_Screen(cairo_t *cr)
{
    int i;
        /*  reset variables */
    Ship_X_Pos=0.5*MaxX; /* on a 640 x 480 screen VGA-HI */
    Ship_Y_Pos=0.5*MaxY; /* same as above */
    Ship_X_Speed=0.0;
    Ship_Y_Speed=0.0;
    Ship_Headings=0;
    Mine_Flag=DEAD;
    for(i=0;i<MAX_NO_OF_MISSILES;i++) Missile_Flag[i]=DEAD;
    Missile_Type=VS_FRIEND;
    Missile_Vs_Mine_Only=OFF;
    Missiles_Counter=0;
    Shell_Flag=DEAD;
    Rotate_Input=0; /* joystick left/right */
    Accel_Input=0; /* joystick forward */
    End_Flag=OFF;
    Restart_Flag=OFF;
    Fort_Headings=270;
    Timing_Flag=OFF; /* if screen reset between consecutive presses */
        /* reset screen */
    Draw_Frame(cr);
    Draw_Ship(cr,Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX);
		stroke_in_clip(cr);

            /* reset panel */
    Update_Mines(cr);
    Update_Speed(cr);
    Update_Score(cr);
}  /* end reset screen */

void Init_Aim_Session()
{
//    char a;
//    int i;
//
////    clrscr();  // From graphics.h or something
//    gotoxy(30,5);
//    printf("AIMING TASK TEST");
//    gotoxy(1,8);
//    printf("The default # of games is 3, would you like to change it? (Y/N):");
//    a=Keyboard1();
//    if((a==121)||(a==89))  /* y=121 and Y=89 */
//        {
//         printf("\n\nPlease type new number of games (1-9):");
//         do { a=Keyboard1();
//        if((a>=49)&&(a<=57)) { No_Of_Games=a-48;
//                 printf(" %d\n",No_Of_Games);
//                 delay(500);
//                         }
//        else a=0;
//    } while(a==0);
//        }
////    clrscr();  // From graphics.h or something

    Game_Type=AIMING_TEST;
    Mine_Live_Loops=200;
    Mine_Wait_Loops=10;
}

void Save_Aiming_Game() {
//    header.Number_Of_Planned_Games=No_Of_Games;
//    header.One_Game_Duration=One_Game_Duration;
//
//    Aiming_Game_Results.mines=Mines;
//    Aiming_Game_Results.speed=Speed;
//    Aiming_Game_Results.score=Score;

}

void Run_Aiming(cairo_t *cr)   /* 1- for training 0- for demo */
{
    clock_t elapsed_time;
    clock_t loop_start_time;

    Init_Aim_Session();
    Open_Graphics();
    Initialize_Graphics(cr);
    Set_Graphics_Eraser(cr);
    Game_Counter=0; /* no of games played */
//    Set_Kbd_Rate(0x8); /* to slow repeat rate .. */
    do
    {
	    Game_Counter++;
	    Mines=0; Speed=0; Score=0;
	    Reset_Aim_Screen(cr);
	    Loop_Counter=0;
//    Capture_Kbd(Get_Key); /* redirect KBD interrupts to  Get_Key() */ // Uncomment
//    Time_Counter=0;
//    Capture_Tik(Get_Tik);
//    Set_Timer();
    	do
			{
				loop_start_time=clock();
				Loop_Counter++;
				Get_User_Input(cr);
				   while(Freeze_Flag) Get_User_Input(cr);
				Move_Ship(cr);  /* rotation only */
				Handle_Missile(cr);
				//         if(Sound_Flag>1) Sound_Flag--;
				//         if(Sound_Flag==1) {Sound_Flag--; nosound();}
				Handle_Aim_Mine(cr);
				Test_Collisions(cr);
				if(!Effect_Flag)
				{
					if ( (elapsed_time=((clock()-loop_start_time)/CLOCKS_PER_SEC))*1000 < SF_DELAY)
					{		
						usleep(SF_DELAY-elapsed_time);  /* wait up to 50 milliseconds */
					}
				}
			else Effect_Flag=OFF;  /* no delay necessary */
			} while((!End_Flag)&&(Loop_Counter < 2400));
    /* ESC or three minutes */

//    Restore_Tik();
//    Reset_Timer();
//    Restore_Kbd();
//    Set_Kbd_Rate(0x4); /* to repeat rate 20Hz */
    if((!End_Flag)&&(Game_Counter<No_Of_Games))
                     { Announce_Game_End();
//                         nosound();   /* just in case */
//                         sound(600);
//                         delay(1000);
//                         nosound();
//                         while(keyboard()); /* clear keyboard */
//                         getch();
                     }
//    Set_Kbd_Rate(0x4); /* to slow repeat rate 15Hz */
    Save_Aiming_Game();
    } while((Game_Counter<No_Of_Games)&&(!End_Flag)); /* ESC or all games played */
//    nosound();   /* just in case */
//    sound(400);
//    delay(1000);
//    nosound();
    Announce_Session_End();
//    getch();  /* show results */
    Close_Graphics(cr);
}

//void set_initial_vals(cairo_t *cr)
//{
//	memset(Missile_Should_Update, 0, MAX_NO_OF_MISSILES);
//	memset(Missile_Should_Clean, 0, MAX_NO_OF_MISSILES);
//	Reset_Screen(cr);
//}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	
	// Oddly enough, clipping seems to work different accros surfaces. Therefore it is 
	// sometimes wise to set things to always update here. (a clip within a quartz 	
	// surface erases everything outside of the clipping region)

	if(Initialized_Graphics == 0)
	{
	  set_initial_vals(cr); // Also resets the screen to some good initial object drawing values
		Initialized_Graphics = 1;
	}
	Initialize_Graphics(cr);  // Why is this needed again
	// Main sf stuff
	SF_iteration(cr);
	// keep
	Draw_Frame(cr);
	clean(cr);
	Fort_Should_Update = 1;
	Draw_Hexagone(cr, MaxX/2,MaxY/2,SMALL_HEXAGONE_SIZE_FACTOR*MaxX);
	stroke_in_clip(cr);
	update_drawing(cr);
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

//	for(i = 0; i < 3420; i++)
//	{
//		gtk_widget_queue_draw(darea);
//		while(gtk_events_pending())
//		{
//    	gtk_main_iteration_do(TRUE);
//		}
//		usleep(1000*30); // 500 (?) miliseconds, usleep() is in microseconds
//	}
	// And the clean up here
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

//  gtk_main();

//	stop_drawing(); // GTK handles this I guess

  return 0;
}




