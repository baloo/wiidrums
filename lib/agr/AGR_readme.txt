

============== AGR library Readme ==============


AGR (for Accelerometer Gesture Recogniser) is a library helping the recognition of gestures using an accelerometer device.

This file is aimed to be a short manual on how to use the library. 

More explanation on the library can be found in the documentation generated from the code comments with doxygen.

Feel free to email me for any question, bug report or feature request. 
For anyone interested in the techniques used in this library, I’ll be happy to send my university report on this project.


 * Author:
David Uberti 
(Ruffi04 [at] gmail [dot] com ) 


 * Licence:
Free to use and modify the source code with no limitation, but an email to the author would be appreciated. 


 * FAQ:

Q: How to use AGR with the Wiimote (or any other device) ?
A: AGR don’t provide native support for a specific controller, but has been developed using the Wiimote controller. The GestureCreator sample is using the wiiuse library, and provides a flexible interface for any device, with an implementation using the Wiiuse library for the Wiimote controller.

Q: How to create my own gesture files?
A: The GestureCreator application is aimed to help the creation of the gesture, and is provided as a sample in order to be easily modified (you may want to use another input device or your own movement recognition algorithm for example).

Q: How to add a new FAQ question?
A: Send it to me by email, I’ll reply you and update this section :)




============== AGR definitions ==============


Here is some definition needed in order to understand how AGR works.

- Position: 
3D position in space of the device at an instant t.

- Movement: 
Set of position during an interval of duration d.

- Gesture: 
A vocabulary that characterise a set of similar movement.

- Gesture vocabulary:
A gesture vocabulary is used for the gesture to be recognised. Its size may have a great influence on the recognition results

- Movement creation:
In AGR, the user provides a 3D position at every input update, which will be used to create a movement. As it will be described later, AGR provide some tools to help this process, including automatic movement recognition. 

- Learning process:
With a set of movement, a gesture may be learned. This learning process needs a gesture vocabulary.

- Recognition process:
Having an input movement, the different gestures may be analysed to see in they have been recognised.

- False positive/False negative:
Two kind of errors may be interpreted as recognition errors.
A false positive occurs when a gesture has been recognised when it should not be.
A false negative occurs when a gesture has not been recognised when it should be.




============== Gesture recogniser ==============


An easy way to introduce AGR is by using the cGestureRecogniser class.
This class manipulate the different AGR component and should be sufficient for most application.

[code]
cGestureRecogniser	gestureRecogniser;
[/code]


 * Initialisation:

- For application that only do gesture recognition, the gesture learning process can be processed offline using another tool like the GestureCreator sample, creating a file that may be loaded in the application.

[code]
gestureRecogniser.load( “myGestureFile.bin” );
[/code]

- When the gesture is not loaded from a file, like when AGR is used for learning new gestures, the gesture recogniser object need to be initialised:

[code]
gestureRecogniser.init( 1, 1 );
[code]

The two parameters are the number of gesture vocabulary and the number of gesture that you attend to use within AGR. This is for memory allocation, but if during the application will need more of them, new memory allocation will be done by the AGR.


 * Recognition

Providing an input movement, several recognitions can be processed using the following functions:
 - recognise: Return the id of the gesture recognised, if any. (-1 otherwith)
 - recogniseGesture: Inform if a specified gesture is recognised
 - closerGesture: Return the id of the gesture with the highest likelihood, even if not recognised.
 - gestureLikelihood: return the likelihood factor of a specified gesture
 - gesturesLikelihood: return the likelihood factor of all gestures


 * Learning

The learning process is not handled directly by the cGestureRecogniser class.
The vocabulary and gesture manipulation is explained in a separate part.
A simple demonstration of how learning can be processed is shown in the menuLoadMovementGR function of the “Sample application” sample


 * Movement recognition

In order to help the creation of movement, a movement recognition feature is provided, cMovementRecogniser. It is part of cGestureRecogniser, and is accessed this way

[code]
	cMovementRecogniser* ptMovReco = gestureRecogniser.getMovementRecogniser();
[code]

The movement recognition can handle different device, and analyse their input in order to find a movement. 

[code]
	ptMovReco->initialise( 4, 50 ); // 4 device, with a maximum movement size of 50
[...]
static const unsigned int  device1 = 0;
ptMovReco->addAcceleration(device1, cVector3int( accX, accY, accZ ) ) ;
[code]

The recognition state of a device can be checked using the getState function, which will return a value from the eMovementState enum.
The movement can be use in two modes: automatic and manual. 
In automatic mode, the different positions will be analysed and a eMovementFinished state will be returned when a movement is recognised.
Manual mode is started by calling the manualRecognitionStart function. From this moment, a new movement is created using the device following inputs. The manual mode continues until the manualRecognitionStop function is called, which put the recognition in automatic mode.
The recognised movement can be retrieved using the getMovement or the manualRecognitionStop functions.

[code]
	cMovementRecogniser::eMovementState moveState;
	moveState = ptMovReco->addAcceleration( 0, newVector );	
	if( cMovementRecogniser::eMovementFinished == moveState )
	{
		cMovement currentMovement;
		ptMovRec->getMovement( 0, &currentMovement );
	
		currentGesture = gestureRecogniser.recognise ( currentMovement ) ;
	}
[/code]

The automatic movement recognition function may be overloaded for adding your own movement recognition algorithm. 


 * Example

[code]

#include "GestureRecogniser.h" 

[...]

cGestureRecogniser g_GR;
tGestureId g_currentGesture = -1;

[...]

	GR.load( "GRfile.bin" ) ;
	GR.getMovementRecogniser()->initialise( 1, 50 );

[...]

	cMovementRecogniser::eMovementState moveState;
	moveState = ptMovReco->addAcceleration( 0, newVector );	
	if( cMovementRecogniser::eMovementFinished == moveState )
	{
		cMovement currentMovement;
		ptMovRec->getMovement( 0, &currentMovement );
	
		g_currentGesture = gestureRecogniser.recognise ( currentMovement ) ;
	}

[/code]
