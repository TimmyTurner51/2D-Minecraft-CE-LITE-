
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
#include "RenderEngine.h"
#include "DrawCenteredText.h"
#include "Game.h"
#include "compressAndWrite.h"
#include "pauseMenu.h"

void pauseMenu(void) {
	int16_t posX, posY = 100, y;
	RenderEngine();
	gfx_SetTextFGColor(0);
	while (!(kb_IsDown(kb_KeyClear)) && !(kb_IsDown(kb_Key2nd) && posY == 100)) {
		for (y = 100; y < 175; y += 25) {
			gfx_SetColor(149);
			gfx_FillRectangle(40, y, 130, 20);
			gfx_FillRectangle(41, y + 1, 128, 18);
			gfx_SetColor(0);
			if (posY == y) gfx_SetColor(255);
			gfx_Rectangle(40, y, 130, 20);
			gfx_Rectangle(41, y + 1, 128, 18);
		}
		for (pos = 15; pos < 18; pos++) {
			DrawCenteredText(MenuElements[pos], 105, 106 + ((pos - 15) * 25));
		}
		gfx_BlitBuffer();
		kb_Scan();
		if (kb_IsDown(kb_KeyUp) || kb_IsDown(kb_KeyDown)) delay(80);
		if (kb_IsDown(kb_KeyUp) && posY > 100) posY -= 25;
		if (kb_IsDown(kb_KeyDown) && posY < 150) posY += 25;
		delay(100 * (kb_IsDown(kb_KeyUp) || kb_IsDown(kb_KeyDown)));
		if (kb_IsDown(kb_Key2nd) && posY == 125) {
			Settings(1);
			RenderEngine();
		}
		if (kb_IsDown(kb_Key2nd) && posY == 150) {
			// save the world data, playerX, playerY, curPos, curX, curY, timeofday, etc...
			//world_file = "        ";
			memcpy(world_file, worldNameStr, 8);
			appvar = ti_Open(world_file, "a+");
			int world_offset;
			ti_Write("MCCESV", 6, 1, appvar);
			ti_Write((void *)0xFF0000, 3, 1, appvar); // this is overwritten later
			ti_Write(&curX, 3, 1, appvar);
			ti_Write(&curY, 3, 1, appvar);
			ti_Write(&MaxX, 3, 1, appvar);
			ti_Write(&MaxY, 3, 1, appvar);
			ti_Write(&flymode, 3, 1, appvar);
			ti_Write(&scrollX, 3, 1, appvar);
			ti_Write(&scrollY, 3, 1, appvar);
			ti_Write(&playerX, 3, 1, appvar);
			ti_Write(&playerY, 3, 1, appvar);
			ti_Write(&curPos, 3, 1, appvar);
			ti_Write(&timeofday, 3, 1, appvar);
			ti_Write(&hotbarSel, 3, 1, appvar);
			ti_Write(&hotbar, 5, 3, appvar);
			world_offset = ti_Tell(appvar);
			ti_Seek(6, SEEK_SET, appvar);
			ti_Write(&world_offset, 3, 1, appvar);
			ti_Seek(world_offset, SEEK_SET, appvar);
			compressAndWrite(&WorldData, MaxX * MaxY, appvar);
			ti_SetArchiveStatus(1, appvar);
			ti_Close(appvar);
		}
	}
	delay(200);
	kb_Scan();
	Game();
}
