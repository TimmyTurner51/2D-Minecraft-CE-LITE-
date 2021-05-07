
/* compressor.c
* zx7 compresses data into an output buffer. Designed to run on the TI-84+CE graphing calculator utilizing the C toolchain.
* This routine will also render a progress bar.
* Basic usage:
*   int output_length;
*   zx7_Compress(malloc(input_length), input_data, &output_length, input_length);
* If the data to be compressed is more than 16k, I reccomend using VRAM as a buffer, which then allows for 76800 bytes of compressed output data.
* Code:
*	gfx_SetDrawScreen();
*   *((unsigned int*)0xE30010) = 0xD40000; //make sure VRAM draws from address 0xD40000
*   //draw some kind of save indicator
*   zx7_Compress((void*)0xD52C00, input_data, &output_length, input_length);
*/

#include <fileioc.h>
#include <graphx.h>

#include "compressor.h"

//Change 0xBF to whatever color you want the progress bar to be
#define PROGRESS_BAR_COLOR 0x05
//Change 0xFF to whatever color you want the progress bar's outline to be
#define PROGRESS_BAR_OUTLINE_COLOR 0x00
//next line is put in by TimmyCraft, and changed a little to make only the graphics better. I did NOT change any compression code
//or remember changing any code that may have broke the compression.
#define PROGRESS_BAR_INSIDE_COLOR 0xB5

int bit_mask = 0;
int bit_index = 0;
int output_index = 0;
char *output_data;

//zx7 compress a block of data, return length
int zx7_Compress(char *dest,char *src,int *len,int src_len){
	int last_x,new_x;
	int index=1;
	gfx_SetColor(PROGRESS_BAR_OUTLINE_COLOR);
	gfx_Rectangle(90,120,140,9);
	//next 2 lines were put in by TimmyCraft...
	gfx_SetColor(PROGRESS_BAR_INSIDE_COLOR);
	gfx_FillRectangle(91,121,138,7);
	gfx_SetColor(PROGRESS_BAR_COLOR);
	last_x=91;
	output_data=dest;
	output_data[output_index++]=src[0];
	while ((index=get_optimal(src,src_len,index))<src_len){
		gfx_FillRectangle(last_x,121,1+(new_x=((138*index)/src_len)+91)-last_x,7);
		last_x=new_x;
	}
	write_bit(1);
	for (index=0; index<16; index++){
		write_bit(0);
	}
	write_bit(1);
	*len=output_index;
	return output_index;
}


//find optimal pattern or literal and write it to the file, returning the new input index
unsigned int get_optimal(char *data,unsigned int data_len,unsigned int input_index){
	unsigned int best_cost,best_len,best_offset,match_index,match_len,end_index,cost,offset,bits,value;
	char *input_ptr;
	char *match_ptr;
	char *end_ptr;
	char *start_ptr;
	char cur_c;
	best_len=best_offset=0;
	best_cost = -1;
	cur_c = data[input_index];
	if (input_index<=MAX_OFFSET) end_index=0;
	else end_index=input_index-MAX_OFFSET;
//	sprintf((char*)0xFB0000, "input index: %d, end index: %d, &input index: %X\n", input_index, end_index, &input_index);
	for (match_index=input_index-1; match_index>end_index; match_index--){
		if (data[match_index]==cur_c){
			start_ptr = input_ptr = &data[input_index];
			match_ptr = &data[match_index];
			if (data_len-input_index>MAX_LEN) end_ptr = &data[input_index+MAX_LEN];
			else end_ptr = &data[data_len];
			while (input_ptr<end_ptr){
				if (*++input_ptr != *++match_ptr) break;
			}
			match_len=input_ptr-start_ptr;
			if (match_len > 3){
				offset=input_index-match_index;
				value = match_len-1;
				cost = 1;
				while (value > 1) {
					cost += 2;
					value >>= 1;
				}
				cost += 1 + (offset > 128 ? 12 : 8);
				if (cost<best_cost*match_len){
					best_cost=cost; best_offset=offset; best_len=match_len;
				}
			}
		}
	}
//	sprintf((char*)0xFB0000, "best cost: %d, len: %d\n", best_cost, best_len);
	if (best_cost > best_len*9 || !best_len){
		//write a literal if it's more efficient, or if no matches found
//		sprintf((char*)0xFB0000,"literal %X\n",data[input_index]);
		write_bit(0);
		output_data[output_index++] = data[input_index];
		return input_index+1;
	} else {
		//write a pattern
		int i;
//		sprintf((char*)0xFB0000,"pattern.\n");
		value = best_len - 1;
		write_bit(1);
		for (i = 2; i <= value; i <<= 1) {
			write_bit(0);
		}
		while ((i >>= 1) > 0) {
			write_bit(value & i);
		}
		best_offset-=1;
		if (best_offset<128) {
			output_data[output_index++] = best_offset;
		} else {
			int mask;
			best_offset-=128;
			output_data[output_index++] = (best_offset&127)|128;
			for (mask = 1024; mask > 127; mask >>= 1) {
				write_bit(best_offset & mask);
			}
		}
		return input_index+best_len;
	}
}

//Used internally
void write_bit(int value) {
    if (bit_mask == 0) {
        bit_mask = 128;
        bit_index = output_index;
	output_data[output_index++] = 0;
    }
    if (value) {
	output_data[bit_index] |= bit_mask;
    }
    bit_mask >>= 1;
}


