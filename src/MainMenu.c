
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
#include "MainMenu.h"

void MainMenu()
{
	int8_t scroll = 16, list;
	curY = 125;
	// fix closing the game unnecessarily.
	kb_Scan();
	while (!(kb_IsDown(kb_KeyClear)) && !(kb_IsDown(kb_Key2nd) && curY == 200))
	{
		// draw the main menu
		kb_Scan();
		DrawDirtBackground(scroll);
		gfx_SetTextFGColor(230);
		gfx_PrintStringXY("v1.0.2", 3, 230);
		gfx_SetTextFGColor(0);

		gfx_SetTransparentColor(255);
		gfx_ScaledTransparentSprite_NoClip(logo, 32, 20, 2, 2);
		gfx_SetTransparentColor(7);
		/* buttons */
		for (y = 125; y < 225; y += 25) {
			gfx_SetColor(181);
			gfx_FillRectangle(60, y, 192, 20);
			gfx_SetColor(0);
			gfx_Rectangle(60, y, 192, 20);
			gfx_Rectangle(61, y + 1, 190, 18);
		}
		/* button text */
		for (list = 0; list < 4; list++) {
			DrawCenteredText(MenuElements[list], 160, 131 + (list * 25));
		}
		gfx_SetColor(254);
		gfx_Rectangle(60, curY, 192, 20);
		gfx_Rectangle(61, curY + 1, 190, 18);
		gfx_BlitBuffer();

		scroll++;
		if (scroll > 16)
			scroll = 0;

		if (kb_IsDown(kb_KeyUp) && curY > 125)
		{
			// delay(150);
			curY -= 25;
		}
		if (kb_IsDown(kb_KeyDown) && curY < 200)
		{
			// delay(150);
			curY += 25;
		}
		if (kb_IsDown(kb_Key2nd))
		{
			if (curY == 125)
			{ //"play menu"
				delay(100);
				playMenu();
				y = 125;
				delay(100);
				MainMenu();
			}
			if (curY == 150)
			{ //"achievements"
				delay(100);
				Achievements();
				curY = 150;
				delay(100);
				MainMenu();
			}
			if (curY == 175)
			{ //"Settings"
				delay(100);
				Settings(0);
				curY = 175;
				delay(100);
				MainMenu();
			}
		}
	}
	gfx_End();
	appvar = ti_Open("MCESETT", "w");
	ti_Write(gameSettings, sizeof gameSettings, 1, appvar);
	ti_CloseAll();
	exit(1);
}
