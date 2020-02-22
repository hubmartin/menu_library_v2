
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


#include "m2.h"



int menu2(Menu *menu)
{
	int i = 0;

	unsigned char menuItem = 0;
	unsigned char lastMenuItem = 255;

	unsigned char cursorTopPos = 0;
	unsigned char menuTopPos = 0;

	unsigned long refreshTimer = 0;

	//
	// Get number of items in menu, search for the first NULL
	//
	MenuItem **iList = menu->items;
	int len = 0;
	for (; *iList != 0; ++iList)
	  len++;

	// Functional :)
	// menuItem, menuTopPos, cursorTopPos
	if(menu->selectedIndex != -1)
	{
	  // If item on the first screen
	  if(menu->selectedIndex < MENU_LINES)
	  {
		  menuItem = menu->selectedIndex;
		  cursorTopPos = menu->selectedIndex;
		  menuTopPos = 0;
	  } else {
		 // Item is on other screen
		  menuItem = menu->selectedIndex;
		  cursorTopPos = MENU_LINES - 1;
		  menuTopPos = menu->selectedIndex - cursorTopPos;
	  }
	}

	displayClear();

	// Main menu loop
	for(;;) {

		// If your system allows idle mode
		#ifdef SLEEP
			SysCtlSleep();
		#endif

		if(keyPress) {

		  //
		  // Down
		  //
		if(keyPress == BTN_DOWN)
		  {
			if(menuItem != len-1)
			{
				// move to next item
				menuItem++;

				if(cursorTopPos >= MENU_LINES-1 ||
						(cursorTopPos ==/**/ ((MENU_LINES/**/)/2) && ((len) - menuItem  ) > ((MENU_LINES-1/**/)/2)) )
				  menuTopPos++;
				else
				  cursorTopPos++;

			} else {
				// Last item in menu => go to first item
				menuItem = 0;
				cursorTopPos = 0;
				menuTopPos = 0;
			}
		  }

		  //
		  // Up
		  //
		 if(keyPress == BTN_UP)
		  {
			if(menuItem != 0)
			{
			menuItem--;

			if(cursorTopPos > 0 &&
					!((cursorTopPos == MENU_LINES/2) && (menuItem >= MENU_LINES/2)))
			  cursorTopPos--;
			else
			  menuTopPos--;
			  } else {
				// go to the last item in menu
				menuItem = len-1;

				if(len <= MENU_LINES)
				{
					menuTopPos = 0;
				} else {
					menuTopPos = menuItem;
				}
				if(menuTopPos > len - MENU_LINES && len >= MENU_LINES)
				{
					menuTopPos = len - MENU_LINES;
				}

				cursorTopPos = menuItem - menuTopPos;
			  }
		  }


		  //
		  // Enter
		  //
		 if(keyPress == BTN_ENTER || keyPress == BTN_RIGHT)
		  {
			keyPress = 0;
			menu->selectedIndex = menuItem;
			int flags = menu->items[menu->selectedIndex]->flags;

			// checkbox
			if(flags & MENU_ITEM_IS_CHECKBOX)
			{
				menu->items[menu->selectedIndex]->flags ^= MENU_ITEM_IS_CHECKED;
				// Force refersh
				lastMenuItem = -1;
			}

			// Item is submenu - parameter in callback
			if(flags & MENU_CALLBACK_IS_SUBMENU && menu->items[menu->selectedIndex]->callback)
			{
				menu2((Menu*)menu->items[menu->selectedIndex]->callback);
				// Force refersh
				lastMenuItem = -1;
			}

			// callback
			if(flags & MENU_CALLBACK_IS_FUNCTION && menu->items[menu->selectedIndex]->callback)
			{
				(*menu->items[menu->selectedIndex]->callback)(menu);
				// Force refersh
				lastMenuItem = -1;
			}

			// normal item, so exit
			if((menu->items[menu->selectedIndex]->callback == 0) && ((flags & MENU_ITEM_IS_CHECKBOX) == 0))
			{
				return menuItem;
			}

		  }


		  //
		  // Left - back
		  //
		  if(keyPress == BTN_LEFT)
		  {
			keyPress = 0;
			menu->selectedIndex = 0;
					return -1;
		  }

				keyPress = 0;

			} // if(keyPress)

		//
		// If menu item changed -> refresh screen
		//
		if(lastMenuItem != menuItem || (menu->refresh && MENU_MS_TICK > refreshTimer))
		{
			if(menu->refresh)
				refreshTimer = MENU_MS_TICK + menu->refresh;

		  displayClear();
		  displayString(menu->title[menuLanguage],0,0);

		  // Menu debug
		  //sprintf(buffer, "%d,%d,%d", menuItem, menuTopPos, cursorTopPos);
		  //displayString(buffer,0,0);

		  i = 0;
		  while((i + menuTopPos) < len &&   i < MENU_LINES)
		  {
			int index = menuTopPos + i;
			if(menuItem == index && MENU_LINES > 1)
				displayString(ARROW_SYMBOL, 0, ROW(i+1));

			int posx = ustrlen(menu->items[index]->text[menuLanguage]) + 3;

			if(MENU_LINES > 1)
				displayString(menu->items[index]->text[menuLanguage], COL(1+ARROW_GAP), ROW(i+1));
			else
				displayString(menu->items[index]->text[menuLanguage], COL(0), ROW(i+1));

				if((menu->items[index]->flags & MENU_PARAMETER_MASK) == MENU_PARAMETER_IS_NUMBER)
					lcdBufferNumber(*((int*)menu->items[index]->parameter), COL(posx), ROW(i+1));

				if((menu->items[index]->flags & MENU_PARAMETER_MASK) == MENU_PARAMETER_IS_STRING)
					lcdBufferString((char*)(menu->items[index]->parameter), COL(posx),ROW(i+1));

				if(menu->items[index]->flags & MENU_ITEM_IS_CHECKBOX) {
					if(menu->items[index]->flags & MENU_ITEM_IS_CHECKED)
						lcdBufferString("\x81", COL(1),ROW(i+1));
					else
						lcdBufferString("\x80", COL(1),ROW(i+1));
				}

			i++;
		  }

			#ifdef displayDraw
			  displayDraw();
			#endif

		  lastMenuItem = menuItem;
		}


	}

	return 0;

}
