#include <iostream>
#include <string>
#include <stdio.h>

#include <vector>
#include "GestureRecogniser.h"

#ifdef WIN32
#	include <windows.h>	// for ShellExecute(), used to display the xml file
#endif


// CPPUnit files
#include "cppunit/TestCase.h"
#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/ui/text/TestRunner.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/TestResultCollector.h"
#include "cppunit/CompilerOutputter.h"


// Test files

//#include "SampleTest.h"
#include "TestVector3int.h"
#include "TestMovement.h"
#include "TestClustering.h"
#include "TestVocabulary.h"
#include "TestHMM.h"
#include "TestMovementRecogniser.h"
#include "TestGestureRecogniser.h"


int main( int argc, char* argv[])
{
	CppUnit::TextTestRunner runner;

	std::ofstream ofs( "tests.xml" );
	
	bool useXml = false;
	for( int i=1; i<argc; i++ )
	{
		if( std::string("-xml") == argv[1] )
		{
			useXml = true;
		}
	}

	if( useXml )
	{
		CppUnit::XmlOutputter* xml = new CppUnit::XmlOutputter( &runner.result(), ofs );
		xml->setStyleSheet( "report.xsl" );
		runner.setOutputter( xml );
	}
	else
	{
		runner.setOutputter( new CppUnit::CompilerOutputter( &runner.result(), std::cerr ) );
	}

	//runner.addTest( cSampleTest::suite() );
	runner.addTest( cTestVector3int::suite() );
	runner.addTest( cTestMovement::suite() );
	runner.addTest( cTestClustering::suite() );
	runner.addTest( cTestVocabulary::suite() );
	runner.addTest( cTestHMM::suite() );
	runner.addTest( cTestMovementRecogniser::suite() );
	runner.addTest( cTestGestureRecogniser::suite() );	

	bool success = runner.run();

	// Display the file
	if( useXml )
	{
#ifdef WIN32	
		ShellExecute( NULL, NULL, "tests.xml", NULL, NULL, 0 ); 
#else // Linux
		// TODO
#endif
	}
	else
	{
#ifdef WIN32
		// Keep the command window open to see the results
		std::cout << "Press enter to leave the application" << std::endl;
		char c;
		scanf( &c );
#endif
	}

	return (success ? 0 : -1);
}
