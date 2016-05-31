/* Date 	: 24/04/92

   Programmer   : Jan Roessingh

   Module       : Genform.C

   Status	: Link together with MAINMENU.C

   Purpose      : Form-Handling

   Contents     : Routines for
		  1) Loading SF entry forms
		  2) Displaying SF entry forms
		  3) Editing SF-entry forms
		  4) Saving edited forms


*/


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <tcu.h>
#include <alloc.h>
#include <string.h>
#include <process.h>

#include "intrfc.h"

int far button_handler (TCU_FORM *form, int field);
void handle_form(char form_name[MAX_FORM_NAME]);
void write_formdata_into_file(char form_name[MAX_FORM_NAME]);
void read_filedata_into_form(char form_name[MAX_FORM_NAME]);
void error(char *message);



TCU_FORM   	form;


int far button_handler (TCU_FORM *form, int field)
{
   if (field == tcu_get_field_id (form, "quit", NULL))  /* Is it QUIT button? */
      return (2);                          /* Return saying ESCape form */
   else
      return (1);                          /* Otherwise, save & escape */
}


void handle_form(char form_name[MAX_FORM_NAME])

{

   int		     esc_key,
		     x_pos,
		     y_pos;
  
   TCU_FORM_INFO     info;
   int far           button_handler (TCU_FORM *, int);

	      if (tcu_load_form (&form, form_name) == TCU_ERROR) {  /* Load the form from */
	      printf ("Error opening form file\n");         /* the CFO file */
	      exit (1);
	      }

	      tcu_get_form_info (&form, &info);       /* Calculate where to put the */
	      x_pos = (80-info.width)/2+1;            /* form to get it in the middle */
	      y_pos = (25-info.height)/2+1;           /* of the screen */

	      tcu_set_button_fn (&form, button_handler);  /* Set up the button handler */
	      tcu_display_form (&form, x_pos, y_pos);     /* Get form on screen */
	      read_filedata_into_form(form_name);

		   do {
		      tcu_edit_form (&form, 0, &esc_key);
		      if (esc_key == TCU_FLD_BUTTONESC || esc_key == TCU_FLD_ESCESC)
			if (tcu_get_confirm (25, 10, tcu_colour_attrib (WHITE, RED),
			     tcu_colour_attrib (WHITE, RED),
			     "QUIT: Are you sure? (Y/N)")==1)
			     esc_key = TCU_FLD_ESCESC;
			else esc_key = TCU_FLD_ESCNEXT;
		   } while (esc_key != TCU_FLD_BUTTONESC && esc_key != TCU_FLD_BUTTONSAVE && esc_key != TCU_FLD_ESCESC);

	    
  if (esc_key != TCU_FLD_BUTTONESC && esc_key != TCU_FLD_ESCESC)
       {

	     write_formdata_into_file(form_name);
       }
   else
       {
	     tcu_remove_form (&form);
	     tcu_unload_form (&form);               /* Remove from memory */
       }

} /* end handle_form */



void write_formdata_into_file(char form_name[MAX_FORM_NAME])

{
  TCU_FIELD_VALUE   	contents;
  char		     	put_var[12];
  char			*form_rec;
  int               	size_in_bytes,field_id;
  int 			i=0;
  FILE	 		*fileptr;

  
    if ((contents.v_string = malloc(13)) == NULL)
   {
      error("Not enough memory to allocate buffer\n");
      exit(1);  /* terminate program if out of memory */
   }

  tcu_get_field_id(&form, "filename", &field_id);
  tcu_get_field(&form, field_id, &contents);
  strcpy(put_var, contents.v_string);
  fileptr = fopen(put_var, "wt");
  if (fileptr==0) error("error writing file");

  size_in_bytes = tcu_form_record_size(&form);
  

  if ((form_rec = malloc(size_in_bytes)) == NULL)
   {
      error("Not enough memory to allocate buffer\n"); /* terminate program if out of memory */
   }
  else
  {
   if (tcu_write_formrec(&form, form_rec)==TCU_OK )
   for (i=0; i < size_in_bytes; i++) fputc(form_rec[i], fileptr);
   else  error("Writing form-record to buffer");
   free(form_rec);
  }

   tcu_remove_form (&form);
   tcu_unload_form (&form);

   fclose(fileptr);


} /* end write_formdata_into_file */



void read_filedata_into_form(char form_name[MAX_FORM_NAME])

{
   TCU_FIELD_VALUE   	contents;
   TCU_NOTICE           notice;
   char		     	put_var[12];
   int               	field_id;
   FILE	 		*fileptr;
   char			*form_rec;
   int               	size_in_bytes;
   int 			i=0;

    if ((contents.v_string = malloc(13)) == NULL)
   {
      printf("Not enough memory to allocate buffer\n");
      exit(1);  /* terminate program if out of memory */
   }


  tcu_get_field_id(&form, "filename", &field_id);
  tcu_get_field(&form, field_id, &contents);
  strcpy(put_var, contents.v_string);
  fileptr = fopen(put_var, "rt");
  if (fileptr==0)
	{
	 tcu_prepare_notice (&notice,
		       "Error",
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       TCU_BOX_SINGLE);

	tcu_notice_text (&notice, "File not found");
	tcu_notice_text (&notice, "%s", put_var);

	tcu_display_notice (&notice, 1, 2);

	tcu_clear_notice (&notice);
	}

    if ((form_rec = malloc(size_in_bytes)) == NULL)
   {
      error("Not enough memory to allocate buffer\n");
      exit(1);  /* terminate program if out of memory */
   }
  else for (i=0; i < size_in_bytes; i++) form_rec[i]=fgetc(fileptr);

  if (tcu_read_formrec(&form, form_rec)==TCU_ERROR)
     error("reading form-record to buffer");

  
  free(form_rec);
  free(contents.v_string);
  fclose(fileptr);


} /* end read_filedata_into_form */



void error(char *message)
{
printf("ERROR: %s\n", message);
exit(0);
}