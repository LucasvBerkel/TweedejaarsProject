/*
	Starting Date :	 02-05-'92

	Programmer    :	 Jan Roessingh

	Module 	      :  Interfac.C

	Status	      :  Link together with MAINMENU.C, "A Space Fortress
			 Interface Environment"

	Purpose	      :  Providing a coupling between the SESSIONx.C
			 files, i.e. the links between choices in the menu's
			 and activating the appropriate part-tasks, storing
			 specific data etc.

*/
#include <graphics.h>
#include <io.h>
#include <alloc.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <tcu.h>
#include <stdarg.h>
#include "myconst.h"
#include "myext.h"

#define MAX_TR_STR 300
#define PI 3.1415927

extern int far button_handler (TCU_FORM *, int);

extern char	       trainee_A[13], trainee_B[13];
extern TCU_FORM        screen_form;
extern char 	       Foe_Menu[3][1];


int select_action(int *sel1, int *sel2);
int select_task();
int make_settings(char filename[13]);
int Update_Settings_A();
int Update_Settings_B(char filename[13]);
int Save_Fort_Destruction_Times(char filename[13]);
int give_feedback(int points, int slice);
float scale_to_pi(int feedback_points, int feedback_level_ind, int slice);
int Announce_Foe_Mines();
int Announce_Start_Game();


static unsigned char tasks[6] = {0, 1, 0, 0, 0, 0};

char	     trainee_string[MAX_TR_STR];
char         trial_string[MAX_TR_STR];
TCU_FORM     mine_mem;


/*
receives parameters r1 and r2 from MAIMENU.C main(), which
denote pulldown menu selections
*/

int select_action(int *sel1, int *sel2)
{

 switch(*sel1)
  {
    case 1 :
       switch(*sel2)
	{
	  case 1:
	     select_task();
	  break;
	  case 2:
	     tcu_remove_form(&screen_form);
	     if (task_elements(1)==0)
	      {
	      tcu_display_form(&screen_form,1,2);
	      }
	  break;
	  case 3:
	     tcu_remove_form(&screen_form);
	     if (Run_Aiming(1)==0)
	      {
	      tcu_display_form(&screen_form,1,2);
	      }
	  break;
	 }
    break;
    case 4 :
       switch(*sel2)
	{
	  case 2 :
	     handle_form("sc_game");
	  break;
	 }
    break;
    case 5 :
       switch(*sel2)
	{
	  case 2 :
	     handle_form("sc_user");
	  break;
	}
    break;

  }

return(0);

}



int select_task (void)
{

   int           i;
   int           choice;
   int           opnemen = 4;
   char         *menu_options[] = { "S( ) Schip en Fort" , "M( ) Schip, Fort en Mijnen",
				    "H( ) Hele Spel",  "A( ) Automatisch" ,
				    "O[ ] Opnemen (aan/uit)", NULL };
   TCU_MENU      tasks_menu;

   tcu_save_environment ();    /* Save the environment because we're going */
			       /* to alter the screen here */

      tcu_define_menu (&tasks_menu, "Taak Selectie", /* Create the menu */
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       tcu_colour_attrib (BLACK, LIGHTGRAY),
		       tcu_colour_attrib (BLACK, LIGHTGRAY),
		       tcu_colour_attrib (BLACK, GREEN),
		       tcu_colour_attrib (GREEN, BLACK),
		       TCU_ESC_ESC,
		       TCU_BOX_SINGLE,
		       menu_options,
		       tcu_colour_attrib (RED, LIGHTGRAY));


/* Display the menu on the screen */

      tcu_display_menu (&tasks_menu, 20, 4);


/* Get selections until ESC is pressed */


      while ((choice=tcu_read_menu_selection (&tasks_menu)) > 0) {
	     if (choice==opnemen+1)
	       {
	       if(menu_options[opnemen][2]=='X')
		menu_options[opnemen][2]=' ';
	       else
		menu_options[opnemen][2]='X';
	       }
	     else
	       {
	       for (i=0; i<opnemen; i++) menu_options[i][2]=' ';
	       menu_options[choice-1][2] = 'o';
	       train.level_ind = choice;
	       }
      tcu_set_menu_option (&tasks_menu, choice-1, 1); /* Force display now */
      }


/* Clear the menu from the screen */

      tcu_remove_menu (&tasks_menu);
      tcu_restore_environment ();                 /* Return the environment */

   return (1);
}


