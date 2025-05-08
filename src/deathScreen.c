
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
#include "DrawCenteredText.h"
#include "deathScreen.h"

void deathScreen(void)
{
	int16_t y;
	// death screen
	for (y = 100; y < 140; y += 20) {
		gfx_SetColor(148);
		gfx_FillRectangle(40, y, 240, 15);
		gfx_SetColor(74);
		gfx_Rectangle(40, y, 240, 15);
		gfx_Rectangle(41, y + 1, 238, 13);
	}
	y = 100;
	while (!(kb_IsDown(kb_KeyClear))) {
		kb_Scan();
		DrawCenteredText(MenuElements[19], 169, 105);
		DrawCenteredText(MenuElements[18], 160, 125);
		gfx_SetColor(254);
		if ((kb_IsDown(kb_KeyUp) && y != 100) && (kb_IsDown(kb_KeyDown) && y != 120))
			gfx_SetColor(74);
		gfx_Rectangle(40, y, 240, 15);
		gfx_Rectangle(41, y + 1, 238, 13);
		if (kb_IsDown(kb_KeyUp)) y = 100;
		if (kb_IsDown(kb_KeyDown)) y = 120;
		gfx_BlitBuffer();
	}
}
