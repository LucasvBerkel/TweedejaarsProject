/*
 *  file:    writchan.c
 *  date:    november 20, 1985
 *  author:    Kees Bakker
 *  description:  routines to write a channel to DA converter
 *  version:  1.0  first release
*/

#define    OK    0
#define    WRIT_DAI  0x8

#include "ado.h"

int writchan(int channel,int voutp)
{
  static char cmdbuf[3] = {0,0,0};
  int err;

  if ((err = addawcom(WRIT_DAI)) != OK)
    return(err);
  cmdbuf[0] = (char)(channel & 0x3);    /* channel */
  cmdbuf[1] = (char)(voutp & 0xFF);    /* output voltage */
  if ((err = addawdat(cmdbuf,3)) != OK)
    return(err);
  return OK;
}