int make_settings(char filename[13])
{

   FILE	 		*fileptr;
   char			string1[6],
			string2[6];
    int			length,
			length2;
   long			i;


  if ((fileptr = fopen(filename, "r")) != NULL)
  {
   fgets(trainee_string, MAX_TR_STR, fileptr);  /* first line in file is a command line */
   fgets(trainee_string, MAX_TR_STR, fileptr);  /* read the first status line to measure its length */
   i = strlen(trainee_string) + 1;
   // printf("lengte van traineestring is %d\n",  i);
   if (fseek(fileptr, -i, SEEK_END) != 0)
     {
      show_error("bij het instellen van game-variabelen");
      return(-1);
     }

   fgets(trainee_string, i+1, fileptr);
   sscanf(trainee_string,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
/*			  010203040506070809101112131415161718192021222324252627282930  */
   &train.probe_flag,                  //1
   &train.group_ind,                   //2
   &train.session_ind,                 //3
   &train.game_ind,                    //4
   &train.level_ind,                   //5
   &train.total_points_ind,            //6
   &train.fort_points_ind,             //7
   &train.mine_points_ind,             //8
   &train.trials_end_ind,	       //9	
   &train.alfa_ind,                    //10
   &train.beta_ind,                    //11
   &train.asymp_ind,                   //12
   &train.p_p_ind,                     //13
   &train.varperc_ind,                 //14
   &train.Fprob_ind,                   //15
   &train.perf_ind,                    //16
   &train.criterion_ind1,              //17
   &train.criterion_ind2,              //18
   &train.fort_destruction_ind,        //19
   &train.wraps_ind,                   //20
   &train.hits_ind,                    //21
   &train.misses_ind,                  //22
   &train.mine_damage_ind,             //23
   &train.fort_damage_ind,             //24
   &train.bonus_interval_ind,          //25
   &train.times_bonus_taken_ind,       //26
   &train.times_missiles_taken_ind,    //27
   &train.foe_remember_ind,            //28
   &train.carry_ind,		       //29
   &train.termination_ind);            //30
/*
   printf("probe_flag = %d\n", train.probe_flag);
   printf("group = %d\n", train.group_ind);
   printf("session = %d\n", train.session_ind);
   printf("game_ind = %d\n", train.game_ind);
   printf("level_ind = %d\n", train.level_ind);
   printf("points = %d\n", train.total_points_ind);
   printf("fort points = %d\n", train.fort_points_ind);
   printf("mine points = %d\n", train.mine_points_ind);
   printf("estimated no. of trials to end = %d\n", train.trials_end_ind);
   printf("alfa = %d\n", train.alfa_ind);
   printf("beta = %d\n", train.beta_ind);
   printf("asymp = %d\n", train.asymp_ind);
   printf("p_p = %d\n", train.p_p_ind);
   printf("varperc = %d\n", train.varperc_ind);
   printf("Fprob = %d\n", train.Fprob_ind);
   printf("perf_ind2 = %d\n", train.perf_ind);
   printf("criterion_ind1 = %d\n", train.criterion_ind1);
   printf("criterion_ind2 = %d\n", train.criterion_ind2);
   printf("fort_destruction_ind = %d\n", train.fort_destruction_ind);
   printf("wraps_ind = %d\n", train.wraps_ind);
   printf("hits_ind = %d\n", train.hits_ind);
   printf("misses_ind = %d\n", train.misses_ind);
   printf("mine_damage_ind = %d\n", train.mine_damage_ind);
   printf("fort_damage_ind = %d\n", train.fort_damage_ind);
   printf("bonus_interval_ind = %d\n", train.bonus_interval_ind);
   printf("times_bonus_taken_ind = %d\n", train.times_bonus_taken_ind);
   printf("times_missiles_taken_ind = %d\n", train.times_missiles_taken_ind);
   printf("foe_remember_ind = %d\n", train.foe_remember_ind);
   printf("carry = %d\n", train.carry_ind);
   printf("termination_ind = %d\n", train.termination_ind);
   delay(1000);
*/
   fclose(fileptr);
  }
  else
  {
  show_error("file niet gevonden");
  exit(0);
  }

  if (train.game_ind  >= No_Of_Games)
   {
   train.session_ind++;
   train.game_ind = 0;
   }

  return(0);

} /* end make_settings */


