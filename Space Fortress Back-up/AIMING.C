#include <tcu.h>
#include <stdarg.h>
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

extern volatile unsigned char lastrespokey;

extern void Get_User_Input(void);
extern void Init_Stick(void);
extern void Init_Respo(void);
extern void Close_Respo(void);

extern int far button_handler (TCU_FORM *, int);
extern Set_Kbd_Rate(unsigned char Rate);
extern void Capture_Kbd(void interrupt far (*func) () );
extern void Capture_Respo(void interrupt far (*func) () );
extern void Capture_Tik(void interrupt far (*func) () );
extern void interrupt far Get_Tik();
extern Set_Timer();
extern void interrupt far Get_Key();
extern void interrupt far newkey();
extern Restore_Tik();
extern Restore_Kbd();
extern Reset_Timer();

extern int Freeze_Flag;

TCU_FORM  aim_form;
char S_F_N[20], S_L_N[20];
int Aiming_Score[5] = {0,0,0,0,0};

/*************************************************************************/

		/* Run_Aiming Module  4.4.90 */

/**************************************************************************/

Announce_Game_End()
{
  int svcolor;
  int x,y;

  svcolor=getcolor();
  setcolor(TEXT_COLOR);
  x=0.35*MaxX; y=0.7*MaxY;
  gprintf(&x,&y,"EINDE SPEL !");
  x=0.25*MaxX; y=0.8*MaxY;
  gprintf(&x,&y,"<druk een toets om door te gaan>");
  setcolor(svcolor); /* restore previous color */
  return(0);
}

Announce_Session_End()
{
  int svcolor;
  int x,y;

  svcolor=getcolor();
  setcolor(TEXT_COLOR);
  x=0.35*MaxX; y=0.7*MaxY;
  gprintf(&x,&y,"EINDE TEST-SESSIE !");
  x=0.25*MaxX; y=0.8*MaxY;
  gprintf(&x,&y,"<druk een toets om verder te gaan>");
  setcolor(svcolor); /* restore previous color */
  return(0);
}


void Reset_Aim_Screen()
{
  int i;
		/*  reset variables */
  Ship_X_Pos=0.5*MaxX; /* on a 640 x 480 screen VGA-HI */
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
  Timing_Flag=OFF; /* if screen reset between consecutive presses */
		/* reset screen */
  Draw_Frame();
  Draw_Ship(Ship_X_Pos,Ship_Y_Pos,Ship_Headings,SHIP_SIZE_FACTOR*MaxX);

			/* reset panel */
  Update_Mines();

 }  /* end reset screen */


Init_Aim_Session()
{
  char a;
  int i;

  Init_Respo();
  Init_Stick();
  clrscr();
  No_Of_Games=3;
  Aim_ask_name(S_F_N, S_L_N); /* S's Last Name and First Name */
  clrscr();
  Game_Counter = 0;
  Game_Type=AIMING_TEST;
  Mine_Live_Loops=200;
  Mine_Wait_Loops=10;
  return(0);
}


