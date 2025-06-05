#include <stdint.h>
#include "crafting.h"
#include "globals.h" 

// These arrays are used and must be declared elsewhere:
extern int16_t hotbar[10];
extern int16_t Inventory[55];

int craftingCheck(int16_t inputVal) {
	int16_t item = 0;

	return item;
}

void giveItem(int16_t blockID, int16_t breaking) {
	int16_t pos, movetoinventory = 0;

	if (breaking == 1) {
		// add code for breaking blocks based on their ID.
	}

	// first scan hotbar for the blockID
	for (pos = 0; pos < 5; pos++) {
		if (hotbar[pos] == 0) {
			hotbar[pos] = blockID;
			hotbar[pos + 5]++;
			break;
		}else{
			if (hotbar[pos] == blockID && hotbar[pos + 5] < 64) {
				hotbar[pos + 5]++;
				break;
			}
			if (hotbar[pos] == blockID && hotbar[pos + 5] == 64) {
				movetoinventory = 1;
				break;
			}
		}
	}

	if (movetoinventory == 1) {
		// the hotbar is full, so find an empty space to put the item
		for (pos = 0; pos < 26; pos++) {
			if (Inventory[pos] == 0) {
				Inventory[pos] = blockID;
				Inventory[pos + 27]++;
				movetoinventory = 2;
				break;
			}else{
				if (Inventory[pos] == blockID && Inventory[pos + 27] < 64) {
					Inventory[pos + 27]++;
					movetoinventory = 2;
					break;
				}
			}
		}
	}

	// Check if movetoinventory is still 1.
	// If so, there's not enough room anywhere in player storage for the item

}