int Update_Settings_A()
{
   train.game_ind = Game_Counter;
   train.total_points_ind = Points;
   train.foe_remember_ind = Ask_For_Foe_Mines();
   train.fort_points_ind = Sh_Ctrl_Points;
   train.mine_points_ind = Mine_Handle_Points;
   train.fort_destruction_ind = Fortress_Destroyed;
   train.mine_damage_ind = Ship_Damaged_By_Mines;
   train.fort_damage_ind = Ship_Damaged_By_Fortress;
   train.wraps_ind = No_Of_Wraps;
   train.hits_ind = Shot_Hits;
   train.misses_ind = (Shot_Fired-Shot_Hits )+ Fortress_Destroyed;
   train.bonus_interval_ind = No_Of_Bonus_Intervals;
   train.times_bonus_taken_ind = No_Of_Points_Bonus_Taken;
   train.times_missiles_taken_ind = No_Of_Missiles_Bonus_Taken;
   if (train.trial == 0)
	 train.carry_ind++;
   return(0);
}

int Update_Settings_B(char filename[13])
{
   train.level_ind = Determine_Level(filename);
   return(0);
}

int Save_Game_Parameters(char filename[13])
{
 FILE	 	       *fileptr;
 char                  update_string[MAX_TR_STR];
 int		       i=0;

  if ((fileptr = fopen(filename, "at+")) != NULL)
  {
   strcpy(update_string, trainee_string);

   Update_Trainee_String(update_string, train.probe_flag, i);
   i+=2;
   Update_Trainee_String(update_string, train.session_ind, i);
   i++;
   Update_Trainee_String(update_string, train.game_ind, i);
   i++;
   Update_Trainee_String(update_string, train.level_ind, i);
   i++;
   Update_Trainee_String(update_string, train.total_points_ind, i);
   i++;
   Update_Trainee_String(update_string, train.fort_points_ind, i);
   i++;
   Update_Trainee_String(update_string, train.mine_points_ind, i);
   i++;
   Update_Trainee_String(update_string, train.trials_end_ind, i);
   i++;
   Update_Trainee_String(update_string, train.alfa_ind, i);
   i++;
   Update_Trainee_String(update_string, train.beta_ind, i);
   i++;
   Update_Trainee_String(update_string, train.asymp_ind, i);
   i++;
   Update_Trainee_String(update_string, train.p_p_ind, i);
   i++;
   Update_Trainee_String(update_string, train.varperc_ind, i);
   i++;
   Update_Trainee_String(update_string, train.Fprob_ind, i);
   i++;
   Update_Trainee_String(update_string, train.perf_ind, i);
   i+=3;
   Update_Trainee_String(update_string, train.fort_destruction_ind, i);
   i++;
   Update_Trainee_String(update_string, train.wraps_ind, i);
   i++;
   Update_Trainee_String(update_string, train.hits_ind, i);
   i++;
   Update_Trainee_String(update_string, train.misses_ind, i);
   i++;
   Update_Trainee_String(update_string, train.mine_damage_ind, i);
   i++;
   Update_Trainee_String(update_string, train.fort_damage_ind, i);
   i++;
   Update_Trainee_String(update_string, train.bonus_interval_ind, i);
   i++;
   Update_Trainee_String(update_string, train.times_bonus_taken_ind, i);
   i++;
   Update_Trainee_String(update_string, train.times_missiles_taken_ind, i);
   i++;
   Update_Trainee_String(update_string, train.foe_remember_ind, i);
   i++;
   Update_Trainee_String(update_string, train.carry_ind, i);
   i++;
   Update_Trainee_String(update_string, train.termination_ind, i);

   // printf("TRAINEE STRING %s\n UPDATE_STRING %s\n ", trainee_string, update_string);

   fputs(update_string, fileptr);
   fclose(fileptr);
  }
  else show_error("file niet gevonden");

return(0);
}


