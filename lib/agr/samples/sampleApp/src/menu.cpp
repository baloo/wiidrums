#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "GestureRecogniser.h"
using namespace AGR;

#include "menu.h"

//*******************
// external variables

extern cGestureRecogniser	g_gestureRecogniser;
extern tMenuMap				g_menus;
extern std::string			g_currentMenu;
extern bool					g_finished;


//*******************
// static functions declaration

static void displayMenu();
static bool loadMovementFile( std::string fileName, cMovement** ptMovArray, unsigned int* ptSizeArray );


//*******************
// Menu functions declaration

static void menuQuit();
static void menuNotImplementedYet();
static void menuSetMainMenu();
static void menuSetLoadMenu();
static void menuSetTestMenu();
static void menuSetMovementMenu();
static void menuLoadGRFile();
static void menuLoadMovementGR();
static void menuTestMovementFile();



//*******************
// Function implementation


/*! Update the menu system
 */
void updateMenu()
{
	displayMenu();

	tMenu* pMenu = &g_menus[ g_currentMenu ];
	char choice;
	std::cout << std::endl << std::endl << std::endl  << std::endl << "> ";
	std::cin >> choice;

	
	std::cout << std::endl << std::endl << std::endl << std::endl;
	bool found = false;
	for( tMenu::iterator iter = pMenu->begin(); iter != pMenu->end(); iter++ )
	{
		if( iter->selection == choice )
		{
			if( iter->action )
			{
				found = true;
				iter->action();
				break;
			}
		}
	}
	if( !found )
	{
		std::cout << "Command \"" << choice << "\" not found" << std::endl;
	}

}


/*! Display the current menu
 */
void displayMenu()
{
	tMenu* pMenu = &g_menus[ g_currentMenu ];

	for( tMenu::iterator iter = pMenu->begin(); iter != pMenu->end(); iter++ )
	{
		std::cout << iter->displayMessage << std::endl;
	}

	std::cout << std::endl;
	if( g_gestureRecogniser.isReady() )
	{
		std::cout << "A Gesture Recogniser is loaded and ready to use" << std::endl;
	}
	else
	{
		std::cout << "No Gesture Recogniser currently ready" << std::endl;
	}
}


/*! Init the menu
 */
void initMenu()
{
	tMenu menu;

	// Main menu
	menu.push_back( tMenuItem( "**************************************************************", 0, NULL ) );
	menu.push_back( tMenuItem( "* Accelerometer Gesture Recognition (AGR) Sample Application *", 0, NULL ) );
	menu.push_back( tMenuItem( "**************************************************************", 0, NULL ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "1 Load a Gesture Recogniser", '1', menuSetLoadMenu ) );
	menu.push_back( tMenuItem( "2 Test the Gesture Recogniser", '2', menuSetTestMenu ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "0 Quit", '0', menuQuit ) );
	g_menus[ "Main" ] = menu ;
	
	// Load menu
	menu.clear();
	menu.push_back( tMenuItem( "***********************************", 0, NULL ) );
	menu.push_back( tMenuItem( "**** Load a Gesture Recogniser ****", 0, NULL ) );
	menu.push_back( tMenuItem( "***********************************", 0, NULL ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "1 Load from a file", '1', menuLoadGRFile ) );
	menu.push_back( tMenuItem( "2 Train a new Gesture Recogniser", '2', menuSetMovementMenu ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "0 Back to main menu", '0', menuSetMainMenu ) );
	g_menus[ "Load" ] = menu ;
	
	// Train new gesture
	menu.clear();
	menu.push_back( tMenuItem( "**************************************", 0, NULL ) );
	menu.push_back( tMenuItem( "**** Create a Gesture Recogniser ****", 0, NULL ) );
	menu.push_back( tMenuItem( "*************************************", 0, NULL ) );
	menu.push_back( tMenuItem( "The gesture recogniser will be created using a set of movement", 0, NULL ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "1 Load a movement file", '1', menuLoadMovementGR ) );
	menu.push_back( tMenuItem( "2 Create movement using the wiimote", '2', menuNotImplementedYet ) );
	menu.push_back( tMenuItem( "3 Create movement using the mouse (2D)", '3', menuNotImplementedYet ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "0 Back to main menu", '0', menuSetMainMenu ) );
	g_menus[ "Movement" ] = menu ;

	// Test gesture
	menu.clear();
	menu.push_back( tMenuItem( "*************************************", 0, NULL ) );
	menu.push_back( tMenuItem( "**** Test the Gesture Recogniser ****", 0, NULL ) );
	menu.push_back( tMenuItem( "*************************************", 0, NULL ) );
	menu.push_back( tMenuItem( "The gesture recognition test need some movement to test", 0, NULL ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "1 Test a movement file", '1', menuTestMovementFile ) );
	menu.push_back( tMenuItem( "2 test using the wiimote", '2', menuNotImplementedYet ) );
	menu.push_back( tMenuItem( "3 Test using the mouse (2D)", '3', menuNotImplementedYet ) );
	menu.push_back( tMenuItem( "", 0, NULL ) );
	menu.push_back( tMenuItem( "0 Back to main menu", '0', menuSetMainMenu ) );
	g_menus[ "Test" ] = menu ;
}


/*! Load a movement file (Allocate the necessary memory)
 */
