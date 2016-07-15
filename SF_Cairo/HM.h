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

void Update_Ship_Display();
void Move_Ship();

void Fire_Shell();
void Handle_Fortress(); 

void Handle_Speed_Score();
void Clear_Mine_Type();

void Show_Mine_Type(char Minetype);
void Reset_Mine_Headings();

void Generate_Mine();
void Move_Mine();

void Handle_Mine(cairo_t *cr);
void Generate_Aim_Mine();
void Handle_Aim_Mine();

void Handle_Shell(cairo_t *cr);
void Fire_Missile(int Index);
void Handle_Missile(cairo_t *cr);