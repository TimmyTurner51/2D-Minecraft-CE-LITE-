
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
#include "MoreWorldOptions.h"

void MoreWorldOptions(void) {
	CursorY = 60;
	while (!(kb_IsDown(kb_KeyClear)) && !(kb_IsDown(kb_Key2nd) && CursorY == 140))
	{
		DrawDirtBackground(0);
		for (y = 60; y < 160; y += 20)
		{
			gfx_SetColor(181);
			gfx_FillRectangle(50, y, 220, 16);
			gfx_SetColor(0);
			gfx_Rectangle(50, y, 220, 16);
			gfx_Rectangle(51, y + 1, 218, 14);
		}
		gfx_SetColor(148);
		gfx_FillRectangle(0, 0, 320, 12);
		DrawCenteredText(NewWorldMenuElements[6], 160, 2);
		DrawCenteredText(NewWorldMenuElements[8], 160 - gfx_GetStringWidth(togglesOnOff[genTrees]) / 2, 64);
		gfx_PrintStringXY(togglesOnOff[genTrees], 162 - gfx_GetStringWidth(togglesOnOff[genTrees]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[8]) / 2, 64);
		DrawCenteredText(NewWorldMenuElements[9], 160 - gfx_GetStringWidth(togglesOnOff[genFlowers]) / 2, 84);
		gfx_PrintStringXY(togglesOnOff[genFlowers], 162 - gfx_GetStringWidth(togglesOnOff[genFlowers]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[9]) / 2, 84);
		DrawCenteredText(NewWorldMenuElements[10], 160 - gfx_GetStringWidth(togglesOnOff[genVillages]) / 2, 104);
		gfx_PrintStringXY(togglesOnOff[genVillages], 162 - gfx_GetStringWidth(togglesOnOff[genVillages]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[10]) / 2, 104);
		DrawCenteredText(NewWorldMenuElements[11], 160 - gfx_GetStringWidth(togglesOnOff[genCaves]) / 2, 124);
		gfx_PrintStringXY(togglesOnOff[genCaves], 162 - gfx_GetStringWidth(togglesOnOff[genCaves]) / 2 + gfx_GetStringWidth(NewWorldMenuElements[11]) / 2, 124);
		DrawCenteredText(NewWorldMenuElements[12], 160, 144);
		gfx_SetColor(254);
		gfx_Rectangle(50, CursorY, 220, 16);
		gfx_Rectangle(51, CursorY + 1, 218, 14);
		gfx_BlitBuffer();
		kb_Scan();
		if (kb_IsDown(kb_KeyUp) && (CursorY > 60)) CursorY -= 20;
		if (kb_IsDown(kb_KeyDown) && (CursorY < 140)) CursorY += 20;
		if (kb_IsDown(kb_Key2nd)) {
			delay(100);
			if (CursorY == 60) genTrees = (genTrees == 0);
			if (CursorY == 80) genFlowers = (genFlowers == 0);
			if (CursorY == 100) genVillages = (genVillages == 0);
			if (CursorY == 120) genCaves = (genCaves == 0);
		}
	}
	delay(100);
	kb_Scan();
}
