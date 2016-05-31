#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <graphics.h>
#include<dir.h>
#include<stdlib.h>
#include <string.h>
#include <io.h>


#include "myconst.h"
#include "myext.h"

void change_atrib(int x,int y,int count,int del,char attr);
void user_ident_screen(void);
void user_ident_edit(void);
int readkey(void);
void wait_diskette(void);
void read_input(int ypos,char a[MAXCHAR]);
int destroy_session(char string[10]);
void delete_char(int i,int ypos,int xoffset,int yoffset);
void draw_rectangle(int x,int y);
int check_diskette(void);

int write_file(char filename[],int first_time,int sess);
int read_file(char filename[],int displ,int sess);
void print_header(void);
void displ_screen(int scr_no,char *scr_file);
int display_files( char filename[MAX_DIR_PATH],int sess);
char *select_file(int max_line);
void review_all_files(char name[10],int sess);
void change_atrib(int x,int y,int count,int del,char attr);

struct text_info scr_inf;
typedef char name[MAXCHAR];
name  files_array[MAXFILES];

/**************************************************************************/
/*                                                                        */
/* user_dialogue -Used only for the training session.                     */
/*		  Asks for the user name and ID and decides if to continue*/
/*                the session or to go back to the main menu              */
/* Called by the main                                                     */
/**************************************************************************/

void user_dialogue()
{
  int i,correct;
  gettextinfo(&scr_inf);
  textattr(scr_inf.attribute);

  i=0;correct=0;
  while((i<3)&&(!correct)&&(!go_back))
  {
    wait_diskette();
    user_ident_screen();
    user_ident_edit();
    if(!go_back)
     {
      if (check_id()==0) correct=1;
      else  if (i<2){
	  clrscr();
	  draw_rectangle(12,10);
	  gotoxy(14,12);printf("The name on the diskette does not match trainee's name");
	  gotoxy(14,13);printf("Would you like to try again (Y/N) ?");
	  if (yesno()==0) {destroy_data();correct=1;}
	  }
      i++;
    }
  }
  if ((!correct)&&(!go_back)) destroy_data();

}/* user_dialogue  */

/**************************************************************************/
/*                                                                        */
/* user_ident_screen - Display the user identification screen by reading  */
/*                    the file SCREEN.DAT                                 */
/* Called by user_dialogue                                                */
/*                                                                        */
/**************************************************************************/

void user_ident_screen(void)
 {
  int i;
  clrscr();
  displ_screen(2,"screen.dat");
  gotoxy(6,23);printf("%c",24);
  gotoxy(17,23);printf("%c",25);
 }/*user_ident_screen */

/**************************************************************************/
/*                                                                        */
/* user_ident_edit - Enables the user to type the name and ID number      */
/* Called by user_dialogue                                                */
/*                                                                        */
/**************************************************************************/

