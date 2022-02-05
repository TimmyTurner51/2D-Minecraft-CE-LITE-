////////////////////////////////////////////////////////////////////////////////
//  2D Minecraft CE *lite* (C) v1.0.0a
//  Authors: TimmyTurner51, LogicalJoe, and Beckadamtheinventor
//  License: GNU GPL v3
//  Description: A 2D Minecraft clone made for the TI-84 Plus CE, written in C.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>

#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>

#include <srldrvce.h>
#include <usbdrvce.h>

#include "gfx/gfx.h"

#include <ctype.h>

#include "compressor.h" //zx7 compression routines
#include "defines.h"

//#define usb_callback_data_t usb_device_t

void LoadBlock(char *name, int16_t spritenumber, int16_t spritesize, int16_t num);
void DrawCenteredText(char *inputstr, int centerXpos, int ypos);
void Generator(void);
void Game(void);
void RenderEngine(void);
void deathScreen(void);
void Behaviors(int16_t position, int16_t timerPos);
void survivalInventory(void);
void CacheBlocks(int16_t xa, int16_t ya);
int craftingCheck(int16_t inputVal);
void giveItem(int16_t blockID, int16_t amount);
void creativeInventory(void);
void input(char *string, int size);

// Amount of chunks to render (chunks are 16x16) max is 5 by 5 for now
int16_t renderX = 1, renderY = 1;

// Amount of Pixels to scroll. Ranges from 1 - 16. 1 being smooth but slowest, 16 being jumpy but fastest.
int16_t pixelAmount = 4;
// Amount of Pixels to scroll for falling/gravity (use 1, 2, 4, 6, 8, or 16)
int16_t gravityPixelAmount = 5;

int main(void)
{
	int16_t y;
	gfx_Begin();
	ti_CloseAll();
	gfx_SetClipRegion(-17, -17, 337, 257);
	y = 125;
	gfx_SetDrawBuffer();
	gfx_SetTextFGColor(0);

	MainMenu();
	// quit the game
	gfx_End();
}

void LoadBlock(char *name, int16_t spritenumber, int16_t spritesize, int16_t num)
{
	uint8_t *ptr;
	if (spritenumber >= 0) {
		ptr = os_GetAppVarData(name, NULL)->data;
		sprites[num] = (gfx_sprite_t *)(ptr + (spritenumber * (spritesize + 2)));
		// block is the RLETsprite conversion. Obviously.
		//gfx_ConvertToRLETSprite(sprites[num], block);
	}
}

void DrawCenteredText(char *inputstr, int centerXpos, int ypos)
{
	gfx_PrintStringXY(inputstr, centerXpos - gfx_GetStringWidth(inputstr) / 2, ypos);
}

