////////////////////////////////////////////////////////////////////////////////
//  2D Minecraft CE *lite* (C) v1.0.0a
//  Authors: TimmyTurner51, LogicalJoe, and Beckadamtheinventor
//  License: GNU GPL v3
//  Description: A 2D Minecraft clone made for the TI-84 Plus CE, written in C.
////////////////////////////////////////////////////////////////////////////////

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

int Generator(void);
int WorldEngine(void);
int LoadChunks(int24_t position);
int Behaviors(int24_t position);
int LoadNew(int24_t position);
int creativeInventory(void);


// Amount of chunks to render (chunks are 16x16) max is 5 by 5 for now
int24_t renderX = 1, renderY = 1;

// Amount of Pixels to scroll. Ranges from 1 - 16. 1 being smooth but slowest, 16 being jumpy but fastest.
int24_t pixelAmount = 4;

int main(void) {

    ti_var_t appvar;
    int24_t y, logo;
	gfx_Begin(); //This sets the default palette, no need to set the palette again
	gfx_SetTransparentColor(252);
	ti_CloseAll();
	gfx_SetClipRegion(-17, -17, 337, 257);
	LoadBlocks("MCEDEFT");
	appvar = ti_Open("MC2DDAT", "r");
	logo = ti_GetDataPtr(appvar);
	ti_Close(appvar);
	y = 125;
	gfx_SetDrawBuffer();
	gfx_SetTextFGColor(0);


	Generator();
	WorldEngine();


	gfx_End();

}

int Generator(void)
{
	int24_t x = 0, y = 0, blockType = 0;
	int24_t groundLevel = 18, groundLevelB, pos = 0, posb = 0, posc = 0, posd = 0;

	// 0 = creative, 1 = survival, 2 = adventure
	gamemode = 0;
	// 0 = off, 1 = on
	flymode = 1;

	for (x = 0; x < 200; x++)
	{
		if ((groundLevel > 1) && (groundLevel < 200) && (randInt(0, 12) != 1))
			groundLevel += randInt(-1, 1);

		// water generation
		if ((randInt(0, 6) == 1) && (groundLevel == 30))
		{
			pos = x - randInt(6, 20);
			posd = randInt(2,4);
			groundLevelB = groundLevel + 1;
			for (posb = pos; posb < x; posb++)
			{
				posd += randInt(-1,1);
				for (posc = 0; posc < groundLevelB + posd; posc++)
				{
					WorldData[posb + (posc * 200)] = 0;
					if (posc > groundLevelB) 
						WorldData[posb + (posc * 200)] = WATERENTITYB + 1;
				}
				WorldData[posb + (groundLevelB * 200)] = WATER + 1;
			}
			groundLevel = groundLevelB;
		}

		// tree generation
		if ((randInt(0, 3) == 0) && (WorldData[x + (groundLevel * 200)] != WATER + 1))
		{
			pos = groundLevel - 5;
			WorldData[x + (pos * 200)] = OAKLEAVES + 1;
			WorldData[x - 1 + ((pos + 1) * 200)] = OAKLEAVES + 1;
			WorldData[x + 1 + ((pos + 1) * 200)] = OAKLEAVES + 1;
			WorldData[x + ((pos + 1) * 200)] = OAKLOGS + 1;
			WorldData[x + ((pos + 2) * 200)] = OAKLOGS + 1;
			WorldData[x + ((pos + 3) * 200)] = OAKLOGS + 1;
			WorldData[x + ((pos + 4) * 200)] = OAKLOGS + 1;
		}

		for (y = 0; y < 200; y++)
		{

			if (y == groundLevel)
				blockType = GRASS + 1;
			if (y > groundLevel)
				blockType = DIRT + 1;
			if (y > groundLevel + 4)
				blockType = STONE + 1;
			if ((y > groundLevel + 10) && (y < groundLevel + 80) && (randInt(0, 10) == 0))
				blockType = COALORE + 1;
			if ((y > groundLevel + 60) && (y < groundLevel + 80) && (randInt(0, 10) == 0))
				blockType = IRONORE + 1;
			if ((y > groundLevel + 140) && (y < groundLevel + 190) && (randInt(0, 20) == 0))
				blockType = GOLDORE + 1;
			if ((y > groundLevel + 140) && (y < groundLevel + 190) && (randInt(0, 20) == 0))
				blockType = LAPIZORE + 1;
			if (y == 200)
				blockType = BEDROCK + 1;

			if (y >= groundLevel) WorldData[x + (y * 200)] = blockType;
		}

	}

}