void user_ident_edit(void)
 {
  int keyp,ypos,xoffset,yoffset,i,last_ypos,count,del;
  char a[MAXCHAR];
  char ch;

  gettextinfo(&scr_inf);
  textattr(scr_inf.attribute);

  xoffset=45;
  yoffset=11;
  ypos=1;
  gotoxy(xoffset,yoffset);
  del=0;
  count=MAXCHAR;
  go_back=0;
  change_atrib(xoffset,yoffset,count,del,0x70);
  do{
    strcpy(a,"              ");
    last_ypos=wherey();
    switch (keyp=readkey())
	       {
/* cursor up */   case 328: ypos=(ypos+6)%8;
			   gotoxy(xoffset,yoffset+ypos-1);break;

/* cursor down */ case 336: ypos=(ypos+2)%8;
			   gotoxy(xoffset,yoffset+ypos-1);break;

/* ESC */         case 27:/* strcpy(a,"               ");
			   for (i=1;i<6;i+=2)
			     read_input(i-1,a);
			     */
			   go_back=1;
			   return;

/* CR */          case 13:
			  if (ypos==7) return;
			  gotoxy(xoffset,yoffset+ypos-1);
			  textattr(scr_inf.attribute);
			  ypos=(ypos+2)%8;
			  gotoxy(xoffset,yoffset+ypos-1);break;

/* TAB */         case 9 :
			   del=1;
			   change_atrib(xoffset,last_ypos,count,del,0x70);
			   del=0;
			   strcpy(a,"              ");
			   read_input(ypos-1,a);
			   break;

		  default : if (ypos!=7)
			    {
			    if (isalnum(keyp))
			     if ((ypos==5)&&(isdigit(keyp)))
			       {
				   a[0]=(char)keyp;
				   cprintf("%c",a[0]);
				   i=1;
				   while(((ch=getch())!=13)&&(i<3))
/*  write number */                   if(isdigit(ch)){
					  cprintf("%c",ch);
					  a[i++]=ch;}
/* delete character */		      else if ((ch==8)&&(i>0)){
					  delete_char(i,ypos,xoffset,yoffset);
					  a[i--]=32;}
				  a[i]=0;
				  if(i==0) printf("0");
				  gotoxy(xoffset,yoffset+ypos-1);
				  textattr(scr_inf.attribute);
				  ypos=(ypos+2)%8;
				  gotoxy(xoffset,yoffset+ypos-1);
				  }
			    else if((ypos!=5)&&(isalpha(keyp)))
			      {
				 a[0]=(char)keyp;
				 cprintf("%c",a[0]);
				 i=1;
				 count=MAXCHAR;
				 while(((ch=getch())!=13)&&(i<MAXCHAR))
/*  write alpha character */                  if(isalpha(ch)){
						   cprintf("%c",ch);
						   a[i++]=ch;}
/* delete character */		              else if ((ch==8)&&(i>0)){
						  delete_char(i,ypos,xoffset,yoffset);
						  a[i--]=32;}
				 a[i]=0;
				 ypos=(ypos+2)%8;
				 gotoxy(xoffset,yoffset+ypos-1);
			      }
			    gotoxy(xoffset,yoffset+ypos-1);
			    textattr(scr_inf.attribute);
			    read_input(ypos-3,a);
			    }
			    break;
		 }
	change_atrib(xoffset,last_ypos,count,del,scr_inf.attribute);
	change_atrib(xoffset,yoffset+ypos-1,count,del,0x70);
  }
   while(keyp!=27);
   textattr(scr_inf.attribute);

   } /*user_ident_edit */

/**************************************************************************/
/*                                                                        */
/* wait diskette - Waites for the diskette to be inserted in drive a:     */
/* Called by user_dialogue                                                */
/*                                                                        */
/**************************************************************************/

void wait_diskette(void)
{
   int i;
   clrscr();

  gotoxy(8,10);printf("ษอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป");
  for (i=11;i<14;i++){
    gotoxy(8,i);printf("บ                                                                 บ");
    }
  gotoxy(8,14);printf("ศอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ");
   gotoxy(14,12);
   printf("Please insert trainee's diskette in drive A: and press %c%c%c",17,196,217);
   gotoxy(10,20);
   while ((readkey()!=13)||(check_diskette()==-1));
   }

/**************************************************************************/
/*                                                                        */
/* check_diskette - checks if the diskette is inserted in drive a:        */
/* Called by wait_diskette                                                */
/*                                                                        */
/**************************************************************************/

int check_diskette(void)
 {
  union REGS regs;
  int ret;
  regs.h.ah=0x1c;
  regs.h.dl=1;
  ret=intdos(&regs,&regs);
/*  if (regs.h.al==0xFF) return(-1);*/
  if(ret==0xFF) return(-1);
  return(0);
  }  /*check diskette */


/**************************************************************************/
/*                                                                        */
/* change_atrib - Changes the color of the foreground and background by   */
/*                writing directly to the memory to the screen address    */
/* Parameters:                                                            */
/*    x-x coordinate                                                      */
/*    y-y coordinate                                                      */
/*    count - number of characters                                        */
/*    del - indicator for erasing the character or just change the color  */
/*                                                                        */
/**************************************************************************/

void change_atrib(int x,int y,int count,int del,char attr)
 {
  int i,offset;
  offset=2*((y-1)*80 + (x-1)) + 1;
  for(i=0;i<count;i++){
    if(del==1) poke(scr_addr,offset-1,(int)(0x20+(attr<<8))); /* to delete charactre */
    else pokeb(scr_addr,offset,attr); /* to change color */
    offset=offset+2;
   }
 } /* change_atrib*/


/**************************************************************************/
/*                                                                        */
/* delete_char  - Deletes one character                                   */
/* Parameters:                                                            */
/*    i-character index                                                   */
/*    ypos-y coordinate                                                   */
/*    xofset- begining of the text(x-coordinate)                          */
/*    yoffset-begining of the text(y-coordinate)                          */
/*                                                                        */
/**************************************************************************/

