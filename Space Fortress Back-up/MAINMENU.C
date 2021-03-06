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
void show_error(char *note);
int test_name_nr(char filename[13]);

extern int make_settings(char filename[13]);
extern int select_action(int *sel1, int *sel2);
extern int far button_handler (TCU_FORM *, int);


char               *headings[] = { "TTraining", "PProefpersoon", "RReplay", NULL };

TCU_FORM            screen_form;
TCU_PULLDOWN        pd;
char                trainee_A[13], trainee_B[13];
TCU_FIELD_VALUE     First_Name;





int main (void)
{





   TCU_MENU            training_m,
		       proefpersoon_m,
		       replay_m,
		      *mlist[3];

   TCU_FIELD_VALUE     frm_val;

   int                 r1,r2;
   char		       msg[26];


   void far            help_handler (int),
	far            p_help_handler (int);

   char               *training_options[] =
	     {  "DDeeltaak ", "SStart", "AAiming", NULL };

   char               *proefpersoon_options[] =
	     { "TToevoegen", "WWijzigen", NULL };

   char               *replay_options[] =
	     { NULL };

   tcu_set_mouse_mode (1);

   r1 = tcu_define_menu (&training_m,
			 "Kiezen Taak",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),     /* Title */
			 tcu_colour_attrib (BLACK, WHITE),         /* Box */
			 tcu_colour_attrib (BLACK, WHITE),         /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),    /* Selected */
			 tcu_colour_attrib (LIGHTGRAY, WHITE),  /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 training_options,
			 tcu_colour_attrib (RED, WHITE));

   r1 = tcu_define_menu (&proefpersoon_m,
			 "Instellen",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),         /* Title */
			 tcu_colour_attrib (BLACK, WHITE),        /* Box */
			 tcu_colour_attrib (BLACK, WHITE),        /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),          /* Selected */
			 tcu_colour_attrib (DARKGRAY, LIGHTGRAY), /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 proefpersoon_options,
			 tcu_colour_attrib (RED, WHITE));

   r1 = tcu_define_menu (&replay_m,
			 "Afspelen",
			 tcu_colour_attrib (BLUE,  LIGHTGRAY),         /* Title */
			 tcu_colour_attrib (BLACK, WHITE),        /* Box */
			 tcu_colour_attrib (BLACK, WHITE),        /* Options */
			 tcu_colour_attrib (BLACK, LIGHTGREEN),          /* Selected */
			 tcu_colour_attrib (DARKGRAY, LIGHTGRAY), /* Deselected */
			 TCU_ESC_ESC | TCU_ESC_CNTL_C | TCU_ESC_PGUP | TCU_ESC_FUNC,
			 TCU_BOX_DOUBLE,
			 replay_options,
			 tcu_colour_attrib (RED, WHITE));


   do delay(10); while (test_name_nr(trainee_A)==0);
   make_settings(trainee_A);
   tcu_set_menu_help (&proefpersoon_m, help_handler);
   tcu_set_pulldown_help (&pd, p_help_handler);
   mlist[0] = &training_m;
   mlist[1] = &proefpersoon_m;
   mlist[2] = &replay_m;
   mlist[5] = NULL;

   tcu_define_pulldown  (&pd,
			   tcu_colour_attrib (DARKGRAY, LIGHTGRAY),
			   tcu_colour_attrib (BLACK, LIGHTGRAY),
			   tcu_colour_attrib (BLACK, GREEN),
			   headings,
			   tcu_colour_attrib (RED, LIGHTGRAY),
			   mlist);

   tcu_set_pulldown_help (&pd, p_help_handler);

   if (train.group_ind == 69)
    {
      do {
	tcu_read_pulldown_selection (&pd, &r1, &r2);
	if (r1) select_action(&r1, &r2);
	 }
      while (r1);
    }
   else
    {
      r1 = 1; r2 = 2;
      select_action(&r1, &r2);
    }



   if (train.group_ind == 69) tcu_remove_pulldown(&pd);
   tcu_remove_form (&screen_form);

   free(First_Name.v_string);

}/* END MAIN *************************************************************/



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

void show_error(char *note)

