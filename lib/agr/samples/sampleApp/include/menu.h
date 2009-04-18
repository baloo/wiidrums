/**
 * \file menu.h
 * \brief Declaration for sample application menus
 * \date 07/03/08
 */


#ifndef _MENU_H_
#define _MENU_H_

// Function pointer used for menus items
typedef void (*ptMenuFunction)();

// Menu item
struct tMenuItem
{
	std::string		displayMessage;
	char			selection;
	ptMenuFunction	action;

	tMenuItem( std::string _displayMessage, char _selection, ptMenuFunction _action ):
		displayMessage(_displayMessage), selection(_selection), action(_action)	{}

};

// Menu object
typedef std::vector<tMenuItem> tMenu;

// Collection of menus
typedef std::map<std::string,tMenu> tMenuMap;

void initMenu();
void updateMenu();


#endif // _MENU_H_
