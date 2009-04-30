/**
 * vim: ts=4 sw=4 expandtab:
 */


/**
 * @file
 *
 * Lancement de l'application wiidrums
 * 
 * @author David Bonnet & Arthur Gautier pour Polytech'Wiidrums Team
 * @date 19/04/2009
 * @since 19/04/2009
 * @version 0.0
 */

#include <QApplication>
#include <iostream>
#include "gui/simplegui.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    app.setApplicationName("wiidrums");
    app.setQuitOnLastWindowClosed(true);

    cout << "WiiGuard  Copyright (C) 2009  David BONNET - Vincent GUYOMARD"
         << endl
         << "WiiDrums  Copyright (C) 2009  David BONNET - Arthur GAUTIER"
         << endl << endl
         << "This program comes with ABSOLUTELY NO WARRANTY." << endl
         << "This is free software, and you are welcome to redistribute it "
         << "under certain conditions."
         << endl << endl;

    SimpleGUI simpleGui;
    simpleGui.show();

    return app.exec();
}

