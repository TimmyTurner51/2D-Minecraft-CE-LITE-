
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
#include "LoadResourcesScreen.h"
#include "MainMenu.h"
#include "main.h"

int main(void)
{
	gfx_Begin();
	
	appvar = ti_Open("MC2DDAT", "r");
	logo = ti_GetDataPtr(appvar);

	ti_CloseAll();
	// find a better way than using the rtc for things like block timings, 
	// time between up presses to toggle fly, etc. May need to use this, but more 
	// research may be needed in the near future.
	//timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_UP);
	gfx_SetClipRegion(-17, -17, 337, 257);
	gfx_SetDrawBuffer();
	gfx_SetTextFGColor(0);
	LoadResourcesScreen();
	MainMenu();
	// quit the game
	gfx_End();
	appvar = ti_Open("MCESETT", "w+");
	ti_Write(gameSettings, sizeof gameSettings, 1, appvar);
	ti_CloseAll();
}