
/* ----TIMRROUT.C----- */
void timrinit(void); 
long timrread(void); /* read timers 0 and 1 as 32 bit number */
           /*  timer 1 goes to next value on FFFF of timer0 */
void waits(int time); /* wacht [time] ms */
void timrset(void); /* dummy */


