#include <stdlib.h>
#include <io.h>
#include <alloc.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <tcu.h>
#include <string.h>
#include <stdarg.h>
#include "myconst.h"
#include "myext.h"

extern char trainee_A[13];
extern char trainee_B[13];
extern TCU_FIELD_VALUE First_Name;

extern void Regression();
extern int Update_Settings_B(char filename[13]);
extern int Display_Model(double T_A, double BETA, int E, double ALFA,
		  double T_A_OLD, double BETA_OLD, int E_OLD, double ALFA_OLD);


int Determine_Level(char filename[13]);
int Trials_To_Go();
int Announce_Transfer_Task();
int Get_Part_Of_Column(int column, char filename[13]);
int Display_Intermediate_Results();
int Search_Max(int Number);
int Get_Max_Points_For_FB();
int Ind_Round(double x);

int			     dummy[MAX_COLUMNS];


int Determine_Level(char filename[13])
{

  int 			 level=1;      /* determines train.level_ind (part-task) */
  int			 pass_stage_flag = OFF;
  double		 factor;

  if (train.probe_flag==0)
   {
     if (train.wraps_ind < 30) train.probe_flag=train.level_ind;
     train.trials_end_ind = -1;
     train.alfa_ind       = -1;
     train.beta_ind       = -1;
     train.asymp_ind      = -1;
     train.p_p_ind        = -1;
     train.varperc_ind    = -1;
     train.Fprob_ind      = -1;
     train.perf_ind       = -1;
     return(train.level_ind);
   }
  else
   {
     NN=Get_Part_Of_Column(FORT_TIMES_COL, filename);
   }

  if (train.probe_flag == 1)
     {
	LOP_E = Prior_Practice - 1;
	LOP_T_A = 4.0;
	factor = (double)train.criterion_ind1/100;
        if (train.criterion_ind2 == 0) pass_stage_flag = ON;
     }
  else
     {
	LOP_E = Prior_Practice - 1;
	LOP_T_A = 4.0;
	factor = (double)train.criterion_ind2/100;
     }

  if(NN >= (int)(log10(Prior_Practice+10.0)*50.0))
  {
  if (train.Fprob_ind == -1) lin_reg_flag = ON;
  else lin_reg_flag = OFF;
  Regression();
  train.trials_end_ind = NN + Trials_To_Go();
  if (train.trials_end_ind == NN) transfer_task_flag = ON;
  train.perf_ind = Ind_Round(((float)NN/(float)train.trials_end_ind)*100.0);
  }
  else
  {
  train.trials_end_ind = -1;
  train.alfa_ind       = -1;
  train.beta_ind       = -1;
  train.asymp_ind      = -1;
  train.p_p_ind        = -1;
  train.varperc_ind    = -1;
  train.Fprob_ind      = -1;
  train.perf_ind       = -1;
  return(train.probe_flag);
  }


  if (train.probe_flag < 3)
   {
    if((NN>(int)(factor * (float)train.trials_end_ind))&&(LOP_ALFA<=0.00))
      {
       train.probe_flag++;
       if(pass_stage_flag == ON) train.probe_flag++;
       level=train.probe_flag;
       train.carry_ind = 0;
       lin_reg_flag = 0;
       return(level);
      }
    else
      {
       level = train.probe_flag;
       return(level);
      }
    }
   else
    {
      if((transfer_task_flag == ON)||(train.probe_flag == 4))
       {
        train.probe_flag = 4;
        level = 4;
        return(level);
       }
      else
       {
        train.probe_flag = 3;
        level = 3;
        return(level);
       }
    }
}


