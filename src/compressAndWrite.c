
//#include <stdio.h>
//#include <stdbool.h>
//#include <stddef.h>
//#include <stdint.h>
#include <tice.h>
#include <math.h>
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <debug.h>

#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

// include when multiplayer is reasonable
//#include <srldrvce.h>
//#include <usbdrvce.h>


#include <ctype.h>

#include "compressAndWrite.h"
#include "defines.h"
#include "compressor.h" //zx7 compression routines
#include "DrawDirtBackground.h"
#include "main.h"

void compressAndWrite(void *data, int len, ti_var_t fp)
{
	//	int i, j, k, num, num_i, new;
	//	uint8_t b, m;
	//	int *huffman_table = (void*)0xD52C00;
	//	uint8_t *huffman_table_2 = &huffman_table[256];
	//	uint8_t *huffman_output = &huffman_table_2[256];
	//	void *back_buffer = (void*)0xD52C00;
	//	uint8_t *ptr;
	int new_len;
	*((int *)0xE30010) = 0xD40000; // force the lcd to use the first half of VRAM so we can use the second half.
	gfx_SetDrawScreen();
	DrawDirtBackground(0);
	gfx_SetTextFGColor(0xDF);
	gfx_PrintStringXY("Saving World...", 113, 110);

	/* Maybe do huffman coding at some point
	//start by counting the occurences of each unique byte in the data
	ptr = data;
	for (i=0; i<len; i++){
		huffman_table[*ptr++]++;
	}
	//then sort by highest number of occurences
	k = 1;
	for (i=0; i<256; i++){
		num = num_i = 0;
		for (j=0; j<256; j++) {
			if ((new=huffman_table[j]) > num) {
				num = new;
			}
		}
		if (!num) break;
		huffman_table_2[num_i] = k++;
		huffman_table[j] = 0;
	}
	ptr = data;
	m = 1;
	for (i=0; i<len; i++){
		b = huffman_table_2[*ptr++];
		k = 256;
		while (k>>=1){
			if (b & k){
				while (b) {

				}
			}
		}
	}
	*/

	zx7_Compress((void *)0xD52C00, data, &new_len, len);
	ti_Write((void *)0xD52C00, new_len, 1, fp);
	main();
}
