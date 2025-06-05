#include <graphx.h>
#include <ti/getcsc.h>
#include <ti/screen.h>
#include <ti/vars.h>
#include <string.h>
#include "Resources.h"
#include "globals.h"

//int16_t pixelAmount = 4;

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

void LoadResourcesScreen(void) {
	int16_t pos, num, speedVal = 1;
	pixelAmount = 4; // default speed
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