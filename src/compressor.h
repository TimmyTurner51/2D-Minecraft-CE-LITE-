
#ifndef __loadblocks_h__
#define __loadblocks_h__

//MAX_OFFSET and MAX_LEN may be lowered at the cost of compression ratio.
//MAX_OFFSET is the maximum number of bytes that a pattern may be located in
//MAX_LEN is the maximum pattern length, and has less effect on speed so should be left as-is.
#define MAX_OFFSET  400 //2176  /* range 1..2176 */
#define MAX_LEN    65536  /* range 2..65536 */


#define STONE 1
#define GRASS 2
#define DIRT 3
#define COBBLESTONE 4
#define OAKPLANK 5
#define OAKSAPLING 6
#define BEDROCK 7
#define WATER 8
#define NOTHING1 9
#define LAVA 10
#define NOTHING2 11
#define SAND 12
#define GRAVEL 13
#define GOLDORE 14
#define IRONORE 15
#define COALORE 16
#define OAKLOGS 17
#define OAKLEAVES 18
#define SPONGE 19
#define GLASS 20
#define LAPIZORE 21
#define LAPIZBLOCK 22
#define DISPENSER 23
#define SANDSTONE 24
#define NOTEBLOCK 25
#define BEDBACK 26
#define BEDFRONT 27
#define REDSTONEDUSTOFF 28
#define REDSTONEDUSTON 29
#define REGULARPISTONRIGHTOFF 30
#define COBWEB 31

#define WATERENTITY 32
#define LAVAENTITY 33


int zx7_Compress(char *dest,char *src,int *len,int src_len);

unsigned int get_optimal(char *data,unsigned int data_len,unsigned int input_index);

void write_bit(int value);

#endif
