// To try: define stuff in every file individually with guards (myvars is not needed in DE)
#ifndef GLOBALS 
#define GLOBALS
#include "myconst.h"
#include "myext.h"
//#include "myvars.h"
#endif

#include <cairo.h>
#include <gtk/gtk.h>

void SF_iteration(cairo_t *cr);
void animation_loop(GtkWidget *darea);
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void set_initial_vals(cairo_t *cr);
void handle_F3();

void Set_Timer();

void Reset_Timer();
void ms_sleep(unsigned long miliseconds);
//Set_Kbd_Rate(unsigned char Rate);

//int keyboard (void);

//void interrupt far Get_Key();

/****** capture any keyboard input via indicated routine **********/
//void Capture_Kbd(void interrupt far (*func) () );

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1);
void Restore_Kbd();

void Get_User_Input();

//char Keyboard1();

//int gprintf( int *xloc, int *yloc, char *fmt, ... );

void Set_Graphics_Eraser();

void Show_Score(cairo_t *cr, int val, int x, int y);

/* Every update_X function here had a "return(0)" zero statement on it's last line, without  
specifying a return type. I removed all of these return statements and modified the function 
return type to void to surpress warnings. */ 

void Update_Points(cairo_t *cr);
void Update_Control(cairo_t *cr);
void Update_Velocity(cairo_t *cr);

void Update_Vulner(cairo_t *cr);
void Update_Interval(cairo_t *cr);
void Update_Speed(cairo_t *cr);
void Update_Shots(cairo_t *cr);

void Clear_Interval();

void Find_Interval();
//void Reset_Screen(cairo_t *cr);

void Init_Session();

void Init_Game();

void Display_Bonus_Char(cairo_t *cr, char Bonus_Char);
void Set_Bonus_Chars(cairo_t *cr);

void Xor_Bonus_Char(cairo_t *cr, int n);
void Set_Bonus_Message();
void Write_Bonus_Message();

void Check_Bonus_Input(cairo_t *cr);
int Generate_Non_Bonus_Char();

void Generate_Resource_Character();

void Handle_Bonus();

int Run_SF(cairo_t *cr);

/*************************************************************************/

        /* Run_Aiming Module  4.4.90 */

/**************************************************************************/

void Announce_Game_End();

void Announce_Session_End();
void Update_Mines(cairo_t *cr);
void Update_Score(cairo_t *cr);

void Reset_Aim_Screen(cairo_t *cr);

void Init_Aim_Session();

void Run_Aiming(cairo_t *cr);


