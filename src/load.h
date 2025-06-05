#ifndef LOAD_H
#define LOAD_H

#include <stdbool.h>

/**
 * Load a world state from an AppVar with the given name.
 * Automatically decompresses world data into memory.
 *
 * @param name AppVar name (must match saved format)
 * @return true if loaded successfully, false otherwise
 */
bool loadWorldFromAppvar(const char *name);

#endif // LOAD_H