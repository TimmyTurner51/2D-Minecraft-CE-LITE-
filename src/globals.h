
#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include <stdbool.h>
#include <fileioc.h>
#include <graphx.h>
#include <tice.h>


extern char version[14];

extern ti_var_t appvar;
extern gfx_sprite_t *logo;
extern int16_t WATERENTITY;
extern int16_t LAVAENTITY;
extern int16_t shadowing;
extern int16_t lightVal;
extern int16_t MaxSprites[4];
extern gfx_sprite_t *BlockTextures[221];
extern gfx_sprite_t *ItemTextures[226];
extern gfx_sprite_t *PlantTextures[75];
extern gfx_sprite_t *FoliageTextures[8];
extern int16_t WorldData[144 * 144];
extern int16_t MaxX, MaxY;
extern int WorldDataTimer[32 * 32];
extern int BlockLightVals[32 * 32];
extern int16_t Inventory[55];
extern int16_t armor[9];
extern int16_t crafting[18];
extern int16_t hotbar[10];
extern int16_t worldType;
extern int16_t timeofday;
extern int16_t gamemode;
extern int16_t flymode;
extern int16_t redraw, playerX, playerY, WorldTimerPosX, WorldTimerPosY;
extern int16_t curPos, curX, curY;
extern int16_t hotbarSel;
extern int16_t pos, render, x, y, drawX, drawY, counter, fps;
extern int16_t jump, gravityPixelAmount, dayTimer;
extern int16_t scrollX, scrollY, error;
extern int16_t health, hunger, damageAmount, damageDealt;
extern int16_t dayColors[5];
extern int16_t dialog, dialogTimer;
extern char dialogString[50];
extern int16_t testVar, testVarB, listPos, timer, rtc;
extern int16_t CursorY, i, scroll, scrollYb, tab, pixelAmount;
extern int16_t worldSize, cheats;
extern int16_t genTrees, genFlowers, genVillages, genCaves;
extern int16_t gameSettings[4];
extern int16_t gameSettingsMaxVals[4];
extern char *gameSettingsStr[4];
extern char *languages[2];
extern char *gamemodeStr[3];
extern char *worldSizeStr[3];
extern char *worldTypesStr[3];
extern char *togglesOnOff[2];
extern char *SpeedStr[5];
extern char *GameOptionStr[4];
extern char *MenuElements[21];
extern char *NewWorldMenuElements[13];
extern char *About[12];
extern char *gamemodeStrEN[3];
extern char *worldSizeStrEN[3];
extern char *worldTypesStrEN[3];
extern char *togglesOnOffEN[2];
extern char *SpeedStrEN[5];
extern char *GameOptionStrEN[4];
extern char *MenuElementsEN[21];
extern char *NewWorldMenuElementsEN[13];
extern char *AboutEN[12];
extern char *gamemodeStrNL[3];
extern char *worldSizeStrNL[3];
extern char *worldTypesStrNL[3];
extern char *togglesOnOffNL[2];
extern char *SpeedStrNL[5];
extern char *GameOptionStrNL[4];
extern char *MenuElementsNL[21];
extern char *NewWorldMenuElementsNL[13];
extern char *AboutNL[12];
extern char world_file[9];
extern char worldNameStr[50];
extern char WorldsList[20][9]; // Assuming max 10 worlds (adjust if needed)
extern char seedStr[50];
extern bool loaded_world;
extern uint8_t foundCount;


#endif
