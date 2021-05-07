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

//#define usb_callback_data_t usb_device_t

int Generator(void);
int WorldEngine(void);
int Behaviors(void);


char WorldData[ 200 * 200 ] = { 0 };

gfx_sprite_t *sprites[254];
gfx_sprite_t dummy_sprite = {1, 1, 0};


// Amount of blocks to render (similar to Chunk distance, but not all may not be drawn). X amount * Y amount
int24_t renderDistance = 32 * 32;

// Amount of Pixels to scroll. Ranges from 1 - 16. 1 being smooth but slowest, 16 being jumpy but fastest.
int24_t pixelAmount = 4;


// Best not to change these...
int24_t redraw = 1, playerX = 0, playerY = 0, curPos = 0, curX = 0, curY = 0;
int24_t blockSel = 0;

#define GRASS = 1
#define DIRT = 2
#define STONE = 3
#define BEDROCK = 4

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
				blockType = 1;
			if (y > groundLevel)
				blockType = 2;
			if (y > groundLevel + 4)
				blockType = 3;
			if (y == 200)
				blockType = 4;

			WorldData[x + (y * 200)] = blockType;
		}
	}

}

int WorldEngine(void)
{

	int24_t pos = 0, render = 0, x = 0, y = 0, drawX = 0, drawY = 0, count = 1;
	int24_t scrollX = 0, scrollY = 0;
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
				if (WorldData[pos] == 1)
				{
					gfx_TransparentSprite(sprites[2], drawX, drawY);
				}
				if (WorldData[pos] == 2)
				{
					gfx_TransparentSprite(sprites[3], drawX, drawY);
				}
				if (WorldData[pos] == 3)
				{
					gfx_TransparentSprite(sprites[1], drawX, drawY);
				}
				if (WorldData[pos] == 4)
				{
					gfx_TransparentSprite(sprites[7], drawX, drawY);
				}
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
			gfx_BlitBuffer();

		}


		if (kb_IsDown(kb_KeyLeft) && (playerX > 0))
		{
            scrollX += pixelAmount;
			if (scrollX > 0)
			{
				scrollX = -16;
				playerX--;
			}
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyRight) && (playerX < 200))
		{
            scrollX -= pixelAmount;
			if (scrollX < -16)
			{
				scrollX = 0;
				playerX++;
			}
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyUp) && (playerY > 0))
		{
            scrollY += pixelAmount;
			if (scrollY > 0)
			{
				scrollY = -16;
				playerY--;
			}
			redraw = 1;
		}
		if (kb_IsDown(kb_KeyDown) && (playerY < 200))
		{
            scrollY -= pixelAmount;
			if (scrollY < -16)
			{
				scrollY = 0;
				playerY++;
			}
			redraw = 1;
		}

	}


}

int Behaviors(void) {



}