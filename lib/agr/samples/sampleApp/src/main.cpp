#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "GestureRecogniser.h"
using namespace AGR;

#include "menu.h"


//*******************
// global variable

cGestureRecogniser	g_gestureRecogniser;
tMenuMap			g_menus;
std::string			g_currentMenu = "Main";
bool				g_finished = false;



/*! Main function
 */
int main()
{
	initMenu();
	
	while( !g_finished )
	{
		updateMenu();
	}

	return 0;
}


