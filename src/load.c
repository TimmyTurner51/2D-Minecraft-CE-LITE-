#include <fileioc.h>
#include <string.h>
#include <graphx.h>
#include "render.h"
#include "globals.h"

#define MAGIC "2DMC"
#define VERSION 1

uint8_t *decompress_buf = (uint8_t*)0xD52C00;

bool loadWorldFromAppvar(const char *name) {
    gfx_SetDrawScreen();
    *((uint32_t*)0xE30010) = 0xD40000;
    DrawDirtBackground(0);
    gfx_SetTextFGColor(0xDF);
    gfx_PrintStringXY("Loading World...", 113, 110);

    // Draw progress bar shell
    gfx_SetColor(0x00); // outline
    gfx_Rectangle(90, 120, 140, 9);
    gfx_SetColor(0xB5); // background
    gfx_FillRectangle(91, 121, 138, 7);
    gfx_BlitBuffer();

    ti_var_t var = ti_Open(name, "r");
    if (!var) return false;

    // Validate header
    char magic[4];
    ti_Read(magic, 4, 1, var);
    if (memcmp(magic, MAGIC, 4) != 0) return false;

    uint8_t version;
    ti_Read(&version, 1, 1, var);
    if (version != VERSION) return false;

    // Load world metadata
    ti_Read(&MaxX, sizeof(MaxX), 1, var);
    ti_Read(&MaxY, sizeof(MaxY), 1, var);
    ti_Read(&playerX, sizeof(playerX), 1, var);
    ti_Read(&playerY, sizeof(playerY), 1, var);
    ti_Read(&scrollX, sizeof(scrollX), 1, var);
    ti_Read(&scrollY, sizeof(scrollY), 1, var);
    ti_Read(&timeofday, sizeof(timeofday), 1, var);
    ti_Read(&hotbarSel, sizeof(hotbarSel), 1, var);
    ti_Read(hotbar, sizeof(hotbar), 1, var);

    // Load compressed size and data
    int compressed_size;
    ti_Read(&compressed_size, sizeof(int), 1, var);
    ti_Read(decompress_buf, compressed_size, 1, var);

    // Decompress with progress bar
    const uint8_t *src = decompress_buf;
    uint8_t *dst = WorldData;
    int expected_len = MaxX * MaxY;
    int in = 0, out = 0, last_x = 91;

    while (in < compressed_size) {
        uint8_t flag = src[in++];
        if (flag == 0xFF) {
            uint8_t val = src[in++];
            uint8_t count = src[in++];
            memset(dst + out, val, count);
            out += count;
        } else {
            dst[out++] = src[in++];
        }

        // Progress bar update
        int new_x = ((138 * out) / expected_len) + 91;
        if (new_x > last_x) {
            gfx_SetColor(0x05);
            gfx_FillRectangle(last_x, 121, new_x - last_x, 7);
            gfx_BlitBuffer();
            last_x = new_x;
        }
    }

    ti_Close(var);
    return true;
}