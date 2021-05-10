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

#include "gfx/gfx.h"

gfx_TempSprite(logo, 16, 16);
int natureBlocks[13] = {GRASS, DIRT, STONE, COBBLESTONE, SAND, GRAVEL, OAKLOGS, OAKLEAVES, BEDROCK, COALORE, IRONORE, GOLDORE, LAPIZORE};
int buildingBlocks[3] = {OAKPLANK, GLASS, SPONGE};
int redstoning[3] = {REDSTONEDUSTOFF, NOTEBLOCK, REGULARPISTONRIGHTOFF};
int toolsEtc[3] = { BEDBACK, WATER, LAVA };

int typesvalues[4] = {13, 3, 3, 3};


//#define usb_callback_data_t usb_device_t

int Generator(void);
int WorldEngine(void);
int Behaviors(void);
int creativeInventory(void);


char WorldData[ 200 * 200 ] = { 0 };

gfx_sprite_t *sprites[254];
gfx_sprite_t dummy_sprite = {1, 1, 0};


// Amount of blocks to render (similar to Chunk distance, but not all may not be drawn). X amount * Y amount
int24_t renderDistance = 32 * 32;

// Amount of Pixels to scroll. Ranges from 1 - 16. 1 being smooth but slowest, 16 being jumpy but fastest.
int24_t pixelAmount = 4;


// Best not to change these...
int24_t redraw = 1, playerX = 0, playerY = 0, curPos = (200 * 2) + 6, curX = 96, curY = 32;
int24_t blockSel = 0, hotbarSel = 0, timeofday = 0;
int24_t pos = 0, render = 0, x = 0, y = 0, drawX = 0, drawY = 0, count = 1;
int24_t scrollX = 0, scrollY = 0;
int24_t hotbar[5] = {0}, dayColors[5] = {191, 158, 125, 51, 9};

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
	int24_t groundLevel = 8;
	for (x = 0; x < 200; x++)
	{
		if ((groundLevel > 1) && (groundLevel < 200))
			groundLevel += randInt(-1, 1);
		for (y = 0; y < 200; y++)
		{
			blockType = 0;

			if (y == groundLevel)
				blockType = GRASS + 1;
			if (y > groundLevel)
				blockType = DIRT + 1;
			if (y > groundLevel + 4)
				blockType = STONE + 1;
			if (y == 200)
				blockType = BEDROCK + 1;

			WorldData[x + (y * 200)] = blockType;
		}
	}

}

int WorldEngine(void)
{
	redraw = 1;
	gfx_SetClipRegion(0-17, 0-17, 337, 257);
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
			for (render = 0; render < renderDistance; render++)
			{
				if (WorldData[pos] != 0)
					gfx_TransparentSprite(sprites[WorldData[pos] - 1], drawX, drawY);
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


			gfx_PrintStringXY("v1.0.00a by TimmyCraft", 2, 2);

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
			gfx_BlitBuffer();

		}

		if (kb_IsDown(kb_KeyStat) || kb_IsDown(kb_KeyAlpha) || kb_IsDown(kb_KeyApps) || kb_IsDown(kb_KeyMode))
			redraw = 1;

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
		if (kb_IsDown(kb_Key2nd)) {
			WorldData[curPos] = blockSel + 1;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyDel)) {
			redraw = 1;
			WorldData[curPos] = 0;
		}

		if (kb_IsDown(kb_KeyLeft) && (playerX > 0))
		{
			if (scrollX > -1)
			{
				curPos--;
				scrollX = -16;
				curX -= 16;
				playerX--;
			}
            scrollX += pixelAmount;
			curX += pixelAmount;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyRight) && (playerX < 200))
		{
			if (scrollX < -15)
			{
				curPos++;
				scrollX = 0;
				curX += 16;
				playerX++;
			}
            scrollX -= pixelAmount;
			curX -= pixelAmount;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyUp) && (playerY > 0))
		{
			if (scrollY > -1)
			{
				curPos -= 200;
				scrollY = -16;
				curY -= 16;
				playerY--;
			}
            scrollY += pixelAmount;
			curY += pixelAmount;
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyDown) && (playerY < 200))
		{
			if (scrollY < -15)
			{
				curPos += 200;
				scrollY = 0;
				curY += 16;
				playerY++;
			}
            scrollY -= pixelAmount;
			curY -= pixelAmount;
			redraw = 1;
		}

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

int Behaviors(void) {



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