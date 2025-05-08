
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
#include "DrawDirtBackground.h"

void DrawDirtBackground(int16_t scrollVal) {
	for (x = 0; x < 320; x += 16) {
		for (y = 0 - scrollVal; y < 240 + scrollVal; y += 16) {
			gfx_TransparentSprite(BlockTextures[77], x, y);
		}
	}
}
