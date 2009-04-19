/*
    WiiGuard - Home guardian with Wiimotes
    Copyright (C) 2009  David BONNET - Vincent GUYOMARD

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file
 *
 * Lancement de l'application WiiGuard qui fait la sécurité
 * d'une salle à l'aide de wiimotes
 * 
 * @author David Bonnet pour Polytech'WiiGuard Team
 * @date 13/03/2009 14:30
 * @since 17/03/2009 22:51
 * @version 1.0
 */

#include <QApplication>
#include <iostream>
#include "gui/simple/SimpleGUI.h"

int main(int argc, char *argv[]){

	QApplication app(argc, argv);

	cout << "WiiGuard  Copyright (C) 2009  David BONNET - Vincent GUYOMARD\n\nThis program comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it under certain conditions." << endl << endl;

	SimpleGUI simpleGui;
	simpleGui.show();

	return app.exec();
}

