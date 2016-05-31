#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <dir.h>
#include <dos.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>

#include "myconst.h"
#include "myext.h"  /* modify to myext.h */


   struct user_identification{
           int Trainee_ID;
	   char Trainee_Last_Name[MAXCHAR];
	   char Trainee_First_Name[MAXCHAR];
	   }user_id;

     char temp_string[3];
     char string[10];

     int readkey(void);
     int write_file(char filename[],int first_time,int sess);
     int read_file(char filename[],int displ,int sess);
     int check_id(void);
     void user_dialogue(void);
     int ask_training_session(void);
     void opening_screen(void);
     int ask_session(void);
     int ask_game(void);
     char *ask_session_nr(char filename[MAX_DIR_PATH],char string[10]);
     void final_display(void);
     char *current_directory(char *path);
     void erase_files(char path1[]);

     void open_graphics(void);
     int draw_text1(char *str);
     int draw_text2(int ypos,char *str);
     int draw_text();

/**************************************************************************/
/*                                                                        */
/*                            MAIN PROGRAM                                */
/*                                                                        */
/**************************************************************************/


main()
{
    int choice;

    char temp_str[MAX_DIR_PATH];
    char temp_str1[MAX_DIR_PATH];
    char select_file[MAX_DIR_PATH];


    scr_addr = (((peekb(0x40,0x10)&0x30)==0x30) ? 0xb000:0xb800);
    current_directory(curdir);

  opening_screen(); 
  Open_Graphics();
  Set_Bonus_Message();
  Set_Bonus_Chars();
  Set_Graphics_Eraser();
  Close_Graphics(); 

  strcpy(select,"");
  textbackground(TEXT_BACKGROUND);
  textcolor(TEXT_COLOR);

  clrscr();
  gotoxy(30,5);
  printf("SPACE  FORTRESS ");
  gotoxy(15,10);
  printf("The system is being initialized, kindly wait ... \n");
  randomize();
  delay(0);
  clrscr();

	/****  Top Level Menu: 1 - Demo , 2 - Training, 3 - Quit ****/

  go_back=0;
  while (((choice=ask_session())!=51)||(go_back==1))
  {
       go_back=0;
       if (choice==49)
			/*** Demo Session ***/

	  while ((choice=ask_game())!=51)
			{
			  if (choice==49)       Run_Session(0);
			  else if (choice==50)  Run_Aiming(0);
			}

       else
       if (choice==50)
			/*** Training Session ***/
	  {
	    user_dialogue();
	    if (!go_back)
	       do
	       {
		 choice=ask_training_session();
		 switch(choice)
		 {
		   case 49:       /*  Run session game  */

			    strcpy(filename,"A:\\SESSION\\SESS");
			    strcpy(string,"Session");
			    strcpy(filename,ask_session_nr(filename,string));
			    Run_Session(1);
			    break;

		   case 50:       /*  Run aiming task  */

			    strcpy(filename,"A:\\AIMING\\AIM");
			    strcpy(string,"Aiming");
			    strcpy(filename,ask_session_nr(filename,string));
			    Run_Aiming(1);
			    break;

		   case 51:       /*  Review session files  */

			    strcpy(path,"A:\\SESSION\\");
			    strcpy(filename,"A:\\SESSION\\SESS");
			    review_all_files(filename,1);
			    strcpy(temp_str1,"\\SESSION\\");
			    strcat(temp_str1,select);
			    if (strcmp(select,"")!=0)
			       {
				 strcpy(temp_str,"A:");
				 strcpy(select_file,strcat(temp_str,temp_str1));
				 review_one_file(select_file,1);
			       }
			    break;

		   case 52:       /*  Review aiming files  */

			    strcpy(path,"A:\\AIMING\\");
			    strcpy(filename,"A:\\AIMING\\AIM");
			    review_all_files(filename,0);
			    strcpy(temp_str1,"\\AIMING\\");
			    strcat(temp_str1,select);
			    if (strcmp(select,"")!=0)
				{
				  strcpy(temp_str,"A:");
				  strcpy(select_file,strcat(temp_str,temp_str1));
				  review_one_file(select_file,0);
				}
			    break;

		   case 53: return;
		 } /* end switch */
	       } while(choice!=53);  /* end do */
	  } /* end if(Training Session) */
  } /* end while(Main Menu) */

  textbackground(BLACK);
  textcolor(WHITE);
  return;

} /* end main() */