void delete_char(int i,int ypos,int xoffset,int yoffset)
 {
  int del;
  char ch;
  ch=8;
  cprintf("%c",ch);
  textattr(scr_inf.attribute);
  del=1;
  change_atrib(xoffset+i-1,yoffset+ypos-1,1,del,0x70);
  del=0;
  gotoxy(xoffset+i-1,yoffset+ypos-1);
  }


/**************************************************************************/
/*                                                                        */
/* read_input - Updates the global variables header.Trainee_First_Name    */
/*               header.Trainee_Last_Name,Trainee_ID                      */
/* Parameters:                                                            */
/*    ypos-y coordinate                                                   */
/*    a - updated string                                                  */
/* Called by user_ident_edit                                              */
/*                                                                        */
/**************************************************************************/

void read_input(int ypos,char a[MAXCHAR])
 {
 switch(ypos){
   case 0:strcpy(header.Trainee_First_Name,a);break;
   case 2:strcpy(header.Trainee_Last_Name,a);break;
   case 4:header.Trainee_ID=atoi(a);break;
   }
 }/* read_input*/


/**************************************************************************/
/*                                                                        */
/* char ask_session- ask the session number                               */
/* Parameters:                                                            */
/*    filename - File name(SESS\AIM)                                      */
/*    string - Parameter for the text                                     */
/* Return - New file name                                                 */
/* Call by main                                                           */
/*                                                                        */
/**************************************************************************/

char *ask_session_nr(char filename[],char string[])
 {
 char str[3],temp_string[10],ch;
 char temp_filename[MAX_DIR_PATH];
 int i,del,count,xpos,ypos,correct;

  correct=0;
  while (!correct)
  {
  clrscr();
  gotoxy(17,10);printf("ษออออออออออออออออออออออออออออออออออออออออออออป");
  for (i=11;i<14;i++){
    gotoxy(17,i);printf("บ                                            บ");
    }
  gotoxy(17,14);printf("ศออออออออออออออออออออออออออออออออออออออออออออผ");
  gotoxy(20,12);printf("Please type < New %s number >: ",string);
  del=1;
  count=MAXINT;
  xpos=wherex();
  ypos=wherey();
  change_atrib(xpos,ypos,count,del,0x70);
  del=0;
  i=0;
  strcpy(str,"   ");
  strcpy(temp_filename,filename);
  while(((ch=getch())!=13)&&(i<MAXINT))
/*  write number */      if(isdigit(ch)){
				cprintf("%c",ch);
				str[i++]=ch;}
/* delete character */   else if ((ch==8)&&(i>0)){
				 cprintf("%c",ch);
				 textattr(scr_inf.attribute);
				 del=1; count=1;
				 change_atrib(xpos+i-1,ypos,count,del,0x70);
				 del=0;
				 gotoxy(xpos+i-1,ypos);
				 str[i--]=32;}
			 str[i]=0;
			 if(i==0) printf("0");
			 gotoxy(xpos,ypos);
			 textattr(scr_inf.attribute);
			 change_atrib(xpos,ypos,count,del,0x70);
  header.Session_Number=atoi(str);
  strcpy(temp_filename,strcat(temp_filename,itoa(header.Session_Number,temp_string,10)));
  if (searchpath(temp_filename)!=NULL){
	  draw_rectangle(12,18);
	  gotoxy(23,20);printf("%s file # %d allready exists.",string,header.Session_Number);
	  gotoxy(23,21);printf("Would you like to try again (Y/N) ?");
	  if (yesno()==0) if (!destroy_session(string)) correct=0;
			  else correct=1;
	  else correct=0;
	  }
  else
  correct=1;
  }
  return(temp_filename);
}


/**************************************************************************/
/*                                                                        */
/* int destroy_session - Ask the user if to erase the data file of the    */
/*                     	 specific session                                 */
/* Parameters:                                                            */
/*    string - Parameter for the text                                     */
/* Return - 0 - To erase                                                  */
/*          1 - Not to erase                                              */
/* Called by ask_session                                                    */
/*                                                                        */
/**************************************************************************/

int destroy_session(char string[])
      {
	   draw_rectangle(12,18);
	   gotoxy(20,20);printf("The old %s  file will be destroyded!!!",string);
	   gotoxy(20,21);printf("Please approve file destruction (Y/N): ");
	   if (yesno()==0) return(0);
	   return(1);
       } /*destroy_session*/


