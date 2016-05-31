
extern int far addawcom(int command);  // addawcom - routine to write a command to DT2808
				   // returns:  OK if OK ,   ERR_TMO if timed out

				   // addawdat - routine to write data to DT2808
extern int far addawdat(char *buffer,  // pointer to data buffer
		    int size);     // number of bytes to write in DATA IN
				   // returns:  OK if OK, ERR_TMO    if timed out

				   // addardat - routine to read data from DT2808
extern int far addardat(char *buffer,  // pointer to data buffer
		   int size);      // number of bytes to read from DATA OUT
				   // returns:  OK  if OK, ERR_TMO    if timed out


extern int far addachek(void); // addachek - routine to check status of DT2808
			   // returns:  OK if OK,  ERR if error see _addaerr
			   //            ERR_TMO    if timed out


extern int far addainit(void);// addainit - routine to reset the DT2808
			  // returns:  OK  if OK ,   ERR_TMO  if timeout on reset

