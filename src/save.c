#include <fileioc.h>
#include <string.h>
#include <graphx.h>
#include "globals.h"
#include "render.h"
#include "compressor.h"

#define MAGIC "MCCESV"
#define VERSION 1

extern int rle_compress(uint8_t *dst, const uint8_t *src, int len);

bool saveWorldToAppvar(const char *name) {
    int len = MaxX * MaxY;
    uint8_t *compress_buf = malloc(len * 2); // safe inside function

    if (!compress_buf) return false;

    gfx_SetDrawBuffer();
    DrawDirtBackground(0);
    gfx_SetTextFGColor(0xDF);
    gfx_PrintStringXY("Saving World...", 113, 110);
    gfx_SetColor(0x00); gfx_Rectangle(90, 120, 140, 9);
    gfx_SetColor(0xB5); gfx_FillRectangle(91, 121, 138, 7);
    gfx_BlitBuffer();

    ti_var_t var = ti_Open(name, "w");
    if (!var) {
        free(compress_buf);
        return false;
    }

    ti_Write("2DMC", 4, 1, var);
    uint8_t version = 1;
    ti_Write(&version, 1, 1, var);
    ti_Write(&MaxX, sizeof(MaxX), 1, var);
    ti_Write(&MaxY, sizeof(MaxY), 1, var);
    ti_Write(&playerX, sizeof(playerX), 1, var);
    ti_Write(&playerY, sizeof(playerY), 1, var);
    ti_Write(&scrollX, sizeof(scrollX), 1, var);
    ti_Write(&scrollY, sizeof(scrollY), 1, var);
    ti_Write(&timeofday, sizeof(timeofday), 1, var);
    ti_Write(&hotbarSel, sizeof(hotbarSel), 1, var);
    ti_Write(hotbar, sizeof(hotbar), 1, var);

    int out = 0, i = 0, last_x = 91;
    uint8_t *dst = compress_buf;
    const uint8_t *src = (const uint8_t*)WorldData; // safe if block IDs < 256

    while (i < len) {
        int run_len = 1;
        while (i + run_len < len && src[i] == src[i + run_len] && run_len < 255)
            run_len++;

        if (run_len >= 4) {
            dst[out++] = 0xFF;
            dst[out++] = src[i];
            dst[out++] = run_len;
            i += run_len;
        } else {
            dst[out++] = 0x00;
            dst[out++] = src[i++];
        }

        int new_x = ((138 * i) / len) + 91;
        if (new_x > last_x) {
            gfx_SetColor(0x05);
            gfx_FillRectangle(last_x, 121, new_x - last_x, 7);
            gfx_BlitBuffer();
            last_x = new_x;
        }
    }

    ti_Write(&out, sizeof(int), 1, var);
    ti_Write(dst, out, 1, var);
    ti_SetArchiveStatus(1, var);
    ti_Close(var);
    free(compress_buf);
    return true;
}