/**************************************************************************/
/*                                                                        */
/* int check_id(void) -                                                   */
/* If the diskette is new creates the USER_ID file and the subdir. SESSION*/
/*   and AIMING.                                                          */
/* If the USER_ID file already exists checks if the name and ID are the   */
/*   same with the ones typed by the user.                                */
/* Returns   0 if the comparison was succesfull.                          */
/*          -1 if the name and ID don't match the ones typed by tha user  */
/* Global variables:header                                                */
/* Called by user_dialogue                                                */
/*                                                                        */
/**************************************************************************/

int check_id(void)
{

   FILE *f1;
    if ((f1=fopen("A:USER_ID","rb"))==NULL)  /* No USER ID file . Create a new one */
      {
	mkdir("A:\\SESSION");
	mkdir("A:\\AIMING");
	if ((f1=fopen("A:USER_ID","wb"))==NULL) return(-1);
	strcpy(user_id.Trainee_Last_Name,header.Trainee_Last_Name);
	strcpy(user_id.Trainee_First_Name,header.Trainee_First_Name);
	user_id.Trainee_ID=header.Trainee_ID;
	if(fwrite(&user_id,sizeof(user_id),1,f1)!=1) {fclose(f1);return(-1);} else {fclose(f1);return(0);}
      }
    else  {if((f1=fopen("A:USER_ID","rb"))==NULL) return(-1);
	   if(fread(&user_id,sizeof(user_id),1,f1)!=1) {fclose(f1);return(-1);}
	   if((strcmp(user_id.Trainee_Last_Name,header.Trainee_Last_Name)==0)&&
	      (strcmp(user_id.Trainee_First_Name,header.Trainee_First_Name)==0)&&
	      (user_id.Trainee_ID==header.Trainee_ID)) {fclose(f1);return(0);}
	    else {fclose(f1);return(1);}
	   }
} /* check_id */

/**************************************************************************/
/*                                                                        */
/*  int readkey -Reads the keyboard                                       */
/*  Returns the ascii code of the pressed key                             */
/*                                                                        */
/**************************************************************************/

int readkey(void)
{
  int ch;
  if ((ch = getch()) == 0)  ch=256 + getch();
  return(ch);
}/*readkey*/

/**************************************************************************/
/*                                                                        */
/* destroy_data(void) - Ask the user if the data files should be destroyed*/
/*                                                                        */
/**************************************************************************/

destroy_data(void)
{
	   draw_rectangle(12,10);
	   gotoxy(17,12);printf("All data on this diskette will be destroyed !!!");
	   gotoxy(17,13);printf("Please approve data destruction (Y/N): ");
	   if (yesno()==1) {unlink("A:USER_ID");
			    check_id();
			    erase_files("A:\\SESSION\\");
			    erase_files("A:\\AIMING\\");
			   }

	   else user_dialogue();
	   return(0);
} /*destroy_data*/

/**************************************************************************/
/*                                                                        */
/*  erase_files - Erases all the SESS and AIM files.                      */
/*  Called by destroy_data                                                */
/*                                                                        */
/**************************************************************************/

void erase_files(char path1[MAX_DIR_PATH])
{
  char temp_str[MAX_DIR_PATH];
  int done,i;
  struct ffblk ffblk;

  strcpy(temp_str,path1);
  strcat(temp_str,"*.*");
  done=findfirst(temp_str,&ffblk,0);
  while (!done)
    {
      strcpy(temp_str,path1);
      strcat(temp_str,ffblk.ff_name);
      unlink(temp_str);
      done=findnext(&ffblk);
     }
}  /* erase files  */

/**************************************************************************/
/*                                                                        */
/*  int yesno(void) - reads 'Y' or 'N' from the keyboard.                 */
/*  Returns the ascii code.                                               */
/*                                                                        */
/**************************************************************************/


