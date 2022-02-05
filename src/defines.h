
// only need to define blocks that require gameplay mechanics
// (gravity, etc., like water and lava)

// Values for non-permanent blocks, WATER and LAVA entities include the max horizontal flow of 7 blocks.
int16_t WATERENTITY = 549;
int16_t LAVAENTITY = 549 + 7;


// blocks, items, plants, foliage 
int16_t MaxSprites[4] = { 221, 226, 75, 8 };

// Best not to change these...
// sprites array dimensions are the maximum possible blocks that can be drawn on the screen
// plus 1 for non-world rendering (e.g, blocks in hotbar)
gfx_sprite_t *sprites[20 * 15 + 1];

gfx_TempSprite(logo, 16, 16);
//gfx_rletsprite_t *block;
gfx_UninitedRLETSprite(block, 16 * 16);

int16_t WorldData[ 160 * 150 ] = { 0 };
int16_t WorldDataSprites[20 * 15] = { 0 };
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
int16_t testX = 0, testY = 0, jump = 0, dayTimer = 0;
int16_t scrollX = 0, scrollY = 0, error = 0;
int16_t health = 0, hunger = 0, damageAmount = 0, damageDealt = 0;
int16_t dayColors[5] = {191, 158, 125, 51, 9};
int16_t dialog = 0, dialogTimer = 0;
char dialogString[50] = "";
int16_t keyPresses[8] = { 0 }, testVar, testVarB, listPos, timer;

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


// copied from previous 2D MC CE code

char worldNameStr[50] = "My World";
char seedStr[50] = "Random Seed";
char* world_file = "MCCESAVE";
//{ name of world 1, name of world 2, etc. }
char WorldsList[20][9];
//{ name of server 1, name of server 2, etc. }
char Servers[20][20];
char FriendsList[20][20];
void MainMenu(void);
void playMenu(void);
void Achievements(void);
void Settings(void);
void findAppvars(const char *str);
int16_t worldType = 0, foundCount = 0;
bool loaded_world = 0;
ti_var_t appvar;
void compressAndWrite(void *data, int len, ti_var_t fp); //this routine compresses using zx7_Compression