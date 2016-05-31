  /* addawcom - routine to write a command to DT2808 */
extern int addawcom(int command);  /* returns:  OK if OK ,   ERR_TMO if timed out */
  /* addawdat - routine to write data to DT2808 */
extern int addawdat(char *buffer,  /* pointer to data buffer */
                    int size);  /* number of bytes to write in DATA IN */
                                /* returns:  OK if OK, ERR_TMO    if timed out */
  /* addardat - routine to read data from DT2808 */
extern int addardat(char *buffer,  /* pointer to data buffer */
                   int size);   /* number of bytes to read from DATA OUT */
                                /* returns:  OK  if OK, ERR_TMO    if timed out */
  /* addachek - routine to check status of DT2808 */
extern int addachek(void);  /* returns:  OK if OK,  ERR if error see _addaerr
                                         ERR_TMO    if timed out */
  /* addainit - routine to reset the DT2808 */
extern int addainit(void);  /* returns:  OK  if OK ,   ERR_TMO  if timeout on reset */

extern unsigned int  addaerr; // error code