int Save_Fort_Destruction_Times(char filename[13])
{

FILE 	 *fileptr;
char     update_string[MAX_TR_STR];
int 	 i=0, j=0, k=0;


  if ((fileptr = fopen(filename, "at+")) != NULL)
  {
   fgets(trial_string, MAX_TR_STR, fileptr);  /* first line in file is a command line */
   fgets(trial_string, MAX_TR_STR, fileptr);  /* read the first status line to measure its length */
   i = strlen(trial_string) + 1;
   if (fseek(fileptr, -i, SEEK_END) != 0)
     {
      show_error("lezen van file B (tijden)");
      return(-1);
     }
   fgets(trial_string, i+1, fileptr);
   fclose(fileptr);
  }
  else show_error("file niet gevonden");

  strcpy(update_string, trial_string);
  if((train.trial > 0)&&(train.carry_ind > 0))
    {
      if (train.carry_ind > 4) train.carry_ind = 4; 
      train.fort_destruction_times[0] = 
         train.fort_destruction_times[0] + (train.carry_ind*One_Game_Duration*60*20);
      train.carry_ind = 0;
    }

  if ((fileptr = fopen(filename, "at+")) != NULL)
  {
   if(train.trial > 0)
   {
    for(j=0; j < train.trial; j++)
    {
     k=0;
     Update_Trainee_String(update_string, train.probe_flag, k);
     k++;
     Update_Trainee_String(update_string, train.session_ind, k);
     k++;
     Update_Trainee_String(update_string, train.game_ind, k);
     k++;
     Update_Trainee_String(update_string, j+1, k);
     k++;
     Update_Trainee_String(update_string, train.fort_destruction_times[j], k);
     k++;
     Update_Trainee_String(update_string, train.total_points_ind, k);
     fputs(update_string, fileptr);
    }
   }
   fclose(fileptr);
  }
  else show_error("file niet gevonden");

return(0);
}

int Update_Trainee_String(char updstr[MAX_TR_STR], int ind, int i)
{
   char  indstr[7];

   itoa(ind, indstr, 10);

   switch(strlen(indstr))
    {
      case 1:

	updstr[6*i]   = ' ';
	updstr[6*i+1] = ' ';
	updstr[6*i+2] = ' ';
	updstr[6*i+3] = ' ';
	updstr[6*i+4] = indstr[0];
      break;

      case 2:

	updstr[6*i]   = ' ';
	updstr[6*i+1] = ' ';
	updstr[6*i+2] = ' ';
	updstr[6*i+3] = indstr[0];
	updstr[6*i+4] = indstr[1];
      break;

      case 3:

	updstr[6*i] = ' ';
	updstr[6*i+1] = ' ';
	updstr[6*i+2] = indstr[0];
	updstr[6*i+3] = indstr[1];
	updstr[6*i+4] = indstr[2];
      break;

      case 4:

	  updstr[6*i]   = ' ';
	  updstr[6*i+1] = indstr[0];
	  updstr[6*i+2] = indstr[1];
	  updstr[6*i+3] = indstr[2];
	  updstr[6*i+4] = indstr[3];
       break;

      case 5:

	  updstr[6*i]   = indstr[0];
	  updstr[6*i+1] = indstr[1];
	  updstr[6*i+2] = indstr[2];
	  updstr[6*i+3] = indstr[3];
	  updstr[6*i+4] = indstr[4];
       break;

       case 6:

       if (ind <= -9999)
	{
	  updstr[6*i]   = '-';
	  updstr[6*i+1] = '9';
	  updstr[6*i+2] = '9';
	  updstr[6*i+3] = '9';
	  updstr[6*i+4] = '9';
	}
       else
	{
	  updstr[6*i]   = '9';
	  updstr[6*i+1] = '9';
	  updstr[6*i+2] = '9';
	  updstr[6*i+3] = '9';
	  updstr[6*i+4] = '9';
	}
       break;

    }

 return(0);

}


