
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
#include "LoadResourcesScreen.h"

void LoadResourcesScreen(void) {
	int16_t pos, num, speedVal = 1;
	appvar = ti_Open("MCESETT", "r");
	if (appvar) ti_Read(gameSettings, sizeof gameSettings, 1, appvar);
	ti_Close(appvar);
	if (gameSettings[1] > 0) {
		for (pos = 0; pos < gameSettings[1]; pos++) {
			speedVal = speedVal + speedVal;
		}
	}
	pixelAmount = speedVal;
	shadowing = gameSettings[2];
	// load languages/menu elements
	if (gameSettings[0] == 0) {
		memcpy(gamemodeStr, gamemodeStrEN, sizeof gamemodeStrEN);
		memcpy(worldSizeStr, worldSizeStrEN, sizeof worldSizeStrEN);
		memcpy(worldTypesStr, worldTypesStrEN, sizeof worldTypesStrEN);
		memcpy(togglesOnOff, togglesOnOffEN, sizeof togglesOnOffEN);
		memcpy(SpeedStr, SpeedStrEN, sizeof SpeedStrEN);
		memcpy(GameOptionStr, GameOptionStrEN, sizeof GameOptionStrEN);
		memcpy(MenuElements, MenuElementsEN, sizeof MenuElementsEN);
		memcpy(NewWorldMenuElements, NewWorldMenuElementsEN, sizeof NewWorldMenuElementsEN);
		memcpy(About, AboutEN, sizeof AboutEN);
	}
	if (gameSettings[0] == 1) {
		memcpy(gamemodeStr, gamemodeStrNL, sizeof gamemodeStrNL);
		memcpy(worldSizeStr, worldSizeStrNL, sizeof worldSizeStrNL);
		memcpy(worldTypesStr, worldTypesStrNL, sizeof worldTypesStrNL);
		memcpy(togglesOnOff, togglesOnOffNL, sizeof togglesOnOffNL);
		memcpy(SpeedStr, SpeedStrNL, sizeof SpeedStrNL);
		memcpy(GameOptionStr, GameOptionStrNL, sizeof GameOptionStrNL);
		memcpy(MenuElements, MenuElementsNL, sizeof MenuElementsNL);
		memcpy(NewWorldMenuElements, NewWorldMenuElementsNL, sizeof NewWorldMenuElementsNL);
		memcpy(About, AboutNL, sizeof AboutNL);
	}

	// load default textures
	gfx_FillScreen(224);
	// rectangle outline
	gfx_SetColor(255);
	gfx_Rectangle(157 - 2 * 30, 139, 4 * 30, 20);
	gfx_Rectangle(158 - 2 * 30, 140, 4 * 30 - 2, 18);
	for (pos = 1; pos < 5; pos++) {
		gfx_FillRectangle(160 - 2 * 30, 142, (pos * 30) - 6, 14);
		gfx_BlitBuffer();
		for (num = 0; num < MaxSprites[pos - 1]; num++) {
			if (pos == 1) LoadTextures("CLASSICB", num, 16 * 16, pos);
			if (pos == 2) LoadTextures("CLASSICI", num, 16 * 16, pos);
			if (pos == 3) LoadTextures("CLASSICP", num, 16 * 16, pos);
			if (pos == 4) LoadTextures("CLASSICF", num, 16 * 16, pos);
		}
	}
}
