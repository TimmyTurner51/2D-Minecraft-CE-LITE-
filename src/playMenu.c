
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
#include "DrawCenteredText.h"
#include "Game.h"
#include "compression.h"
#include "playMenu.h"

void playMenu(void)
{
	gamemode = 0;
	cheats = 0;
	findAppvars("MCCESV");
	tab = 0;
	scroll = 0;
	CursorY = 40;
	kb_Scan();
	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();
		DrawDirtBackground(0);
		gfx_SetColor(181);
		gfx_FillRectangle(20, 20, 280, 200);
		gfx_SetColor(0);
		gfx_Rectangle(20, 20, 280, 200);
		gfx_Rectangle(20, 20, 280, 20);
		gfx_SetColor(148);
		gfx_FillRectangle(21 + (tab * 89), 21, 100, 18);
		DrawCenteredText(MenuElements[8], 70, 25);
		DrawCenteredText(MenuElements[9], 160, 25);
		DrawCenteredText(MenuElements[10], 250, 25);
		if (tab == 0)
		{
			gfx_SetColor(148);
			gfx_FillRectangle(21, 40, 278, 25);
			DrawCenteredText(MenuElements[11], 160, 47);
			gfx_SetColor(254);
			gfx_Rectangle(21, CursorY, 278, 25 - ((CursorY != 40) * 8));
			gfx_Rectangle(22, CursorY + 1, 276, 23 - ((CursorY != 40) * 8));
			if (foundCount == 0) {
				gfx_PrintStringXY(MenuElements[12], 90, 120);
			}else{
				y = 0;
				for (i = scroll; i < scroll + 2; i++)
				{
					if (i < foundCount)
						gfx_PrintStringXY(WorldsList[i], 40, 70 + y);
					y += 17;
				}
				if (kb_IsDown(kb_KeyUp) && (CursorY == 65))
				{
					delay(50);
					CursorY = 40;
					scrollYb = -1;
				}
				else
				{
					if (kb_IsDown(kb_KeyUp) && (scrollYb > 0) && (CursorY != 40))
					{
						delay(50);
						CursorY -= 17;
						scrollYb--;
					}
				}
				if (kb_IsDown(kb_KeyDown) && (CursorY == 40) && (foundCount != 0))
				{
					delay(50);
					CursorY = 65;
					scrollYb = 0;
					scroll = 0;
				}
				else
				{
					if (kb_IsDown(kb_KeyDown) && (scrollYb < foundCount - 1) && (scrollYb < 10))
					{
						delay(50);
						CursorY += 17;
						scrollYb++;
					}
					if (kb_IsDown(kb_KeyDown) && (scrollYb < foundCount - 1) && (scrollYb > 9))
					{
						delay(50);
						scroll++;
						scrollYb++;
					}
				}
			}
			if (kb_IsDown(kb_Key2nd) && (CursorY > 40))
			{
				world_file = WorldsList[scrollYb];
				appvar = ti_Open(world_file, "r");
				if (!memcmp(ti_GetDataPtr(appvar), "MCCESV", 6))
				{
					int world_offset;
					ti_Seek(6, SEEK_SET, appvar);
					ti_Read(&world_offset, 3, 1, appvar);
					ti_Read(&curX, 3, 1, appvar);
					ti_Read(&curY, 3, 1, appvar);
					ti_Read(&MaxX, 3, 1, appvar);
					ti_Read(&MaxY, 3, 1, appvar);
					ti_Read(&flymode, 3, 1, appvar);
					ti_Read(&scrollX, 3, 1, appvar);
					ti_Read(&scrollY, 3, 1, appvar);
					ti_Read(&playerX, 3, 1, appvar);
					ti_Read(&playerY, 3, 1, appvar);
					ti_Read(&curPos, 3, 1, appvar);
					ti_Read(&timeofday, 3, 1, appvar);
					ti_Read(&hotbarSel, 3, 1, appvar);
					ti_Read(&hotbar, 5, 3, appvar);
					ti_Seek(world_offset, SEEK_SET, appvar);
					zx7_Decompress(&WorldData, ti_GetDataPtr(appvar));
					ti_Close(appvar);
					loaded_world = 1;
					Game();
				}
			}
			if (kb_IsDown(kb_Key2nd) && (CursorY == 40)) {
				genTrees = 1;
				genCaves = 1;
				genFlowers = 1;
				genVillages = 1;
				worldSize = 0;
				worldType = 0;
				CursorY = 60;
				NewWorldScreen();
			}
		}
		if (tab == 1)
		{
			gfx_SetColor(148);
			gfx_FillRectangle(21, 40, 278, 25);
			DrawCenteredText(MenuElements[13], 160, 47);
			gfx_SetColor(254);
			gfx_Rectangle(21, CursorY, 278, 25 - ((CursorY != 40) * 8));
			gfx_Rectangle(22, CursorY + 1, 276, 23 - ((CursorY != 40) * 8));
		}
		if (tab == 2)
		{
			DrawCenteredText(MenuElements[14], 160, 47);
		}
		gfx_BlitBuffer();

		if (kb_IsDown(kb_KeyLeft) && (tab > 0))
		{
			delay(200);
			tab--;
			CursorY = 40;
		}
		if (kb_IsDown(kb_KeyRight) && (tab < 2))
		{
			delay(200);
			tab++;
			CursorY = 40;
		}
	}
	MainMenu();
}