int give_feedback(int points, int slice)
{
   int 			midx, midy, svcolor, svcolor2, Max_FB;
   char			msg[30],
			str[30];
   float		i;
   float 		percentage;
   int                  text_x, text_y,
			dx, dy, dx_old, dy_old;

   svcolor=getcolor();
   if (slice==1)
    {
     setcolor(BLUE);
     setviewport(0, 0, 95, 140, 1);
     midx= 40;
     midy= 70;
     setcolor(RED);
     strcpy(str, "SCORE");
     Max_FB = Max_Points;
     i = scale_to_pi(points, Max_FB, slice);
    }

   if (slice==2)
    {
     setcolor(BLUE);
     setviewport(0, 141, 95, 280, 0);
     midx= 40;
     midy= 210;
     setcolor(RED);
     strcpy(str, "SCORE");
     Max_FB = Max_Points;
     i = scale_to_pi(points, Max_FB, slice);
    }

   dx = (int)(-sin(2*i)*(FB_RADIUS-1));
   dy = (int)(-cos(2*i)*(FB_RADIUS-1));

   if ((Vulner_Counter == 0))
    {
    clearviewport();
    circle(midx, midy, FB_RADIUS+1);
    circle(midx, midy, FB_RADIUS+2);
    svcolor2=getcolor();
    setcolor(TEXT_COLOR);
    text_x = midx - FB_RADIUS + 15;
    text_y = midy - FB_RADIUS - 20;
    moveto(text_x, text_y);
    putimage(text_x, text_y,buffer1, COPY_PUT);
					   /* erase garbage */
    gprintf(&text_x, &text_y, "%s", str);
    moveto(midx, midy);
    setlinestyle(SOLID_LINE, 0xFFFF, THICK_WIDTH);
    lineto(midx, midy - FB_RADIUS);
    setlinestyle(SOLID_LINE, 0xFFFF, NORM_WIDTH);
    setcolor(svcolor2);
    }

    setfillstyle(SOLID_FILL, BLUE);
    fillellipse(midx, midy, FB_RADIUS+1, FB_RADIUS+1);
    moveto(midx, midy);
    linerel(dx, dy);

    setcolor(TEXT_COLOR);
    text_x = midx - FB_RADIUS + 15;
    text_y = midy + FB_RADIUS + 15;
    moveto(text_x, text_y);
    putimage(text_x, text_y,buffer1, COPY_PUT);  /* erase garbage */
    if (Max_FB <= 0) percentage = 100.0;
	else percentage = (float)Max_FB;
    if( points < 0) points = 0;
    percentage =  ((float)points*100)/percentage;
    gprintf(&text_x, &text_y,"%d %s",
	(int)(percentage), "pct.");
    setviewport(Xmargin, 0, Xmargin + MaxX, MaxY, 1);
    setcolor(svcolor);
    return(0);
}


float scale_to_pi(int feedback_points, int feedback_level_ind, int slice)
{

float scaled_points;

if (feedback_level_ind <= 0) feedback_level_ind = 100;
scaled_points =((float)(feedback_points)/(float)feedback_level_ind)* -PI;
if (feedback_points < 0)
{
  scaled_points = 0.0;
  return(scaled_points);
}

if (scaled_points <= -PI) scaled_points = scaled_points + 2*PI;
return(scaled_points);
}


