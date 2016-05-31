/*
 *  file:    readchan.c
 *  date:    june 7, 1985
 *  author:    Kees Bakker
 *  description:  routines to read a channel from AD converter
 *  version:  1.0  first release
*/

#define    READ_ADI  12
#include "ado.h"

int readchan(int channel)
{
  static char cmdbuf[2] = {0,0};
  static char buffer[2];
  int err;

  if ( (err=addawcom(READ_ADI)) )
    return(err);
  cmdbuf[1] = (char) channel;    /* channel */
  if ((err = addawdat(cmdbuf,2)))
    return(err);
  if ((err = addardat(buffer,2)))
    return(err);
  if ((err = addachek()))
    return(err);
  return(*(int *)buffer);
}