{
   TCU_NOTICE        notice;

   tcu_prepare_notice (&notice,
		       "FOUT",
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       TCU_BOX_SINGLE);

   tcu_notice_text (&notice, note);

   tcu_display_notice (&notice, 25, 20);

   tcu_clear_notice (&notice);
}


int test_name_nr(char filename[13])
 {



  TCU_FIELD_VALUE trainee_file,
		  file_extens;

  FILE	          *fileptr;
  int		  esc_key,
		  name_nr = 0,  /* is zero if check for name and number failed */
		  Voorn,        /* name for form-field containing first name */
		  Achtern,      /* ..                             surname */
		  length,       /* length of name */
		  Ident_nr;


  char		  ext[3],       /* extension of trainee's file (Id_nr) */
		  *sp_ptr,      /* pointer to strange characters in trainee's name */
		  point[2];     /* point between name and extension */


  tcu_load_form (&screen_form, "pulldown");

  tcu_get_field_id (&screen_form, "Voorn", &Voorn);
  tcu_get_field_id (&screen_form, "Achtern", &Achtern);
  tcu_get_field_id (&screen_form, "Id_nr", &Ident_nr);


  tcu_set_field_mode (&screen_form, Voorn, TCU_FORM_EDIT);
  tcu_set_field_mode (&screen_form, Achtern, TCU_FORM_EDIT);
  tcu_set_field_mode (&screen_form, Ident_nr, TCU_FORM_EDIT);

  tcu_set_button_fn (&screen_form, button_handler);
  tcu_display_form (&screen_form, 1, 2);



  do {
      tcu_edit_form (&screen_form, 1, &esc_key);
      if (esc_key == TCU_FLD_BUTTONESC || esc_key == TCU_FLD_ESCESC)
	if (tcu_get_confirm (25, 10, tcu_colour_attrib (WHITE, RED),
	     tcu_colour_attrib (WHITE, RED),
	     "Stoppen: Bevestigen met Y/N")==1)
	     esc_key = TCU_FLD_ESCESC;
	else esc_key = TCU_FLD_ESCNEXT;
   } while (esc_key != TCU_FLD_BUTTONESC && esc_key != TCU_FLD_BUTTONSAVE && esc_key != TCU_FLD_ESCESC);


 if (esc_key == TCU_FLD_BUTTONSAVE)
   {

    if ((trainee_file.v_string = (char *) malloc(20)) == NULL)
    show_error(" te weinig geheugen voor buffer");

    tcu_get_field(&screen_form, Achtern, &trainee_file);

    sp_ptr=strstr(trainee_file.v_string, "  ");

    strxfrm(sp_ptr, "\0", 1);

    do
    { sp_ptr=strpbrk(trainee_file.v_string," -.'`/\:;""");
      if (sp_ptr) strxfrm(sp_ptr, "X", 1);
    } while (sp_ptr != NULL);

    length=strlen(trainee_file.v_string);
    if (length > 8) length=8;
    strncpy(filename, trainee_file.v_string, length);
    filename[length]='\0';


    strcpy(point,".");
    strncat(filename, point, 1);
    filename[length+1] = '\0';

    if ((file_extens.v_string = (char *) malloc(20)) == NULL)
    show_error(" te weinig geheugen voor buffer");

    if ((First_Name.v_string = (char *) malloc(20)) == NULL)
    show_error(" te weinig geheugen voor buffer");

    tcu_get_field(&screen_form, Voorn, &First_Name);
    tcu_get_field(&screen_form, Ident_nr, &file_extens);

    strcpy(trainee_B, filename);
    strncat(filename, file_extens.v_string, 2);
    strncat(trainee_B, file_extens.v_string, 2);
    strncat(filename, "a", 1);
    strncat(trainee_B, "b", 1);
    filename[length+4]='\0';
    trainee_B[length+4]='\0';
    free(file_extens.v_string);
    free(trainee_file.v_string);
    fileptr = fopen(filename, "rt");
    if (fileptr==NULL)
    show_error("naam of nummer klopt niet");
    else { name_nr=1; fclose(fileptr); }
   }

 else { exit(0); }

 return(name_nr);
}