int yesno(void)
{
       char ch;

	while ((toupper(ch=getch())!='Y')&&(toupper(ch)!='N'));
	if (toupper(ch)=='Y') {printf("Y");delay(300);return(1);}
	else {printf("N");delay(300);return(0);}
} /* yesno */

/**************************************************************************/
/*                                                                        */
/*  final display - Displays the screen with all the game results.        */
/*  Called by Run Session                                                 */
/*                                                                        */
/**************************************************************************/

void final_display(void)
{
 clrscr();
 displ_screen(4,"screen.dat");
 gotoxy(44,8);printf("%d",game.Score);
 gotoxy(39,12);printf("%d",game.No_Of_Ship_Damaged_By_Fortress);
 gotoxy(42,14);printf("%d",game.No_Of_Times_Ship_Damaged_By_Mines);
 gotoxy(50,16);printf("%d",game.No_Of_Times_Fortress_Distroyed_By_Trainee);
 gotoxy(11,21);printf("%d",game.Points);
 gotoxy(30,21);printf("%d",game.Control);
 gotoxy(50,21);printf("%d",game.Velocity);
 gotoxy(67,21);printf("%d",game.Speed);
 gotoxy(27,23);printf("%d",game.No_Of_Bonus_Intervals);
 gotoxy(48,23);printf("%d",game.No_Of_Points_Bonus_Taken);
 gotoxy(70,23);printf("%d",game.No_Of_Missiles_Bonus_Taken);
 getch();
}

/**************************************************************************/
/*                                                                        */
/*  char *current directory(char *path)                                   */
/*  Returns the current directory                                         */
/*  Called by main                                                        */
/*							                  */
/**************************************************************************/

char *current_directory(char *path)
{
 strcpy(path,"x:\\");
 path[0]='A'+getdisk();
 getcurdir(0,path+3);
 return(path);
}

/**************************************************************************/
/*                                                                        */
/*  opening_screen - Displays the first screen of the game by opening     */
/*                   the graphic mode.At the end of the routine it closes */
/*                   the graphic mode.                                    */
/*  Called by main                                                        */
/*							                  */
/**************************************************************************/

 void opening_screen(void)
 {
   open_graphics();
   if ( draw_text() ) getch();
   closegraph();
 }

/**************************************************************************/
/*                                                                        */
/*  open_graphics  - Opens the graphic mode                               */
/*  Called by opening_screen                                                 */
/*							                  */
/**************************************************************************/

 void open_graphics(void)
 {
   int g_driver,g_mode,g_error;

   g_driver=DETECT;
   initgraph(&g_driver,&g_mode,"c:\tc");
   g_error=graphresult();
   if(g_error<0)
     { printf("INITGRAPH ERROR : %s.\n",grapherrormsg(g_error));
       exit(1);
     }
  } /* open_graphics  */

/**************************************************************************/
/*                                                                        */
/*  draw_text - Displays the text on the first screen                     */
/*  Called by open_screen                                                 */
/*							                  */
/**************************************************************************/

 int draw_text(void)
 {
  char *str;
  float ypos;
  float y;

  str=(char *)malloc(80);


 settextjustify(LEFT_TEXT,TOP_TEXT);
 setactivepage(1);
 settextstyle(TRIPLEX_FONT,HORIZ_DIR,6);
 strcpy(str,"SPACE FORTRESS II");
 if (!draw_text1(str)) return(0);

 setviewport(0,0,getmaxx(),getmaxy(),0);
 y=getmaxy();
 ypos=3*y/7;
 settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
 strcpy(str,"   Complex Skill Game Trainer");
 if(!draw_text2(ypos,str)) return(0);

 ypos=textheight(str)+y/10;
 ypos=19*y/35;
 settextstyle(SMALL_FONT,HORIZ_DIR,6);
 strcpy(str,"Copyright @ By");
 if(!draw_text2(ypos,str)) return(0);

 ypos=textheight(str)+y/10;
 ypos=3*y/5;
 settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
 strcpy(str,"     Human Engineering Laboratory");
 if(!draw_text2(ypos,str)) return(0);

 ypos=textheight(str)+y/10;
 ypos=5*y/7;
 strcpy(str,"     Technion - Israel Institute of Technology");
 if(!draw_text2(ypos,str)) return(0);

 ypos=textheight(str)+y/10;
 ypos=29*y/35;
 strcpy(str,"     Haifa, Israel 32000");
 if(!draw_text2(ypos,str)) return(0);

 settextstyle(SMALL_FONT,HORIZ_DIR,6);
 outtextxy(100,y-y/20,"          Press any key to continue                   ");
 return(1); /*  when entire screen is displayed */
}/*draw_text */

