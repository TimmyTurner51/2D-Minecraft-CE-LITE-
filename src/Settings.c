
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
#include "RenderEngine.h"
#include "DrawCenteredText.h"
#include "LoadResourcesScreen.h"
#include "Settings.h"

void Settings(bool ingameTrue)
{
	int16_t tab = 0, itemScroll = 0, pos, option, curpX = 10, curpY = 40;
	DrawDirtBackground(0);
	if (ingameTrue == 1) RenderEngine();
	while (!(kb_IsDown(kb_KeyClear)) && !(kb_IsDown(kb_Key2nd) && tab == 3))
	{
		kb_Scan();
		gfx_SetColor(74);
		gfx_FillRectangle(5, 5, 310, 230);
		gfx_SetColor(0);
		gfx_Rectangle(5, 5, 310, 230);
		gfx_Rectangle(6, 6, 308, 228);
		for (y = 40; y < 140; y += 20) {
			gfx_SetColor(181);
			if (y < 120) gfx_FillRectangle(10, y, 80, 16);
			if (tab < 2) gfx_FillRectangle(100, y, 180, 16);
			// selected tab/category
			gfx_SetColor(148);
			gfx_FillRectangle(10, 40 + tab * 20, 80, 16);
			gfx_SetColor(0);
			if (y < 120) {
				gfx_Rectangle(10, y, 80, 16);
				gfx_Rectangle(11, y + 1, 78, 14);
			}
			if (tab < 2) gfx_Rectangle(100, y, 180, 16);
			if (tab < 2) gfx_Rectangle(101, y + 1, 178, 14);
		}
		for (pos = 4; pos < 8; pos++) {
			DrawCenteredText(MenuElements[pos], 50, 44 + ((pos - 4) * 20));
		}
		gfx_SetColor(254);
		gfx_Rectangle(curpX, curpY, 180 - (100 * (curpX == 10)), 16);
		gfx_Rectangle(curpX + 1, curpY + 1, 178 - (100 * (curpX == 10)), 14);
		if (tab == 0) {
			gameSettingsStr[0] = languages[gameSettings[0]];
			gameSettingsStr[1] = SpeedStr[gameSettings[1]];
			gameSettingsStr[2] = togglesOnOff[gameSettings[2]];
			gameSettingsStr[3] = togglesOnOff[gameSettings[3]];
			for (pos = 0; pos < 4; pos++) {
				gfx_SetTextXY(105, 44 + (pos * 20));
				gfx_PrintString(GameOptionStr[pos]);
				gfx_PrintString(": ");
				gfx_PrintString(gameSettingsStr[pos]);
			}
		}
		// about tab
		if (tab == 2) {
			gfx_SetColor(148);
			gfx_FillRectangle(100, 30, 200, 130);
			gfx_FillRectangle(10, 180, 300, 30);
			gfx_SetColor(0);
			gfx_Rectangle(100, 30, 200, 130);
			gfx_Rectangle(101, 31, 198, 128);
			gfx_Rectangle(10, 180, 300, 30);
			gfx_Rectangle(11, 181, 298, 28);
			gfx_SetTextFGColor(0);
			gfx_SetTextXY(15, 185);
			gfx_PrintString("Discord:");
			gfx_SetTextFGColor(26);
			gfx_PrintString("https://discord.gg/xavH5eTJP2");
			gfx_SetTextXY(15, 196);
			gfx_SetTextFGColor(0);
			gfx_PrintString("Github:");
			gfx_SetTextFGColor(26);
			gfx_PrintString("https://github.com/TimmyTurner51");
			gfx_SetTextFGColor(0);
			for (pos = 0; pos < 12; pos++) {
				gfx_PrintStringXY(About[pos], 105, 35 + (pos * 10));
			}
		}
		gfx_BlitBuffer();

		if (kb_IsDown(kb_Key2nd) && curpX == 10) {
			tab = (curpY - 40) / 20;
		}
		if (kb_IsDown(kb_Key2nd) && curpX != 10) {
			option = (curpY - 40) / 20;
			// 4 is the amount of current options
			gameSettings[option + (tab * 4)]++;
			if (gameSettings[option + (tab * 4)] > gameSettingsMaxVals[option + (tab * 4)]) gameSettings[option + (tab * 4)] = 0;
			delay(200);
		}
		if (kb_IsDown(kb_KeyUp) || kb_IsDown(kb_KeyDown) || kb_IsDown(kb_KeyLeft) || kb_IsDown(kb_KeyRight))
		delay(80);
		if (kb_IsDown(kb_KeyUp) && curpY > 40) curpY -= 20;
		if (kb_IsDown(kb_KeyDown) && curpY < 120 - (20 * (curpX == 10))) curpY += 20;
		if (kb_IsDown(kb_KeyLeft)) {
			curpX = 10;
			curpY = 40 + tab * 20;
		}
		if (kb_IsDown(kb_KeyRight) && tab != 2) curpX = 100;
	}
	delay(100);
	appvar = ti_Open("MCESETT", "w");
	ti_Write(gameSettings, sizeof gameSettings, 1, appvar);
	ti_Close(appvar);
	LoadResourcesScreen();
}
