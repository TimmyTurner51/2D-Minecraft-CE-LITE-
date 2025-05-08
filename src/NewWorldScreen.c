
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
#include "input.h"
#include "Generator.h"
#include "Game.h"
#include "Behaviors.h"
#include "NewWorldScreen.h"

void NewWorldScreen(void) {
	while (!(kb_IsDown(kb_KeyClear)))
	{
		DrawDirtBackground(0);
		for (y = 60; y < 220; y += 20)
		{
			gfx_SetColor(181);
			gfx_FillRectangle(50, y, 220, 16);
			gfx_SetColor(0);
			gfx_Rectangle(50, y, 220, 16);
			gfx_Rectangle(51, y + 1, 218, 14);
		}
		gfx_SetColor(148);
		gfx_FillRectangle(0, 0, 320, 12);
		DrawCenteredText(MenuElements[11], 160, 2);
		DrawCenteredText(NewWorldMenuElements[0], 160 - gfx_GetStringWidth(worldNameStr) / 2, 64);
		gfx_PrintStringXY(worldNameStr, 162 - gfx_GetStringWidth(worldNameStr) / 2 + gfx_GetStringWidth(NewWorldMenuElements[0]) / 2, 64);
		DrawCenteredText(NewWorldMenuElements[1], 160 - gfx_GetStringWidth(seedStr) / 2, 84);
		gfx_PrintStringXY(seedStr, 162 - gfx_GetStringWidth(seedStr) / 2 + gfx_GetStringWidth(NewWorldMenuElements[1]) / 2, 84);
		DrawCenteredText(NewWorldMenuElements[2], 160 - gfx_GetStringWidth(gamemodeStr[gamemode]) / 2, 104);
		gfx_PrintStringXY(gamemodeStr[gamemode], 162 - gfx_GetStringWidth(gamemodeStr[gamemode]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[2]) / 2, 104);
		DrawCenteredText(NewWorldMenuElements[3], 160 - gfx_GetStringWidth(worldSizeStr[worldSize]) / 2, 124);
		gfx_PrintStringXY(worldSizeStr[worldSize], 162 - gfx_GetStringWidth(worldSizeStr[worldSize]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[3]) / 2, 124);
		DrawCenteredText(NewWorldMenuElements[4], 160 - gfx_GetStringWidth(togglesOnOff[cheats]) / 2, 144);
		gfx_PrintStringXY(togglesOnOff[cheats], 162 - gfx_GetStringWidth(togglesOnOff[cheats]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[4]) / 2, 144);
		DrawCenteredText(NewWorldMenuElements[5], 160 - gfx_GetStringWidth(worldTypesStr[worldType]) / 2, 164);
		gfx_PrintStringXY(worldTypesStr[worldType], 162 - gfx_GetStringWidth(worldTypesStr[worldType]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[5]) / 2, 164);
		DrawCenteredText(NewWorldMenuElements[6], 160, 184);
		DrawCenteredText(NewWorldMenuElements[7], 160, 204);
		gfx_SetColor(254);
		gfx_Rectangle(50, CursorY, 220, 16);
		gfx_Rectangle(51, CursorY + 1, 218, 14);
		gfx_BlitBuffer();
		kb_Scan();
		if (kb_IsDown(kb_KeyUp) && (CursorY > 60))
		{
			CursorY -= 20;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyDown) && (CursorY < 200))
		{
			CursorY += 20;
			redraw = 1;
		}
		if (kb_IsDown(kb_Key2nd))
		{
			redraw = 1;
			if (CursorY == 60)
			{
				input(worldNameStr, 40);
				delay(100);
			}
			if (CursorY == 80)
			{
				input(seedStr, 40);
				delay(100);
			}
			if (CursorY == 100)
			{
				gamemode++;
				if (gamemode > 2)
					gamemode = 0;
			}
			if (CursorY == 120)
			{
				worldSize++;
				if (worldSize > 2)
					worldSize = 0;
			}
			if (CursorY == 140)
			{
				cheats = (cheats == 0);
			}
			if (CursorY == 160)
			{
				worldType++;
				if (worldType > 2)
				{
					worldType = 0;
				}
			}
			if (CursorY == 180) {
				MoreWorldOptions();
				CursorY = 180;
			}
			if (CursorY == 200)
			{
				if (worldSize == 0)
				{
					MaxX = 16 * 4;
					MaxY = 16 * 4;
				}
				if (worldSize == 1)
				{
					MaxX = 16 * 7;
					MaxY = 16 * 7;
				}
				if (worldSize == 2)
				{
					MaxX = 16 * 9;
					MaxY = 16 * 9;
				}
				// supposed to take the first 8 letters of the worldName string and copy to world_file
				memcpy(world_file, worldNameStr, 8);

				Generator();
				Game();
				delay(200);
				redraw = 1;
				break;
			}
		}
	}
	delay(100);
	kb_Scan();
	playMenu();
}