/**************************************************************************/
/*                                                                        */
/*  draw_text1- Displays "SPACE FORTRESS" text                            */
/*  Called by draw_text                                                   */
/*							                  */
/**************************************************************************/

 int draw_text1(char* str)
 {
  char *stor;
  int x1,y1,x2,y2,l1,l2,m1,m2,k,n,xdist,ydist,l,m,i,j;

   x1=47;y1=47;
   x2=getmaxx()-x1;y2=getmaxy()-y1;
   setviewport(x1,y1,x2,y2,0);
   setcolor(EGA_YELLOW);
 l1=10;m1=10;
 l2=l1+textwidth(str);
 m2=m1+textheight(str);
 stor=(char *)malloc((unsigned int)imagesize(l1,m1,l2,m2));
 cleardevice();
 outtextxy(12,8,str);
 setactivepage(0);
 rectangle((x2-x1)/2-(l2-l1)/2-27,(y2-y1)/5-(m2-m1)/2-23,(x2-x1)/2+(l2-l1)/2+27,(y2-y1)/5+(m2-m1)/2+23);
 rectangle((x2-x1)/2-(l2-l1)/2-25,(y2-y1)/5-(m2-m1)/2-21,(x2-x1)/2+(l2-l1)/2+25,(y2-y1)/5+(m2-m1)/2+21);

 k=0;n=0;
 xdist=50;
 ydist=20;
 for(l=0;l<xdist;l++)
  for(m=0;m<ydist;m++){
    for(j=n;j<m2-m1;j+=ydist){
       for (i=k;i<(l2-l1);i+=xdist){
	 setactivepage(1);
	 if ((getpixel(l1+i,m1+j)!=0)|(getpixel(l1+i+1,m1+j+1)!=0)){
	   getimage(l1+i,m1+j,l1+i+1,m1+j+1,stor);
           setactivepage(0);
	   putimage((x2-x1)/2-(l2-l1)/2+l1+i-8,(y2-y1)/5-(m2-m1)/2+m1+j-14,stor,COPY_PUT);

         }
      }
	 if(keyboard()) return(0); /* break in the middle */
	 if(k<=xdist) k++;
	 else k=k-xdist;
    }
       if(n<ydist) n++;
       else n=n-ydist;
  }
  return(1);
}


/**************************************************************************/
/*                                                                        */
/*  draw_text2- Displays the other text lines of the first screen         */
/*  Called by draw_text                                                   */
/*							                  */
/**************************************************************************/

 int draw_text2(int ypos,char *str)
{
char *stor;
  int x1,x2,l1,l2,m1,m2,k,n,xdist,ydist,l,m,i,j;
   x1=47;
   x2=getmaxx()-x1;
 l1=20;m1=20;
 l2=l1+textwidth(str);
 m2=m1+textheight(str);

 setfillstyle(SOLID_FILL,EGA_MAGENTA);
 setcolor(EGA_YELLOW);
 stor=(char *)malloc((unsigned int)imagesize(l1,m1,l2,m2));
 setactivepage(1);
 clearviewport();
 outtextxy(24,20,str);
 for (i=l1;i<l2;i++){
      setactivepage(1);
      getimage(i,m1,i+1,m2+6,stor);
      setactivepage(0);
      putimage((x2-x1)/2-textwidth(str)/2+i,ypos,stor,COPY_PUT);
      if(keyboard()) return(0);  /* skip rest of the screen */
   }
  free((char *)stor);
  return(1);
}/*draw_text2*/