int WorldEngine(void)
{
	redraw = 1;
	gfx_SetClipRegion(0-17, 0-17, 337, 257);
	
	//LoadBlocks("MCEDEFT");

	while (!(kb_IsDown(kb_KeyClear)))
	{
		kb_Scan();

		if (redraw == 1)
		{

			redraw = 0;
			gfx_FillScreen(191);
			count = 1;
			pos = (playerX + (playerY * 200));
			drawX = scrollX;
			drawY = scrollY;
			gfx_SetColor(32);
			LoadChunks(pos);
			if (playerY % renderY)
				LoadBlocks("MCEDEFT");
			for (render = 0; render < 21 * 16; render++)
			{
				// draw the shadowing box (not for water, lava, etc.)
				if ((WorldData[pos] != 0) && (WorldData[pos] != BEDBACK + 1) && (WorldData[pos] != GLASS + 1) && (WorldData[pos] != BEDFRONT + 1) && (WorldData[pos] != WATER + 1) && (WorldData[pos] != LAVA + 1) && (WorldData[pos] != WATERENTITYB + 1) && (WorldData[pos] != LAVAENTITYB + 1))
					gfx_FillRectangle(drawX, drawY, 16, 16);
				if ((WorldData[pos] != 0) && ((WorldData[pos] == WATER + 1) || (WorldData[pos] == LAVA + 1) || (WorldData[pos - 1] == 0) || (WorldData[pos + 1] == 0) || (WorldData[pos - 200] == 0) || (WorldData[pos + 200] == 0)))
					gfx_TransparentSprite(sprites[WorldData[pos] - 1], drawX, drawY);
				if ((WorldData[pos] == WATERENTITY + 1) || (WorldData[pos] == WATERENTITYB + 1))
					gfx_TransparentSprite(sprites[WATER], drawX, drawY);
				if ((WorldData[pos] == LAVAENTITY + 1) || (WorldData[pos] == LAVAENTITYB + 1))
					gfx_TransparentSprite(sprites[LAVA], drawX, drawY);
				drawX += 16;
				count++;
				pos++;
				if (count == 22)
				{
					count = 1;
					drawX = scrollX;
					drawY += 16;
					pos += (200 - 21);
				}
			}


			gfx_PrintStringXY("v1.0.0a by TimmyCraft", 2, 2);

			gfx_SetTextFGColor(224);
			gfx_SetTextXY(15, 15);
			gfx_PrintInt(scrollX, 1);

			gfx_SetColor(0);
			gfx_Rectangle(curX, curY, 16, 16);

			//hotbar
			for (x = 0; x < 5; x++)
			{
				if (x == hotbarSel)
				{
					gfx_SetColor(5);
				}
				else
				{
					gfx_SetColor(0);
				}
				gfx_Rectangle(117 + (x * 18), 220, 18, 18);
				if (hotbar[x] != 0)
				{
					gfx_TransparentSprite(sprites[hotbar[x]], 118 + (x * 18), 221);
				}
				else
				{
					gfx_SetColor(181);
					gfx_FillRectangle(118 + (x * 18), 221, 16, 16);
				}
			}
			
			gfx_TransparentSprite(Head_1, 16 * 9 + 3, 16 * 5 + 16);
			gfx_TransparentSprite(Body_1, 16 * 9 + 5, 16 * 5 + 24);
			gfx_TransparentSprite(Leg_1, 16 * 9 + 5, 16 * 5 + 35);

			gfx_BlitBuffer();

		}
		

		if (dialog != 0)
		{
			gfx_SetTextFGColor(224);
			gfx_PrintStringXY(dialogString, 2, 32);
			gfx_BlitBuffer();
			dialogTimer--;
			if (dialogTimer == 0)
			{
				redraw = 1;
				dialog = 0;
			}
		}

		if (kb_IsDown(kb_KeyStat) || kb_IsDown(kb_KeyAlpha) || kb_IsDown(kb_KeyApps) || kb_IsDown(kb_KeyMode))
		{
			redraw = 1;
			delay(100);
		}

		// x 96 and 224
		// y 32 and 210
		if ((curX < 0) && (!(kb_IsDown(kb_KeyRight)))) {
			curX += 16;
			curPos++;
		}
		if ((curX > 320) && (!(kb_IsDown(kb_KeyLeft)))) {
			curX -= 16;
			curPos--;
		}
		if ((curY < 0) && (!(kb_IsDown(kb_KeyDown)))) {
			curY += 16;
			curPos += 200;
		}
		if ((curY > 240) && (!(kb_IsDown(kb_KeyUp)))) {
			curY += 16;
			curPos -= 200;
		}

		blockSel = hotbar[hotbarSel];
		if (kb_IsDown(kb_Key2nd) && (WorldData[curPos] == 0) && (blockSel != 0)) {
			WorldData[curPos] = blockSel + 1;
			
			if (blockSel == GRASS) WorldDataTimer[curPos] = 200;
			if (blockSel == WATER) WorldDataTimer[curPos] = 10;
			if (blockSel == LAVA) WorldDataTimer[curPos] = 30;
			if (blockSel == GRAVEL) WorldDataTimer[curPos] = 3;
			if (blockSel == SAND) WorldDataTimer[curPos] = 3;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyDel)) {
			redraw = 1;
			WorldData[curPos] = 0;
		}

		if (kb_IsDown(kb_KeyMath))
		{
			delay(140);
			dialogTimer = 100;
			dialog = 1;
			redraw = 1;
			// 0 = creative, 1 = survival, 2 = adventure
			gamemode++;
			if (gamemode > 2) gamemode = 0;
			if (gamemode == 0) dialogString = "Gamemode switched to Creative";
			if (gamemode == 1) dialogString = "Gamemode switched to Survival";
			if (gamemode == 2) dialogString = "Gamemode switched to Adventure";
		}
		
		if (kb_IsDown(kb_KeyPrgm))
		{
			delay(140);
			dialogTimer = 100;
			dialog = 1;
			redraw = 1;
			// 0 = off, 1 = on
			flymode++;
			if (flymode > 1) flymode = 0;
			if (flymode == 0) dialogString = "Fly Mode toggled Off";
			if (flymode == 1) dialogString = "Fly Mode toggled On";
		}

		testX = playerX + 9;
		testY = playerY + 5;
		
		if (kb_IsDown(kb_KeyLeft) && ((playerX > 0) || (scrollX < -1)) && (WorldData[(testX + ((testY + 1) * 200))] == 0))
		{
			if (scrollX > -1)
			{
				LoadNew(1);
				curPos--;
				scrollX = -16;
				curX -= 16;
				playerX--;
			}
            scrollX += pixelAmount;
			curX += pixelAmount;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyRight) && (playerX < 200) && (WorldData[((testX + 1) + ((testY + 1) * 200))] == 0))
		{
            scrollX -= pixelAmount;
			curX -= pixelAmount;
			if (scrollX < -15)
			{
				LoadNew(2);
				curPos++;
				scrollX = 0;
				curX += 16;
				playerX++;
			}
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyUp) && (playerY > 0) && ((WorldData[(testX + (testY * 200))] == 0) || (scrollY < -1)))
		{
			if (scrollY > -1)
			{
				LoadNew(3);
				curPos -= 200;
				scrollY = -16;
				curY -= 16;
				playerY--;
			}
            scrollY += pixelAmount;
			curY += pixelAmount;
			redraw = 1;
		}
		
		if (kb_IsDown(kb_KeyDown) && (playerY < 200 - 15) && (WorldData[(testX + ((testY + 3) * 200))] == 0))
		{
			scrollY -= pixelAmount;
			curY -= pixelAmount;
			if (scrollY < -15)
			{
				LoadNew(4);
				curPos += 200;
				scrollY = 0;
				curY += 16;
				playerY++;
			}
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyDown) && (playerY < 200 - 15) && (WorldData[(testX + ((testY + 3) * 200))] == 0))
		{
			scrollY -= pixelAmount;
			curY -= pixelAmount;
			if (scrollY < -15)
			{
				LoadNew(4);
				curPos += 200;
				scrollY = 0;
				curY += 16;
				playerY++;
			}
			redraw = 1;
		}

		// fix noclip collision problems
		if ((playerY > 0) && (WorldData[(testX + ((testY + 2) * 200))] != 0))
		{
			LoadNew(3);
			curPos -= 200;
			scrollY = 0;
			playerY--;
			redraw = 1;
		}

		// fix Cursor not snapped to grid


		if (kb_IsDown(kb_KeyStat) && (curX < 320)) {
			curX += 16;
			curPos += 1;
		}
		if (kb_IsDown(kb_KeyAlpha) && (curX > 0)) {
			curX -= 16;
			curPos -= 1;
		}
		if (kb_IsDown(kb_KeyApps) && (curY < 240)) {
			curY += 16;
			curPos += 200;
		}
		if (kb_IsDown(kb_KeyMode) && (curY > 0)) {
			curY -= 16;
			curPos -= 200;
		}


		if (kb_IsDown(kb_KeyGraphVar))
			creativeInventory();

		
		if (kb_IsDown(kb_KeyYequ))
		{
			hotbarSel = 0;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyWindow))
		{
			hotbarSel = 1;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyZoom))
		{
			hotbarSel = 2;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyTrace))
		{
			hotbarSel = 3;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyGraph))
		{
			hotbarSel = 4;
			redraw = 1;
		}


	}


}

