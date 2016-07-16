/* test collision and generate effects   */

#include <stdio.h>
#include <stdlib.h>
//#include <graphics.h>
//#include <process.h>
//#include <process.h>
//#include <bios.h>
//#include <alloc.h>
//#include <dos.h>
#include <time.h>
#include <math.h>
#include <cairo.h>

#include "myconst.h"
#include "myext.h"

// Addded
//#include "DE"
//#include "myvars.h"

//extern Get_Ship_Input(); // not used or something

// ------- Uncomment --------------- //
//extern Update_Vulner();
//extern Update_Velocity();
//extern Update_Control();
//extern Update_Points();
//extern Reset_Screen();
//extern Mydelay(unsigned Timedelay);
// --------------------------------- // 

char *Small_Expl_Buffer;
int Data_Update_Counter=20;
int Last_Center_Dist;
int Wrap_Around_Flag=OFF;

float deg2rad(int deg);
void Gen_Explosion(cairo_t *cr, int X_Pos,int Y_Pos,int Radius);

void Gen_Snap_Effect();
void Zero_Vulner_Sound();

void Jitter_Ship(cairo_t *cr);
void Reset_All_Missiles(cairo_t *cr);

void Test_Collisions(cairo_t *cr);

void Accumulate_Data(cairo_t *cr);
