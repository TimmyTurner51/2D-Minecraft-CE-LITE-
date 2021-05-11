
#define STONE 1
#define GRASS 2
#define DIRT 3
#define COBBLESTONE 4
#define OAKPLANK 5
#define OAKSAPLING 6
#define BEDROCK 7
#define WATER 8
#define NOTHING1 9
#define LAVA 10
#define NOTHING2 11
#define SAND 12
#define GRAVEL 13
#define GOLDORE 14
#define IRONORE 15
#define COALORE 16
#define OAKLOGS 17
#define OAKLEAVES 18
#define SPONGE 19
#define GLASS 20
#define LAPIZORE 21
#define LAPIZBLOCK 22
#define DISPENSER 23
#define SANDSTONE 24
#define NOTEBLOCK 25
#define BEDBACK 26
#define BEDFRONT 27
#define REDSTONEDUSTOFF 28
#define REDSTONEDUSTON 29
#define REGULARPISTONRIGHTOFF 30
#define COBWEB 31

#define WATERENTITY 32
#define LAVAENTITY 33

int natureBlocks[13] = {GRASS, DIRT, STONE, COBBLESTONE, SAND, GRAVEL, OAKLOGS, OAKLEAVES, BEDROCK, COALORE, IRONORE, GOLDORE, LAPIZORE};
int buildingBlocks[3] = {OAKPLANK, GLASS, SPONGE};
int redstoning[3] = {REDSTONEDUSTOFF, NOTEBLOCK, REGULARPISTONRIGHTOFF};
int toolsEtc[3] = { BEDBACK, WATER, LAVA };

int typesvalues[4] = {13, 3, 3, 3};


// Best not to change these...
gfx_sprite_t *sprites[254];
gfx_sprite_t dummy_sprite = {1, 1, 0};

gfx_TempSprite(logo, 16, 16);

char WorldData[ 200 * 200 ] = { 0 };
int24_t WorldDataTimer[ 80 * 80 ] = { 0 };
int24_t redraw = 1, playerX = 0, playerY = 0, curPos = (200 * 2) + 6, curX = 96, curY = 32;
int24_t blockSel = 0, hotbarSel = 0, timeofday = 0;
int24_t pos = 0, render = 0, x = 0, y = 0, drawX = 0, drawY = 0, count = 1;
int24_t scrollX = 0, scrollY = 0, error = 0;
int24_t hotbar[5] = {0}, dayColors[5] = {191, 158, 125, 51, 9};
