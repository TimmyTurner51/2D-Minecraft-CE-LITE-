
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

#include "gfx/gfx.h"
#include "compressor.h"
#include "main.h"
// only need to define blocks that require gameplay mechanics
// (gravity, etc., like water and lava)

// Values for non-permanent blocks, WATER and LAVA entities include the max horizontal flow of 7 blocks.
static int16_t WATERENTITY = 549;
static int16_t LAVAENTITY = 556;
static int16_t lightVal = 16;       // 7

void MainMenu(void);
void playMenu(void);
void NewWorldScreen(void);
void MoreWorldOptions(void);
void Achievements(void);
void Settings(bool ingameTrue);
void findAppvars(const char *str);

// blocks, items, plants, foliage 
static int16_t MaxSprites[4] = { 221, 226, 75, 8 };

static gfx_sprite_t *BlockTextures[221];
static gfx_sprite_t *ItemTextures[226];
static gfx_sprite_t *PlantTextures[75];
static gfx_sprite_t *FoliageTextures[8];
 

static gfx_sprite_t *logo;
// static gfx_TempSprite(logo, 16, 16);

//gfx_rletsprite_t *block;
//gfx_UninitedRLETSprite(block, 16 * 16);

static int16_t WorldData[ 144 * 144 ] = { 0 };
static int16_t MaxX = 144, MaxY = 144;

// only used by the behaviors and/or world rendering
static int WorldDataTimer[ 32 * 32 ] = { 0 };
static int BlockLightVals[ 32 * 32 ] = { 0 };

// personal inventory
// Block ID's first (0 - 26), then the amounts for each (27 - 54), or health if it's armor, etc.
static int16_t Inventory[55] = { 0 };
// armor slots ( 0 - 3 = armors, 4 - 8 = armors health)
static int16_t armor[9] = { 0 };
// crafting table slots (0 - 9 for the item IDs in the slots, 10 - 18 for the amount of items in that slot)
static int16_t crafting[18] = { 0 };
// hotbar, same as usual, IDs for the first 5 slots, then their amounts (amounts only count in non-creative ofc)
static int16_t hotbar[10] = { 0 };

// pixels
static int16_t pixelAmount = 1, gravityPixelAmount = 2;

static int16_t redraw = 1, playerX = 0, playerY = 0, WorldTimerPosX = 0, WorldTimerPosY = 0;
static int16_t curPos, curX = 11, curY = 6;
static int16_t hotbarSel = 0;
static int16_t pos = 0, render = 0, x = 0, y = 0, drawX = 0, drawY = 0, counter = 0, fps = 0;
static int16_t jump = 0, dayTimer = 0;
static int16_t scrollX = 0, scrollY = 0, error = 0;
static int16_t health = 0, hunger = 0, damageAmount = 0, damageDealt = 0;
static int16_t dayColors[5] = {191, 158, 125, 51, 9};
static int16_t dialog = 0, dialogTimer = 0;
static char dialogString[50] = "";
static int16_t testVar, testVarB, listPos, timer, rtc;
static int16_t CursorY, x, i, scroll, scrollY, scrollYb, redraw, tab;

static int16_t worldSize, cheats = 0;
// generator settings
static int16_t genTrees, genFlowers, genVillages, genCaves;
// game settings
// { language, speed, shadowing, debug stats }
static int16_t gameSettings[4] = { 0, 2, 0, 0 };
// game settings max values
static int16_t gameSettingsMaxVals[4] = { 1, 4, 1, 1 };


static char *gameSettingsStr[4];

static char *languages[2] = { "English", "Dutch"};
static char *gamemodeStr[3];
static char *worldSizeStr[3];
static char *worldTypesStr[3];
static char *togglesOnOff[2];
static char *SpeedStr[5];
static char *GameOptionStr[4];
static char *MenuElements[21];
static char *NewWorldMenuElements[13];
static char *About[12];

