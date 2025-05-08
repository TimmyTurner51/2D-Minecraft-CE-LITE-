
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
#include "Game.h"
#include "survivalInventory.h"

void survivalInventory(void) {
	int16_t xpos = 80, ypos = 140, num = 0, pos = 0;
	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();
		gfx_SetColor(148);
		gfx_FillRectangle(40, 60, 240, 162);
		// main inventory slots
		pos = 0;
		for (y = 140; y < 140 + (3 * 18); y += 18) {
			for (x = 80; x < 80 + (9 * 18); x += 18) {
				gfx_SetColor(0);
				gfx_Rectangle(x, y, 18, 18);
				if (Inventory[pos] > 0) {
				if (Inventory[pos] <= MaxSprites[0])
						gfx_TransparentSprite(BlockTextures[Inventory[pos] - 1], x + 1, y + 1);
					if (Inventory[pos] > MaxSprites[0] && Inventory[pos] <= MaxSprites[1])
						gfx_TransparentSprite(ItemTextures[Inventory[pos] - 1], x + 1, y + 1);
					if (Inventory[pos] > MaxSprites[1] && Inventory[pos] <= MaxSprites[2])
						gfx_TransparentSprite(PlantTextures[Inventory[pos] - 1], x + 1, y + 1);
					if (Inventory[pos] > MaxSprites[2] && Inventory[pos] <= MaxSprites[3])
						gfx_TransparentSprite(FoliageTextures[Inventory[pos] - 1], x + 1, y + 1);
					gfx_SetTextFGColor(254);
				}else{
					gfx_SetColor(74);
					gfx_FillRectangle(x + 1, y + 1, 16, 16);
				}
				pos++;
			}
		}
		pos = 0;
		for (y = 140; y < 140 + (3 * 18); y += 18) {
			for (x = 80; x < 80 + (9 * 18); x += 18) {
				if (Inventory[pos] > 0) {
					gfx_SetTextXY(x + 10, y + 12);
					gfx_PrintInt(Inventory[pos + 27], 1);
				}
				pos++;
			}
		}
		// hotbar
		for (x = 0; x < 5; x++) {
			if (x == hotbarSel) {
				gfx_SetColor(5);
			}else{
				gfx_SetColor(0);
			}
			gfx_Rectangle(117 + (x * 18), 200, 18, 18);

			if (hotbar[x] != 0) {
				if (hotbar[x] <= MaxSprites[0])
					gfx_TransparentSprite(BlockTextures[hotbar[x] - 1], 118 + (x * 18), 201);
				// check if it's foliage
				if (hotbar[x] > MaxSprites[0] && hotbar[x] <= MaxSprites[0] + MaxSprites[3])
					gfx_TransparentSprite(FoliageTextures[hotbar[x] - 1 - MaxSprites[0]], 118 + (x * 18), 201);
				// check if it's plants
				if (hotbar[x] > MaxSprites[0] + MaxSprites[3] && hotbar[x] <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
					gfx_TransparentSprite(PlantTextures[hotbar[x] - 1 - (MaxSprites[0] + MaxSprites[3])], 118 + (x * 18), 201);
				gfx_SetTextFGColor(0);
				gfx_SetTextXY(117 + (x * 18) + 10, 231);
				if (hotbar[x] != 0) gfx_PrintInt(hotbar[x + 5], 1);
			}else{
				gfx_SetColor(181);
				gfx_FillRectangle(118 + (x * 18), 201, 16, 16);
			}
		}
		gfx_BlitBuffer();

	}
	delay(200);
	kb_Scan();
	Game();
}
