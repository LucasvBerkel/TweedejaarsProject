/* ******************   Handle_Mine and all others elements   ***************** */

#include <cairo.h>

int Mine_Alive_Counter=0;
int Mine_Dead_Counter=0;
int Missile_Delay_Counter=0;

char Char_Set[10][1]={"Y","M","P","B","Q","K","C","W","R","Z"};
char Tmp_Char_Set[10][1];

char Foe_Menu[3][1];
char Friend_Menu[3][1];
char Mine_Indicator;
mine_type Mine_Type;

//int Get_Random_Index(int vec[]);
void Select_Mine_Menus();

void Update_Ship_Dynamics();

void Update_Ship_Display(cairo_t *cr);
void Move_Ship(cairo_t *cr);

void Fire_Shell(cairo_t *cr);
void Handle_Fortress(cairo_t *cr); 

void Handle_Speed_Score();
void Clear_Mine_Type(cairo_t *cr);

void Show_Mine_Type(cairo_t *cr, char Minetype);
void Reset_Mine_Headings();

void Generate_Mine(cairo_t *cr);
void Move_Mine(cairo_t *cr);

void Handle_Mine(cairo_t *cr);
void Generate_Aim_Mine(cairo_t *cr);
void Handle_Aim_Mine(cairo_t *cr);

void Handle_Shell(cairo_t *cr);
void Fire_Missile(cairo_t *cr, int Index);
void Handle_Missile(cairo_t *cr);