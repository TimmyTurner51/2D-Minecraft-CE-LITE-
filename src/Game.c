
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
#include "RenderEngine.h"
#include "survivalInventory.h"
#include "creativeInventory.h"
#include "deathScreen.h"
#include "giveItem.h"
#include "pauseMenu.h"
#include "input.h"
#include "Game.h"

void Game(void)
{
	int16_t gravityVar = gravityPixelAmount, listPosition = 0;
	int16_t blockAboveHead = 0, blockAtCenter = 0;
	int16_t blockLeftCenter = 0, blockLeftBottom = 0;
	int16_t blockRightCenter = 0, blockRightBottom = 0;
	int16_t blockAtFeet = 0, blockBelowFeet = 0;
	int16_t Up_Count;
	gfx_SetClipRegion(0 - 17, 0 - 17, 337, 257);

	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();

		RenderEngine();

		if (gameSettings[3] == 1) {
			// debug
			gfx_SetTextFGColor(0);
			gfx_SetTextXY(25, 20);
			gfx_PrintString("X:");
			gfx_PrintInt(playerX, 1);
			gfx_SetTextXY(98, 20);
			gfx_PrintString("Y:");
			gfx_PrintInt(playerY, 1);
			gfx_SetTextXY(25, 30);
			gfx_PrintString("FPS:");
			gfx_PrintInt(fps, 1);
			gfx_SetTextXY(85, 30);
			gfx_PrintString("Block_AtCursor:");
			gfx_PrintInt(WorldData[curPos], 1);
		}

		// cursor
		if (timeofday <= 2) gfx_SetColor(0);
		if (timeofday > 2 || WorldData[curPos] != 0) gfx_SetColor(148);
		gfx_Rectangle(scrollX + (curX * 16), scrollY + (curY * 16), 16, 16);

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

		if (health <= 0) deathScreen();

		// dialog/pop up text
		if (dialog != 0)
		{
			gfx_SetColor(181);
			gfx_FillRectangle(2, 2, 316, 14);
			gfx_SetTextFGColor(0);
			gfx_PrintStringXY(dialogString, 6, 6);
		}


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
		}else{
			// revamped hotbar
			// (update: Disappears when entering the inventory, becuase this was visible even when overlayed)
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
						gfx_TransparentSprite(BlockTextures[hotbar[x] - 1], 119 + (x * 18), 221);
					// check if it's foliage
					if (hotbar[x] > MaxSprites[0] && hotbar[x] <= MaxSprites[0] + MaxSprites[3])
						gfx_TransparentSprite(FoliageTextures[hotbar[x] - 1 - MaxSprites[0]], 119 + (x * 18), 221);
					// check if it's plants
					if (hotbar[x] > MaxSprites[0] + MaxSprites[3] && hotbar[x] <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
						gfx_TransparentSprite(PlantTextures[hotbar[x] - 1 - (MaxSprites[0] + MaxSprites[3])], 119 + (x * 18), 221);
				}
			}
			if (gamemode == 0 || gamemode == 2) {
				for (x = 0; x < 5; x++) {
					gfx_SetTextXY(117 + (x * 18) + 10, 231);
					if (hotbar[x] != 0) gfx_PrintInt(hotbar[x + 5], 1);
				}
			}
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
		// allows movement in a 1-block wide area
		if (kb_IsDown(kb_KeyLeft) && playerX > 0 && (blockLeftCenter != 0) && scrollX < 0) scrollX += pixelAmount;
		if (kb_IsDown(kb_KeyRight) && playerX < MaxX && (blockRightCenter != 0) && scrollX > -16 + 6) scrollX -= pixelAmount;
		// standard left/right movement
		if (kb_IsDown(kb_KeyLeft) && playerX > 0 && (blockLeftCenter == 0 || blockLeftCenter > MaxSprites[0])) scrollX += pixelAmount;
		if (kb_IsDown(kb_KeyRight) && playerX < MaxX && (blockRightCenter == 0 || blockRightCenter > MaxSprites[0])) scrollX -= pixelAmount;
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
				if ((blockBelowFeet == 0 || blockAtFeet >= WATERENTITY) && jump != 1) {
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
			//scrollX = -16 + (pixelAmount - -scrollX);
			scrollX -= 16;
			WorldTimerPosX--;
		}
		if (scrollX < -16) {
			curPos++;
			//scrollX = (scrollX + pixelAmount) + 16;
			scrollX += 16;
			WorldTimerPosX++;
		}
		if (scrollY > 0) {
			curPos -= MaxX;
			//scrollY = -16 + (pixelAmount - -scrollY);
			scrollY -= 15;
			WorldTimerPosY--;
		}
		if (scrollY < -16) {
			curPos += MaxX;
			//scrollY = (scrollY + pixelAmount) + 16;
			scrollY += 16;
			WorldTimerPosY++;
		}


		if (gamemode != 0 && gamemode != 2) {
			// double up to toggle fly mode
			if (kb_IsDown(kb_KeyUp)) {
				if (Up_Count == 1 && rtc + 1 == rtc_Seconds) {
					rtc = rtc_Seconds;
					timer = 0;
					dialogTimer = 50;
					dialog = 1;
					flymode = (flymode == 0);
					strcpy(dialogString, "Fly Mode toggled Off");
					if (flymode == 1)
						strcpy(dialogString, "Fly Mode toggled On");
				}
				Up_Count = (Up_Count == 0);
				if (Up_Count == 0) rtc = rtc_Seconds;
			}
			// turn off fly mode if you touch the ground
			if (kb_IsDown(kb_KeyDown) && flymode == 1 && playerY < MaxY - 15 && WorldData[(playerX + ((playerY + 3) * MaxX))] != 0 && WorldData[(playerX + ((playerY + 3) * MaxX))] != 233 && WorldData[(playerX + ((playerY + 3) * MaxX))] != WATERENTITY)
				flymode = 0;
		}

		gfx_BlitBuffer();
	}
	kb_Scan();
	pauseMenu();
}