Run_Aiming(int Training_Session)   /* 1- for training 0- for demo */
{
  unsigned elapsed_time;
  unsigned long loop_start_time;

  Init_Aim_Session();
  Open_Graphics();
  Initialize_Graphics();
  Set_Graphics_Eraser();
  Game_Counter=0; /* no of games played */
  Set_Kbd_Rate(0x8); /* to slow repeat rate .. */
  do
  {
  Game_Counter++;
  Mines=0;
  Reset_Aim_Screen();
  Loop_Counter=0;
  Capture_Kbd(Get_Key); /* redirect KBD interrupts to  Get_Key() */
  Capture_Respo(newkey);
  Time_Counter=0;
  Capture_Tik(Get_Tik);
  Set_Timer();
  do
    {
     loop_start_time=Time_Counter;
     Loop_Counter++;
     Get_User_Input();
      while(Freeze_Flag) Get_User_Input();
     Move_Ship();  /* rotation only */
     Handle_Missile();
     if(Sound_Flag>1) Sound_Flag--;
     if(Sound_Flag==1) {Sound_Flag--; nosound();}
     Handle_Aim_Mine();
     Test_Collisions();
     if(!Effect_Flag)
       {
	 if ( (elapsed_time=Time_Counter-loop_start_time) < 50)
	    Mydelay(50-elapsed_time);  /* wait up to 50 milliseconds */
       }
     else Effect_Flag=OFF;  /* no delay necessary */
    } while((!End_Flag)&&(Loop_Counter < 2400));
	/* ESC or three minutes */
  Aiming_Score[Game_Counter - 1] = Mines;
  Restore_Tik();
  Reset_Timer();
  Restore_Kbd();
  Restore_Respo();
  Set_Kbd_Rate(0x4); /* to repeat rate 20Hz */
  if((!End_Flag)&&(Game_Counter<No_Of_Games))
				   {
				     Announce_Game_End();
				     nosound();   /* just in case */
				     sound(600);
				     delay(1000);
				     nosound();
				     while(keyboard()); /* clear keyboard */
				     getch();
				   }
  Set_Kbd_Rate(0x4); /* to slow repeat rate 15Hz */
  } while((Game_Counter<No_Of_Games)&&(!End_Flag)); /* ESC or all games played */

  nosound();   /* just in case */
  sound(400);
  delay(1000);
  nosound();
  Save_Aim_Results();
  Announce_Session_End();
  getch();	/* show results */
  Close_Respo();
  Close_Graphics();
  return(0);
}


int Aim_ask_name(char Subject_First_Name[20], char Subject_Last_Name[20])
 {

  TCU_FIELD_VALUE First_Name, Last_Name;
  FILE	          *fileptr;
  int		  esc_key,
		  Voorn,        /* name for form-field containing first name */
		  Achtern;      /* ..                             surname */

  tcu_load_form (&aim_form, "aimtask");
  tcu_get_field_id (&aim_form, "Voorn", &Voorn);
  tcu_get_field_id (&aim_form, "Achtern", &Achtern);
  tcu_set_field_mode (&aim_form, Voorn, TCU_FORM_EDIT);
  tcu_set_field_mode (&aim_form, Achtern, TCU_FORM_EDIT);
  tcu_set_button_fn (&aim_form, button_handler);
  tcu_display_form (&aim_form, 1, 2);
  do {
      tcu_edit_form (&aim_form, 1, &esc_key);
      if (esc_key == TCU_FLD_BUTTONESC || esc_key == TCU_FLD_ESCESC)
	if (tcu_get_confirm (25, 10, tcu_colour_attrib (WHITE, RED),
	     tcu_colour_attrib (WHITE, RED),
	     "Stoppen ?: Bevestigen met Y/N")==1)
	     esc_key = TCU_FLD_ESCESC;
	else esc_key = TCU_FLD_ESCNEXT;
   } while (esc_key != TCU_FLD_BUTTONESC && esc_key != TCU_FLD_BUTTONSAVE && esc_key != TCU_FLD_ESCESC);
 if (esc_key == TCU_FLD_BUTTONSAVE)
   {
    if ((First_Name.v_string = (char *) malloc(20)) == NULL)
    show_error(" te weinig geheugen voor buffer");
    if ((Last_Name.v_string = (char *) malloc(20)) == NULL)
    show_error(" te weinig geheugen voor buffer");

    tcu_get_field(&aim_form, Voorn, &First_Name);
    tcu_get_field(&aim_form, Achtern, &Last_Name);
    strcpy(Subject_First_Name, First_Name.v_string);
    strcpy(Subject_Last_Name,  Last_Name.v_string);

    free(First_Name.v_string);
    free(Last_Name.v_string);
   }
 else exit(0);
 tcu_remove_form(&aim_form);
 return(1);
}

int Save_Aim_Results()
{
FILE     *fileptr;
char     *buffer;

  if ((fileptr = fopen("subjects.aim", "at+")) != NULL)
  {
  if ((buffer = malloc(80)) == NULL)
  {
   printf("Not enough memory to allocate buffer\n");
   exit(1);  /* terminate program if out of memory */
  }

  sprintf(buffer, "%s\t%s\t%d\t%d\t%d\n", S_F_N, S_L_N, Aiming_Score[0],
		   Aiming_Score[1], Aiming_Score[2]);
  fputs(buffer, fileptr);
  free(buffer);
  }
  else show_error("file niet gevonden");

  fclose(fileptr);

return(0);
}