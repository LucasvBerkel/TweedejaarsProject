#include <graphics.h>
#include <alloc.h>

#include "myext.h"

extern int Resource_Display_Interval;
extern int No_Resource_Display_Interval;

extern Check_Bonus_Input();


Check_Bonus_Input();
Handle_Bonus();
Display_Bonus_Char(char Bonus_Char);
Set_Bonus_Chars();
Set_Bonus_Message();
Write_Bonus_Message();
Xor_Bonus_Char(int n);

extern Generate_Resource_Character();
extern int Generate_Non_Bonus_Char();


Handle_Bonus()
{

  if(!Resource_Flag)   /* resource is off */
    {
      Resource_Off_Counter++;
      if(Resource_Off_Counter>=No_Resource_Display_Interval)
	{
	  Resource_Flag=ON;
	  Resource_On_Counter=0;
	  Generate_Resource_Character();
	}
   }
 else   /* Resource_Flag=ON; */
   {
     Resource_On_Counter++;
     if(Resource_On_Counter>=Resource_Display_Interval)
       {
	 Resource_Flag=OFF;
	 Resource_Off_Counter=0;
	 Bonus_Display_Flag=NOT_PRESENT; /* in case bonus is pressed after
					    $ disappears */
	 if (Bonus_Granted)
	    {
	      Write_Bonus_Message();     /* erase bonus message */
	      Bonus_Granted=OFF;
	    }
	 else
	 Xor_Bonus_Char(rn); 	/* Erase_Resource_Char */
       }
   }
return(0);
}


Display_Bonus_Char(char Bonus_Char)
{
  int svcolor;
  int x,y;
  svcolor=getcolor();
  setcolor(TEXT_COLOR);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
  x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
  y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
  gprintf(&x,&y,"%c",Bonus_Char);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,0);
  setcolor(svcolor); /* restore previous color */
  return(0);
}

Set_Bonus_Chars()
{
  int size,i,j;
  int x,y;

	/* set character Size */
  size=imagesize(0,0,16,16);
	/* get right location */
  x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
  y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;

 for (i=0;i<10;i++)
     {
	  bc[i]=malloc(size);
	  Display_Bonus_Char(Bonus_Char_Vector[i][0]);
	  getimage(x,y,x+16,y+16,bc[i]);
	  putimage(x,y,bc[i],XOR_PUT);
     }
 return(0);
}

Set_Bonus_Message()
{
  int size;
  int svcolor;
  int x,y;

  svcolor=getcolor();
  setcolor(TEXT_COLOR);
  x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
  y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
  gprintf(&x,&y,"Bonus");
  setcolor(svcolor); /* restore previous color */

  size=imagesize(0,0,40,9);        /*length of 5 characters*/
  buffer2=malloc(size);
  getimage(x,y,x+40,y+9,buffer2);
  putimage(x,y,buffer2,XOR_PUT);
  setcolor(svcolor);
  return(0);
}

Write_Bonus_Message()
{
 int x,y;

 x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
 y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
 putimage(x,y,buffer2,XOR_PUT);
 return(0);
}


Xor_Bonus_Char(int n)   /* write and erase bonus character */
{
  int x,y;

	/* get right location */
  x=MaxX/2 - 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;
  y=MaxY/2 + 1.2*SMALL_HEXAGONE_SIZE_FACTOR*MaxX;

  putimage(x,y,bc[n],XOR_PUT);
  return(0);
}

