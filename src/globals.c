#include "globals.h"

// Sprites
gfx_sprite_t *logo;

// Game Data
int16_t WATERENTITY = 549;
int16_t LAVAENTITY = 556;
int16_t lightVal = 16;
int16_t MaxSprites[4] = {221, 226, 75, 8};
gfx_sprite_t *BlockTextures[221];
gfx_sprite_t *ItemTextures[226];
gfx_sprite_t *PlantTextures[75];
gfx_sprite_t *FoliageTextures[8];
int16_t rtc, timer;
int16_t gameSettingsMaxVals[4] = { 1, 4, 1, 1 };
// { language, speed, shadowing, debug stats }
int16_t gameSettings[4] = { 0, 2, 0, 0 };
char *gameSettingsStr[4];

// World Data
int16_t WorldData[144 * 144] = {0};
int16_t MaxX = 144, MaxY = 144;
int WorldDataTimer[32 * 32] = {0};
int BlockLightVals[32 * 32] = {0};
char world_file[9] = {0};
char *SpeedStr[5] = {0};
char WorldsList[20][9] = {{0}}; // Initializes all to zero
uint8_t foundCount = 0;
int16_t worldType = 0, worldSize;
int16_t genTrees = 1, genFlowers = 1, genVillages = 1, genCaves = 1;
bool loaded_world = false;
int16_t dayColors[5] = {191, 158, 125, 51, 9};

// Player and Inventory
int16_t Inventory[55] = {0};
int16_t armor[9] = {0};
int16_t crafting[18] = {0};
int16_t hotbar[10] = {0};
int16_t timeofday = 0;
int16_t gamemode = 0, health, hunger, cheats;
int16_t flymode = 0;
int16_t redraw = 1, playerX = 0, playerY = 0, WorldTimerPosX = 0, WorldTimerPosY = 0;
int16_t curPos, curX = 11, curY = 6;
int16_t hotbarSel = 0;
int16_t pos = 0, render = 0, x = 0, y = 0, drawX = 0, drawY = 0, counter = 0, fps = 0;
int16_t jump = 0, dayTimer = 0, shadowing = 1, dialog, dialogTimer;
int16_t damageAmount = 0, damageDealt = 0;

// Scroll
int16_t scroll, scrollX = 0, scrollY = 0, scrollYb, tab, i;
int16_t CursorY;
int16_t pixelAmount = 1;
int16_t gravityPixelAmount = 5;

// Strings / UI
char worldNameStr[50] = "My World";
char seedStr[50] = "Random Seed";
char *MenuElements[21];
char *NewWorldMenuElements[13];
char *About[12];
char dialogString[50];
char *SpeedStr[5];
char *languages[2] = { "English", "Dutch"};
char *SpeedStrEN[5] = {"Smoothest", "Smooth", "Normal", "Fast", "Fastest"};
char *SpeedStrNL[5] = {"Soepelste", "Soepel", "Normaal", "Snel", "Snelst"};
char *GameOptionStr[4];
char *GameOptionStrEN[4] = {"Language", "Screen Speed", "Block Shadowing", "Debug Stats"};
char *GameOptionStrNL[4] = {"Language","Schermsnelheid", "Blokschaduwen", "debug-statistieken"};
char *gamemodeStr[3];
char *gamemodeStrEN[3] = {"Creative", "Survival", "Adventure"};
char *gamemodeStrNL[3] = {"Creatief", "Overleving", "Avontuur"};
char *worldSizeStr[3];
char *worldSizeStrEN[3] = {"Small", "Medium", "Large"};
char *worldSizeStrNL[3] = {"Klein", "Gemiddeld", "Groot"};
char *worldTypesStr[3];
char *worldTypesStrEN[3] = {"Flat", "Floating", "Default"};
char *worldTypesStrNL[3] = {"Vlak", "Zwevend", "Standaard"};
char *togglesOnOff[2];
char *togglesOnOffEN[2] = {"Off", "On"};
char *togglesOnOffNL[2] = {"Uit", "Aan"};
char *MenuElementsEN[21] = {"Play", "Achievements", "Settings", "Quit", "Gameplay", "Linking", "About", "Back", "My Worlds", "Servers", "Friends", "Create New World", "No Worlds were found!", "Add Server", "Friends are not available", "Back to Game", "Options", "Quit to menu", "Respawn", "Generating World", "Building Terrain"};
char *MenuElementsNL[21] = {"Speel", "Prestaties", "Instellingen", "Stoppen", "Gameplay", "Linken", "Over", "Terug", "Mijn werelden", "Servers", "Vrienden", "Genereer Nieuwe Wereld", "Geen wereld gevonden!", "Voeg server toe", "Vrienden niet beschikbaar", "Terug naar spel", "Opties", "Verlaat naar menu", "Respawn", "Wereld generen", "Terrein bouwen"};
char *NewWorldMenuElementsEN[13] = {"Name:", "Seed:", "Gamemode:", "World Size:", "Cheats:", "World Type:", "More World Options", "Generate", "Generate Trees:", "Generate Flowers:", "Village Generation:", "Cave Generation:", "Done"};
char *NewWorldMenuElementsNL[13] = {"Naam:", "Seed:", "Spelmode:", "Wereld grootte:", "Cheats:", "Wereld type:", "Meer wereld opties", "Genereren", "Genereer bomen:", "Genereer bloemen:", "Dorp generatie:", "Grot generatie:", "Klaar"};
char *AboutEN[12] = {"About: v1.0.2a by TimmyCraft", "Thanks to:", "- MateoC for the CE C", "Toolchain", "- TIny_Hacker for textures.","- BeckAdamTheInventor for", "Compression routines", "- My girlfriend for ", "translating to Dutch.", "- All those who've given", "feedback on cemetech.net", "and my discord server."};
char *AboutNL[12] = {"Over: v1.0.2a Door TimmyCraft", "Met dank aan:", "- MateoC voor de CE C", "Toolchain", "- TIny_Hacker voor ", "- BeckAdamTheInventor voor", "Compression routines", "- Mijn vriendin voor ", "Vertalen naar Nederlands.", "- Feedback van iedereen", "op cemetech.net", "en mijn discord server."};


// Misc
ti_var_t appvar;