int LoadNew(int24_t position) {

	// position: 1 = up, 2 = down, 3 = left, 4 = right (not added fully yet)
	int24_t x = 0, y = 0;
	for (x = 0; x < renderX * 16; x++) {
		for (y = 0; y < renderY * 16; y++) {
			WorldDataTimer[x * y] = 0;
		}
	}

}

int LoadChunks(int24_t position) {

	int24_t scan = position;
	if (position < 0) error = 1;
	for (y = 0; y < 16 * renderY; y++) {
		for (x = 0; x < 16 * renderX; x++) {
			Behaviors(scan);
			scan++;
		}
		scan += 200 - (renderX * 16);
	}

}

int Behaviors(int24_t position) {
	
	if ((WorldDataTimer[position] > 0) && (WorldData[position] != GRASS + 1)) {
		WorldDataTimer[position]--;
		redraw = 1;
	}

	// grass turns to dirt
	if ((WorldData[position] == GRASS + 1) && (WorldData[position - 200] != 0) && (WorldDataTimer[position] > -1))
	{
		if (WorldDataTimer[position] == 0) WorldData[position] = DIRT + 1;
		WorldDataTimer[position]--;
		redraw = 1;
	}

	// water flows downward
	if ((WorldData[position] == WATER + 1) && (WorldData[position + 200] == 0) && (WorldDataTimer[position] == 0))
	{
		WorldData[position + 200] = WATERENTITY + 1;
		WorldDataTimer[position + 200] = 10;
	}
	if ((WorldData[position] == WATERENTITY + 1) && (WorldData[position + 200] == 0) && (WorldDataTimer[position] == 0))
	{
		WorldData[position + 200] = WATERENTITY + 1;
		WorldDataTimer[position + 200] = 10;
	}

	// water flows sideways
	if (((WorldData[position] == WATER + 1) || (WorldData[position] == WATERENTITY + 1)) && (WorldData[position + 200] != 0) && (WorldData[position + 200] != WATERENTITY + 1) && (WorldDataTimer[position] == 0))
	{
		if (WorldData[position + 200 - 1] == 0)
		{
			WorldData[position + 200 - 1] = WATERENTITY + 1;
			WorldDataTimer[position + 200 - 1] = 10;
		}
		if (WorldData[position + 200 + 1] == 0)
		{
			WorldData[position + 200 + 1] = WATERENTITY + 1;
			WorldDataTimer[position + 200 + 1] = 10;
		}
	}
	if (((WorldData[position] == WATER + 1) || (WorldData[position] == WATERENTITY + 1)) && (WorldData[position + 200] != 0) && (WorldData[position + 200] != WATERENTITY + 1) && (WorldDataTimer[position] == 0))
	{
		if (WorldData[position - 1] == 0)
		{
			WorldData[position - 1] = WATERENTITY + 1;
			WorldDataTimer[position - 1] = 10;
		}
		if (WorldData[position + 1] == 0)
		{
			WorldData[position + 1] = WATERENTITY + 1;
			WorldDataTimer[position + 1] = 10;
		}
	}

	// lava flows downward
	if ((WorldData[position] == LAVA + 1) && (WorldData[position + 200] == 0) && (WorldDataTimer[position] == 0))
	{
		WorldData[position + 200] = LAVAENTITY + 1;
		WorldDataTimer[position + 200] = 30;
	}
	if ((WorldData[position] == LAVAENTITY + 1) && (WorldData[position + 200] == 0) && (WorldDataTimer[position] == 0))
	{
		WorldData[position + 200] = LAVAENTITY + 1;
		WorldDataTimer[position + 200] = 30;
	}
	
	// lava flows sideways
	if (((WorldData[position] == LAVA + 1) || (WorldData[position] == LAVAENTITY + 1)) && (WorldData[position + 200] != 0) && (WorldData[position + 200] != LAVAENTITY + 1) && (WorldDataTimer[position] == 0))
	{
		if (WorldData[position + 200 - 1] == 0)
		{
			WorldData[position + 200 - 1] = LAVAENTITY + 1;
			WorldDataTimer[position + 200 - 1] = 30;
		}
		if (WorldData[position + 200 + 1] == 0)
		{
			WorldData[position + 200 + 1] = LAVAENTITY + 1;
			WorldDataTimer[position + 200 + 1] = 30;
		}
	}
	if (((WorldData[position] == LAVA + 1) || (WorldData[position] == LAVAENTITY + 1)) && (WorldData[position + 200] != 0) && (WorldData[position + 200] != LAVAENTITY + 1) && (WorldDataTimer[position] == 0))
	{
		if (WorldData[position - 1] == 0)
		{
			WorldData[position - 1] = LAVAENTITY + 1;
			WorldDataTimer[position - 1] = 10;
		}
		if (WorldData[position + 1] == 0)
		{
			WorldData[position + 1] = LAVAENTITY + 1;
			WorldDataTimer[position + 1] = 10;
		}
	}

	// sand falls
	if ((WorldData[position] == SAND + 1) && (WorldDataTimer[position] == 0))
	{
		if (WorldData[position + 200] == WATER + 1) 
		{
			WorldData[position] = 0;
			if (WorldData[position + 400] != WATERENTITYB + 1) WorldData[position + 200] = SAND + 1;
			WorldData[position + 400] = SAND + 1;
			WorldDataTimer[position + 400] = 3;
		}
		if (WorldData[position + 200] == LAVA + 1) 
		{
			WorldData[position] = 0;
			if (WorldData[position + 400] != LAVAENTITYB + 1) WorldData[position + 200] = SAND + 1;
			WorldData[position + 400] = SAND + 1;
			WorldDataTimer[position + 400] = 3;
		}
		if (WorldData[position + 200] == WATERENTITYB + 1)
		{
			WorldData[position] = LAVAENTITYB;
			WorldData[position + 200] = SAND + 1;
			WorldDataTimer[position + 200] = 3;
		}
		if (WorldData[position + 200] == LAVAENTITYB + 1)
		{
			WorldData[position] = WATERENTITYB;
			WorldData[position + 200] = SAND + 1;
			WorldDataTimer[position + 200] = 3;
		}
		if ((WorldData[position + 200] == 0) || (WorldData[position + 200] == WATERENTITY + 1) || (WorldData[position + 200] == LAVAENTITY + 1))
		{
			WorldData[position] = 0;
			WorldData[position + 200] = SAND + 1;
			WorldDataTimer[position + 200] = 3;
		}
	}
	// gravel falls
	if ((WorldData[position] == GRAVEL + 1) && (WorldDataTimer[position] == 0))
	{
		if (WorldData[position + 200] == WATER + 1) 
		{
			WorldData[position] = 0;
			if (WorldData[position + 400] != WATERENTITYB + 1) WorldData[position + 200] = GRAVEL + 1;
			WorldData[position + 400] = GRAVEL + 1;
			WorldDataTimer[position + 400] = 3;
		}
		if (WorldData[position + 200] == LAVA + 1) 
		{
			WorldData[position] = 0;
			if (WorldData[position + 400] != LAVAENTITYB + 1) WorldData[position + 200] = GRAVEL + 1;
			WorldData[position + 400] = GRAVEL + 1;
			WorldDataTimer[position + 400] = 3;
		}
		if (WorldData[position + 200] == WATERENTITYB + 1)
		{
			WorldData[position] = LAVAENTITYB;
			WorldData[position + 200] = GRAVEL + 1;
			WorldDataTimer[position + 200] = 3;
		}
		if (WorldData[position + 200] == LAVAENTITYB + 1)
		{
			WorldData[position] = WATERENTITYB;
			WorldData[position + 200] = GRAVEL + 1;
			WorldDataTimer[position + 200] = 3;
		}
		if ((WorldData[position + 200] == 0) || (WorldData[position + 200] == WATERENTITY + 1) || (WorldData[position + 200] == LAVAENTITY + 1))
		{
			WorldData[position] = 0;
			WorldData[position + 200] = GRAVEL + 1;
			WorldDataTimer[position + 200] = 3;
		}
	}

}

