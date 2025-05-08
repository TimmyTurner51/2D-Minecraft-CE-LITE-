
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

#include "giveItem.h"
#include "Behaviors.h"

void Behaviors(int16_t position, int16_t timerPos)
{

	// grass turns to dirt

	// remove flowing down water when source was removed
	if (WorldData[position] == WATERENTITY && WorldData[position - MaxX] != 215 && WorldData[position - MaxX] < WATERENTITY && WorldDataTimer[timerPos - MaxX] < 1) {
		WorldData[position] = 0;
		WorldDataTimer[timerPos] = 3;
	}
	if (WorldData[position] >= WATERENTITY && WorldData[position - MaxX] == 0 && WorldData[position - 1] == 0 && WorldData[position + 1] == 0 && WorldDataTimer[timerPos - MaxX] < 1) {
		WorldData[position] = 0;
		WorldDataTimer[timerPos] = 3;
	}
	// water flows sideways
	if (WorldData[position + MaxX] != 0 && WorldData[position + MaxX] < WATERENTITY && WorldDataTimer[timerPos] < 1) {
		if (WorldData[position] == WATERENTITY + 7 && WorldData[position - 1] < WATERENTITY && WorldData[position + 1] < WATERENTITY)
			WorldData[position] = 0;
		if ((WorldData[position - 1] == 0 || WorldData[position - 1] == WATERENTITY || WorldData[position - 1] >= MaxSprites[0] + MaxSprites[3] + 1) && WorldDataTimer[timerPos] < 1 && WorldData[position] >= WATERENTITY && WorldData[position] < WATERENTITY + 7 && WorldData[position - 1] + 2 <= WorldData[position]) {
			if (WorldData[position - 1] >= MaxSprites[0] + MaxSprites[3] + 1)
				giveItem(WorldData[position - 1], 0);
			WorldData[position - 1] = WorldData[position] + 1;
			WorldDataTimer[timerPos - 1] = 3;
		}
		if ((WorldData[position + 1] == 0 || WorldData[position + 1] == WATERENTITY || WorldData[position + 1] >= MaxSprites[0] + MaxSprites[3] + 1) && WorldDataTimer[timerPos] < 1 && WorldData[position] >= WATERENTITY && WorldData[position] < WATERENTITY + 7 && WorldData[position + 1] + 2 <= WorldData[position]) {
			if (WorldData[position - 1] >= MaxSprites[0] + MaxSprites[3] + 1)
				giveItem(WorldData[position + 1], 0);
			WorldData[position + 1] = WorldData[position] + 1;
			WorldDataTimer[timerPos + 1] = 3;
		}
	}
	// removal of water (flowing sideways)
	if (WorldData[position] > WATERENTITY && WorldData[position] < WATERENTITY + 7 && WorldData[position] > WorldData[position - 1] + 1 && WorldData[position] > WorldData[position - MaxX] && WorldData[position] < WorldData[position + 1] + 1)
		WorldData[position]++;
	if (WorldData[position] > WATERENTITY && WorldData[position] < WATERENTITY + 7 && WorldData[position] < WorldData[position - 1] + 1 && WorldData[position] > WorldData[position - MaxX] && WorldData[position] > WorldData[position + 1] + 1)
		WorldData[position]++;
	if (WorldData[position] > WATERENTITY && WorldData[position] < WATERENTITY + 7 && WorldData[position - 1] >= WorldData[position] && WorldData[position + 1] >= WorldData[position])
		WorldData[position]++;
	if (WorldData[position] == WATERENTITY + 7 && (WorldData[position - 1] == 0 || WorldData[position - 1] == WorldData[position]) && (WorldData[position + 1] == 0 || WorldData[position - 1] == WorldData[position]) && WorldData[position - MaxX] < WorldData[position]) {
		WorldData[position] = 0;
		WorldDataTimer[timerPos] = 3;
	}
	// water flows downward
	if ((WorldData[position] == 215 || (WorldData[position] >= WATERENTITY && WorldData[position] <= WATERENTITY + 7)) && (WorldData[position + MaxX] == 0 || WorldData[position + MaxX] >= MaxSprites[0] + MaxSprites[3] + 1) && WorldDataTimer[timerPos] < 1) {
		if (WorldData[position + MaxX] >= MaxSprites[0] + MaxSprites[3] + 1 && WorldData[position] < WATERENTITY)
			giveItem(WorldData[position], 0);
		if (WorldData[position + MaxX] != WATERENTITY && WorldData[position + MaxX] != LAVAENTITY) {
			WorldData[position + MaxX] = WATERENTITY;
			WorldDataTimer[timerPos + 80] = 3;
		}
	}

}
