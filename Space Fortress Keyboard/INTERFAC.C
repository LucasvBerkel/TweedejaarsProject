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

#include <stdio.h>
#include <conio.h>
#include <tcu.h>

extern TCU_FORM        screen_form;


int select_action(int sel1, int sel2);
int select_task();


static unsigned char tasks[6] = { 0, 1, 0, 0, 0, 0};
int                  level_indicator = 0;




/* receives parameters r1 and r2 from MAIMENU.C main(), which
   denote pulldown menu selections */

int select_action(int sel1, int sel2)
{



 switch(sel1)
  {
    case 1 :
     {
       switch(sel2)
	{
	  case 2:
	   {
	     select_task();
	     break;
	   }
	   case 4:
	   {
	     tcu_remove_form(&screen_form);
	     if (task_elements(level_indicator)==0)
	      {
	      tcu_display_form(&screen_form,1,2);
	      }
	     break;
	   }
	}
       break;
     }
    case 4 :
     {
       switch(sel2)
	{
	  case 2 :
	   {
	     handle_form("sc_game");
	     break;
	   }
	}
       break;
     }
    case 5 :
     {
       switch(sel2)
	{
	  case 2 :
	   {
	     handle_form("sc_user");
	     break;
	   }
	}
       break;
     }
  }
}






int select_task (void)
{

   int           i;
   int           choice;
   int           opnemen = 5;
   char         *menu_options[] = { "S( ) Schip en Fort" , "M( ) Schip, Fort en Mijnen",
				    "H( ) Hele Spel",  "A( ) Automatisch" , "R( ) Richt-taak",
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
	       menu_options[choice-1][2] = 'o'; /* in the option line in the */
	       level_indicator = choice;
	       }
      tcu_set_menu_option (&tasks_menu, choice-1, 1); /* Force display now */
      }


/* Clear the menu from the screen */

      tcu_remove_menu (&tasks_menu);
      tcu_restore_environment ();                 /* Return the environment */

   return (0);
}

