#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dir.h>
#include <io.h>
#include <tcu.h>


extern TCU_FORM TASKER;                        /* Linked-in form object */

static unsigned char taken[5] = { 1, 0, 0, 1, 0 }; /* Memory models to use */
static int           dirs[5];                  /* Field IDs of the directory */
                                               /* fields in the form */
void select_task (void)
{
   TCU_WINDOW      screen;
   TCU_NOTICE      notice;
   TCU_FORM        form;
   int             do_cpp,
                   fkey,
                   i, j;
   char            copylib[10],
                   dev[3],
                   dir[65],
                   direct[65],
                   field_name[7],
                  *mmname[] = { "Small", "Compact", "Medium", "Large", "Huge" },
                   tc_include[81],
                   tc_lib[81];
   TCU_FIELD_VALUE val;

   int         far task_handler   (TCU_FORM *, int),

               far fn_keys          (TCU_FORM *, int, int);
   int             checkout         (void),
                   create_directory (char *),
                   merge_libs       (char *, char *);

   void            copy_file        (char *, char *),
               far help             (TCU_FORM *, int);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* Load the linked-in form into the form object */

   tcu_load_image_form (&form, &TASKER);

   tcu_set_form_fnkey_fn (&form, fn_keys);     /* Function key handler */

   tcu_set_button_fn (&form, task_handler);  /* Button field handler */
   tcu_set_form_help (&form, help);            /* Help function */


/* Prevent escape keys quitting form entry */

   tcu_set_form_mode (&form, TCU_FORM_NOESCS);


/* Show the form and edit.  Only funtion key handler has the ability to */
/* stop the edit as escape keys have been disabled. */

   tcu_display_form (&form, 4, 6);

   tcu_edit_form (&form, 1, &fkey);


/* We can remove the form from the screen now that we've finished with it */

   tcu_remove_form (&form);


/* If an abort was requested, shut down... */

   if (fkey == TCU_FLD_FNKEYESC) {
      tcu_unload_form (&form);
      return;
   }

   while (!kbhit()); getch();
   tcu_unload_form (&form);
}



/* FUNCTION KEY HANDLER */

int far fn_keys (TCU_FORM *form, int field, int key)
{
   switch (key) {
      case 9:      tcu_save_environment ();
                   if (tcu_get_confirm (24, 11,
                                        tcu_colour_attrib (WHITE, MAGENTA),
                                        tcu_colour_attrib (WHITE, MAGENTA),
					"Stoppen (Y/N)?")) {
		      tcu_restore_environment ();
		      return (2);    /* 2 = Escape request */
		   } else {
		      tcu_restore_environment ();
		      return (0);    /* Ignore... */
		   }
      case 10:     return (1);       /* 1 = Save request */
      default:     tcu_warnbeep();
                   return (0);       /* 0 = Ignore */
   }
#pragma warn -par
}
#pragma warn .par




/* BUTTON FIELD HANDLER */

int far task_handler (TCU_FORM *form, int field)
{
   int           choice,
		 ttaken;
   char         *menu_options[] = { "S  Schip en Fort" , "M  Schip, Fort en Mijnen",
				    "H  Hele Spel",  "A  Automatisch" , NULL };
   TCU_MENU      taken_menu;


   tcu_save_environment ();    /* Save the environment because we're going */
                               /* to alter the screen here */

   tcu_get_field_id (form, "TTAKEN", &ttaken);  /* Get button field ID */

   if (field == ttaken) {     /* Should be because it's the only button! */

      tcu_define_menu (&taken_menu, "Taak Selectie", /* Create the menu */
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

      tcu_display_menu (&taken_menu, 25, 10);


/* Get selections until ESC is pressed */

      while ((choice=tcu_read_menu_selection (&taken_menu)) > 0) {
	 taken[choice-1] ^= 1;                /* Toggle selection */
	 if (taken[choice-1])                 /* Set the selection marker */
            menu_options[choice-1][1] = 'û';   /* in the option line in the */
         else                                  /* menu. */
            menu_options[choice-1][1] = ' ';
	 tcu_set_menu_option (&taken_menu, choice-1, 1); /* Force display now */
      }


/* Clear the menu from the screen */

      tcu_remove_menu (&taken_menu);
   }

   tcu_restore_environment ();                 /* Return the environment */

   return (0);
}



/* HELP FUNCTION */

void far help (TCU_FORM *form, int field)
{
   TCU_NOTICE         notice;


/* Get the notice ready */

   tcu_prepare_notice (&notice, "Help!",
                       tcu_colour_attrib (WHITE, MAGENTA),
                       tcu_colour_attrib (WHITE, MAGENTA),
                       tcu_colour_attrib (WHITE, MAGENTA),
                       TCU_BOX_SINGLE);


/* Write the appropriate text into the notice for the selected field */



   if (field == tcu_get_field_id (form, "TTAKEN", NULL)) {
      tcu_notice_text (&notice, "Kies met de '='-toets of de muis om een menu");
      tcu_notice_text (&notice, "met taken te openen.  Maak uit dit menu ‚‚n ");
      tcu_notice_text (&notice, "keuze voor je training. Deze keuze kan je straks");
      tcu_notice_text (&notice, "via 'Start' laden.");
   } else {

      tcu_notice_text (&notice, "Sorry, hier geen hulp !");
   }


/* Finish off notice, display, clear and exit */

   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "          -- Press RETURN to continue --");
   tcu_display_notice (&notice, 16, 17);

   tcu_clear_notice (&notice);
}