static bool loadMovementFile( std::string fileName, 
							  cMovement** ptMovArray,
							  unsigned int* ptSizeArray )
{
	if( NULL == ptSizeArray ||
		NULL == ptMovArray  )
	{
		return false; 
	}

	std::ifstream file;
	file.open( fileName.c_str() );
	if( file.is_open() == false )
	{
		return false; 
	}

	file >> *ptSizeArray;
	if( *ptSizeArray == 0 )
	{
		return false; 
	}

	(*ptMovArray) = new cMovement[ *ptSizeArray ];

	for( unsigned int i=0; i<*ptSizeArray; i++ )
	{
		unsigned int nbPoint = 0;
		file >> nbPoint;
		(*ptMovArray)[i].initialise( nbPoint );
		cVector3int* buffer = (*ptMovArray)[i].modifyBuffer();
		for( unsigned int j=0; j<nbPoint; j++ )
		{
			file >> buffer[j].x >> buffer[j].y >>buffer[j].z ;
		}
	}

	file.close();

	return true;
}


/*! Quit the application
 */
void menuQuit()
{
	g_finished = true;
}


/*! Error message for function not yet implemented
 */
void menuNotImplementedYet()
{
	std::cout << "Sorry, this function is not yet implemented" << std::endl;
}


/*! Set current menu to "Main"
 */
void menuSetMainMenu() 
{
	g_currentMenu = "Main";
}


/*! Set current menu to "Load"
 */
void menuSetLoadMenu()
{
	g_currentMenu = "Load";
}

/*! Set current menu to "Load"
 */
void menuSetMovementMenu()
{
	g_currentMenu = "Movement";
}


/*! Set current menu to "Test"
 */
void menuSetTestMenu()
{
	if( g_gestureRecogniser.isReady() == false )
	{
		std::cout << "No Gesture Recogniser currently ready for testing" << std::endl;
		return;
	}
	
	g_currentMenu = "Test";
}


/*! Load a GR file
 */
void menuLoadGRFile()
{
	std::cout << "Please enter the file name" << std::endl;
	std::cout << "> " ;

	std::string str;
	std::cin >> str;
	bool res = g_gestureRecogniser.load( str.c_str() );

	if( res == false )
	{
		std::cout << "Loading failed" << std::endl;
	}
	else
	{
		std::cout << "Loading successfull" << std::endl;
		g_currentMenu = "Main";
	}
}


/*! Load a movement file and create a GR with it
 */
void menuLoadMovementGR()
{
	std::cout << "Please enter the movement file name" << std::endl;
	std::cout << "> " ;

	std::string str;
	std::cin >> str;
	cMovement* movArray;
	unsigned int sizeArray;
	bool res = loadMovementFile( str.c_str(), &movArray, &sizeArray );

	if( res == false ) {
		std::cout << "Loading failed" << std::endl;
		return;
	}
	
	std::cout << "Movement loading failed" << std::endl;
	std::cout << "Processing GR creation..." << std::endl;
	g_gestureRecogniser.init( 1, 1 );

	cVocabulary* ptVocabulary	= g_gestureRecogniser.getVocabulary( 0 );
	cGesture* ptGest			= g_gestureRecogniser.getGesture( 0 );
	
	if( NULL == ptVocabulary	|| 
		NULL == ptGest			)
	{
		std::cout << "GR initialisation failed" << std::endl;
		return;
	}

	
	std::cout << "Processing vocabulary creation..." << std::endl;
	cKmeansClustering clusterer;
	res = ptVocabulary->initialise( &clusterer, 5 );
	if( false == res )
	{
		std::cout << "Vocabulary initialisation" << std::endl;
		delete [] movArray;
		return;
	}

	int iRes = ptVocabulary->createClusters( movArray, sizeArray );
	if( iRes <= 0 )
	{
		std::cout << "Vocabulary creation failed" << std::endl;
		delete [] movArray;
		return;
	}

	std::cout << "Vocabulary creation success" << std::endl;
	std::cout << "Processing gesture creation..." << std::endl;
	res = ptGest->initialise( 8, ptVocabulary );
	if( false == res )
	{
		std::cout << "Gesture initialisation failed" << std::endl;
		delete [] movArray;
		return;
	}

	res = ptGest->createHMM( &clusterer, movArray, sizeArray );
	if( false == res )
	{
		std::cout << "Gesture's HMM creation failed" << std::endl;
		delete [] movArray;
		return;
	}
	
	std::cout << "Gesture creation success" << std::endl;
	std::cout << "Optimisation and file save..." << std::endl;
	res = ptGest->BaumWelch( movArray, sizeArray );
	if( false == res )
	{
		std::cout << "BaumWelch optimisation failed" << std::endl;
	}

	res = g_gestureRecogniser.save( "autoSaveGR.txt" );
	if( false == res )
	{
		std::cout << "GR automaticaly saved in \"autoSaveGR.txt\"" << std::endl;
	}

	std::cout << "GR creation success!!" << std::endl;
	delete [] movArray;
}


/*! Test a GR using a movement file
 */
void menuTestMovementFile()
{
	std::cout << "Please enter the movement file name" << std::endl;
	std::cout << "> " ;

	std::string str;
	std::cin >> str;
	cMovement* movArray;
	unsigned int sizeArray;
	bool res = loadMovementFile( str.c_str(), &movArray, &sizeArray );

	if( res == false ) {
		std::cout << "Loading failed" << std::endl;
		return;
	}
	
	unsigned int input = 1;
	while( input != 0 ) // will leave before
	{
		std::cout << sizeArray << " movements loaded." << std::endl;
		std::cout << "Wich one would you want to test (0 to leave)" << std::endl;
		std::cout << "> " ;
		std::cin >> input;

		if( input > sizeArray )
		{
			std::cout << "Bad input" << std::endl;
			continue;
		}
		if( 0 == input ) {
			break;
		}
		
		tGestureId gestId = g_gestureRecogniser.recognise( movArray[input-1] );

		if( gestId == -1 )
			std::cout << "Gesture not recognised" << std::endl;
		else
			std::cout << "Gesture id " << gestId << " recognised" << std::endl;
		
	}

	delete [] movArray;
}
