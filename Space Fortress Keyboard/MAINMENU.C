/* Starting Date : 24-04-'92

   Programmer    : Jan Roessingh

   Module        : MAINMENU.C   A Space Fortress Interface Environment

   Purposes	 : 1) Quick parameter changes, without recompiling;
		   2) Data selection;
		   2) Part-Task selection;
		   3) Trainee simulation;  */


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dir.h>
#include <dos.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <tcu.h>
#include "intrfc.h"
#include "myconst.h"
#include "myext.h"

void far help_handler (int option);
void far p_help_handler (int option);

extern int select_action(int sel1, int sel2);


char               *headings[] = { "TTraining", "OOvergangen", "RReplay",
				   "VTaak-Var.", "UUitvoer", NULL };

TCU_FORM       screen_form;
TCU_PULLDOWN        pd;

void main (void)
{
   TCU_MENU            training_m,
		       overgangen_m,
		       replay_m,
		       taak_var_m,
		       uitvoer_m,
		      *mlist[5];
   TCU_FIELD_VALUE     frm_val;
   int                 message,
                       menu,
                       option,
                       r1,r2;
   char                msg[26];
   
   void far            help_handler (int),
        far            p_help_handler (int);

   char               *training_options[] =
	     {  "IIdentificatie", "TTaak ", "DDuur ", "SStart", NULL };

   char               *overgangen_options[] =
	     { "11 -> 2", "22 -> 3", NULL };

   char               *replay_options[] =
	     { NULL };

   char               *taak_var_options[] =
	     { "SStandaard", "VVerander",  NULL };

   char               *uitvoer_options[] =
	     { NULL };

   tcu_set_mouse_mode (1);

   r1 = tcu_define_menu (&training_m,
			 "Training",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),         /* Title */
			 tcu_colour_attrib (BLACK, WHITE),        /* Box */
			 tcu_colour_attrib (BLACK, WHITE),        /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),          /* Selected */
			 tcu_colour_attrib (DARKGRAY, LIGHTGRAY), /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 training_options,
			 tcu_colour_attrib (RED, WHITE));

   r1 = tcu_define_menu (&overgangen_m,
			 "Deeltaken",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),         /* Title */
			 tcu_colour_attrib (BLACK, WHITE),        /* Box */
			 tcu_colour_attrib (BLACK, WHITE),        /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),          /* Selected */
			 tcu_colour_attrib (DARKGRAY, LIGHTGRAY), /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 overgangen_options,
			 tcu_colour_attrib (RED, WHITE));

   r1 = tcu_define_menu (&replay_m,
			 "en Opnemen",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),         /* Title */
			 tcu_colour_attrib (BLACK, WHITE),        /* Box */
			 tcu_colour_attrib (BLACK, WHITE),        /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),          /* Selected */
			 tcu_colour_attrib (DARKGRAY, LIGHTGRAY), /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 replay_options,
			 tcu_colour_attrib (RED, WHITE));

   r1 = tcu_define_menu (&taak_var_m,
			 "Instellen",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),         /* Title */
			 tcu_colour_attrib (BLACK, WHITE),        /* Box */
			 tcu_colour_attrib (BLACK, WHITE),        /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),          /* Selected */
			 tcu_colour_attrib (DARKGRAY, LIGHTGRAY), /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 taak_var_options,
			 tcu_colour_attrib (RED, WHITE));

   r1 = tcu_define_menu (&uitvoer_m,
			 "van Data",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),         /* Title */
			 tcu_colour_attrib (BLACK, WHITE),        /* Box */
			 tcu_colour_attrib (BLACK, WHITE),        /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),          /* Selected */
			 tcu_colour_attrib (DARKGRAY, LIGHTGRAY), /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 uitvoer_options,
			 tcu_colour_attrib (RED, WHITE));

   tcu_set_menu_help (&overgangen_m, help_handler);
   tcu_set_pulldown_help (&pd, p_help_handler);

   tcu_set_menu_option (&training_m, 3, 0);

   mlist[0] = &training_m;
   mlist[1] = &overgangen_m;
   mlist[2] = &replay_m;
   mlist[3] = &taak_var_m;
   mlist[4] = &uitvoer_m;
   mlist[5] = NULL;

   tcu_define_pulldown    (&pd,
			   tcu_colour_attrib (DARKGRAY, LIGHTGRAY),
			   tcu_colour_attrib (BLACK, LIGHTGRAY),
			   tcu_colour_attrib (BLACK, GREEN),
			   headings,
			   tcu_colour_attrib (RED, LIGHTGRAY),
		           mlist);

   tcu_set_pulldown_help (&pd, p_help_handler);

   tcu_load_form (&screen_form, "pulldown");

   tcu_get_field_id (&screen_form, "menu", &menu);
   tcu_get_field_id (&screen_form, "option", &option);
   tcu_get_field_id (&screen_form, "message", &message);

   tcu_set_field_mode (&screen_form, menu, TCU_FORM_NOENTER);
   tcu_set_field_mode (&screen_form, option, TCU_FORM_NOENTER);
   tcu_set_field_mode (&screen_form, message, TCU_FORM_CONFIRM);

   tcu_display_form (&screen_form, 1, 2);

   do {
      tcu_read_pulldown_selection (&pd, &r1, &r2);
      if (r1) {
         frm_val.v_int = r1;
	 tcu_put_field (&screen_form, menu, &frm_val);
	 frm_val.v_int = r2;
	 tcu_put_field (&screen_form, option, &frm_val);
	 select_action(r1, r2);
	 if (r2) strcpy (msg, "Currently Not in Use");
	 frm_val.v_string = msg;
	 tcu_put_field (&screen_form, message, &frm_val);
      }
   } while (r1);

   tcu_remove_pulldown    (&pd);

   tcu_remove_form (&screen_form);
}

void far help_handler (int option)
{
   TCU_NOTICE        notice;

   tcu_prepare_notice (&notice,
                       "Help",
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       TCU_BOX_SINGLE);

   tcu_notice_text (&notice, "Sorry, no help defined currently");
   tcu_notice_text (&notice, "for option %d.", option);

   tcu_display_notice (&notice, 25, 20);

   tcu_clear_notice (&notice);
}

void far p_help_handler (int option)
{
   TCU_NOTICE       notice;

   tcu_prepare_notice (&notice,
                       "HELP",
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
                       tcu_colour_attrib (BLUE, LIGHTGRAY),
                       TCU_BOX_DOUBLE);

   tcu_notice_text (&notice,
                "Sorry, no help defined currently");
   tcu_notice_text (&notice,
                "for option %s.", headings[option-1]);

   tcu_display_notice (&notice,
                       23, 20);

   tcu_clear_notice (&notice);
}
