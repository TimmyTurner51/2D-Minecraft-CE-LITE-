
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
#include "LoadTextures.h"
#include "Generator.h"

void Generator(void)
{
	uint16_t x = 0, y = 0, xb = 91, blockType = 0, terrainVal, biomeVal = 0;
	uint16_t xa, ya;
	uint16_t lengthA = 0, lengthB = 0, testX = 0, testY = 0;
	uint16_t groundLevel = 33, groundLevelB, pos = 0, posb = 0, posc = 0, posd = 0;
	memset(WorldData, 0, sizeof WorldData);
	DrawDirtBackground(0);
	gfx_SetTextFGColor(254);
	DrawCenteredText(MenuElements[19], 160, 90);
	DrawCenteredText(MenuElements[20], 160, 104);
	gfx_SetColor(148);
	gfx_FillRectangle(90, 120, 320 - 180, 7);
	gfx_SetColor(0);
	gfx_Rectangle(90, 120, 320 - 180, 7);
	gfx_SetColor(6);
	gfx_SetTextFGColor(0);
	// 0 = off, 1 = on
	flymode = 0;
	x = 0;

	while (x < MaxX)
	{
		gfx_VertLine(xb, 121, 5);
		gfx_BlitBuffer();
		if (xb < 228)
			xb++;
		// tries to further randomize the world, to prevent repetitive chunks
		if (x % 20) srand(rtc_Time() * x / 20);

		// hills
		if (worldType != 1) {
			// randomize biome
			// 0 = plains, 1 = desert, 2 = forest, 3 = pond/lake/ocean, 4 = plains village, 5 = sand village
			if (randInt(0, 8) == 0) biomeVal = randInt(0, 5);
			terrainVal = 1 + (randInt(0, 1) * randInt(0, 3) * (biomeVal != 3));
			if ((groundLevel > 1) && (groundLevel < MaxY) && (randInt(0, 12) != 1))
				groundLevel += (worldType != 1) * randInt(0 - terrainVal, terrainVal);
		}else{
			biomeVal = 0;
			groundLevel = 34;
		}

		if (genTrees == 1 && worldType != 1) {
			// tree generation
			if (biomeVal != 1 && randInt(0, 8 - ((biomeVal == 1) * 3)) == 0 && WorldData[x + (groundLevel * MaxX)] != 234)
			{
				// oak tree
				pos = groundLevel - 7;
				WorldData[x + ((pos + 2) * MaxX)] = 151;
				WorldData[x + ((pos + 3) * MaxX)] = 151;
				WorldData[x + ((pos + 4) * MaxX)] = 151;
				WorldData[x + ((pos + 5) * MaxX)] = 151;
				WorldData[x + ((pos + 6) * MaxX)] = 151;
				WorldData[x + ((pos + 7) * MaxX)] = 151;
				WorldData[x + (pos * MaxX)] = 222;
				WorldData[x - 1 + (pos * MaxX)] = (222 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + 1 + (pos * MaxX)] = (222 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + ((pos + 1) * MaxX)] = 222;
				WorldData[x - 1 + ((pos + 1) * MaxX)] = 222;
				WorldData[x + 1 + ((pos + 1) * MaxX)] = 222;
				WorldData[x + ((pos + 2) * MaxX)] = 222;
				WorldData[x - 1 + ((pos + 2) * MaxX)] = 222;
				WorldData[x - 1 + ((pos + 3) * MaxX)] = 222;
				WorldData[x - 2 + ((pos + 2) * MaxX)] = (222 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x - 2 + ((pos + 3) * MaxX)] = (222 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + 1 + ((pos + 2) * MaxX)] = 222;
				WorldData[x + 1 + ((pos + 3) * MaxX)] = 222;
				WorldData[x + 2 + ((pos + 2) * MaxX)] = (222 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + 2 + ((pos + 3) * MaxX)] = (222 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
			}
			if (biomeVal != 1 && (randInt(0, 8 - ((biomeVal == 1) * 3)) == 0) && (WorldData[x + (groundLevel * MaxX)] != 234))
			{
				// spruce
				pos = groundLevel - 6;
				WorldData[x + (pos * MaxX)] = 223;
				WorldData[x + ((pos + 1) * MaxX)] = 223;
				WorldData[x - 1 + ((pos + 1) * MaxX)] = 223;
				WorldData[x + 1 + ((pos + 1) * MaxX)] = 223;
				WorldData[x + ((pos + 2) * MaxX)] = 199;
				WorldData[x + ((pos + 3) * MaxX)] = 199;
				WorldData[x + ((pos + 4) * MaxX)] = 199;
				WorldData[x + ((pos + 5) * MaxX)] = 199;
				if (randInt(0, 3) == 1)
				{
					WorldData[x - 1 + ((pos + 3) * MaxX)] = 223;
					WorldData[x + 1 + ((pos + 3) * MaxX)] = 223;
					WorldData[x - 1 + ((pos + 5) * MaxX)] = 223;
					WorldData[x + 1 + ((pos + 5) * MaxX)] = 223;
					WorldData[x - 2 + ((pos + 5) * MaxX)] = 223;
					WorldData[x + 2 + ((pos + 5) * MaxX)] = 223;
				}
			}
			// jungle = 118
			// dark oak = 72
			// acacia = 3
		}

		// foliage start at MaxSprites[0] + 1
		// plants start at MaxSprites[0] + MaxSprites[3] + 1
		// flowers (anything non-desert)
		if (genFlowers == 1 && (biomeVal == 0 || biomeVal == 2) && randInt(0, 2) == 0 && WorldData[x + ((groundLevel - 1) * MaxX)] == 0)
		{
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 2;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 3;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 9;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 19;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 23;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 31;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 37;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 38;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 41;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 42;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 48;
			// berries
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 56;
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 74;		// 73
			// rose bush
			if (randInt(0, 2) == 0)
			{
				if (randInt(0, 2) == 0)
				{
					WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 49;
					WorldData[x + ((groundLevel - 2) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 50;
				}
				else
				{
					WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 50;
				}
			}
			if (randInt(0, 2) == 0)
			{
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 29;
				WorldData[x + ((groundLevel - 2) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 30;
			}
			if (randInt(0, 2) == 0)
			{
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 39;
				WorldData[x + ((groundLevel - 2) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 40;
			}
		}
		// desert foliage (dead shrubs, cactus, etc.)
		if (biomeVal == 1 && randInt(0, 2) == 0 && WorldData[x + ((groundLevel - 1) * MaxX)] == 0)
		{
			// dead shrubs
			if (randInt(0, 2) == 0)
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 25;
			// cactus
			if (randInt(0, 2) == 0)
			{
				for (terrainVal = groundLevel - 1 - randInt(0, 2); terrainVal < groundLevel; terrainVal++)
				{
					WorldData[x + (terrainVal * MaxX)] = 40;
				}
			}
		}

		
		// Village Generation
		lengthA = randInt(3, 7);
		if (genVillages == 1 && biomeVal == 4 && (groundLevel >= 34 || worldType == 1) && x < MaxX - lengthA * 7)
		{
			// 7 is the size of a 5-block long/small house, plus 2 for 1 block ground spacing on each side
			// we can generate wider buildings, that are 12 blocks long + 2 for ground blocks (again, 1 on each side)
			lengthB = 0;
			posd = 8;
			// left-right
			for (posb = x; posb < x + lengthA * 7; posb++)
			{
				// up-down
				for (posc = groundLevel; posc < groundLevel + posd; posc++)
				{
					WorldData[posb + (posc * MaxX)] = 0;
					// gravel path
					if (posc == groundLevel)
						WorldData[posb + (posc * MaxX)] = 84;
					// dirt
					if (posc > groundLevel)
						WorldData[posb + (posc * MaxX)] = 78;
				}
				// houses
				// small house (5x5)
				// Oak planks (ID is 152)
				if (posb >= (x + (lengthA * 7)) + (lengthB + 1) && posb <= (x + (lengthA * 7)) + (lengthB + 5)) {
					// left wall
					WorldData[(lengthB + 1) + ((groundLevel) * MaxX)] = 151;
					WorldData[(lengthB + 1) + ((groundLevel - 1) * MaxX)] = 151;
					WorldData[(lengthB + 1) + ((groundLevel - 2) * MaxX)] = 151;
					WorldData[(lengthB + 1) + ((groundLevel - 3) * MaxX)] = 151;
					WorldData[(lengthB + 1) + ((groundLevel - 4) * MaxX)] = 151;
					// right wall
					WorldData[(lengthB + 5) + ((groundLevel) * MaxX)] = 151;
					WorldData[(lengthB + 5) + ((groundLevel - 1) * MaxX)] = 151;
					WorldData[(lengthB + 5) + ((groundLevel - 2) * MaxX)] = 151;
					WorldData[(lengthB + 5) + ((groundLevel - 3) * MaxX)] = 151;
					WorldData[(lengthB + 5) + ((groundLevel - 4) * MaxX)] = 151;
					// roof
					WorldData[(lengthB + 2) + ((groundLevel - 4) * MaxX)] = 152;
					WorldData[(lengthB + 3) + ((groundLevel - 4) * MaxX)] = 152;
					WorldData[(lengthB + 4) + ((groundLevel - 4) * MaxX)] = 152;
					// fill
					for (testX = lengthB + 2; testX < lengthB + 5; testX++) {
						for (testY = 0; testY < 4; testY++) {
							WorldData[testX + ((groundLevel - testY) * MaxX)] = ((testY == 0) * 54);
						}
					}
					// randomize for chest, crafting table, and/or bed
						testX = randInt(0, 4);
					// chest
					if (testX == 0) {
						WorldData[(lengthB + 4) + ((groundLevel - 1) * MaxX)] = 9;
						// add more later for its contents
					}
					// crafting table
					if (testX == 1) {
						WorldData[(lengthB + 4) + ((groundLevel - 1) * MaxX)] = 58;
					}
					// bed
					if (testX == 2) {
						WorldData[(lengthB + 2) + ((groundLevel - 1) * MaxX)] = 7;
						WorldData[(lengthB + 3) + ((groundLevel - 1) * MaxX)] = 8;
						testX = randInt(0, 3);
						// crafting table
						if (testX == 0) {
							WorldData[(lengthB + 4) + ((groundLevel - 1) * MaxX)] = 58;
						}
						// chest
						if (testX == 1) {
							WorldData[(lengthB + 4) + ((groundLevel - 1) * MaxX)] = 58;
							// add more later for it's contents
						}
					}
				}
				// large house (12x5)


				if (posb == (x + (lengthA * 7)) + lengthB + 7) lengthB += 7;
			}
		}

		// cave generation
		if (genCaves == 1 && randInt(0, 2) == 1 && x >= 21) {
			for (xa = x - 20; xa < x - 20 + randInt(6, 19); xa++) {
				for (ya = groundLevel + 10 + randInt(1, 4); ya < groundLevel + 10 + randInt(3, 6); ya++) {
					WorldData[xa + ya * MaxX] = 0;
				}
			}
		}

		// water generation
		if ((biomeVal == 3 && groundLevel == 34) || (randInt(1, 3) == 2 && groundLevel >= 34 && worldType != 1))
		{
			groundLevelB = groundLevel;
			// pos is left-to-right size
			lengthA = randInt(6, 20);
			for (posb = x - lengthA; posb < x; posb++)
			{
				// generates up to down
				for (posc = 0; posc < groundLevel - posd + randInt(2, 5); posc++)
				{
					WorldData[posb + (posc * MaxX)] = 0;
					if (posc >= groundLevel)
						WorldData[posb + (posc * MaxX)] = 215;
				}
			}
			groundLevel = groundLevelB;
		}

		// terrain
		for (y = groundLevel; y < MaxY; y++)
		{
			// grass block
			blockType = 95;
			// dirt
			if (y > groundLevel)
				blockType = 78;
			// desert
			if (biomeVal == 1 && biomeVal != 5)
			{
				// sand
				blockType = 186;
				// sandstone
				if (y > groundLevel + randInt(0, 4))
					blockType = 187;
			}
			// stone
			if (y > groundLevel + 4)
				blockType = 202;
			// ores
			if ((y > groundLevel + 10) && (y < groundLevel + 80) && (randInt(0, 10) == 0))
				blockType = 52;
			if ((y > groundLevel + 18) && (y < groundLevel + 80) && (randInt(0, 10) == 0))
				blockType = 51;
			if ((y > groundLevel + 100) && (y < groundLevel + 130) && (randInt(0, 20) == 0))
				blockType = 76;
			if ((y > groundLevel + 130) && (y < groundLevel + 140) && (randInt(0, 20) == 0))
				blockType = 83;
			WorldData[x + (y * MaxX)] = blockType;
		}
		// bedrock
		WorldData[x + ((y - 1) * MaxX)] = 15;

		x++;
	}
	playerX = 10;
	playerY = 0;
	curPos = MaxX + 11;
	curX = 11;
	curY = 6;
	health = 18;
	hunger = 18;
	damageAmount = 0;
	damageDealt = 0;
	while (WorldData[(playerX + ((playerY + 3) * MaxX))] == 0) {
		playerY++;
		curPos += MaxX;
		WorldTimerPosY++;
		if (WorldTimerPosY > 32) WorldTimerPosY = 0;
	}
	scrollX = 0;
	scrollY = 0;

	hotbar[0] = 215;
	hotbar[5] = 32;

}
