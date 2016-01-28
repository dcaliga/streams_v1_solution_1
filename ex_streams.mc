/* $Id: ex05.mc,v 2.1 2005/06/14 22:16:47 jls Exp $ */

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>


void subr (int64_t In[], int64_t Out[], int num, int64_t *dma_time, int64_t *compute_time, int mapnum) {

    OBM_BANK_A (AL, int64_t, MAX_OBM_SIZE)
    OBM_BANK_C (CL, int64_t, MAX_OBM_SIZE)

    int64_t t0, t1, t2;
    int i;
    
    Stream_64 SA,SB,SOut;

    read_timer (&t0);

#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SA, PORT_TO_STREAM, In, num*sizeof(int64_t));
}
#pragma src section
{
    int i;
    int64_t i64;

    for (i=0;i<num;i++)  {
       get_stream_64 (&SA, &i64);

       AL[i] = i64;
    }
}
}
    read_timer (&t1);

#pragma src parallel sections
{
#pragma src section
{
    int i;
    for (i=0; i<num; i++)
      put_stream_64 (&SB, AL[i] + 42, 1);
}
#pragma src section
{
    int i;
    int64_t i64;
  
    for (i=0; i<num; i++) {
       get_stream_64 (&SB, &i64);
       CL[i] = i64 * 17;
    }
}
}

    read_timer (&t2);

    *dma_time = t1 - t0;
    *compute_time = t2 - t1;

#pragma src parallel sections
{
#pragma src section
{
    int i;
    int64_t i64,j64;

    for (i=0;i<num;i++)  {
       i64 = CL[i];
       put_stream_64 (&SOut, i64, 1);
    }
}
#pragma src section
{
    streamed_dma_cpu_64 (&SOut, STREAM_TO_PORT, Out, num*sizeof(int64_t));
}
}
    }
