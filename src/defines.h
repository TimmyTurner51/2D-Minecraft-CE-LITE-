
// only need to define blocks that require gameplay mechanics
// (gravity, etc., like water and lava)

// Values for non-permanent blocks, WATER and LAVA entities include the max horizontal flow of 7 blocks.
int16_t WATERENTITY = 549;
int16_t LAVAENTITY = 549 + 7;

void MainMenu(void);
void playMenu(void);
void NewWorldScreen(void);
void MoreWorldOptions(void);
void inGameOptions(void);
void Achievements(void);
void Settings(void);
void findAppvars(const char *str);

// blocks, items, plants, foliage 
int16_t MaxSprites[4] = { 221, 226, 75, 8 };

gfx_sprite_t *BlockTextures[221];
gfx_sprite_t *ItemTextures[226];
gfx_sprite_t *PlantTextures[75];
gfx_sprite_t *FoliageTextures[8];


gfx_TempSprite(logo, 16, 16);
//gfx_rletsprite_t *block;
//gfx_UninitedRLETSprite(block, 16 * 16);

int16_t WorldData[ 160 * 150 ] = { 0 };
int16_t MaxX = 160, MaxY = 150;

// only used by the behaviors and/or world rendering
int16_t WorldDataTimer[ 80 * 80 ] = { 0 };

// personal inventory
// Block ID's first (0 - 26), then the amounts for each (27 - 54), or health if it's armor, etc.
int16_t Inventory[55] = { 0 };
// armor slots ( 0 - 3 = armors, 4 - 8 = armors health)
int16_t armor[9] = { 0 };
// crafting table slots (0 - 9 for the item IDs in the slots, 10 - 18 for the amount of items in that slot)
int16_t crafting[18] = { 0 };
// hotbar, same as usual, IDs for the first 5 slots, then their amounts (amounts only count in non-creative ofc)
int16_t hotbar[10] = { 0 };

int16_t redraw = 1, playerX = 0, playerY = 0, WorldTimerPosX = 0, WorldTimerPosY = 0;
int16_t curPos, curX = 11, curY = 6;
int16_t hotbarSel = 0;
int16_t pos = 0, render = 0, x = 0, y = 0, drawX = 0, drawY = 0, count = 1;
int16_t jump = 0, dayTimer = 0;
int16_t scrollX = 0, scrollY = 0, error = 0;
int16_t health = 0, hunger = 0, damageAmount = 0, damageDealt = 0;
int16_t dayColors[5] = {191, 158, 125, 51, 9};
int16_t dialog = 0, dialogTimer = 0;
char dialogString[50] = "";
int16_t keyPresses[8] = { 0 }, testVar, testVarB, listPos, timer;
int16_t CursorY, x, i, scroll, scrollY, scrollYb, redraw, tab;

int16_t worldSize, cheats = 0;
// generator settings
int16_t genTrees, genFlowers, genVillages, genCaves;
// game settings
// { language, speed, shadowing, debug stats }
int16_t gameSettings[4] = { 0, 2, 1, 0 };
// game settings max values
int16_t gameSettingsMaxVals[4] = { 1, 4, 1, 1 };


char *gameSettingsStr[4];

char *languages[2] = { "English", "Dutch"};
char *gamemodeStr[3];
char *worldSizeStr[3];
char *worldTypesStr[3];
char *togglesOnOff[2];
char *SpeedStr[5];
char *GameOptionStr[4];
char *MenuElements[21];
char *NewWorldMenuElements[13];
char *About[12];

