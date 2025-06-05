#include <stdint.h>
#include <graphx.h>
#include <ti/screen.h>
#include "achievements.h"
#include "render.h"
#include "globals.h"

void Achievements(void)
{
	int16_t x, y;
	DrawDirtBackground(0);
	gfx_SetColor(181);
	gfx_FillCircle(10, 10, 5);
	gfx_FillCircle(309, 10, 5);
	gfx_FillCircle(10, 229, 5);
	gfx_FillCircle(309, 229, 5);
	gfx_FillRectangle(10, 5, 300, 230);
	gfx_FillRectangle(5, 10, 310, 220);
	gfx_SetTextFGColor(0);
	DrawCenteredText(MenuElements[1], 160, 20);
	gfx_SetTextFGColor(255);
	for (x = 2; x < 18; x++)
	{
		for (y = 2; y < 13; y++)
		{
			gfx_TransparentSprite(BlockTextures[201], x * 16, y * 16);
		}
	}
	gfx_BlitBuffer();

	while (!(os_GetCSC()));
	delay(100);
}