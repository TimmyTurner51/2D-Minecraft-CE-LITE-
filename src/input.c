
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

#include "compressor.h" //zx7 compression routines
#include "defines.h"
#include "input.h"

void input(char *string, int size)
{
	const char *charsUpper = "\0\0\0\0\0\0\0\0\0\0\"WRMH\0\0?[VQLG\0\0:ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
	const char *charsLower = "\0\0\0\0\0\0\0\0\0\0\"wrmh\0\0?[vqlg\0\0:zupkfc\0 ytojeb\0\0xsnida\0\0\0\0\0\0\0\0";
	const char *charsNum = "\0\0\0\0\0\0\0\0\0\0\+-*/\0\0\0?369(\0\0\0\0258)\0\0\00147,\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	uint8_t key, length = strlen(string), scroll = 0, i = 0, chartype = 0;
	// set i to the first occurence of a null character
	for (i = 0; i < length; i++) {
		if (i > 31) scroll++;
		if (string[i] == 0) break;
	}
	gfx_SetColor(181);
	gfx_FillRectangle(0, 216, 320, 24);
	gfx_SetColor(74);
	gfx_Rectangle(0, 216, 320, 24);
	gfx_Rectangle(1, 217, 318, 22);
	gfx_SetColor(181);
	while((key = os_GetCSC()) != sk_Enter) {
		gfx_SetColor(181);
		gfx_FillRectangle(1, 217, 318, 22);
		gfx_SetColor(0);
		gfx_FillRectangle(4, 223, 10, 10);
		gfx_SetTextFGColor(254);
		if (chartype == 0) gfx_PrintStringXY("A", 5, 224);
		if (chartype == 1) gfx_PrintStringXY("a", 5, 224);
		if (chartype == 2) gfx_PrintStringXY("1", 5, 224);
		gfx_SetTextFGColor(0);
		gfx_SetTextXY(18, 224);
		pos = scroll;
		for (pos = scroll; pos < scroll + 32; pos++)
		{
			gfx_SetTextFGColor(0);
			if (pos < i)
			gfx_PrintChar(string[pos]);
		}
		gfx_BlitBuffer();

		if (kb_IsDown(kb_KeyAlpha)) {
			delay(200);
			chartype++;
			if (chartype > 2) chartype = 0;
		} 

		if(charsUpper[key] && i <= size) {
			if (chartype == 0) string[i] = charsUpper[key];
			if (chartype == 1) string[i] = charsLower[key];
			if (chartype == 2) string[i] = charsNum[key];
			string[i + 1] = 0;
			i++;
			if (i > 32) scroll++;
		}
		if (kb_IsDown(kb_KeyDel) && i > 0) {
			delay(80);
			string[i] = 0;
			if (i > 32 && scroll > 0) scroll--;
			i--;
		}
	}
}