int Get_Part_Of_Column(int column, char filename[13])
{
  FILE	 		 *fileptr;
  char			 *str;
  int			 last_dummy[MAX_COLUMNS];
  int			 Flag,i=0, j=0, Points_Read= 0;

  if ((fileptr = fopen(filename, "rt")) != NULL)
  {
    if ((str = (char *) malloc(MAX_TR_STR))==NULL)
    {
     printf("Not enough memory to allocate buffer\n");
     exit(1);  /* terminate program if out of memory */
    }

   fgets(str, MAX_TR_STR, fileptr);  // comment string
   Prior_Practice = 0;
   do
     {
      str = fgets(str, MAX_TR_STR, fileptr);  // integer strings
      sscanf(str,"%d", &Flag);
      Prior_Practice++;
     }
   while ((Flag != train.probe_flag)&&(str != NULL));
   Prior_Practice--;

   for (j=0; j < column; j++)
    {
     sscanf(str,"%d", &dummy[j]);
//   printf("dummy[%d] = %d\n", j, dummy[j]);
     str+=6;
    }
   column_data[Points_Read] = dummy[column-1];       // first datapoint
// printf("column_data[%d] =   %d\n",Points_Read , dummy[column-1]);
   Points_Read++;                                   // has been obtained
   do
     {
     str = (char *)realloc(str, MAX_TR_STR);
     str = fgets(str, MAX_TR_STR, fileptr);
     for (j=0; j < column; j++)
       {
       last_dummy[j] = dummy[j];
       sscanf(str,"%d", &dummy[j]);
       str +=6;
       }
     if ((dummy[0] == last_dummy[0])&&(dummy[1] == last_dummy[1])&&
	(dummy[2] == last_dummy[2])&&(dummy[3] == last_dummy[3])&&
	(dummy[4] == last_dummy[4])) i=1;

     column_data[Points_Read] = dummy[column-1];
     Points_Read++;
//   printf("probe_flag nr.%d %d %d\n", Points_Read, train.probe_flag, dummy[0]);
     }
   while ((dummy[0] == train.probe_flag)&&(i !=1 ));

   Points_Read--;
// printf("Points_Read*: %d\n", Points_Read);

   free(str);
   fclose(fileptr);
   }
   else Show_Error("file niet gevonden");
   return(Points_Read);
}


int Trials_To_Go()
{
int		games_played, i=1;
int             minutes_to_end;
long 		seconds_to_end;
double 	 	t=0.0, T_N;

if (LOP_ALFA > 1) return(-1);

games_played = ((train.session_ind - 1) * No_Of_Games) + Game_Counter;
minutes_to_end = TOTAL_TRAINING_TIME - games_played*One_Game_Duration;
seconds_to_end = 60*(long int)minutes_to_end;

if (seconds_to_end <= 0)
 {
  transfer_task_flag = ON;
  return(-1);
 }

do
  {
   T_N= LOP_T_A + (LOP_BETA * pow((double)(NN + LOP_E + i), LOP_ALFA));
   i = i + 1;
   t = t + T_N;
  }
while (t < ((double)seconds_to_end));
i = i - (int)((float)minutes_to_end/(float)One_Game_Duration);
return(i);
}

int Announce_Transfer_Task()
{
   TCU_NOTICE        notice;
   static int 	     i=1;	

   tcu_prepare_notice (&notice,
		       "Space Fortress III",
		       tcu_colour_attrib (RED, LIGHTGRAY),
		       tcu_colour_attrib (LIGHTBLUE, LIGHTGRAY),
		       tcu_colour_attrib (BLUE, LIGHTGRAY),
		       TCU_BOX_DOUBLE);

   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "Speciaal spel nr. %d van 4", i);
   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "MIJNEN KOMEN SNELLER ACHTER ELKAAR");
   tcu_notice_text (&notice, "Doe je best, %s !!", First_Name.v_string);
   tcu_notice_text (&notice, "");
   tcu_notice_text (&notice, "druk Enter <ÄÙ om door te gaan");
   tcu_display_notice (&notice, 25, 12);
   tcu_clear_notice (&notice);
   i++;
   return(0);
}