static char *gamemodeStrEN[3] = {"Survival", "Creative", "Adventure"};
static char *worldSizeStrEN[3] = {"Small", "Medium", "Large"};
static char *worldTypesStrEN[3] = {"Standard", "Superflat", "Large Biomes"};
static char *togglesOnOffEN[2] = {"Off", "On"};
static char *SpeedStrEN[5] = {"Smoothest", "Smooth", "Normal", "Fast", "Fastest"};
static char *GameOptionStrEN[4] = {"Language", "Screen Speed", "Block Shadowing", "Debug Stats"};
static char *MenuElementsEN[21] = {"Play", "Achievements", "Settings", "Quit", "Gameplay", "Linking", "About", "Back", "My Worlds", "Servers", "Friends", "Create New World", "No Worlds were found!", "Add Server", "Friends are not available", "Back to Game", "Options", "Quit to menu", "Respawn", "Generating World", "Building Terrain"};
static char *NewWorldMenuElementsEN[13] = {"Name:", "Seed:", "Gamemode:", "World Size:", "Cheats:", "World Type:", "More World Options", "Generate", "Generate Trees:", "Generate Flowers:", "Village Generation:", "Cave Generation:", "Done"};
static char *AboutEN[12] = {"About: v1.0.2a by TimmyCraft", "Thanks to:", "- MateoC for the CE C", "Toolchain", "- TIny_Hacker for textures.","- BeckAdamTheInventor for", "Compression routines", "- My girlfriend for ", "translating to Dutch.", "- All those who've given", "feedback on cemetech.net", "and my discord server."};

static char *gamemodeStrNL[3] = {"Overleving", "Creatief", "Avontuur"};
static char *worldSizeStrNL[3] = {"Klein", "Gemiddeld", "Groot"};
static char *worldTypesStrNL[3] = {"Standaard", "Superplat", "Grote Biomen"};
static char *togglesOnOffNL[2] = {"Uit", "Aan"};
static char *SpeedStrNL[5] = {"Soepelste", "Soepel", "Normaal", "Snel", "Snelst"};
static char *GameOptionStrNL[4] = {"Language","Schermsnelheid", "Blokschaduwen", "debug-statistieken"};
static char *MenuElementsNL[21] = {"Speel", "Prestaties", "Instellingen", "Stoppen", "Gameplay", "Linken", "Over", "Terug", "Mijn werelden", "Servers", "Vrienden", "Genereer Nieuwe Wereld", "Geen wereld gevonden!", "Voeg server toe", "Vrienden niet beschikbaar", "Terug naar spel", "Opties", "Verlaat naar menu", "Respawn", "Wereld generen", "Terrein bouwen"};
static char *NewWorldMenuElementsNL[13] = {"Naam:", "Seed:", "Spelmode:", "Wereld grootte:", "Cheats:", "Wereld type:", "Meer wereld opties", "Genereren", "Genereer bomen:", "Genereer bloemen:", "Dorp generatie:", "Grot generatie:", "Klaar"};
static char *AboutNL[12] = {"Over: v1.0.2a Door TimmyCraft", "Met dank aan:", "- MateoC voor de CE C", "Toolchain", "- TIny_Hacker voor ", "- BeckAdamTheInventor voor", "Compression routines", "- Mijn vriendin voor ", "Vertalen naar Nederlands.", "- Feedback van iedereen", "op cemetech.net", "en mijn discord server."};
// 1 = on/off toggle, 2 = speedstr toggle
static int16_t GameOptionTypes[2] = { 1, 2 };
 
// can be changed with cheats in-game
// 0 = survival, 1 = creative, 2 = adventure, 3 = spectator
static int16_t gamemode = 0;
// 0 = peaceful, 1 = easy, 2 = normal, 3 = hard
static int16_t difficulty = 0;
// 0 = off, 1 = on
static int16_t flymode = 0;
// 0 = no shadowing, 1 = shadowing on
static int16_t shadowing = 1;
// 0 = clear, 1 = rain only, 2 = thunderstorm
static int16_t weather = 0;
// 0 = sunrise, 1 = midday, 2 = day, 3 = sunset, 4 = night
static int16_t timeofday = 0;

static char worldNameStr[50] = "My World";
static char seedStr[50] = "Random Seed";
static char* world_file = "MCCESAVE";
//{ name of world 1, name of world 2, etc. }
static char WorldsList[20][9];
//{ name of server 1, name of server 2, etc. }
static char Servers[20][20];
static char FriendsList[20][20];
static int16_t worldType = 0, foundCount = 0;
static bool loaded_world = 0;
static ti_var_t appvar;
// static void compressAndWrite(void *data, int len, ti_var_t fp); //this routine compresses using zx7_Compression