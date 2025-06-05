#ifndef GAME_H
#define GAME_H
#include <fileioc.h>
void Game(void);
bool pauseMenu(void);
void compressAndWrite(void *data, int len, ti_var_t fp);
void findAppvars(const char *str);

#endif