char *gamemodeStrEN[3] = {"Survival", "Creative", "Adventure"};
char *worldSizeStrEN[3] = {"Small", "Medium", "Large"};
char *worldTypesStrEN[3] = {"Standard", "Superflat", "Large Biomes"};
char *togglesOnOffEN[2] = {"Off", "On"};
char *SpeedStrEN[5] = {"Smoothest", "Smooth", "Normal", "Fast", "Fastest"};
char *GameOptionStrEN[4] = {"Language", "Screen Speed", "Block Shadowing", "Debug Stats"};
char *MenuElementsEN[21] = {"Play", "Achievements", "Settings", "Quit", "Gameplay", "Linking", "About", "Back", "My Worlds", "Servers", "Friends", "Create New World", "No Worlds were found!", "Add Server", "Friends are not available", "Back to Game", "Options", "Quit to menu", "Respawn", "Generating World", "Building Terrain"};
char *NewWorldMenuElementsEN[13] = {"Name:", "Seed:", "Gamemode:", "World Size:", "Cheats:", "World Type:", "More World Options", "Generate", "Generate Trees:", "Generate Flowers:", "Village Generation:", "Cave Generation:", "Done"};
char *AboutEN[12] = {"About: v1.0.2a by TimmyCraft", "Thanks to:", "- MateoC for the CE C", "Toolchain", "- TIny_Hacker for textures.","- BeckAdamTheInventor for", "Compression routines", "- My girlfriend for ", "translating to Dutch.", "- All those who've given", "feedback on cemetech.net", "and my discord server."};

char *gamemodeStrNL[3] = {"Overleving", "Creatief", "Avontuur"};
char *worldSizeStrNL[3] = {"Klein", "Gemiddeld", "Groot"};
char *worldTypesStrNL[3] = {"Standaard", "Superplat", "Grote Biomen"};
char *togglesOnOffNL[2] = {"Uit", "Aan"};
char *SpeedStrNL[5] = {"Soepelste", "Soepel", "Normaal", "Snel", "Snelst"};
char *GameOptionStrNL[4] = {"Language","Schermsnelheid", "Blokschaduwen", "debug-statistieken"};
char *MenuElementsNL[21] = {"Speel", "Prestaties", "Instellingen", "Stoppen", "Gameplay", "Linken", "Over", "Terug", "Mijn werelden", "Servers", "Vrienden", "Genereer Nieuwe Wereld", "Geen wereld gevonden!", "Voeg server toe", "Vrienden niet beschikbaar", "Terug naar spel", "Opties", "Verlaat naar menu", "Respawn", "Wereld generen", "Terrein bouwen"};
char *NewWorldMenuElementsNL[13] = {"Naam:", "Seed:", "Spelmode:", "Wereld grootte:", "Cheats:", "Wereld type:", "Meer wereld opties", "Genereren", "Genereer bomen:", "Genereer bloemen:", "Dorp generatie:", "Grot generatie:", "Klaar"};
char *AboutNL[12] = {"Over: v1.0.2a Door TimmyCraft", "Met dank aan:", "- MateoC voor de CE C", "Toolchain", "- TIny_Hacker voor ", "- BeckAdamTheInventor voor", "Compression routines", "- Mijn vriendin voor ", "Vertalen naar Nederlands.", "- Feedback van iedereen", "op cemetech.net", "en mijn discord server."};
// 1 = on/off toggle, 2 = speedstr toggle
int16_t GameOptionTypes[2] = { 1, 2 };

// can be changed with cheats in-game
// 0 = survival, 1 = creative, 2 = adventure, 3 = spectator
int16_t gamemode = 0;
// 0 = peaceful, 1 = easy, 2 = normal, 3 = hard
int16_t difficulty = 0;
// 0 = off, 1 = on
int16_t flymode = 0;
// 0 = no shadowing, 1 = shadowing on
int16_t shadowing = 1;
// 0 = clear, 1 = rain only, 2 = thunderstorm
int16_t weather = 0;
// 0 = sunrise, 1 = midday, 2 = day, 3 = sunset, 4 = night
int16_t timeofday = 0;

char worldNameStr[50] = "My World";
char seedStr[50] = "Random Seed";
char* world_file = "MCCESAVE";
//{ name of world 1, name of world 2, etc. }
char WorldsList[20][9];
//{ name of server 1, name of server 2, etc. }
char Servers[20][20];
char FriendsList[20][20];

int16_t worldType = 0, foundCount = 0;
bool loaded_world = 0;
ti_var_t appvar;
void compressAndWrite(void *data, int len, ti_var_t fp); //this routine compresses using zx7_Compression