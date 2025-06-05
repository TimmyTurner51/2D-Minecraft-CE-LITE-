#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>

/**
 * Save the current world state (player data, world size, world blocks, etc.)
 * to an AppVar with the given name. Automatically compresses world data.
 *
 * @param name AppVar name (8-character limit, no extension)
 * @return true if saved successfully, false otherwise
 */
bool saveWorldToAppvar(const char *name);

#endif // SAVE_H