int Ask_For_Foe_Mines()
{

  int		   i=10,j=10,k=10, letter=0;
  TCU_FIELD_VALUE Char_1, Char_2, Char_3;
  int		  Mine_1, Mine_2, Mine_3;
  int             esc_key;

if (train.level_ind > 2)
 {

     tcu_load_form (&screen_form, "mijnen");
     tcu_get_field_id (&screen_form, "Mine_1", &Mine_1);
     tcu_get_field_id (&screen_form, "Mine_2", &Mine_2);
     tcu_get_field_id (&screen_form, "Mine_3", &Mine_3);
     tcu_set_field_mode (&screen_form, Mine_1, TCU_FORM_EDIT);
     tcu_set_field_mode (&screen_form, Mine_2, TCU_FORM_EDIT);
     tcu_set_field_mode (&screen_form, Mine_3, TCU_FORM_EDIT);
     tcu_set_button_fn (&screen_form, button_handler);
     tcu_display_form (&screen_form, 1, 2);

     do
       {
	tcu_edit_form (&screen_form, 1, &esc_key);
	if (esc_key == TCU_FLD_BUTTONESC || esc_key == TCU_FLD_ESCESC)
	if (tcu_get_confirm (25, 10, tcu_colour_attrib (WHITE, RED),
	     tcu_colour_attrib (WHITE, RED),
	     "Stoppen ??: Roep nu eerst de proefleider (N= nee) !")==1)
	     esc_key = TCU_FLD_ESCESC;
	else esc_key = TCU_FLD_ESCNEXT;
	}
     while (esc_key != TCU_FLD_BUTTONESC && esc_key != TCU_FLD_BUTTONSAVE && esc_key != TCU_FLD_ESCESC);


   if (esc_key == TCU_FLD_BUTTONSAVE)
      {
       if ((Char_1.v_string = (char *) malloc(2)) == NULL)
       show_error(" te weinig geheugen voor buffer");
       if ((Char_2.v_string = (char *) malloc(2)) == NULL)
       show_error(" te weinig geheugen voor buffer");
       if ((Char_3.v_string = (char *) malloc(2)) == NULL)
       show_error(" te weinig geheugen voor buffer");
       tcu_get_field(&screen_form, Mine_1, &Char_1);
       tcu_get_field(&screen_form, Mine_2, &Char_2);
       tcu_get_field(&screen_form, Mine_3, &Char_3);
       tcu_remove_form(&mine_mem);

       for(i=0;i<3;i++)
	   {
	    if( ( (int)Char_1.v_string[0]==(int)Foe_Menu[i][0]   ) ||
		( (int)Char_1.v_string[0]==(int)(Foe_Menu[i][0]+32)))
	     {
	     letter++;
	     j = i;
	     }
	   }

       for(i=0;i<3;i++)
	   {
	    if( ( (int)Char_2.v_string[0]==(int)Foe_Menu[i][0]   ) ||
		( (int)Char_2.v_string[0]==(int)(Foe_Menu[i][0]+32)))
	     if (i != j)
	      {
	       letter++;
	       k = i;
	      }
	   }

       for(i=0;i<3;i++)
	   {
	    if( ( (int)Char_3.v_string[0]==(int)Foe_Menu[i][0]   ) ||
		( (int)Char_3.v_string[0]==(int)(Foe_Menu[i][0]+32)))
		     if ((i != j)&&(i != k)) letter++;
	   }

       return(letter);

      }
    else return(-1);
 }

return(-1);
}

int Announce_Foe_Mines()
{
   TCU_NOTICE        notice;

   tcu_prepare_notice (&notice,
		       "Space Fortress III",
		       tcu_colour_attrib (RED, LIGHTGRAY),
		       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       TCU_BOX_DOUBLE);

   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "De Speciale Mijnen zijn:");
   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "  %c      %c     %c", Foe_Menu[0][0], Foe_Menu[1][0], Foe_Menu[2][0]);
   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "druk Enter <ды om te starten");
   tcu_display_notice (&notice, 25, 12);
   tcu_clear_notice (&notice);
   return(0);
}

int Announce_Start_Game()
{
   TCU_NOTICE        notice;

   tcu_prepare_notice (&notice,
		       "SPACE FORTRESS III",
		       tcu_colour_attrib (RED, LIGHTGRAY),
		       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       TCU_BOX_DOUBLE);

   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "druk Enter <ды om te starten");
   tcu_notice_text (&notice, "");
   tcu_display_notice (&notice, 25, 12);
   tcu_clear_notice (&notice);
   return(0);
}