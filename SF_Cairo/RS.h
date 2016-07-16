//void Mydelay(unsigned Time);
//{
//    unsigned long end;
//
//    end=Time_Counter+Time;
//    while(Time_Counter<end);
//
//}

//void interrupt far Get_Tik()

    /****** capture system clock tiks via indicated routine **********/

//void Capture_Tik(void interrupt far (*func) () )
//Restore_Tik();

void Set_Timer();

void Reset_Timer();

//Set_Kbd_Rate(unsigned char Rate);

//int keyboard (void);

//void interrupt far Get_Key();

/****** capture any keyboard input via indicated routine **********/
//void Capture_Kbd(void interrupt far (*func) () );

void Restore_Kbd();

void Get_User_Input();

//char Keyboard1();

//int gprintf( int *xloc, int *yloc, char *fmt, ... );

void Set_Graphics_Eraser();

void Show_Score(int val, int x, int y);

/* Every update_X function here had a "return(0)" zero statement on it's last line, without  
specifying a return type. I removed all of these return statements and modified the function 
return type to void to surpress warnings. */ 

void Update_Points();
void Update_Control();
void Update_Velocity();

void Update_Vulner();
void Update_Interval();
void Update_Speed();
void Update_Shots();

void Clear_Interval();

void Find_Interval();
void Reset_Screen();

void Init_Session();

void Init_Game();

void Display_Bonus_Char(char Bonus_Char);
void Set_Bonus_Chars();

void Xor_Bonus_Char(int n);
void Set_Bonus_Message();
void Write_Bonus_Message();

void Check_Bonus_Input();
int Generate_Non_Bonus_Char();

void Generate_Resource_Character();

void Handle_Bonus();

int Run_SF();

/*************************************************************************/

        /* Run_Aiming Module  4.4.90 */

/**************************************************************************/

void Announce_Game_End();

void Announce_Session_End();
void Update_Mines();
void Update_Score();

void Reset_Aim_Screen();

void Init_Aim_Session();

void Run_Aiming();