/**************************************************************************/
/*                                                                        */
/*     draw_rectangle - Draw rectangle                                    */
/* Parameters:                                                            */
/*    x-x coordinate                                                      */
/*    y-y coordinate                                                      */
/*                                                                        */
/**************************************************************************/

void draw_rectangle(int x,int y)
	{
   gotoxy(x,y);printf("ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ");
   gotoxy(x,y+1);printf("ณ                                                         ณ");
   gotoxy(x,y+2);printf("ณ                                                         ณ");
   gotoxy(x,y+3);printf("ณ                                                         ณ");
   gotoxy(x,y+4);printf("ณ                                                         ณ");
   gotoxy(x,y+5);printf("ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู");
}/* draw_rectangle */


/**************************************************************************/
/*                                                                        */
/* review_all_files - Display one of the directories(SESSION\AIMING)      */
/* Parameters:                                                            */
/*    name- file name                                                     */
/*    sess-indicator for SESS\AIM                                         */
/*                                                                        */
/**************************************************************************/

void review_all_files(char name[],int sess)
{
  int max_files;
  clrscr();
  displ_screen(1,"screen.dat");
  gotoxy(2,23);printf("%c",24);
  gotoxy(11,23);printf("%c",25);
  gotoxy(22,23);printf("%c%c",17,196);
  gotoxy(33,23);printf("%c%c",196,16);
  gotoxy(47,23);printf("%c%c%c",17,196,217);
  max_files=display_files(name,sess);
  strcpy(select,select_file(max_files));
}


/**************************************************************************/
/*                                                                        */
/* int display_files - Display one of the directories(SESSION\AIMING)     */
/* Parameters:                                                            */
/*    sess- file name                                                     */
/*    ses-indicator for SESS\AIM                                          */
/* Returns the maximum number of files                                    */
/* Called by review_all_files                                             */
/*                                                                        */
/**************************************************************************/

int display_files( char sess[],int ses)
{
 struct ffblk ffblk;
 int done,i,j,max_files;
 char temp_str[MAX_DIR_PATH];

 strcat(sess,"*.*");
 done=findfirst(sess,&ffblk,0);
 gotoxy(29,3);printf("%s",header.Trainee_First_Name);
 gotoxy(29,4);printf("%s",header.Trainee_Last_Name);
 gotoxy(29,5);printf("%d",header.Trainee_ID);
 j=10; i=6; max_files=0;
 while (!done)
  {
   if (j==20) {j=10;i=44;max_files=10;}
   gotoxy(i,j);printf("%s",ffblk.ff_name);
   strcpy(files_array[max_files],ffblk.ff_name);

   strcpy(temp_str,path);
   strcat(temp_str,ffblk.ff_name);
   read_file(temp_str,0,ses);

   gotoxy(i+22,j);
   done=findnext(&ffblk);
   gotoxy(i+22,j);printf("%d-%d-%d",header.File_Date[1],header.File_Date[0],header.File_Date[2]);
   j++; max_files++;
   }
   if (max_files >20) {
       max_files=20;
       gotoxy(50,4);printf("%c Too many files !!!",7);
       }
   return(max_files);
}


/**************************************************************************/
/*                                                                        */
/* int select_files - Selects the file to be displayed                    */
/* Parameters:                                                            */
/*    max_files maximum number of files                                   */
/* Returns the selected file                                              */
/* Called by review_all_files                                             */
/*                                                                        */
/**************************************************************************/

char *select_file(int max_files)
{
  int xpos,ypos,xoffset1,xoffset2,yoffset,last_xpos,last_ypos;
  int count,del,keyp,lines1,lines2;
  char *select;

  struct text_info scr_inf;
  gettextinfo(&scr_inf);


  lines1=min(max_files,10);
  if (lines1==0) lines1=1;
  lines2=min(max_files-10,10);
  xoffset1=6;xoffset2=44;
  yoffset=10;
  xpos=0;ypos=0;
  gotoxy(xoffset1,yoffset);
  del=0;
  count=10;
  change_atrib(xoffset1,yoffset,count,del,0x70);
  do{
    last_ypos=wherey();
    last_xpos=wherex();
    switch (keyp=readkey())
	       {
/* cursor up */   case 328: if (wherex()==xoffset1) ypos=(ypos+lines1-1)%lines1;
			    else ypos=(ypos+lines2-1)%lines2;
			    gotoxy(wherex(),yoffset+ypos);break;

/* cursor down */ case 336: if (wherex()==xoffset1) ypos=(ypos+1)%lines1;
			    else ypos=(ypos+1)%lines2;
			    gotoxy(wherex(),yoffset+ypos);break;

/* cursor left */ case 331: if (max_files>10){
				    if(wherex()==xoffset1) xpos=xoffset2;
				    else xpos=xoffset1;
				    ypos=0;
				    gotoxy(xpos,yoffset);}
				    break;

/* cursor write*/ case 333: if(max_files>10){
				    if(wherex()==xoffset1) xpos=xoffset2;
				    else xpos=xoffset1;
				    ypos=0;
				    gotoxy(xpos,yoffset);}
				    break;

/* ESC */         case 27:  return("");

/* CR */          case 13:  if (wherex()==xoffset1) select=files_array[ypos];
			    else select=files_array[ypos+10];
			    textattr(scr_inf.attribute);
			    return(select);


		  default:;
	       }
    change_atrib(last_xpos,last_ypos,count,del,scr_inf.attribute);
    change_atrib(wherex(),wherey(),count,del,0x70);
    }
    while(keyp!=27);
    return(select);
}/*select_file */