int creativeInventory(void)
{
	int24_t tab, redraw, selX, selY, posB, oldBlock, newBlock, timer, selXb, selYb, selPos, i, maxSize;
	char *names[4] = { "Nature", "Building", "Redstone", "Tools" };
	gfx_FillScreen(dayColors[timeofday / 6000]);
	gfx_SetTextFGColor(0);
	LoadBlocks("MCEDEFT");
	selX = 10;
	selY = 30;
	posB = 1;
	tab = 0;
	oldBlock = 0;
	newBlock = 0;
	timer = 0;
	redraw = 1;
	while (!(kb_IsDown(kb_KeyClear)))
	{
		if (redraw == 1)
		{
			timer = 0;
			redraw = 0;
			maxSize = typesvalues[tab];
			gfx_SetColor(181);
			gfx_FillCircle(10, 10, 5);
			gfx_FillCircle(309, 10, 5);
			gfx_FillCircle(10, 229, 5);
			gfx_FillCircle(309, 229, 5);
			gfx_FillRectangle(10, 5, 300, 230);
			gfx_FillRectangle(5, 10, 310, 220);
			i = (tab != 0);
			pos = 14 + ((tab != 0) * 20);
			for (x = pos; x < pos + (3 * 82); x += 82)
			{
				if (tab != i)
					gfx_SetColor(148);
				if (tab == i)
					gfx_SetColor(181);
				gfx_FillRectangle(x, 11, 82, 20);
				gfx_SetColor(0);
				gfx_Rectangle(x, 11, 82, 20);
				gfx_PrintStringXY(names[i], x + 18, 16);
				if (i == 0)
					gfx_TransparentSprite(sprites[natureBlocks[0]], x + 1, 12);
				if (i == 1)
					gfx_TransparentSprite(sprites[buildingBlocks[0]], x + 1, 12);
				if (i == 2)
					gfx_TransparentSprite(sprites[redstoning[0]], x + 1, 12);
				if (i == 3)
					gfx_TransparentSprite(sprites[toolsEtc[0]], x + 1, 12);
				i++;
			}
			gfx_SetColor(148);
			if (tab < 1) gfx_FillRectangle(x, 11, 20, 20);
			if (tab > 0) gfx_FillRectangle(14, 11, 20, 20);
			gfx_SetColor(0);
			if (tab < 1) gfx_Rectangle(x, 11, 20, 20);
			if (tab > 0) gfx_Rectangle(14, 11, 20, 20);
			gfx_SetColor(4);
			if (tab < 1) gfx_FillTriangle(x + 6, 15, x + 6, 25, x + 14, 20);
			if (tab > 0) gfx_FillTriangle(28, 15, 28, 25, 19, 20);
			pos = 0;
			
			gfx_SetTextFGColor(0);
			for (y = 30; y < 10 * 18; y += 18)
			{
				for (x = 10; x < 10 * 28; x += 18)
				{
					gfx_SetColor(148);
					gfx_FillRectangle(x, y, 18, 18);
					gfx_SetColor(0);
					gfx_Rectangle(x, y, 18, 18);
					if (pos < typesvalues[tab])
					{
						if (tab == 0)
							gfx_TransparentSprite(sprites[natureBlocks[pos++]], x + 1, y + 1);
						if (tab == 1)
							gfx_TransparentSprite(sprites[buildingBlocks[pos++]], x + 1, y + 1);
						if (tab == 2)
							gfx_TransparentSprite(sprites[redstoning[pos++]], x + 1, y + 1);
						if (tab == 3)
							gfx_TransparentSprite(sprites[toolsEtc[pos++]], x + 1, y + 1);
					}
				}
			}
			//hotbar
			for (x = 0; x < 5; x++)
			{
				if (x == hotbarSel)
				{
					gfx_SetColor(5);
				}
				else
				{
					gfx_SetColor(0);
				}
				gfx_Rectangle(117 + (x * 18), 210, 18, 18);
				if (hotbar[x] != 0)
				{
					gfx_TransparentSprite(sprites[hotbar[x]], 118 + (x * 18), 211);
				}
				else
				{
					gfx_SetColor(148);
					gfx_FillRectangle(118 + (x * 18), 211, 16, 16);
				}
			}

			//gfx_TransparentSprite(inventory, 4, 4);

			if (newBlock != 0) gfx_TransparentSprite(sprites[newBlock], selX + 8, selY + 8);
			//cursor
			gfx_SetColor(254);
			gfx_FillRectangle(selX + 3, selY + 8, 5, 2);
			gfx_FillRectangle(selX + 10, selY + 8, 5, 2);
			
			gfx_FillRectangle(selX + 8, selY + 3, 2, 5);
			gfx_FillRectangle(selX + 8, selY + 10, 2, 5);

			gfx_BlitBuffer();
		}
		kb_Scan();

		if (kb_IsDown(kb_KeyYequ) && (tab > 0))
		{
			tab--;
			redraw = 1;
			delay(100);
		}
		if (kb_IsDown(kb_KeyGraph) && (tab < 3))
		{
			tab++;
			redraw = 1;
			delay(100);
		}

		if (selY == 210)
		{
			if (kb_IsDown(kb_Key2nd))
			{
				dbg_sprintf(dbgout, "%u", newBlock);
				oldBlock = hotbar[posB];
				hotbar[posB] = newBlock;
				newBlock = oldBlock;
				oldBlock = 0;
				redraw = 1;
				delay(100);
			}
		}
		else
		{
			if (kb_IsDown(kb_Key2nd) && (newBlock != 0))
			{

				newBlock = oldBlock;
				redraw = 1;
				delay(100);
			}
			if (kb_IsDown(kb_Key2nd) && (newBlock != 0) && (posB > maxSize))
			{

				newBlock = 0;
				redraw = 1;
				delay(100);
			}

			if (kb_IsDown(kb_Key2nd) && (newBlock == 0) && (posB < maxSize + 1))
			{
				if (tab == 0)
					newBlock = natureBlocks[posB - 1];
				if (tab == 1)
					newBlock = buildingBlocks[posB - 1];
				if (tab == 2)
					newBlock = redstoning[posB - 1];
				if (tab == 3)
					newBlock = toolsEtc[posB - 1];
				dbg_sprintf(dbgout, "%u", newBlock);
				redraw = 1;
				delay(100);
			}
		}

		if (selY < 9 * 18)
		{
			if ((kb_IsDown(kb_KeyLeft)) && (selX > 10))
			{
				selX -= 18;
				posB--;
				redraw = 1;
				delay(80);
			}
			if ((kb_IsDown(kb_KeyRight)) && (selX < 14 * 18))
			{
				selX += 18;
				posB++;
				redraw = 1;
				delay(80);
			}
		}
		else
		{
			if (kb_IsDown(kb_KeyLeft) && (selX > 117))
			{
				posB--;
				selX -= 18;
				redraw = 1;
				delay(80);
			}
			if (kb_IsDown(kb_KeyRight) && (selX < 189))
			{
				posB++;
				selX += 18;
				redraw = 1;
				delay(80);
			}
		}

		if (kb_IsDown(kb_KeyUp) && (selY == 210))
		{
			selX = selXb;
			selY = selYb - 18;
			posB = selPos - 15;
			redraw = 1;
			delay(80);
		}

		if ((kb_IsDown(kb_KeyUp)) && (selY > 30) && (selY < 10 * 18))
		{
			selY -= 18;
			posB -= 15;
			redraw = 1;
			delay(80);
		}
		if (kb_IsDown(kb_KeyDown))
		{
			if (selY < 210)
			{
				selY += 18;
				posB += 15;
				redraw = 1;
				delay(80);
			}
			if (selY == 30 + 9 * 18)
			{
				selXb = selX;
				selYb = selY;
				selPos = posB;
				selX = 117;
				selY = 210;
				posB = 0;
				redraw = 1;
				delay(80);
			}
		}
	}
	delay(200);
	kb_Scan();
	WorldEngine();
}