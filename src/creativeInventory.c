
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
#include "Game.h"
#include "creativeInventory.h"

void creativeInventory(void)
{
	int16_t xpos = 28, ypos = 25, num, spritenum, tab = 0, pos = 0, hotbarpos = 0, lastPos;
	int16_t scroll = 0, InvCurPos = 0, lastXpos = 0, selectedFromTab = 0;
	int16_t newBlock = 0, newBlockCount = 0, oldBlock = 0, oldBlockCount = 0;
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
		if (tab == 0) gfx_TransparentSprite(BlockTextures[94], 30, 8);
		if (tab == 1) gfx_TransparentSprite(ItemTextures[0], 30, 8);
		if (tab == 2) gfx_TransparentSprite(PlantTextures[0], 30, 8);
		if (tab == 3) gfx_TransparentSprite(FoliageTextures[0], 30, 8);
		if (tab == 4) gfx_TransparentSprite(BlockTextures[10], 30, 8);
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
					gfx_TransparentSprite(BlockTextures[spritenum], x + 1, y + 1);
				if (tab == 1 && spritenum <= MaxSprites[1])
					gfx_TransparentSprite(ItemTextures[spritenum], x + 1, y + 1);
				if (tab == 2 && spritenum <= MaxSprites[2])
					gfx_TransparentSprite(PlantTextures[spritenum], x + 1, y + 1);
				if (tab == 3 && spritenum <= MaxSprites[3])
					gfx_TransparentSprite(FoliageTextures[spritenum], x + 1, y + 1);
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
					gfx_TransparentSprite(BlockTextures[hotbar[x] - 1], 118 + (x * 18), 211);
				// check if it's foliage
				if (hotbar[x] > MaxSprites[0] && hotbar[x] <= MaxSprites[0] + MaxSprites[3])
					gfx_TransparentSprite(FoliageTextures[hotbar[x] - 1 - MaxSprites[0]], 118 + (x * 18), 211);
				// check if it's plants
				if (hotbar[x] > MaxSprites[0] + MaxSprites[3] && hotbar[x] <= MaxSprites[0] + MaxSprites[3] + MaxSprites[2])
					gfx_TransparentSprite(PlantTextures[hotbar[x] - 1 - (MaxSprites[0] + MaxSprites[3])], 118 + (x * 18), 211);
			}else{
				gfx_SetColor(181);
				gfx_FillRectangle(118 + (x * 18), 211, 16, 16);
			}
		}
		// draw a item if it was selected
		if (newBlock != 0)
		{
			if (selectedFromTab == 0 && newBlock <= MaxSprites[0])
				gfx_TransparentSprite(BlockTextures[newBlock - 1], xpos + 5, ypos + 5);
			if (selectedFromTab == 1 && newBlock <= MaxSprites[1])
				gfx_TransparentSprite(ItemTextures[newBlock - 1], xpos + 5, ypos + 5);
			if (selectedFromTab == 2 && newBlock <= MaxSprites[2])
				gfx_TransparentSprite(PlantTextures[newBlock - 1], xpos + 5, ypos + 5);
			if (selectedFromTab == 3 && newBlock <= MaxSprites[3])
				gfx_TransparentSprite(FoliageTextures[newBlock - 1], xpos + 5, ypos + 5);
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
		if (kb_IsDown(kb_Key2nd) || kb_IsDown(kb_KeyUp) || kb_IsDown(kb_KeyDown) || kb_IsDown(kb_KeyLeft) || kb_IsDown(kb_KeyRight))
		delay(80);
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
			oldBlockCount = hotbar[hotbarpos + 5];
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
				hotbar[hotbarpos] = 0;
				hotbar[hotbarpos + 5] = 0;
			}else{
				hotbar[hotbarpos] = newBlock;
				hotbar[hotbarpos + 5] = newBlockCount;
			}
			newBlock = oldBlock;
			newBlockCount = oldBlockCount;
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