/**************************************************************************/
/*                                                                        */
/*  int ask_session - ask the user witch session he wants to play         */
/*  Returns the choosed option (Demonstration/Real training)              */
/*  Called by main                                                        */
/*							                  */
/**************************************************************************/

 int ask_session()
  {
   int i;
       i=0;
      while (i==0)
      {
       clrscr();
       gotoxy(22,7);
       printf("ษออออออออออออออออออออออออออออออออออออป");
       gotoxy(22,8);
       printf("บ                                    บ");
       gotoxy(22,9);
       printf("บ         SPACE FORTRESS             บ");
       gotoxy(22,10);
       printf("บ                                    บ");
       gotoxy(22,11);
       printf("บ  (1) Run Demonstration  Session    บ");
       gotoxy(22,12);
       printf("บ  (2) Run Real Training Session     บ");
       gotoxy(22,13);
       printf("บ  (3) Quit                          บ");
       gotoxy(22,14);
       printf("บ                                    บ");
       gotoxy(22,15);
       printf("ศออออออออออออออออออออออออออออออออออออผ");
       gotoxy(10,20);
       printf("Select and type option number(1/2/3)");
       i=getch();
       if ((i==49)||(i==50)||(i==51)) return(i);
       else i=0;
      }
      return(i);
   }/*ask_session*/

/**************************************************************************/
/*                                                                        */
/* int ask_game - ask the user witch demonstrtion session he wants to play*/
/* Returns the choosed option (Run game/ Run aiming test                  */
/* Called by main                                                         */
/*							                  */
/**************************************************************************/

 int ask_game()
  {
   int i;
       i=0;
      while (i==0)
      {
       clrscr();
       gotoxy(22,7);
       printf("ษออออออออออออออออออออออออออออออออออออออออป");
       gotoxy(22,8);
       printf("บ                                        บ");
       gotoxy(22,9);
       printf("บ SPACE FORTRESS - DEMONSTRATION SESSION บ");
       gotoxy(22,10);
       printf("บ                                        บ");
       gotoxy(22,11);
       printf("บ            (1) Run Game                บ");
       gotoxy(22,12);
       printf("บ            (2) Run Aiming Test         บ");
       gotoxy(22,13);
       printf("บ            (3) Quit                    บ");
       gotoxy(22,14);
       printf("บ                                        บ");
       gotoxy(22,15);
       printf("ศออออออออออออออออออออออออออออออออออออออออผ");
       gotoxy(10,20);
       printf("Select and type option number(1/2/3)");
       i=getch();
       if ((i==49)||(i==50)||(i==51)) return(i);
       else i=0;
      }
      return(i);
   }/*ask_game*/

/**************************************************************************/
/*                                                                        */
/* int ask_training_session  displays menu                                */
/* Returns the choosed option (Run game/ Run aiming task/Review SESSION   */
/*                             files/Review AIMING files)                 */
/* Called by main                                                         */
/*							                  */
/**************************************************************************/


int ask_training_session(void)
{
       int i;
       clrscr();
       gotoxy(22,7);
       printf("ษออออออออออออออออออออออออออออออออออออออออป");
       gotoxy(22,8);
       printf("บ                                        บ");
       gotoxy(22,9);
       printf("บ SPACE FORTRESS - TRAINNING SESSION     บ");
       gotoxy(22,10);
       printf("บ                                        บ");
       gotoxy(22,11);
       printf("บ            (1) Run Game                บ");
       gotoxy(22,12);
       printf("บ            (2) Run Aiming Task         บ");
       gotoxy(22,13);
       printf("บ            (3) Review Session Files    บ");
       gotoxy(22,14);
       printf("บ            (4) Review Aiming Files     บ");
       gotoxy(22,15);
       printf("บ            (5) Quit                    บ");
       gotoxy(22,16);
       printf("บ                                        บ");
       gotoxy(22,17);
       printf("ศออออออออออออออออออออออออออออออออออออออออผ");
       gotoxy(10,20);
       printf("Select and type option number(1/2/3/4/5)");
       i=getch();
       if ((i==49)||(i==50)||(i==51)||(i==52)||(i==53)) return(i);
       else i=0;
       return(i);
} /* trainning session */

