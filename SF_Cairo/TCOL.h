#include <cairo.h>

void Gen_Explosion(cairo_t *cr, int X_Pos,int Y_Pos,int Radius);

void Gen_Snap_Effect();
void Zero_Vulner_Sound();

void Jitter_Ship(cairo_t *cr);
void Reset_All_Missiles(cairo_t *cr);

void Test_Collisions(cairo_t *cr);

void Accumulate_Data(cairo_t *cr);
