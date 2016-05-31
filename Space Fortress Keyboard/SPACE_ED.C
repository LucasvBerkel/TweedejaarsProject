#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <graphics.h>

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

struct text_info scr_inf;

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

void user_ident_screen(void)
 {
  int i;
  clrscr();
  displ_screen(2,"screen.dat");
  gotoxy(6,23);printf("%c",24);
  gotoxy(17,23);printf("%c",25);
 }/*user_ident_screen */

void user_ident_edit(void)
 {
  int keyp,xpos,ypos,xoffset,yoffset,i,last_ypos,count,del;
  char a[MAXCHAR];
  char ch;

  gettextinfo(&scr_inf);
  textattr(scr_inf.attribute);

  xoffset=45;
  yoffset=11;
  xpos=0;ypos=1;
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

int check_diskette(void)
 {
  union REGS regs;
  int ret;
  regs.h.ah=0x1c;
  regs.h.dl=1;
  ret=intdos(&regs,&regs);
/*  if (regs.h.al==0xFF) return(-1);*/
  if(ret==0xFF) return(-1);
  }  /*check diskette */




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


void read_input(int ypos,char a[MAXCHAR])
 {
 switch(ypos){
   case 0:strcpy(header.Trainee_First_Name,a);break;
   case 2:strcpy(header.Trainee_Last_Name,a);break;
   case 4:header.Trainee_ID=atoi(a);break;
   }
 }/* read_input*/



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

int destroy_session(char string[])
      {
	   draw_rectangle(12,18);
	   gotoxy(20,20);printf("The old %s  file will be destroided!!!",string);
	   gotoxy(20,21);printf("Please approve file destruction (Y/N): ");
	   if (yesno()==0) return(0);
	   return(1);
       } /*destroy_session*/

void draw_rectangle(int x,int y)
	{
   gotoxy(x,y);printf("ฺฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฟ");
   gotoxy(x,y+1);printf("ณ                                                         ณ");
   gotoxy(x,y+2);printf("ณ                                                         ณ");
   gotoxy(x,y+3);printf("ณ                                                         ณ");
   gotoxy(x,y+4);printf("ณ                                                         ณ");
   gotoxy(x,y+5);printf("ภฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤู");
}/* draw_rectangle */