void Generator(void)
{
	uint16_t x = 0, y = 0, xb = 91, blockType = 0, terrainVal, biomeVal = 0;
	uint16_t lengthA = 0, lengthB = 0, testX = 0, testY = 0;
	uint16_t groundLevel = 33, groundLevelB, pos = 0, posb = 0, posc = 0, posd = 0;
	memset(WorldData, 0, sizeof WorldData);
	LoadBlock("CLASSICB", 77, 16 * 16, 0);
	for (x = 0; x < 320; x += 16)
	{
		for (y = 0; y < 240; y += 16)
		{
			gfx_TransparentSprite(sprites[0], x, y);
		}
	}
	gfx_SetTextFGColor(254);
	gfx_PrintStringXY("Generating World", 102, 90);
	gfx_PrintStringXY("Building Terrain", 104, 104);
	gfx_PrintStringXY("v1.0.1a by TimmyCraft", 2, 230);
	gfx_SetColor(148);
	gfx_FillRectangle(90, 120, 320 - 180, 7);
	gfx_SetColor(0);
	gfx_Rectangle(90, 120, 320 - 180, 7);
	gfx_SetColor(6);
	gfx_SetTextFGColor(0);
	// 0 = off, 1 = on
	flymode = 0;
	x = 0;

	while (x <= MaxX)
	{
		gfx_VertLine(xb, 121, 5);
		gfx_BlitBuffer();
		if (xb < 228)
			xb++;
		// tries to further randomize the world, to prevent repetitive chunks
		if (x % 20)
			srand(x);
		// randomize biome
		// 0 = plains, 1 = desert, 2 = forest, 3 = pond/lake/ocean, 4 = plains village, 5 = sand village
		if (worldType == 1) biomeVal = 0;
		if (worldType != 1 && randInt(0, 8) == 0)
			biomeVal = randInt(0, 5);

		// since sand village generation isn't implemented yet, change it to desert instead until it's added
		if (biomeVal == 5)
			biomeVal = 1;

		// hills
		if (worldType != 1) {
			terrainVal = 1 + (randInt(0, 1) * randInt(0, 3) * (biomeVal != 3));
			if ((groundLevel > 1) && (groundLevel < MaxY) && (randInt(0, 12) != 1))
				groundLevel += (worldType != 1) * randInt(0 - terrainVal, terrainVal);
		}

		if (worldType != 1) {
			// tree generation
			if (biomeVal != 1 && randInt(0, 8 - ((biomeVal == 1) * 3)) == 0 && WorldData[x + (groundLevel * MaxX)] != 234)
			{
				// oak tree
				pos = groundLevel - 7;
				WorldData[x + (pos * MaxX)] = 240;
				WorldData[x - 1 + (pos * MaxX)] = (240 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + 1 + (pos * MaxX)] = (240 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + ((pos + 1) * MaxX)] = 240;
				WorldData[x - 1 + ((pos + 1) * MaxX)] = 240;
				WorldData[x + 1 + ((pos + 1) * MaxX)] = 240;
				WorldData[x + ((pos + 2) * MaxX)] = 240;
				WorldData[x - 1 + ((pos + 2) * MaxX)] = 240;
				WorldData[x - 1 + ((pos + 3) * MaxX)] = 240;
				WorldData[x - 2 + ((pos + 2) * MaxX)] = (240 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x - 2 + ((pos + 3) * MaxX)] = (240 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + 1 + ((pos + 2) * MaxX)] = 240;
				WorldData[x + 1 + ((pos + 3) * MaxX)] = 240;
				WorldData[x + 2 + ((pos + 2) * MaxX)] = (240 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + 2 + ((pos + 3) * MaxX)] = (240 * ((randInt(0, 3) == 1) * WorldData[x - 2 + ((pos + 2) * MaxX)] == 0));
				WorldData[x + ((pos + 2) * MaxX)] = 150;
				WorldData[x + ((pos + 3) * MaxX)] = 150;
				WorldData[x + ((pos + 4) * MaxX)] = 150;
				WorldData[x + ((pos + 5) * MaxX)] = 150;
				WorldData[x + ((pos + 6) * MaxX)] = 150;
			}
			if (biomeVal != 1 && (randInt(0, 8 - ((biomeVal == 1) * 3)) == 0) && (WorldData[x + (groundLevel * MaxX)] != 234))
			{
				// spruce
				pos = groundLevel - 6;
				WorldData[x + (pos * MaxX)] = 241;
				WorldData[x + ((pos + 1) * MaxX)] = 241;
				WorldData[x - 1 + ((pos + 1) * MaxX)] = 241;
				WorldData[x + 1 + ((pos + 1) * MaxX)] = 241;
				WorldData[x + ((pos + 2) * MaxX)] = 200;
				WorldData[x + ((pos + 3) * MaxX)] = 200;
				WorldData[x + ((pos + 4) * MaxX)] = 200;
				WorldData[x + ((pos + 5) * MaxX)] = 200;
				if (randInt(0, 3) == 1)
				{
					WorldData[x - 1 + ((pos + 3) * MaxX)] = 241;
					WorldData[x + 1 + ((pos + 3) * MaxX)] = 241;
					WorldData[x - 1 + ((pos + 5) * MaxX)] = 241;
					WorldData[x + 1 + ((pos + 5) * MaxX)] = 241;
					WorldData[x - 2 + ((pos + 5) * MaxX)] = 241;
					WorldData[x + 2 + ((pos + 5) * MaxX)] = 241;
				}
			}
			// jungle = 118
			// dark oak = 72
			// acacia = 3
		}

		// foliage start at MaxSprites[0] + 1
		// plants start at MaxSprites[0] + MaxSprites[3] + 1
		// flowers (anything non-desert)
		if (biomeVal != 1 && randInt(0, 2) == 0 && WorldData[x + ((groundLevel - 1) * MaxX)] == 0)
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
				WorldData[x + ((groundLevel - 1) * MaxX)] = MaxSprites[0] + MaxSprites[3] + 74;
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
					WorldData[x + (terrainVal * MaxX)] = 43;
				}
			}
		}

		// Village Generation
		if (biomeVal == 4 && (groundLevel >= 34 || worldType == 1) && x >= 21)
		{
			// 7 is the size of a 5-block long/small house, plus 2 for 1 block ground spacing on each side
			// we can generate wider buildings, that are 12 blocks long + 2 for ground blocks (again, 1 on each side)
			lengthA = randInt(3, 7);
			lengthB = 0;
			posd = 8;
			// left-right
			for (posb = x - lengthA * 7; posb < x; posb++)
			{
				// up-down
				for (posc = groundLevel; posc < groundLevel + posd; posc++)
				{
					//WorldData[posb + (posc * MaxX)] = 0;
					// gravel path
					if (posc == groundLevel)
						WorldData[posb + (posc * MaxX)] = 84;
					// dirt
					if (posc > groundLevel)
						WorldData[posb + (posc * MaxX)] = 78;
				}
				// houses
				// small house (5x5)
				// Oak planks (ID is 164)
				if (posb >= (x - (lengthA * 7)) + (lengthB + 1) && posb <= (x - (lengthA * 7)) + (lengthB + 5)) {
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


				if (posb == (x - (lengthA * 7)) + lengthB + 7) lengthB += 7;
			}
		}

		// water generation
		if (biomeVal == 3 && groundLevel >= 34)
		{
			groundLevelB = groundLevel;
			// pos is left-to-right size
			lengthA = randInt(6, 20);
			for (posb = x - lengthA; posb < x; posb++)
			{
				// generates up to down
				for (posc = 0; posc < groundLevel - posd + randInt(2, 5); posc++)
				{
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
		if (WorldTimerPosY > 70) WorldTimerPosY = 0;
	}
	scrollX = 0;
	scrollY = 0;

	hotbar[0] = 215;
	hotbar[5] = 32;

}

void Game(void)
{
	int16_t gravityVar = gravityPixelAmount, listPosition = 0;
	int16_t blockAboveHead = 0, blockAtCenter = 0;
	int16_t blockLeftCenter = 0, blockLeftBottom = 0;
	int16_t blockRightCenter = 0, blockRightBottom = 0;
	int16_t blockAtFeet = 0, blockBelowFeet = 0;
	gfx_SetClipRegion(0 - 17, 0 - 17, 337, 257);
	RenderEngine();

	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();

		RenderEngine();

		blockAboveHead =  WorldData[playerX + (playerY * MaxX)];
		blockLeftCenter =  WorldData[playerX - 1 + ((playerY + 1) * MaxX)];
		blockLeftBottom =  WorldData[playerX - 1 + ((playerY + 2) * MaxX)];
		blockRightCenter =  WorldData[playerX + 1 + ((playerY + 1) * MaxX)];
		blockRightBottom =  WorldData[playerX + 1 + ((playerY + 2) * MaxX)];
		blockAtCenter = WorldData[playerX + ((playerY + 1) * MaxX)]; 
		blockAtFeet =  WorldData[playerX + ((playerY + 2) * MaxX)];
		blockBelowFeet =  WorldData[playerX + ((playerY + 3) * MaxX)];

		if (kb_IsDown(kb_KeyStat) || kb_IsDown(kb_KeyAlpha) || kb_IsDown(kb_KeyApps) || kb_IsDown(kb_KeyMode)) {
			delay(100);
			curPos += (kb_IsDown(kb_KeyStat) && curX < 20) - (kb_IsDown(kb_KeyAlpha) && curX > 1);
			curX += (kb_IsDown(kb_KeyStat) && curX < 20) - (kb_IsDown(kb_KeyAlpha) && (curX > 1));
			curPos += ((kb_IsDown(kb_KeyApps) && curY < 15) * MaxX) - ((kb_IsDown(kb_KeyMode) && curY > 1) * MaxX);
			curY += (kb_IsDown(kb_KeyApps) && curY < 15) - (kb_IsDown(kb_KeyMode) && curY > 1);
		}

		if (kb_IsDown(kb_KeyGraphVar)) {
			if (gamemode == 0 || gamemode == 2) survivalInventory();
			if (gamemode == 1) creativeInventory();
		}

		if (kb_IsDown(kb_KeyYequ)) hotbarSel = 0;
		if (kb_IsDown(kb_KeyWindow)) hotbarSel = 1;
		if (kb_IsDown(kb_KeyZoom)) hotbarSel = 2;
		if (kb_IsDown(kb_KeyTrace)) hotbarSel = 3;
		if (kb_IsDown(kb_KeyGraph)) hotbarSel = 4;


		if (dialog != 0) {
			dialogTimer--;
			if (dialogTimer == 0) dialog = 0;
		}

		// gives max count (64) to selected block/item in hotbar if in creative mode
		if (gamemode == 1 && hotbar[hotbarSel] != 0) hotbar[hotbarSel + 5] = 64;

		if (kb_IsDown(kb_Key2nd) && hotbar[hotbarSel] != 0 && hotbar[hotbarSel + 5] > 0)
		{
			if (WorldData[curPos] == 0 || WorldData[curPos] == 233 || WorldData[curPos] >= WATERENTITY) {
				WorldData[curPos] = hotbar[hotbarSel];
				hotbar[hotbarSel + 5]--;
				if (hotbar[hotbarSel + 5] < 1) hotbar[hotbarSel] = 0;
			}
		}
		if (kb_IsDown(kb_KeyDel) && WorldData[curPos] != 0) {
			WorldData[curPos] = 0;
			// add code here for survival and adventure (block breaks)
			giveItem(WorldData[curPos], 1);
		}

		if (kb_IsDown(kb_KeyMath))
		{
			input(dialogString, 40);
			dialogTimer = 100;
			dialog = 1;
			// 0 = survival, 1 = creative, 2 = adventure
		}

		listPosition++;
		if (listPosition > 7)
			listPosition = 0;

		keyPresses[listPosition] = (kb_IsDown(kb_KeyUp));
		if (kb_IsDown(kb_KeyLeft) && playerX > 10 && blockLeftCenter != 0 && scrollX < 0) scrollX += pixelAmount;
		if (kb_IsDown(kb_KeyRight) && playerX < MaxX && blockRightCenter != 0 && scrollX > -16 + 6) scrollX -= pixelAmount;
		if (kb_IsDown(kb_KeyLeft) && playerX > 10 && (blockLeftCenter == 0 || blockLeftCenter > MaxSprites[0])) scrollX += pixelAmount;
		if (kb_IsDown(kb_KeyRight) && playerX < MaxX && (blockRightCenter == 0 || blockLeftCenter > MaxSprites[0])) scrollX -= pixelAmount;
		if (flymode == 1) {
			if (kb_IsDown(kb_KeyUp) && playerY > 5 && blockAboveHead == 0) scrollY += pixelAmount;
			if (kb_IsDown(kb_KeyDown) && playerY < MaxY && blockBelowFeet == 0) scrollY -= pixelAmount;
			// try fixing the scrollY var glitch when you jump and noclip into a block
			if (scrollY < 0 && jump == 1 && flymode == 0 && (WorldData[(playerX + ((playerY + 1) * MaxX))] != 0 || WorldData[(playerX + ((playerY + 2) * MaxX))] != 0))
			{
				scrollY = 0;
				curY--;
				curPos -= MaxX;
				jump = 0;
			}
		}else{
			// auto jumps on a block
			if ((playerY > 0) && (jump == 0) && (blockAtFeet > 0) && (blockAtFeet - 2 < MaxSprites[0] + 4) && (blockAtFeet != 233) && (blockAtFeet != WATERENTITY))
			{
				curPos -= MaxX;
				scrollY = 0;
				playerY--;
				WorldTimerPosY--;
				// need to overwrite this variable so gravity isn't buggy after auto-jumping
				blockBelowFeet = 1;
			}
			if (kb_IsDown(kb_KeyUp) && playerY > 5 && blockAboveHead == 0 && jump == 0) {
				jump = 1;
				scrollY += 16;
				curY++;
				playerY--;
				WorldTimerPosY--;
			}else{
				if (blockBelowFeet == 0 && jump != 1) {
					scrollY -= pixelAmount;
					if (scrollY <= -16) {
						playerY++;
						WorldTimerPosY++;
						curPos += MaxX;
						scrollY = 0;
						jump = 0;
					}
				}
				if (jump == 1) {
					scrollY -= pixelAmount;
					if (scrollY <= -16) {
						playerY++;
						curY--;
						WorldTimerPosY++;
						scrollY = -16;
						jump = 0;
					}
				}
			}
		}
		playerX += (scrollX < -16) - (scrollX > 0);
		playerY += (scrollY < -16) - (scrollY > 0);
		WorldTimerPosX += (scrollX < -16) - (scrollX > 0);
		WorldTimerPosY += (scrollY < -16) - (scrollY > 0);
		if (scrollX > 0) {
			curPos--;
			scrollX = -16 + (pixelAmount - -scrollX);
			WorldTimerPosX--;
		}
		if (scrollX < -16) {
			curPos++;
			scrollX = (scrollX + pixelAmount) + 16;
			WorldTimerPosX++;
		}
		if (scrollY > 0) {
			curPos -= MaxX;
			scrollY = -16 + (pixelAmount - -scrollY);
			WorldTimerPosY--;
		}
		if (scrollY < -16) {
			curPos += MaxX;
			scrollY = (scrollY + pixelAmount) + 16;
			WorldTimerPosY++;
		}


		// double up to toggle fly mode
		if (gamemode != 0 && gamemode != 2) {
			testVar = 0;
			x = 0;
			y = 0;
			for (pos = 0; pos < 8; pos++)
			{
				if (keyPresses[pos] != 0 && keyPresses[pos + 1] == 0 && x != 0 && y == 0)
					y = pos;
				if (keyPresses[pos] != 0 && keyPresses[pos + 1] == 0 && x == 0)
					x = pos;
			}
			// 0 1 1 0 1 0 0 0 (as an example)
			if (y - x >= 1) // && (!(kb_IsDown(kb_KeyUp))))
			{
				timer = 0;
				dialogTimer = 50;
				dialog = 1;
				// 0 = off, 1 = on
				flymode++;
				if (flymode > 1)
					flymode = 0;
				if (flymode == 0)
					strcpy(dialogString, "Fly Mode toggled Off");
				if (flymode == 1)
					strcpy(dialogString, "Fly Mode toggled On");
				memset(keyPresses, 0, sizeof keyPresses);
				listPosition = 0;
			}
			// turn off fly mode if you touch the ground
			if (kb_IsDown(kb_KeyDown) && flymode == 1 && playerY < MaxY - 15 && WorldData[(playerX + ((playerY + 3) * MaxX))] != 0 && WorldData[(playerX + ((playerY + 3) * MaxX))] != 233 && WorldData[(playerX + ((playerY + 3) * MaxX))] != WATERENTITY)
				flymode = 0;
		}
		
		if (WorldTimerPosX > 59) WorldTimerPosX = 0;
		if (WorldTimerPosX < 1) WorldTimerPosX = 60;
		if (WorldTimerPosY > 64) WorldTimerPosY = 0;
		if (WorldTimerPosY < 1) WorldTimerPosY = 65;

	}

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

void RenderEngine(void)
{
	int16_t blockValLeft = 0, blockValRight = 0, blockValTop = 0, blockValBottom = 0, timerX, timerY;
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

//	count = 0;
//	CacheBlocks(playerX, playerY);

	timerX = WorldTimerPosX;
	timerY = WorldTimerPosY;
	testX = playerX - 10;
	testY = playerY - 5;
	for (render = 0; render < 20 * 15; render++)
	{
		pos = (testX + (testY * MaxX));
		Behaviors(pos, timerX + timerY * 80);
		testX++;
		timerX++;
		if (timerX > WorldTimerPosX) timerX = WorldTimerPosX;
		if (testX >= playerX + 21) {
			testX = playerX - 10;
			testY++;
			timerY++;
			if (timerY > WorldTimerPosY) timerY = WorldTimerPosY;
		}
	}
	timerX = WorldTimerPosX;
	timerY = WorldTimerPosY;
	testX = playerX - 10;
	testY = playerY - 5;
	drawX = scrollX - 8;
	drawY = scrollY;
	for (render = 0; render < 22 * 16; render++)
	{
		gfx_SetColor(32);
		pos = (testX + (testY * MaxX));
		// draw the shadowing box (not for water, lava, etc.)
		blockValLeft = WorldData[pos - 1];
		blockValRight = WorldData[pos + 1];
		blockValTop = WorldData[pos - MaxX];
		blockValBottom = WorldData[pos + MaxX];
		// overrides shadowing for water and any values above or equal to what WATERENTITY is defined as.
		if (WorldData[pos] == 233 || WorldData[pos] >= WATERENTITY)
			blockValTop = 0;
		if ((WorldData[pos] != 0) && ((shadowing == 0) || (shadowing != 0 && ((blockValLeft == 0 || blockValRight == 0 || blockValTop == 0 || blockValBottom == 0) || (blockValLeft > MaxSprites[0] || blockValRight > MaxSprites[0] || blockValTop > MaxSprites[0] || blockValBottom > MaxSprites[0])))))
		{
			// check if it's a block
			if (WorldData[pos] <= MaxSprites[0])			// WorldData[pos] - 1
				LoadBlock("CLASSICB", WorldData[pos] - 1, 16 * 16, 0);
			// check if it's foliage
			if (WorldData[pos] >= MaxSprites[0] && WorldData[pos] <= MaxSprites[0] + MaxSprites[3])
				LoadBlock("CLASSICF", WorldData[pos] - 1 - MaxSprites[0], 16 * 16, 0);
			// check if it's plants
			if (WorldData[pos] >= MaxSprites[0] + MaxSprites[3] && WorldData[pos] <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
				LoadBlock("CLASSICP", WorldData[pos] - 1 - (MaxSprites[0] + MaxSprites[3]), 16 * 16, 0);
			// load water sprite if neccessary
			if (WorldData[pos] >= WATERENTITY && WorldData[pos] <= WATERENTITY + 7)
				LoadBlock("CLASSICB", 214, 16 * 16, 0);
//			gfx_SetTextXY(5, 5);
//			gfx_PrintInt(count, 1);
//			for (listPos = 0; listPos < count; listPos++) {
//				if (WorldData[pos] == WorldDataSprites[listPos]) {
//					gfx_ConvertToRLETSprite(sprites[0], block);
//					gfx_RLETSprite(block, drawX, drawY);
//				}
//			}

			if (WorldData[pos] != 0) gfx_TransparentSprite(sprites[0], drawX, drawY);
			gfx_SetColor(dayColors[timeofday]);
			if (WorldData[pos] >= WATERENTITY && WorldData[pos] <= WATERENTITY + 7)
				gfx_FillRectangle(drawX, drawY, 16, (WorldData[pos] - WATERENTITY) * 2);
		}else{
			if (WorldData[pos] != 0)
				gfx_FillRectangle(drawX, drawY, 16, 16);
		}
		drawY += 16;
		testY++;
		timerY++;
		if (timerY > WorldTimerPosY) timerY = WorldTimerPosY;
		if (testY >= playerY + 11) {
			testY = playerY - 5;
			testX++;
			timerX++;
			if (timerX > WorldTimerPosX) timerX = WorldTimerPosX;
			drawY = scrollY;
			drawX += 16;
		}
	}

	gfx_SetTextFGColor(0);
	gfx_SetTextXY(25, 25);
	gfx_PrintString("X:");
	gfx_PrintInt(playerX, 1);
	gfx_SetTextXY(98, 25);
	gfx_PrintString("Y:");
	gfx_PrintInt(playerY, 1);


	gfx_SetTextXY(10, 10);
	gfx_PrintInt(WorldData[curPos], 1);


	// cursor
	if (timeofday <= 2) gfx_SetColor(0);
	if (timeofday > 2) gfx_SetColor(254);
	gfx_Rectangle(scrollX + (curX * 16) - 8, scrollY + (curY * 16), 16, 16);

	// add health, hunger, and exp bars for survival and adventure modes
	if (gamemode == 0 || gamemode == 2) {
		gfx_SetTransparentColor(148);
		y = 0;
		for (x = 4; x < 9 * 8; x += 8) {
			if (y > health) {
			gfx_TransparentSprite(heart_empty, x, 4);
			}else{
				if (y <= health)
				gfx_TransparentSprite(heart_full, x, 4);
				if (!(y % 2) && y == health)
				gfx_TransparentSprite(heart_half, x, 4);
			}
			y += 2;
		}
		gfx_SetTransparentColor(7);
	}


	// revamped hotbar
	// (update: Disappears when entering the inventory, becuase this was visible even when overlayed)
	if (!kb_IsDown(kb_KeyGraphVar) || health <= 0)
	{
		gfx_SetColor(149);
		gfx_SetTextFGColor(254);
		gfx_Rectangle(116, 218, 5 * 18 + 2, 20);
		for (x = 0; x < 5; x++) {
			gfx_SetColor(149);
			if (x == hotbarSel) gfx_SetColor(74);
			gfx_Rectangle(117 + (x * 18), 219, 18, 18);
			gfx_Rectangle(118 + (x * 18), 220, 17, 17);
			if (hotbar[x] != 0) {
				if (hotbar[x] <= MaxSprites[0])
					LoadBlock("CLASSICB", hotbar[x] - 1, 16 * 16, 0);
				// check if it's foliage
				if (hotbar[x] > MaxSprites[0] && hotbar[x] <= MaxSprites[0] + MaxSprites[3])
					LoadBlock("CLASSICF", hotbar[x] - 1 - MaxSprites[0], 16 * 16, 0);
				// check if it's plants
				if (hotbar[x] > MaxSprites[0] + MaxSprites[3] && hotbar[x] <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
					LoadBlock("CLASSICP", hotbar[x] - 1 - (MaxSprites[0] + MaxSprites[3]), 16 * 16, 0);
				gfx_TransparentSprite(sprites[0], 119 + (x * 18), 221);
			}
		}
		if (gamemode == 0 || gamemode == 2) {
			for (x = 0; x < 5; x++) {
				gfx_SetTextXY(117 + (x * 18) + 10, 231);
				if (hotbar[x] != 0) gfx_PrintInt(hotbar[x + 5], 1);
			}
		}
	}

	if (health <= 0) deathScreen();

	// dialog/pop up text
	if (dialog != 0)
	{
		gfx_SetColor(181);
		gfx_FillRectangle(2, 2, 316, 14);
		gfx_SetTextFGColor(0);
		gfx_PrintStringXY(dialogString, 6, 6);
	}

	// player
	gfx_TransparentSprite(Head_1, 16 * 9 + 9, 16 * 5 + 15);
	gfx_TransparentSprite(Body_1, 16 * 9 + 11, 16 * 5 + 23);
	gfx_TransparentSprite(Leg_1, 16 * 9 + 11, 16 * 5 + 35);

	gfx_BlitBuffer();
}

void deathScreen(void)
{
	int16_t y;
	// death screen
	for (y = 100; y < 140; y += 20) {
		gfx_SetColor(148);
		gfx_FillRectangle(40, y, 240, 15);
		gfx_SetColor(74);
		gfx_Rectangle(40, y, 240, 15);
		gfx_Rectangle(41, y + 1, 238, 13);
	}
	y = 100;
	while (!(kb_IsDown(kb_KeyClear))) {
		kb_Scan();
		DrawCenteredText("Respawn", 169, 105);
		DrawCenteredText("Back to Main Menu", 160, 125);
		gfx_SetColor(254);
		if ((kb_IsDown(kb_KeyUp) && y != 100) && (kb_IsDown(kb_KeyDown) && y != 120))
			gfx_SetColor(74);
		gfx_Rectangle(40, y, 240, 15);
		gfx_Rectangle(41, y + 1, 238, 13);
		if (kb_IsDown(kb_KeyUp)) y = 100;
		if (kb_IsDown(kb_KeyDown)) y = 120;
		gfx_BlitBuffer();
	}
}

void Behaviors(int16_t position, int16_t timerPos)
{

		WorldDataTimer[timerPos] -= (WorldDataTimer[timerPos] != 0);

		// grass turns to dirt


		// remove water entities if water source was deleted
		// vertical
		if (WorldData[position] == WATERENTITY && WorldDataTimer[timerPos] < 1) {
			if (WorldData[position - MaxX] != 215 && WorldData[position - MaxX] < WATERENTITY)
			{
				WorldData[position] = 0;
				WorldDataTimer[timerPos + 80] = 3;
			}
		}
		// water flows downward
		if (WorldDataTimer[timerPos] < 1 && (WorldData[position] == 215 || (WorldData[position] >= WATERENTITY && WorldData[position] <= WATERENTITY + 7)) && (WorldData[position + MaxX] == 0 || WorldData[position + MaxX] > WATERENTITY || WorldData[position + MaxX] >= MaxSprites[0] + MaxSprites[3] + 1))
		{
			if (WorldData[position + MaxX] >= MaxSprites[0] + MaxSprites[3] + 1 && WorldData[position + MaxX] < WATERENTITY) {
				// give the player the flower the water broke.
				giveItem(WorldData[position + MaxX], 0);
			}
			WorldData[position + MaxX] = WATERENTITY;
			WorldDataTimer[timerPos + 80] = 3;
		}
		// remove flowing water entities when the source was deleted
		if (WorldData[position] > WATERENTITY && WorldData[position] < WATERENTITY + 7) {
			if (WorldData[position - 1] < WorldData[position] && WorldData[position + 1] < WorldData[position] && WorldData[position - MaxX] != 233 && WorldData[position - MaxX] != WATERENTITY) {
				WorldData[position]++;
				//WorldDataTimer[timerPos - 1] = 3;
				//WorldDataTimer[timerPos + 1] = 3;
			}
		}
		// add water entities if the source is placed on top of a block
		if (WorldData[position + MaxX] < WATERENTITY && WorldData[position + MaxX] != 0) {
			if (WorldData[position] == 215 && WorldDataTimer[timerPos] < 1 && WorldData[position - 1] == 0) {
				WorldData[position - 1] = WATERENTITY;
				WorldDataTimer[timerPos - 1] = 3;
			}
			if (WorldData[position] == 215 && WorldDataTimer[timerPos] < 1 && WorldData[position + 1] == 0) {
				WorldData[position + 1] = WATERENTITY;
				WorldDataTimer[timerPos + 1] = 3;
			}
		}
		// water flows sideways
		if (WorldDataTimer[timerPos] < 1 && (WorldData[position] == 215 || (WorldData[position] >= WATERENTITY && WorldData[position] <= WATERENTITY + 7)) && WorldData[position + MaxX] < WATERENTITY)
		{
			if (WorldData[position] >= WATERENTITY && WorldData[position - 1] >= WorldData[position] + 2)
				WorldData[position - 1]--;
			if (WorldData[position - 1] == 0 && WorldData[position] != 0)
			{
				WorldData[position - 1] = WorldData[position] + 1;
				WorldDataTimer[timerPos - 1] = 3;
			}
			if (WorldData[position] >= WATERENTITY && WorldData[position + 1] >= WorldData[position] + 2)
				WorldData[position + 1]--;
			if (WorldData[position + 1] == 0 && WorldData[position] != 0)
			{
				WorldData[position + 1] = WorldData[position] + 1;
				WorldDataTimer[timerPos + 1] = 3;
			}
		}


}

void CacheBlocks(int16_t xa, int16_t ya)
{
	int16_t blockValLeft, blockValRight, blockValTop, blockValBottom;
	int countVal = 0, shadowVal;
	for (render = 0; render < 20 * 15; render++) {
		pos = ya * MaxX + xa;
		blockValLeft = WorldData[pos - 1];
		blockValRight = WorldData[pos + 1];
		blockValTop = WorldData[pos - MaxX];
		blockValBottom = WorldData[pos + MaxX];
		shadowVal = 0;
		if (blockValLeft == 0 || blockValRight == 0 || blockValTop == 0 || blockValBottom == 0) shadowVal = 1;
		if (blockValLeft > MaxSprites[0] || blockValRight > MaxSprites[0] || blockValTop > MaxSprites[0] || blockValBottom > MaxSprites[0]) shadowVal = 1;
		// add more tests for bed, doors, glass and panes, etc.
		if (shadowing == 0) shadowVal = 1;
		if (WorldData[pos] != 0 && shadowVal == 1)
		{
			// check from beginning to the first empty position
			countVal = 0;
			for (listPos = 0; listPos < count; listPos++) {
				if (WorldData[pos] != WorldDataSprites[listPos]) countVal++;
			}
			if (countVal == count) {
				// the array doesn't have the ID saved, so save it now
				WorldDataSprites[countVal] = WorldData[pos];
				// check if it's a block
				if (WorldData[pos] - 1 <= MaxSprites[0])
					LoadBlock("CLASSICB", WorldData[pos] - 1, 16 * 16, count);
				// check if it's foliage
				if (WorldData[pos] - 1 >= MaxSprites[0] && WorldData[pos] - 1 <= MaxSprites[0] + MaxSprites[3])
					LoadBlock("CLASSICF", WorldData[pos] - 1 - MaxSprites[0], 16 * 16, count);
				// check if it's plants
				if (WorldData[pos] - 1 >= MaxSprites[0] + MaxSprites[3] && WorldData[pos] - 1 <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
					LoadBlock("CLASSICP", WorldData[pos] - 1 - (MaxSprites[0] + MaxSprites[3]), 16 * 16, count);
				// load water sprite if neccessary
				if (WorldData[pos] - 1 >= WATERENTITY && WorldData[pos] <= WATERENTITY + 7)
					LoadBlock("CLASSICB", 232, 16 * 16, count);
				count++;
			}
		}
		xa++;
		if (xa == playerX + 20) {
			xa = playerX;
			ya++;
		}
	}
}

void survivalInventory(void) {
	int16_t xpos = 80, ypos = 140, num = 0, pos = 0;
	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();
		gfx_SetColor(148);
		gfx_FillRectangle(40, 60, 240, 162);
		// main inventory slots
		pos = 0;
		for (y = 140; y < 140 + (3 * 18); y += 18) {
			for (x = 80; x < 80 + (9 * 18); x += 18) {
				gfx_SetColor(0);
				gfx_Rectangle(x, y, 18, 18);
				if (Inventory[pos] > 0) {
				if (Inventory[pos] <= MaxSprites[0])
					LoadBlock("CLASSICB", Inventory[pos] - 1, 16 * 16, 0);
				if (Inventory[pos] > MaxSprites[0] && Inventory[pos] <= MaxSprites[1])
					LoadBlock("CLASSICI", Inventory[pos] - 1, 16 * 16, 0);
				if (Inventory[pos] > MaxSprites[1] && Inventory[pos] <= MaxSprites[2])
					LoadBlock("CLASSICP", Inventory[pos] - 1, 16 * 16, 0);
				if (Inventory[pos] > MaxSprites[2] && Inventory[pos] <= MaxSprites[3])
					LoadBlock("CLASSICF", Inventory[pos] - 1, 16 * 16, 0);
				gfx_TransparentSprite(sprites[0], x + 1, y + 1);
				gfx_SetTextFGColor(254);
				}else{
					gfx_SetColor(74);
					gfx_FillRectangle(x + 1, y + 1, 16, 16);
				}
				pos++;
			}
		}
		pos = 0;
		for (y = 140; y < 140 + (3 * 18); y += 18) {
			for (x = 80; x < 80 + (9 * 18); x += 18) {
				if (Inventory[pos] > 0) {
					gfx_SetTextXY(x + 10, y + 12);
					gfx_PrintInt(Inventory[pos + 27], 1);
				}
				pos++;
			}
		}
		// hotbar
		for (x = 0; x < 5; x++) {
			if (x == hotbarSel) {
				gfx_SetColor(5);
			}else{
				gfx_SetColor(0);
			}
			gfx_Rectangle(117 + (x * 18), 200, 18, 18);

			if (hotbar[x] != 0) {
				if (hotbar[x] <= MaxSprites[0])
					LoadBlock("CLASSICB", hotbar[x] - 1, 16 * 16, 0);
				// check if it's foliage
				if (hotbar[x] > MaxSprites[0] && hotbar[x] <= MaxSprites[0] + MaxSprites[3])
					LoadBlock("CLASSICF", hotbar[x] - 1 - MaxSprites[0], 16 * 16, 0);
				// check if it's plants
				if (hotbar[x] > MaxSprites[0] + MaxSprites[3] && hotbar[x] <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
					LoadBlock("CLASSICP", hotbar[x] - 1 - (MaxSprites[0] + MaxSprites[3]), 16 * 16, 0);
				gfx_TransparentSprite(sprites[0], 118 + (x * 18), 201);
			}else{
				gfx_SetColor(181);
				gfx_FillRectangle(118 + (x * 18), 201, 16, 16);
			}
		}
		gfx_BlitBuffer();

	}
	delay(200);
	kb_Scan();
	Game();
}

int craftingCheck(int16_t inputVal) {
	int16_t item = 0;

	return item;
}

void giveItem(int16_t blockID, int16_t breaking) {
	int16_t pos, movetoinventory = 0;

	if (breaking == 1) {
		// add code for breaking blocks based on their ID.
	}

	// first scan hotbar for the blockID
	for (pos = 0; pos < 5; pos++) {
		if (hotbar[pos] == 0) {
			hotbar[pos] = blockID;
			hotbar[pos + 5]++;
			break;
		}else{
			if (hotbar[pos] == blockID && hotbar[pos + 5] < 64) {
				hotbar[pos + 5]++;
				break;
			}
			if (hotbar[pos] == blockID && hotbar[pos + 5] == 64) {
				movetoinventory = 1;
				break;
			}
		}
	}

	if (movetoinventory == 1) {
		// the hotbar is full, so find an empty space to put the item
		for (pos = 0; pos < 26; pos++) {
			if (Inventory[pos] == 0) {
				Inventory[pos] = blockID;
				Inventory[pos + 27]++;
				movetoinventory = 2;
				break;
			}else{
				if (Inventory[pos] == blockID && Inventory[pos + 27] < 64) {
					Inventory[pos + 27]++;
					movetoinventory = 2;
					break;
				}
			}
		}
	}

	// Check if movetoinventory is still 1.
	// If so, there's not enough room anywhere in player storage for the item

}

void creativeInventory(void)
{
	int16_t xpos = 28, ypos = 25, num, spritenum, tab = 0, pos = 0, hotbarpos = 0, lastPos;
	int16_t scroll = 0, oldBlock = 0, newBlock = 0, InvCurPos = 0, lastXpos = 0, selectedFromTab = 0;
	int16_t newBlockCount = 0, oldBlockCount = 0;
	char *tabNames[5] = { "Building Blocks", "Items", "Plants", "Foliage", "Player" };
	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();
		num = 0;
		gfx_SetColor(148);
		gfx_FillRectangle(15, 5, 290, 230);
		// scroll bar rectangle
		gfx_SetColor(74);
		gfx_FillRectangle(28 + (14 * 18), 24, 7, 182);
		// scroll bar level indicator
		gfx_SetColor(181);
		gfx_FillRectangle(29 + (14 * 18), 25 + (scroll * (180 / 9)), 5, 180 / 9);
		// tab names
		if (tab == 0) LoadBlock("CLASSICB", 101, 16 * 16, 0);
		if (tab == 1) LoadBlock("CLASSICI", 0, 16 * 16, 0);
		if (tab == 2) LoadBlock("CLASSICP", 0, 16 * 16, 0);
		if (tab == 3) LoadBlock("CLASSICF", 0, 16 * 16, 0);
		if (tab == 4) LoadBlock("CLASSICB", 112, 16 * 16, 0);
		gfx_TransparentSprite(sprites[0], 30, 8);
		gfx_SetColor(181);
		gfx_FillRectangle(48, 8, 14 * 18 - 20, 10);
		gfx_SetTextFGColor(0);
		gfx_PrintStringXY(tabNames[tab], 50, 9);
		for (y = 25; y < 25 + (10 * 18); y += 18) {
			for (x = 28; x < 28 + (14 * 18); x += 18) {
				gfx_SetColor(0);
				gfx_Rectangle(x, y, 18, 18);
				gfx_SetColor(74);
				spritenum = scroll * 14 + num;
				if (tab == 0 && spritenum <= MaxSprites[0])
					LoadBlock("CLASSICB", spritenum, 16 * 16, 0);
				if (tab == 1 && spritenum <= MaxSprites[1])
					LoadBlock("CLASSICI", spritenum, 16 * 16, 0);
				if (tab == 2 && spritenum <= MaxSprites[2])
					LoadBlock("CLASSICP", spritenum, 16 * 16, 0);
				if (tab == 3 && spritenum <= MaxSprites[3])
					LoadBlock("CLASSICF", spritenum, 16 * 16, 0);
				if (spritenum < MaxSprites[tab])
					gfx_TransparentSprite(sprites[0], x + 1, y + 1);
				if (spritenum >= MaxSprites[tab])
					gfx_FillRectangle(x + 1, y + 1, 16, 16);
				num++;
			}
		}
		// hotbar
		for (x = 0; x < 5; x++) {
			gfx_SetColor(0);
			if (x == hotbarSel)
			gfx_SetColor(5);
			gfx_Rectangle(117 + (x * 18), 210, 18, 18);
			if (hotbar[x] != 0) {
				if (hotbar[x] <= MaxSprites[0])
					LoadBlock("CLASSICB", hotbar[x] - 1, 16 * 16, 0);
				// check if it's foliage
				if (hotbar[x] > MaxSprites[0] && hotbar[x] <= MaxSprites[0] + MaxSprites[3])
					LoadBlock("CLASSICF", hotbar[x] - 1 - MaxSprites[0], 16 * 16, 0);
				// check if it's plants
				if (hotbar[x] > MaxSprites[0] + MaxSprites[3] && hotbar[x] <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
					LoadBlock("CLASSICP", hotbar[x] - 1 - (MaxSprites[0] + MaxSprites[3]), 16 * 16, 0);
				gfx_TransparentSprite(sprites[0], 118 + (x * 18), 211);
			}else{
				gfx_SetColor(181);
				gfx_FillRectangle(118 + (x * 18), 211, 16, 16);
			}
		}
		// draw a item if it was selected
		if (newBlock != 0)
		{
			if (selectedFromTab == 0 && newBlock <= MaxSprites[0])
				LoadBlock("CLASSICB", newBlock - 1, 16 * 16, 0);
			if (selectedFromTab == 1 && newBlock <= MaxSprites[1])
				LoadBlock("CLASSICI", newBlock - 1, 16 * 16, 0);
			if (selectedFromTab == 2 && newBlock <= MaxSprites[2])
				LoadBlock("CLASSICP", newBlock - 1, 16 * 16, 0);
			if (selectedFromTab == 3 && newBlock <= MaxSprites[3])
				LoadBlock("CLASSICF", newBlock - 1, 16 * 16, 0);
			gfx_TransparentSprite(sprites[0], xpos + 5, ypos + 5);
		}
		// cursor (shaped like a + )
		gfx_SetColor(255);
		gfx_FillRectangle(xpos + 2, ypos + 8, 14, 2);
		gfx_FillRectangle(xpos + 8, ypos + 2, 2, 14);


		gfx_SetTextFGColor(0);
		gfx_SetTextXY(12, 12);
		gfx_PrintInt(pos + 1, 1);


		gfx_SetTextFGColor(254);
		gfx_SetTextXY(xpos + 14, ypos + 14);
		if (newBlockCount > 1) gfx_PrintInt(newBlockCount, 1);
		gfx_BlitBuffer();

		// move cursor down to hotbar if the cursor is at the bottom, and back up
		if (ypos == 25 + (9 * 18) && kb_IsDown(kb_KeyDown) && scroll == 8)
		{
			InvCurPos = 1;
			lastXpos = xpos;
			lastPos = pos;
			hotbarpos = 0;
			xpos = 117;
			ypos = 210;
		}
		if (ypos == 210 && kb_IsDown(kb_KeyUp) && InvCurPos == 1)
		{
			InvCurPos = 0;
			xpos = lastXpos;
			pos = lastPos + 14;
			ypos = 25 + (10 * 18);
		}
		// clears the selected block if user clicks an empty space in the inventory
		if (kb_IsDown(kb_Key2nd) && (pos >= MaxSprites[tab] || (newBlock != pos + 1 && newBlock != 0)) && ypos != 210)
		{
			newBlock = 0;
			newBlockCount = 0;
		}else{
			// select an item if the user presses 2nd in the inventory (not hotbar)
			if (kb_IsDown(kb_Key2nd) && pos < MaxSprites[tab] && newBlockCount < 64 && ypos != 210)
			{
				selectedFromTab = tab;
				if (newBlock == pos + 1) {
					newBlockCount++;
				}else{
					newBlockCount = 1;
					newBlock = pos + 1;
				}
			}
		}
		// select/swap an item if the user presses 2nd in the hotbar only
		if (kb_IsDown(kb_Key2nd) && ypos == 210)
		{
			oldBlock = hotbar[hotbarpos];
			if (newBlock == 0)
			{
				// check if it's blocks
				if (hotbar[hotbarpos] <= MaxSprites[0])
					selectedFromTab = 0;
				// check if it's items
				if (hotbar[hotbarpos] > MaxSprites[0] && hotbar[hotbarpos] <= MaxSprites[0] + MaxSprites[1])
					selectedFromTab = 1;
				// check if it's foliage
				if (hotbar[hotbarpos] > MaxSprites[0] + MaxSprites[1] && hotbar[hotbarpos] <= MaxSprites[0] + MaxSprites[1] + MaxSprites[2])
					selectedFromTab = 2;
				// check if it's plants
				if (hotbar[hotbarpos] > MaxSprites[0] + MaxSprites[1] + MaxSprites[2] && hotbar[hotbarpos] <= MaxSprites[0] + MaxSprites[1] + MaxSprites[2] + MaxSprites[3])
					selectedFromTab = 3;
			}else{
				hotbar[hotbarpos] = newBlock;
			}
			newBlock = oldBlock;
		}
		// tabs
		if (kb_IsDown(kb_KeyYequ) || kb_IsDown(kb_KeyWindow) || kb_IsDown(kb_KeyZoom) || kb_IsDown(kb_KeyTrace) || kb_IsDown(kb_KeyGraph)) {
			if (kb_IsDown(kb_KeyYequ)) tab = 0;
			if (kb_IsDown(kb_KeyWindow)) tab = 1;
			if (kb_IsDown(kb_KeyZoom)) tab = 2;
			if (kb_IsDown(kb_KeyTrace)) tab = 3;
			if (kb_IsDown(kb_KeyGraph)) tab = 4;
			scroll = 0;
			pos = 0;
			xpos = 28;
			ypos = 25;
		}
		// scroll bar movement
		if (ypos == 25 + (9 * 18) && kb_IsDown(kb_KeyDown) && scroll < 8)
		{
			scroll++;
			pos += 14;
		}
		if (ypos == 25 && kb_IsDown(kb_KeyUp) && scroll > 0)
		{
			scroll--;
			pos -= 14;
		}
		// move cursor left and right if cursor is at the hotbar
		xpos += 18 * (kb_IsDown(kb_KeyRight) && xpos < 117 + (4 * 18)) * (ypos == 210) - 18 * (kb_IsDown(kb_KeyLeft) && xpos > 117) * (ypos == 210);
		hotbarpos += (kb_IsDown(kb_KeyRight) && hotbarpos < 5) * (ypos == 210) - (kb_IsDown(kb_KeyLeft) && hotbarpos > 0) * (ypos == 210);
		// cursor movement
		pos += (kb_IsDown(kb_KeyRight) && xpos < 28 + (13 * 18)) * (ypos != 210) - (kb_IsDown(kb_KeyLeft) && xpos > 28) * (ypos != 210);
		pos += 14 * (kb_IsDown(kb_KeyDown) && ypos < 25 + (9 * 18)) * (ypos != 210) - 14 * (kb_IsDown(kb_KeyUp) && ypos > 25) * (ypos != 210);
		xpos += 18 * (kb_IsDown(kb_KeyRight) && xpos < 28 + (13 * 18)) * (ypos != 210) - 18 * (kb_IsDown(kb_KeyLeft) && xpos > 28) * (ypos != 210);
		ypos += 18 * (kb_IsDown(kb_KeyDown) && ypos < 25 + (9 * 18)) * (ypos != 210) - 18 * (kb_IsDown(kb_KeyUp) && ypos > 25) * (ypos != 210);
	}
	delay(200);
	kb_Scan();
	Game();
}

void MainMenu()
{
	int8_t scroll = 16;
	curY = 125;
	// fix closing the game unnecessarily.
	kb_Scan();
	while (!(kb_IsDown(kb_KeyClear)))
	{
		// draw the main menu
		kb_Scan();
		LoadBlock("CLASSICB", 77, 16 * 16, 0);
		for (x = 0; x < 320; x += 16) {
			for (y = scroll - 16; y < 240 + scroll; y += 16) {
				gfx_TransparentSprite(sprites[0], x, y);
			}
		}
		gfx_SetTextFGColor(230);
		gfx_PrintStringXY("v1.0.1", 3, 230);
		gfx_SetTextFGColor(0);
		appvar = ti_Open("MC2DDAT", "r");
		logo = ti_GetDataPtr(appvar);
		ti_Close(appvar);
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
		gfx_PrintStringXY("Play", 144, 130);
		gfx_PrintStringXY("Achievements", 116, 155);
		gfx_PrintStringXY("Settings", 130, 180);
		gfx_PrintStringXY("Quit", 144, 205);
		gfx_SetColor(254);
		gfx_Rectangle(60, curY, 192, 20);
		gfx_Rectangle(61, curY + 1, 190, 18);
		gfx_BlitBuffer();

		scroll--;
		if (scroll < 1)
			scroll = 16;

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
			{
				playMenu();
				y = 125;
				delay(100);
				MainMenu();
			}
			if (curY == 150)
			{ //"achievements"
				Achievements();
				curY = 150;
				delay(100);
				MainMenu();
			}
			if (curY == 175)
			{ //"Settings"
				Settings();
				curY = 175;
				delay(100);
				MainMenu();
			}
		}
	}
}

void Achievements(void)
{
	int16_t x, y;
	LoadBlock("CLASSICB", 122, 16 * 16, 0);
	for (x = 0; x < 320; x += 16)
	{
		for (y = 0; y < 240; y += 16)
		{
			gfx_TransparentSprite(sprites[0], x, y);
		}
	}
	gfx_SetColor(181);
	gfx_FillCircle(10, 10, 5);
	gfx_FillCircle(309, 10, 5);
	gfx_FillCircle(10, 229, 5);
	gfx_FillCircle(309, 229, 5);
	gfx_FillRectangle(10, 5, 300, 230);
	gfx_FillRectangle(5, 10, 310, 220);
	gfx_SetTextFGColor(0);
	gfx_PrintStringXY("Achievements:", 20, 15);
	gfx_SetTextFGColor(255);
	for (x = 2; x < 18; x++)
	{
		for (y = 2; y < 13; y++)
		{
			gfx_TransparentSprite(sprites[0], x * 16, y * 16);
		}
	}
	gfx_BlitBuffer();

	while (!(os_GetCSC()))
		;
	delay(100);
}

void Settings(void)
{
	curX = 10;
	curY = 20;
	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();
		LoadBlock("CLASSICB", 77, 16 * 16, 0);
		for (x = 0; x < 320; x += 16)
		{
			for (y = 0; y < 240; y += 16)
			{
				gfx_TransparentSprite(sprites[0], x, y);
			}
		}
		for (x = 20; x < 180; x += 20)
		{
			gfx_SetColor(181);
			gfx_FillRectangle(10, x, 140, 16);
			gfx_FillRectangle(170, x, 140, 16);
		}
		gfx_SetColor(254);
		gfx_Rectangle(curX, curY, 140, 16);
		gfx_Rectangle(curX + 1, curY + 1, 138, 14);
		gfx_BlitBuffer();

		if (kb_IsDown(kb_KeyUp) && (curY > 20))
			curY -= 20;
		if (kb_IsDown(kb_KeyDown) && (curY < 160))
			curY += 20;
		if (kb_IsDown(kb_KeyLeft))
			curX = 10;
		if (kb_IsDown(kb_KeyRight))
			curX = 170;
	}
	delay(100);
}

void input(char *string, int size)
{
	const char *charsUpper = "\0\0\0\0\0\0\0\0\0\0\"WRMH\0\0?[VQLG\0\0:ZUPKFC\0 YTOJEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
	const char *charsLower = "\0\0\0\0\0\0\0\0\0\0\"wrmh\0\0?[vqlg\0\0:zupkfc\0 ytojeb\0\0xsnida\0\0\0\0\0\0\0\0";
	const char *charsNum = "\0\0\0\0\0\0\0\0\0\0\+-*/\0\0\0?369(\0\0\0\0258)\0\0\00147,\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	uint8_t key, length = strlen(string), scroll = 0, i = 0, chartype = 0;
	// set i to the first occurence of a null character
	for (i = 0; i < length; i++) {
		if (i > 31) scroll++;
		if (string[i] == 0) break;
	}
	gfx_SetColor(181);
	gfx_FillRectangle(0, 216, 320, 24);
	gfx_SetColor(74);
	gfx_Rectangle(0, 216, 320, 24);
	gfx_Rectangle(1, 217, 318, 22);
	gfx_SetColor(181);
	while((key = os_GetCSC()) != sk_Enter) {
		gfx_SetColor(181);
		gfx_FillRectangle(1, 217, 318, 22);
		gfx_SetColor(0);
		gfx_FillRectangle(4, 223, 10, 10);
		gfx_SetTextFGColor(254);
		if (chartype == 0) gfx_PrintStringXY("A", 5, 224);
		if (chartype == 1) gfx_PrintStringXY("a", 5, 224);
		if (chartype == 2) gfx_PrintStringXY("1", 5, 224);
		gfx_SetTextFGColor(0);
		gfx_SetTextXY(18, 224);
		pos = scroll;
		for (pos = scroll; pos < scroll + 32; pos++)
		{
			gfx_SetTextFGColor(0);
			if (pos < i)
			gfx_PrintChar(string[pos]);
		}
		gfx_BlitBuffer();

		if (kb_IsDown(kb_KeyAlpha)) {
			delay(200);
			chartype++;
			if (chartype > 2) chartype = 0;
		} 

		if(charsUpper[key] && i <= size) {
			if (chartype == 0) string[i] = charsUpper[key];
			if (chartype == 1) string[i] = charsLower[key];
			if (chartype == 2) string[i] = charsNum[key];
			string[i + 1] = 0;
			i++;
			if (i > 32) scroll++;
		}
		if (kb_IsDown(kb_KeyDel) && i > 0) {
			delay(80);
			string[i] = 0;
			if (i > 32 && scroll > 0) scroll--;
			i--;
		}
	}
}

void playMenu(void)
{
	int16_t CursorY, x, i, scroll, scrollY, redraw, tab;
	int16_t worldSize, cheats = 0, scrollYb;
	gamemode = 0;
	char *gamemodeStr[3] = {"Survival", "Creative", "Adventure"};
	char *worldSizeStr[3] = {"Small", "Medium", "Large"};
	char *cheatsStr[2] = {"Off", "On"};
	char *worldTypesStr[3] = {"Standard", "Superflat", "Large Biomes"};
	findAppvars("MCCESV");
	LoadBlock("CLASSICB", 77, 16 * 16, 0);
	for (x = 0; x < 320; x += 16)
	{
		for (y = 0; y < 240; y += 16)
		{
			gfx_TransparentSprite(sprites[0], x, y);
		}
	}
	tab = 0;
	scroll = 0;
	CursorY = 40;
	kb_Scan();
	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();
		LoadBlock("CLASSICB", 77, 16 * 16, 0);
		for (x = 0; x < 320; x += 16)
		{
			for (y = 0; y < 240; y += 16)
			{
				gfx_TransparentSprite(sprites[0], x, y);
			}
		}
		gfx_SetColor(181);
		gfx_FillRectangle(20, 20, 280, 200);
		gfx_SetColor(0);
		gfx_Rectangle(20, 20, 280, 200);
		gfx_Rectangle(20, 20, 280, 20);
		gfx_SetColor(148);
		gfx_FillRectangle(21 + (tab * 89), 21, 100, 18);
		gfx_PrintStringXY("My Worlds", 40, 25);
		gfx_PrintStringXY("Servers", 134, 25);
		gfx_PrintStringXY("Friends", 240, 25);

		if (tab == 0)
		{
			gfx_SetColor(148);
			gfx_FillRectangle(21, 40, 278, 25);
			DrawCenteredText("Create New World", 160, 47);
			gfx_SetColor(254);
			gfx_Rectangle(21, CursorY, 278, 25 - ((CursorY != 40) * 8));
			gfx_Rectangle(22, CursorY + 1, 276, 23 - ((CursorY != 40) * 8));
			if (foundCount == 0)
			{
				gfx_PrintStringXY("No Worlds were found!", 90, 120);
			}
			else
			{
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
			if (kb_IsDown(kb_Key2nd) && (CursorY == 40))
			{
				CursorY = 80;
				worldSize = 0;
				worldType = 0;
				while (!(kb_IsDown(kb_KeyClear)))
				{
					LoadBlock("CLASSICB", 77, 16 * 16, 0);
					for (x = 0; x < 320; x += 16)
					{
						for (y = 0; y < 240; y += 16)
						{
							gfx_TransparentSprite(sprites[0], x, y);
						}
					}
					for (y = 80; y < 220; y += 20)
					{
						gfx_SetColor(181);
						gfx_FillRectangle(50, y, 220, 16);
						gfx_SetColor(0);
						gfx_Rectangle(50, y, 220, 16);
						gfx_Rectangle(51, y + 1, 218, 14);
					}
					gfx_SetColor(148);
					gfx_FillRectangle(0, 0, 320, 12);
					DrawCenteredText("Create New World", 160, 2);
					DrawCenteredText("Name:", 160 - gfx_GetStringWidth(worldNameStr) / 2, 84);
					gfx_PrintStringXY(worldNameStr, 162 - gfx_GetStringWidth(worldNameStr) / 2 + gfx_GetStringWidth("Name:") / 2, 84);
					DrawCenteredText("Seed:", 160 - gfx_GetStringWidth(seedStr) / 2, 104);
					gfx_PrintStringXY(seedStr, 162 - gfx_GetStringWidth(seedStr) / 2 + gfx_GetStringWidth("Seed:") / 2, 104);
					DrawCenteredText("Gamemode:", 160 - gfx_GetStringWidth(gamemodeStr[gamemode]) / 2, 124);
					gfx_PrintStringXY(gamemodeStr[gamemode], 162 - gfx_GetStringWidth(gamemodeStr[gamemode]) / 2 + gfx_GetStringWidth("Gamemode:") / 2, 124);
					DrawCenteredText("World Size:", 160 - gfx_GetStringWidth(worldSizeStr[worldSize]) / 2, 144);
					gfx_PrintStringXY(worldSizeStr[worldSize], 162 - gfx_GetStringWidth(worldSizeStr[worldSize]) / 2 + gfx_GetStringWidth("World Size:") / 2, 144);
					DrawCenteredText("Cheats:", 160 - gfx_GetStringWidth(cheatsStr[cheats]) / 2, 164);
					gfx_PrintStringXY(cheatsStr[cheats], 162 - gfx_GetStringWidth(cheatsStr[cheats]) / 2 + gfx_GetStringWidth("Cheats:") / 2, 164);
					DrawCenteredText("World Type:", 160 - gfx_GetStringWidth(worldTypesStr[worldType]) / 2, 184);
					gfx_PrintStringXY(worldTypesStr[worldType], 162 - gfx_GetStringWidth(worldTypesStr[worldType]) / 2 + gfx_GetStringWidth("World Type:") / 2, 184);
					DrawCenteredText("Generate", 160, 204);
					gfx_SetColor(254);
					gfx_Rectangle(50, CursorY, 220, 16);
					gfx_Rectangle(51, CursorY + 1, 218, 14);
					gfx_BlitBuffer();
					kb_Scan();
					if (kb_IsDown(kb_KeyUp) && (CursorY > 80))
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
						if (CursorY == 80)
						{
							input(worldNameStr, 40);
							delay(100);
						}

						if (CursorY == 100)
						{
							input(seedStr, 40);
							delay(100);
						}

						if (CursorY == 120)
						{
							gamemode++;
							if (gamemode > 2)
								gamemode = 0;
						}
						if (CursorY == 140)
						{
							worldSize++;
							if (worldSize > 2)
								worldSize = 0;
						}
						if (CursorY == 160)
						{
							cheats = (cheats == 0);
						}
						if (CursorY == 180)
						{
							worldType++;
							if (worldType > 2)
							{
								worldType = 0;
							}
						}
						if (CursorY == 200)
						{

							if (worldSize == 0)
							{
								MaxX = 60;
								MaxY = 60;
							}
							if (worldSize == 1)
							{
								MaxX = 100;
								MaxY = 100;
							}
							if (worldSize == 2)
							{
								MaxX = 160;
								MaxY = 150;
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
			}
		}
		if (tab == 1)
		{
			gfx_SetColor(148);
			gfx_FillRectangle(21, 40, 278, 25);
			gfx_PrintStringXY("Add Server", 116, 47);
			gfx_SetColor(254);
			gfx_Rectangle(21, CursorY, 278, 25 - ((CursorY != 40) * 8));
			gfx_Rectangle(22, CursorY + 1, 276, 23 - ((CursorY != 40) * 8));
		}
		if (tab == 2)
		{
			gfx_PrintStringXY("Friends are not Available", 80, 47);
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

// Compress data via zx7 compression
void compressAndWrite(void *data, int len, ti_var_t fp)
{
	//	int i, j, k, num, num_i, new;
	//	uint8_t b, m;
	//	int *huffman_table = (void*)0xD52C00;
	//	uint8_t *huffman_table_2 = &huffman_table[256];
	//	uint8_t *huffman_output = &huffman_table_2[256];
	//	void *back_buffer = (void*)0xD52C00;
	//	uint8_t *ptr;
	int new_len;
	*((int *)0xE30010) = 0xD40000; // force the lcd to use the first half of VRAM so we can use the second half.
	gfx_SetDrawScreen();
	LoadBlock("CLASSICB", 77, 16 * 16, 0);
	for (x = 0; x < 320; x += 16)
	{
		for (y = 0; y < 240; y += 16)
		{
			gfx_TransparentSprite(sprites[0], x, y);
		}
	}
	gfx_SetTextFGColor(0xDF);
	gfx_PrintStringXY("Saving World...", 113, 110);

	/* Maybe do huffman coding at some point
	//start by counting the occurences of each unique byte in the data
	ptr = data;
	for (i=0; i<len; i++){
		huffman_table[*ptr++]++;
	}
	//then sort by highest number of occurences
	k = 1;
	for (i=0; i<256; i++){
		num = num_i = 0;
		for (j=0; j<256; j++) {
			if ((new=huffman_table[j]) > num) {
				num = new;
			}
		}
		if (!num) break;
		huffman_table_2[num_i] = k++;
		huffman_table[j] = 0;
	}
	ptr = data;
	m = 1;
	for (i=0; i<len; i++){
		b = huffman_table_2[*ptr++];
		k = 256;
		while (k>>=1){
			if (b & k){
				while (b) {

				}
			}
		}
	}
	*/

	zx7_Compress((void *)0xD52C00, data, &new_len, len);
	ti_Write((void *)0xD52C00, new_len, 1, fp);
	main();
}

void findAppvars(const char *str)
{
#define WORLD_IDENTIFIER str
// we have a limit of 10 appvars
#define WORLD_COUNT_LIMIT 10
	char *foundAppvar;
	void *searchPosition; // used by ti_Detect()
	// code for world scanning given by KryptonicDragon via cemetech.net
	// if you define this somewhere global instead of just in this function, you can also
	// use "WORLD_IDENTIFIER" in the code that saves worlds.
	//
	// also, this needs to be unique to your program so other programs don't mistake
	// your worlds for their appvars (or vice versa), but it also shouldn't be too long
	// since that slows down the search and takes up more space on everyone's calc (remember
	// that *every single* world appvar has this at the beginning of its data)

	// documentation states we must set this to NULL to start the search
	searchPosition = NULL;
	foundCount = 0;

	// note how the next line uses = and not ==, because we want to assign what ti_Detect()
	// returns into the foundAppvar variable. = is for assigning, == is for checking equality
	//
	// this loop will keep running so long as ti_Detect() find something. When it's done,
	// ti_Detect() will return NULL, which will get put into foundAppvar, and then
	// the condition "NULL != (NULL)" will become false and end the loop
	//
	// this loop also stops if we find too many programs (which is set by the "WORLD_COUNT_LIMIT"
	// define.
	/*
		while (foundCount < WORLD_COUNT_LIMIT && NULL != (foundAppvar = ti_Detect(&searchPosition, WORLD_IDENTIFIER))) {
			// now the foundAppvar variable has a C string of the name of an appvar
			// that was found, and we know for sure it's a MC2D world.
				memcpy(WorldsList[foundCount], foundAppvar, 9);
				foundCount++;
		}*/
	for (;;)
	{
		foundAppvar = ti_Detect(&searchPosition, WORLD_IDENTIFIER);
		if (foundAppvar == NULL)
		{
			break;
		}

		memcpy(WorldsList[foundCount++], foundAppvar, 9);
		if (foundCount >= WORLD_COUNT_LIMIT)
		{
			break;
		}
	}
}