/**************************************************************************/
/*                                                                        */
/* review_one file-displays the data of the selected file                 */
/* Parameters:                                                            */
/*    filename-selected file name                                         */
/*    sess-indicator if is SESS\AIM                                       */
/* Called by main                                                         */
/*                                                                        */
/**************************************************************************/

void review_one_file(char filename[MAX_DIR_PATH],int sess)
{
  clrscr();
  if (sess==1)
   {
    displ_screen(3,"screen.dat");
    read_file(filename,1,1);
   }
   else if (!sess)
   {
    displ_screen(5,"screen.dat");
    read_file(filename,1,0);
   }
  while (readkey()!=27);
}/*review_one_file*/

/**************************************************************************/
/*                                                                        */
/* displ_screen Reads the file"SCREEN.DAT"                                */
/* Parameters:                                                            */
/*    scr_no  -number of screen to be read from the file                  */
/*    scr_file -file name("SCREEN.DAT")                                   */
/*                                                                        */
/**************************************************************************/

void displ_screen(int scr_no,char *scr_file)
{
 FILE *f;
 int ch,i;
 if(((f=fopen(scr_file,"r"))==NULL)) {printf("Cannot open %s\n",scr_file);getch();return;}
 i=0;
 while(i<scr_no)
     if ((ch=getc(f))=='@') i++;
 while (((ch=getc(f))!='@')&&(ch!=EOF))
     putchar(ch);
 fclose(f);
} /*displ_screen*/


/**************************************************************************/
/*                                                                        */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/**************************************************************************/

int introduce_data(char filename[],int sess)
 {
   int first_time,i,m;

    m=0;
    clrscr();
    gotoxy(20,10);
    printf("max games :");scanf("%d",&m);
    first_time=1;
    for (i=0;i<m;i++){
    if (sess==1)
    {
       printf("Introduce data :\n");

	   scanf("%d",&game.Score);
	   scanf("%d",&game.Points);
	   scanf("%d",&game.Velocity);
	   scanf("%d",&game.Control);
	   scanf("%d",&game.Speed);
	   scanf("%d",&game.No_Of_Bonus_Intervals);
	   scanf("%d",&game.No_Of_Points_Bonus_Taken);
	   scanf("%d",&game.No_Of_Missiles_Bonus_Taken);
	   scanf("%d",&game.No_Of_Ship_Damaged_By_Fortress);
	   scanf("%d",&game.No_Of_Times_Ship_Damaged_By_Mines);
	   scanf("%d",&game.No_Of_Times_Fortress_Distroyed_By_Trainee);
	   scanf("%d",&game.Normal_Game_Termination);
	   write_file(filename,first_time,1);
	   first_time=0;
	   }
	else if (!sess)
	 {
	  printf("Introduce data :\n");
	  scanf("%d",&Aiming_Game_Results.mines);
	  scanf("%d",&Aiming_Game_Results.speed);
	  scanf("%d",&Aiming_Game_Results.score);
	  write_file(filename,first_time,0);
	  first_time=0;
	  }
      }
   return(0);
 } /* introduce data */

