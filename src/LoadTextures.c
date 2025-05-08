
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
#include "LoadTextures.h"

void LoadTextures(char *name, int16_t spritenumber, int16_t spritesize, int16_t arraynum)
{
	uint8_t *ptr;
	if (spritenumber >= 0 && arraynum > 0 && arraynum <= 4) {
		ptr = os_GetAppVarData(name, NULL)->data;
		if (arraynum == 1) BlockTextures[spritenumber] = (gfx_sprite_t *)(ptr + (spritenumber * (spritesize + 2)));
		if (arraynum == 2) ItemTextures[spritenumber] = (gfx_sprite_t *)(ptr + (spritenumber * (spritesize + 2)));
		if (arraynum == 3) PlantTextures[spritenumber] = (gfx_sprite_t *)(ptr + (spritenumber * (spritesize + 2)));
		if (arraynum == 4) FoliageTextures[spritenumber] = (gfx_sprite_t *)(ptr + (spritenumber * (spritesize + 2)));
	}
}