int Display_Intermediate_Results()
{
char a;
int  b, i, N, midx, midy, xloc, yloc, Max, Height, Width, Max2;

N = Get_Part_Of_Column(TOTAL_POINTS_COL, trainee_A);
column_data[N] = train.total_points_ind;
N++;
if (N < 1) {getch(); return(0);}

Open_Graphics();
Height = textheight("H");
Width = textwidth("Z");
setbkcolor(WHITE);
setcolor(BLUE);
midx = getmaxx()/2;
midy = getmaxy()/2;

if(N>1)
{
  line(0, midy, 2*midx, midy);
  line(1, midy, 1, 0);
  Max = Search_Max(N);
  if (Max <= 0) Max = 50;
  Max2 = Max + (int)(0.2*Max);

if (N < 11)
{
 for(i=0; i<N; i++)
  {
  xloc = (int)(i*2*(float)midx/N);
  yloc = midy+15;
  line(xloc, midy-3, xloc, midy+3);
  gprintf(&xloc, &yloc, "%d", i);
  }
}
else
{
 for(i=0; i<N; i+=(int)(N/10))
  {
  xloc = (int)(i*2*(float)midx/N);
  yloc = midy+15;
  line(xloc, midy-3, xloc, midy+3);
  gprintf(&xloc, &yloc, "%d", i);
  }
}

 for(i=0; i<6; i++)
  {
  xloc = 4;
  yloc = midy - i*midy/6 - 15;
  line(xloc-3, yloc+15, xloc+3, yloc+15);
  gprintf(&xloc, &yloc, "%d", (Max2/6)*i);
  }

  moveto(0, midy);
  setcolor(RED);
  for(i=0; i<N; i++)
    lineto( (int)(((float)i/N)*2*midx), midy - (int)(((float)column_data[i]/Max2)*midy));
  setcolor(BLUE);
  xloc = midx - 6*Width;
  yloc = 1*Height;
  gprintf(&xloc, &yloc, "%s %d", "Leercurve Fase", train.level_ind);
  yloc = 3*Height;
  gprintf(&xloc, &yloc, "%s %d", "Maximale Spelscore", Max);
  i = 4;
  yloc = midy + i*Height;
  gprintf(&xloc, &yloc, "%s", "spelnr. ÄÄ");
}
else i = -8;

xloc = 2*Width;
i+=4;
yloc = midy + i*Height;
gprintf(&xloc, &yloc, "%s %d", "Laatste Spelscore :", train.total_points_ind);

if (Max <= train.total_points_ind)
{
 i+=2;
 yloc = midy + i*Height;
 setcolor(RED);
 gprintf(&xloc, &yloc, "%s %s", "Een verbetering van uw persoonlijk record", First_Name.v_string);
 setcolor(BLUE);
}

i+=2;
yloc = midy + i*Height;
gprintf(&xloc, &yloc, "%s %d %s", "U vernietigde het Fort ", train.fort_destruction_ind, " maal.");
i+=2;
yloc = midy + i*Height;
gprintf(&xloc, &yloc, "%s %d %s", "U werd ", train.fort_damage_ind, " maal door het Fort geraakt.");
if(train.level_ind > 1)
{
  i+=2;
  yloc = midy + i*Height;
  gprintf(&xloc, &yloc, "%s %d %s", "U werd ", train.mine_damage_ind, " maal door de mijnen geraakt.");
}
i+=2;
yloc = midy + i*Height;
gprintf(&xloc, &yloc, "%s %d %s", "U schoot ", train.misses_ind, " maal mis.");
yloc = 2*midy-2*Height;
gprintf(&xloc, &yloc, "%s"," Even geduld a.u.b...");
Update_Settings_B(trainee_B);
gprintf(&xloc, &yloc, "%s","                                      ");
if(train.perf_ind != -1)
{
setcolor(WHITE);
gprintf(&xloc, &yloc, "%s"," Even geduld a.u.b...");
setcolor(BLUE);
gprintf(&xloc, &yloc, "%s","Wilt u uw fortvernietigingstijden zien (j/N)?");
a=Keyboard1();
if((a==106)||(a==74)) b=1;  /* y=121 and Y=89 */
setcolor(WHITE);
gprintf(&xloc, &yloc, "%s","Wilt u uw fortvernietigingstijden zien (j/N)?");
setcolor(BLUE);
}

if (b == 1)
 {
 Close_Graphics();
 Display_Model(LOP_T_A, LOP_BETA, LOP_E, LOP_ALFA, T_A_OLD, BETA_OLD, E_OLD, ALFA_OLD);
 return(0);
 }
else
 {
 setcolor(WHITE);
 gprintf(&xloc, &yloc, "%s"," Even geduld a.u.b...");
 setcolor(BLUE);
 gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
 getch();
 setcolor(WHITE);
 gprintf(&xloc, &yloc, "%s","<Druk een toets om door te gaan.>");
 setcolor(BLUE);
 Close_Graphics();
 return(0);
 }

}

int Search_Max(int Number)
{
int i,j=0;

for (i=0; i<Number; i++)
j = max(column_data[i], j);

return(j);
}


int Get_Max_Points_For_FB()
{
int N;

if ( train.level_ind  <  3)
{
  N = Get_Part_Of_Column(TOTAL_POINTS_COL, trainee_A);
  if (N >= 2) Max_Points = Search_Max(N);
  else Max_Points = column_data[0];
}
else return(0);
return(0);

}

int Ind_Round(double x)
{
double ipart;

if (modf(x, &ipart) > 0.5) return((int)ceil(x));
else return((int)floor(x));
}

