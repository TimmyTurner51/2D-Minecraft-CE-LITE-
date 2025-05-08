
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
#include "Behaviors.h"
#include "RenderEngine.h"

void RenderEngine(void)
{
	int16_t blockVal = 0, blockValLeft = 0, blockValRight = 0, blockValTop = 0, blockValBottom = 0;
	int16_t testX, testY, timerX, timerY;
	int16_t counter = 0, second_Last = rtc_Seconds;
	// draw sky
	// try gfx_Darken(colorValue) as time var increments
	// so maybe day/night transitioning would appear smoother
	// speedup by filling screen red and overwriting if no damage was dealt
	gfx_FillScreen(224);
	if (health >= 0)
	gfx_FillScreen(dayColors[timeofday]);
	damageDealt = 0;
	dayTimer++;
	if (dayTimer >= 500)
	{
		timeofday++;
		dayTimer = 0;
	}
	if (timeofday > 4)
		timeofday = 0;

	testX = playerX - 10;
	testY = playerY - 5;
	if (WorldTimerPosX > 32) WorldTimerPosX = 0;
	if (WorldTimerPosY > 32) WorldTimerPosY = 0;
	timerX = WorldTimerPosX;
	timerY = WorldTimerPosY;
	drawX = scrollX;
	drawY = scrollY;
	for (render = 0; render < 21 * 16; render++)
	{
		gfx_SetColor(0);
		pos = testX + testY * MaxX;
		// draw the shadowing box (not for water, lava, etc.)
		blockVal = WorldData[pos];
		blockValLeft = WorldData[pos - 1];
		blockValRight = WorldData[pos + 1];
		blockValTop = WorldData[pos - MaxX];
		blockValBottom = WorldData[pos + MaxX];
		// overrides shadowing for lava, water, and any values above or equal to what WATERENTITY is defined as.
		if (blockVal == 232 || blockVal == 233 || blockVal >= WATERENTITY)
			blockValTop = 0;
		if ((blockVal != 0) && testX >= 0 && testX <= MaxX - 10 && ((shadowing == 0) || (shadowing != 0 && ((blockValLeft == 0 || blockValRight == 0 || blockValTop == 0 || blockValBottom == 0) || (blockValLeft > MaxSprites[0] || blockValRight > MaxSprites[0] || blockValTop > MaxSprites[0] || blockValBottom > MaxSprites[0])))))
		{
			// check if it's a block
			if (blockVal <= MaxSprites[0])			// WorldData[pos] - 1
				gfx_TransparentSprite(BlockTextures[blockVal - 1], drawX, drawY);
			// check if it's foliage
			if (blockVal >= MaxSprites[0] && blockVal <= MaxSprites[0] + MaxSprites[3])
				gfx_TransparentSprite(FoliageTextures[blockVal - 1 - MaxSprites[0]], drawX, drawY);
			// check if it's plants
			if (blockVal >= MaxSprites[0] + MaxSprites[3] && blockVal <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
				gfx_TransparentSprite(PlantTextures[blockVal - 1 - (MaxSprites[0] + MaxSprites[3])], drawX, drawY);
			// load water sprite if neccessary
			if (blockVal >= WATERENTITY && blockVal <= WATERENTITY + 7)
				gfx_TransparentSprite(BlockTextures[214], drawX, drawY);
			gfx_SetColor(dayColors[timeofday]);
			if (blockVal >= WATERENTITY && blockVal <= WATERENTITY + 7)
				gfx_FillRectangle(drawX, drawY, 16, (blockVal - WATERENTITY) * 2);
		}
		gfx_SetColor(0);
		WorldDataTimer[timerX + timerY * 32] -= WorldDataTimer[timerX + timerY * 32] > 0;
		// shading
		BlockLightVals[timerX + timerY * 32] = 0;
		if (BlockLightVals[timerX + timerY * 32] == 0 && blockVal != 0 && blockVal <= MaxSprites[0] && blockVal != 215 && blockVal != 216 && (blockValLeft != 0 && blockValRight != 0 && blockValTop != 0 && blockValBottom != 0))
			BlockLightVals[timerX + timerY * 32] = lightVal;
		/*if (BlockLightVals[timerX + timerY * 32] > 0 && blockVal != 0 && blockVal <= MaxSprites[0]) {
			if (blockValTop != 0 && BlockLightVals[timerX + timerY * 32] < BlockLightVals[timerX + ((timerY - 1) * 32)]) 
				BlockLightVals[timerX + timerY * 32]--;
		}*/

		if (BlockLightVals[timerX + timerY * 32] > 0 && shadowing == 1)
		gfx_FillRectangle(drawX, drawY, 16, BlockLightVals[timerX + timerY * 32]);
		// fps counter
		counter++;
		if (rtc_Seconds >= second_Last) {
			second_Last = rtc_Seconds;
			fps = counter; // + (pixelAmount - shadowing)
			counter = 0;
		}

		drawX += 16;
		testX++;
		timerX++;
		if (timerX > 32) timerX = 0;
		if (testX == playerX + 11) {
			Behaviors(pos, timerX + timerY * 32);
			testY++;
			timerY++;
			if (timerY > 32) timerY = 0;
			testX = playerX - 10;
			timerX = WorldTimerPosX;
			drawX = scrollX;
			drawY += 16;
		}
	}

	// player
	gfx_TransparentSprite(Head_1, 16 * 9 + 9 + 8, 16 * 5 + 15);
	gfx_TransparentSprite(Body_1, 16 * 9 + 11 + 8, 16 * 5 + 23);
	gfx_TransparentSprite(Leg_1, 16 * 9 + 11 + 8, 16 * 5 + 35);
}