/**************************************************************************/
/*                                                                        */
/* write_file - update a file                                             */
/* Parameters:                                                            */
/*    filename-file name                                                  */
/*    first_time- indicates if the file was opened for the first time     */
/*    sess-indicates if is a SESS\AIM file                                */
/* Returns 0- success                                                     */
/*        -1- error                                                       */
/**************************************************************************/

   int write_file(char filename[],int first_time,int sess)  /* save in the file */
   {
    long a;
    struct ftime ftimep;

   FILE *f1;
   if (first_time==1){
	    if ((f1=fopen(filename,"wb"))==NULL) return(-1);
	    getftime(fileno(f1),&ftimep);
	    header.File_Date[0]=ftimep.ft_day;
	    header.File_Date[1]=ftimep.ft_month;
	    header.File_Date[2]=ftimep.ft_year;
	    if(fwrite(&header,sizeof(header),1,f1)!=1) {fclose(f1);return(-1);}
	    }
   else     if ((f1=fopen(filename,"ab"))==NULL) return(-1);
   fseek(f1,sizeof(header),SEEK_SET);
   if (sess==1)
       {
	if(fwrite(&game,sizeof(game),1,f1)!=1)
	    {fclose(f1);
	    return(-1);}
	}
   else if (sess==0)
	if(fwrite(&Aiming_Game_Results,sizeof(Aiming_Game_Results),1,f1)!=1)
	    {fclose(f1);
	     return(-1);}


   fclose(f1);
   return(0);
   }/*write file */

/**************************************************************************/
/*                                                                        */
/* read_file - Reads a file                                               */
/* Parameters:                                                            */
/*    filename-file name                                                  */
/*    displ- indicates if the read data should be displayed or not        */
/*    sess-indicates if is a SESS\AIM file                                */
/* Returns 0- success                                                     */
/*        -1- error                                                       */
/* Updates the global variables header and game                           */
/* Called by Run Session                                                  */
/**************************************************************************/

   int read_file(char filename[],int displ,int sess)
   {
   long a;
   int b,x;


   FILE *f1;

   if ((f1=fopen(filename,"rb"))==NULL) return(-1);
   fseek(f1,0L,SEEK_SET);
   if(fread(&header,sizeof(header),1,f1)!=1) {fclose(f1);return(-1);}
   if (displ==1) print_header();
   x=10;
   while(!feof(f1)){
       if (sess==1)
       {
	 if(fread(&game,sizeof(game),1,f1)!=1) {fclose(f1);return(-1);}
	 if (displ==1)
	 {
	   gotoxy(x,12);
	   printf("%d",game.Score);
	   gotoxy(x,13);
	   printf("%d",game.Points);
	   gotoxy(x,14);
	   printf("%d",game.Velocity);
	   gotoxy(x,15);
	   printf("%d",game.Control);
	   gotoxy(x,16);
	   printf("%d",game.Speed);
	   gotoxy(x,17);
	   printf("%d",game.No_Of_Bonus_Intervals);
	   gotoxy(x,18);
	   printf("%d",game.No_Of_Points_Bonus_Taken);
	   gotoxy(x,19);
	   printf("%d",game.No_Of_Missiles_Bonus_Taken);
	   gotoxy(x,20);
	   printf("%d",game.No_Of_Ship_Damaged_By_Fortress);
	   gotoxy(x,21);
	   printf("%d",game.No_Of_Times_Ship_Damaged_By_Mines);
	   gotoxy(x,22);
	   printf("%d",game.No_Of_Times_Fortress_Distroyed_By_Trainee);
	   x=x+7;
	 }
	}
       else if(sess==0)
	{
	 if(fread(&Aiming_Game_Results,sizeof(Aiming_Game_Results),1,f1)!=1) {fclose(f1);return(-1);}
	 if (displ==1)
	  {
	    gotoxy(x,12);
	    printf("%d",Aiming_Game_Results.mines);
	    gotoxy(x,14);
	    printf("%d",Aiming_Game_Results.speed);
	    gotoxy(x,16);
	    printf("%d",Aiming_Game_Results.score);
	    x+=7;
	  }
	}
       }
 fclose(f1);
 return(0);
 }
/**************************************************************************/
/*                                                                        */
/* print_header- prints the header(name,ID,Session_Number...)            */
/* Called by read_file                                                    */
/**************************************************************************/

void print_header(void)
{
   gotoxy(29,3);
   printf("%s",header.Trainee_First_Name);
   gotoxy(29,4);
   printf("%s",header.Trainee_Last_Name);
   gotoxy(29,5);
   printf("%d",header.Trainee_ID);
   gotoxy(27,8);
   printf("%d",header.Session_Number);
   gotoxy(63,8);
   printf("%d",header.One_Game_Duration);
 }/*printheader*/
