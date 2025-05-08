
#ifndef __loadblocks_h__
#define __loadblocks_h__

//MAX_OFFSET and MAX_LEN may be lowered at the cost of compression ratio.
//MAX_OFFSET is the maximum number of bytes that a pattern may be located in
//MAX_LEN is the maximum pattern length, and has less effect on speed so should be left as-is.
#define MAX_OFFSET  400 //2176  /* range 1..2176 */
#define MAX_LEN    65536  /* range 2..65536 */


int zx7_Compress(char *dest,char *src,int *len,int src_len);

unsigned int get_optimal(char *data,unsigned int data_len,unsigned int input_index);

void write_bit(int value);

#endif
