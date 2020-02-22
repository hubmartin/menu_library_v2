/*
 *
 *
 * Menu2 v2.0
 * Martin Hubacek
 * 18.3.2013
 * http://martinhubacek.cz
 *
 *
 */


#ifndef __MENU2__
#define __MENU2__


#include "m2Platform.h"


// 0-3 bits
#define MENU_PARAMETER_MASK 0x0F
#define MENU_PARAMETER_IS_NUMBER 1
#define MENU_PARAMETER_IS_STRING 2

// 4. bit checkbox bit
#define MENU_ITEM_IS_CHECKBOX	0x10
// 5bit
#define MENU_ITEM_IS_CHECKED	0x20

// 6.bit - submenu bit
#define MENU_CALLBACK_IS_SUBMENU	0x40

// 7bit - callback bit
#define MENU_CALLBACK_IS_FUNCTION 0x80



typedef struct SMenuItem {
	char* text[MENU_LANGUAGES];
	void (*callback)(void *);
	int flags;
	int parameter;
} MenuItem;


typedef struct SMenu {
	char* title[MENU_LANGUAGES];
	int selectedIndex;
	int refresh;
	MenuItem *items[];
} Menu;



int menu2(Menu *menu);

#endif
