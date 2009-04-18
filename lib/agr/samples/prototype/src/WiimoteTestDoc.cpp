// WiimoteTestDoc.cpp : implementation of the CWiimoteTestDoc class
//

#include "stdafx.h"
#include "WiimoteTest.h"

#include <vector>

#include "WiiAccelerometer.h"

#include "WiimoteTestDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWiimoteTestDoc* g_Doc = NULL;


// CWiimoteTestDoc

IMPLEMENT_DYNCREATE(CWiimoteTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CWiimoteTestDoc, CDocument)
END_MESSAGE_MAP()


// CWiimoteTestDoc construction/destruction

CWiimoteTestDoc::CWiimoteTestDoc()
{		
	cWiimoteManager::get()->update();
	cWiimote* wiimote1 = cWiimoteManager::get()->getWiimote( 1 );
	cWiimote* wiimote2 = cWiimoteManager::get()->getWiimote( 2 );
	m_accWiiA.init( wiimote1 );
	m_accWiiB.init( wiimote2 );
	m_accDeviceA = &m_accWiiA;
	m_accDeviceB = &m_accWiiB;
	
	
	//************
	// Old version
	memset( m_acc1_x_array, 0x00, sizeof(unsigned int)*SIZE_ACC_ARRAY );
	memset( m_acc1_y_array, 0x00, sizeof(unsigned int)*SIZE_ACC_ARRAY );
	memset( m_acc1_z_array, 0x00, sizeof(unsigned int)*SIZE_ACC_ARRAY );
	memset( m_acc2_x_array, 0x00, sizeof(unsigned int)*SIZE_ACC_ARRAY );
	memset( m_acc2_y_array, 0x00, sizeof(unsigned int)*SIZE_ACC_ARRAY );
	memset( m_acc2_z_array, 0x00, sizeof(unsigned int)*SIZE_ACC_ARRAY );
	m_arrayLast1 = 0;
	m_arrayLast2 = 0;
	m_acc1_x_array[ m_arrayLast1 ] = 127;
	m_acc1_y_array[ m_arrayLast1 ] = 127;
	m_acc1_z_array[ m_arrayLast1 ] = 127;
	m_acc2_x_array[ m_arrayLast2 ] = 127;
	m_acc2_y_array[ m_arrayLast2 ] = 127;
	m_acc2_z_array[ m_arrayLast2 ] = 127;

	m_selectedMovement = &m_lastMovement;
	m_selectedId = -1;
	m_lastMovement.resize( 50 );

	m_vocabularySize = 5;
	m_HMM_StateNumber = 9;

	// Old version
	//************

	//************
	// New version
	m_gestureRecogniser.init( 1, 1 );
	
	m_gestureRecogniser.getMovementRecogniser()->initialise( 1, 50 );
	m_GRselectedMovement = &m_GRcurrentMovement;
	// New version
	//************

	g_Doc = this;

}

CWiimoteTestDoc::~CWiimoteTestDoc()
{
	g_Doc = NULL;
	cWiimoteManager::releaseManager();
}

BOOL CWiimoteTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



// CWiimoteTestDoc serialization

void CWiimoteTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CWiimoteTestDoc diagnostics

#ifdef _DEBUG
void CWiimoteTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWiimoteTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWiimoteTestDoc commands


void CWiimoteTestDoc::updateAccelerometer()
{
	cWiimoteManager::get()->update();
	m_accDeviceA->update();
	m_accDeviceB->update();

	if( m_accDeviceA->m_rawAcceleration.x == 0 &&
		m_accDeviceA->m_rawAcceleration.y == 0 &&
		m_accDeviceA->m_rawAcceleration.z == 0 )
	{
		return;
	}

	
	//************
	// Old version

	// movement recognition
	int distX = m_accDeviceA->m_rawAcceleration.x - m_acc1_x_array[ m_arrayLast1 ];
	int distY = m_accDeviceA->m_rawAcceleration.y - m_acc1_y_array[ m_arrayLast1 ];
	int distZ = m_accDeviceA->m_rawAcceleration.z - m_acc1_z_array[ m_arrayLast1 ];
	m_wiimote1SquareDist = distX*distX + distY*distY + distZ*distZ;

	int xAccZeroed = m_accDeviceA->m_rawAcceleration.x - 128;
	int yAccZeroed = m_accDeviceA->m_rawAcceleration.y - 128;
	int zAccZeroed = m_accDeviceA->m_rawAcceleration.z - 128;
	int squareAcceration = xAccZeroed * xAccZeroed + 
						   yAccZeroed * yAccZeroed +
						   zAccZeroed * zAccZeroed ;

#define DIST_THRESHOLD (12*12)

#define GRAVITY_THRESHOLD (25*25)
#define GRAVITY_THRESHOLD_DELTA (5*5)

	static int inarow = 0;
	cVector3int tmp;
	if( m_wiimote1SquareDist > DIST_THRESHOLD &&
		(	squareAcceration > (GRAVITY_THRESHOLD+GRAVITY_THRESHOLD_DELTA) ||
			squareAcceration < (GRAVITY_THRESHOLD-GRAVITY_THRESHOLD_DELTA) 
		) )
	{
		if( inarow<=0 )
		{
			inarow = 4;
			if( !m_lastMovement.empty() )
			{
				m_lastMovement.clear();
			}
			tmp.x = m_acc1_x_array[ m_arrayLast1 ];
			tmp.y = m_acc1_y_array[ m_arrayLast1 ];
			tmp.z = m_acc1_z_array[ m_arrayLast1 ];
			m_lastMovement.push_back( tmp );
		}

	}
	else
	{
		inarow--;
	}
	if( inarow > 0 )
	{
		tmp.x = m_accDeviceA->m_rawAcceleration.x;
		tmp.y = m_accDeviceA->m_rawAcceleration.y;
		tmp.z = m_accDeviceA->m_rawAcceleration.z;
		m_lastMovement.push_back( tmp );
	}

	// array
	if( ++(m_arrayLast1) == SIZE_ACC_ARRAY )
		m_arrayLast1 = 0;
	m_acc1_x_array[ m_arrayLast1 ] = m_accDeviceA->m_rawAcceleration.x;
	m_acc1_y_array[ m_arrayLast1 ] = m_accDeviceA->m_rawAcceleration.y;
	m_acc1_z_array[ m_arrayLast1 ] = m_accDeviceA->m_rawAcceleration.z;

	// Old version
	//************


	//************
	// New version

	cMovementRecogniser* ptMovRec = m_gestureRecogniser.getMovementRecogniser();
	cMovementRecogniser::eMovementState moveState;

	moveState = ptMovRec->addAcceleration( 0, cVector3int( m_accDeviceA->m_rawAcceleration.x, m_accDeviceA->m_rawAcceleration.y, m_accDeviceA->m_rawAcceleration.z ) );
	if( cMovementRecogniser::eMovementFinished == moveState )
	{
		ptMovRec->getMovement( 0, &m_GRcurrentMovement );
	}
	// New version
	//************
}

void CWiimoteTestDoc::calculateProba()
{
	
#if USE_OLD_CODE
	m_movementVectorProb.resize( m_movementVector.size() , 0.0f );

	for( int i=0; i<m_movementVector.size(); i++ )
	{
		m_movementVectorProb[i] = forward( &(m_movementVector[i]) );
	}

#else // use GR
	m_movementVectorProb.resize( m_GRmovementVector.size() , 0.0f );

	for( int i=0; i<m_GRmovementVector.size(); i++ )
	{
		m_movementVectorProb[i] = m_gestureRecogniser.gestureLikelihood( 0, m_GRmovementVector[i] );
	}
#endif

}


void CWiimoteTestDoc::calculateVoca()
{

#if USE_OLD_CODE
	if( !m_movementVocabulary.empty() )
	{
		for( int i=0; i<m_movementVocabulary.size(); i++ )
			m_movementVocabulary[i].clear();
		m_movementVocabulary.clear();
	}

	m_movementVocabulary.resize( m_movementVector.size() );
	for( int i=0; i<m_movementVocabulary.size(); i++ )
	{
		m_movementVocabulary[i].resize( m_normalisedVectorSize );
		computeObservationCluster( &m_movementVector[i], &m_movementVocabulary[i] );
	}

#else // use GR
	if( !m_movementVocabulary.empty() )
	{
		for( int i=0; i<m_movementVocabulary.size(); i++ )
			m_movementVocabulary[i].clear();
		m_movementVocabulary.clear();
	}
	
	cVocabulary* ptVoc = m_gestureRecogniser.getVocabulary( 0 );

	m_movementVocabulary.resize( m_GRmovementVector.size() );
	for( int i=0; i<m_movementVocabulary.size(); i++ )
	{
		unsigned int movSize = m_GRmovementVector[i].getSize();
		unsigned int* clustersBuffer = new unsigned int[ movSize ];
		ptVoc->clusterise( m_GRmovementVector[i], clustersBuffer );

		m_movementVocabulary[i].resize( movSize );
		for( int j=0; j<movSize; j++ )
		{
			m_movementVocabulary[i][j] = (int)( clustersBuffer[j] );
		}

		delete [] clustersBuffer;
	}
#endif
}



void CWiimoteTestDoc::test()
{
	unsigned int nbMove = m_GRmovementVector.size();
	cMovement* movementBuffer = new cMovement[ nbMove ];
	for( unsigned int i=0; i<nbMove; i++ )
	{
		movementBuffer[i].initialise( m_GRmovementVector[i].getSize(), m_GRmovementVector[i].getBuffer() );
	}
	
	cVocabulary* ptVocabulary	= m_gestureRecogniser.getVocabulary( 0 );
	cGesture* ptGest			= m_gestureRecogniser.getGesture( 0 );

	cKmeansClustering clusterer;
	bool bRes = ptVocabulary->initialise( &clusterer, 5 );
	if( false == bRes )
		MessageBox( NULL, "Error", "Error", MB_OK );

	bRes = ptVocabulary->createClusters( movementBuffer, nbMove );
	if( false == bRes )
		MessageBox( NULL, "Error", "Error", MB_OK );

	bRes = ptGest->initialise( 8, ptVocabulary );
	if( false == bRes )
		MessageBox( NULL, "Error", "Error", MB_OK );

	bRes = ptGest->createHMM( &clusterer, movementBuffer, nbMove );
	if( false == bRes )
		MessageBox( NULL, "Error", "Error", MB_OK );

	bRes = m_gestureRecogniser.save( "GR_test.txt" );
	if( false == bRes )
		MessageBox( NULL, "Error", "Error", MB_OK );

	delete [] movementBuffer;

}


void CWiimoteTestDoc::testOld()
{
	// 1) Preprocess : create samples
	// calculate average size, multiply by 2 for normalised size
	// create new normalised vectors (NV vector)
	// Create average vector of normalised vectors (AV)

	// 2) Clusterise
	// Take k cluster, assign them at equal distance on the AV
	// Do the Kmean algo
	// Save the k cluster

	// 3) HMM creation
	// Do the kmean algo again, for the HMM states

	// 4) BF Algo

	// 5) Check result

	//--------------------------
	// 1) Preprocess : create samples
	preprocess();
	// TODO : remove, test only
	m_movementVector.push_back( m_averageVector );

	//--------------------------
	// 2) Clusterise
	kmean( m_vocabularySize, &m_clusterVocabulary );
	// TODO : remove, test only
	m_movementVector.push_back( m_clusterVocabulary );

	
	//--------------------------
	// 3) HMM creation
	// Clusterise HMM
	kmean( m_HMM_StateNumber, &m_HMM_States );
	// TODO : remove, test only
	m_movementVector.push_back( m_HMM_States );
	// Initialise HMM
	initialiseHMM();

}


void CWiimoteTestDoc::normaliseVector(  std::vector<cVector3int>* normalisedVector, const std::vector<cVector3int>* inputVector )
{
	// Buffer for easier manipulation
	unsigned int movementSize = (unsigned int)inputVector->size();

	// Compute normalised vector
	for( unsigned int i=0; i<m_normalisedVectorSize; i++ )
	{
		// Start                  End
		// S-------X-------X------E		Movement
		//       A         B
		// S----------X-----------E		Normalised
		//         C = value to find
		// A B, closed value to C
		//
		// X = value(A)*(distanceBC/distanceAB) + value(B)*(distanceAC/distanceAB)
		// distanceAB = 1/ (movement vector size -1)
		// distanceSA = distanceAB* A index (same for SB)
		// distanceSC = ( 1/ (normalised vector size -1) ) * C index
		// distanceAC = distanceSC - distanceSA
		// distanceBC = distanceSB - distanceSC

		int indexC = i;
		float distanceAB = ( 1.0f/((float)movementSize-1.0f) );
		float distanceSC = ( 1.0f/((float)m_normalisedVectorSize-1.0f) ) * indexC;
		
		int indexA=0, indexB=0;
		for( unsigned int j=0; j<movementSize; j++)
		{
			if(  j*distanceAB == distanceSC )
			{
				indexA = j; 
				indexB = j;
				break;
			}
			if( j*distanceAB > distanceSC )
			{
				indexA = j-1; 
				indexB = j;
				break;
			}
		}

		float distanceSA = distanceAB * indexA;
		float distanceSB = distanceAB * indexB;
		float distanceAC = distanceSC - distanceSA;
		float distanceBC = distanceSB - distanceSC;

		// TODO : clean that
		if( indexA == indexB )
		{
			(*normalisedVector)[i].x = (*inputVector)[indexA].x;
			(*normalisedVector)[i].y = (*inputVector)[indexA].y;
			(*normalisedVector)[i].z = (*inputVector)[indexA].z;
		}
		else
		{
			(*normalisedVector)[i].x = (int)( (*inputVector)[indexA].x*(distanceBC/distanceAB) + (*inputVector)[indexB].x*(distanceAC/distanceAB) );
			(*normalisedVector)[i].y = (int)( (*inputVector)[indexA].y*(distanceBC/distanceAB) + (*inputVector)[indexB].y*(distanceAC/distanceAB) );
			(*normalisedVector)[i].z = (int)( (*inputVector)[indexA].z*(distanceBC/distanceAB) + (*inputVector)[indexB].z*(distanceAC/distanceAB) );
		}
	}
}



void CWiimoteTestDoc::preprocess()
{
	unsigned int nbRecords = (unsigned int)m_movementVector.size();

	//-------------
	// calculate average size, multiply by 2 for normalised size
	m_normalisedVectorSize = 0;
	for( unsigned int i=0; i<nbRecords; i++ )
	{
		m_normalisedVectorSize += (unsigned int)m_movementVector[i].size();
	}
	m_normalisedVectorSize /= 2 ;
	m_normalisedVectorSize /= nbRecords;

	m_vocabularySize = m_normalisedVectorSize;
	m_HMM_StateNumber = m_normalisedVectorSize-1;

	//-------------
	// create new normalised vectors (NV vector)
	m_normalisedVectors.resize( nbRecords );

	// Buffer for easier manipulation
	cVector3int* bufferNormalised = new cVector3int[ m_normalisedVectorSize ];

	for( unsigned int idMovement=0; idMovement<nbRecords; idMovement++ )
	{
		m_normalisedVectors[idMovement].resize( m_normalisedVectorSize );
		normaliseVector( &m_normalisedVectors[idMovement], &m_movementVector[idMovement] );
	}
	// cleaning
	delete[] bufferNormalised ;


	//-------------
	// Create average vector of normalised vectors (AV)
	m_averageVector.resize( m_normalisedVectorSize );
	for( unsigned int i=0; i<m_normalisedVectorSize; i++ )
	{
		m_averageVector[i].x = 0;
		m_averageVector[i].y = 0;
		m_averageVector[i].z = 0;
		for( unsigned int j=0; j<nbRecords; j++ )
		{
			m_averageVector[i].x += m_normalisedVectors[j][i].x;
			m_averageVector[i].y += m_normalisedVectors[j][i].y;
			m_averageVector[i].z += m_normalisedVectors[j][i].z;
		}
		m_averageVector[i].x /= nbRecords;
		m_averageVector[i].y /= nbRecords;
		m_averageVector[i].z /= nbRecords;
	}

	//---------------------------------------------
	// fill the point buffer

	m_pointsBuffer.clear();
	// the movements vectors points
	for( unsigned int i=0; i<nbRecords; i++ )
	{
		unsigned int movementSize = (unsigned int)m_movementVector[i].size();
		for( unsigned int j=0; j<movementSize; j++ )
		{
			m_pointsBuffer.push_back( m_movementVector[i][j] );
		}
	}
	
	// the normalised vectors points
	for( unsigned int i=0; i<nbRecords; i++ )
	{
		for( unsigned int j=0; j<m_normalisedVectorSize; j++ )
		{
			m_pointsBuffer.push_back( m_normalisedVectors[i][j] );
		}
	}

	// the average vector
	for( unsigned int j=0; j<m_normalisedVectorSize; j++ )
	{
		m_pointsBuffer.push_back( m_averageVector[j] );
	}
}





int CWiimoteTestDoc::closerCluster( const cVector3int& point, const std::vector<cVector3int>* clusterBuffer, const int k )
{
	unsigned int closetClusterSquareDistance;

	// initialise the first one
	cVector3int cluster = (*clusterBuffer)[0];
	int closestCluster = 0;
	int distX = cluster.x - point.x;
	int distY = cluster.y - point.y;
	int distZ = cluster.z - point.z;
	closetClusterSquareDistance = distX*distX + distY*distY + distZ*distZ;
	for( int j=1; j<k; j++ )
	{
		cluster = (*clusterBuffer)[j];
		distX = cluster.x - point.x;
		distY = cluster.y - point.y;
		distZ = cluster.z - point.z;
		int squareDist = distX*distX + distY*distY + distZ*distZ;
		if( squareDist < (int)closetClusterSquareDistance )
		{
			closetClusterSquareDistance = squareDist;
			closestCluster = j;
		}
	}
	return closestCluster;
}


bool CWiimoteTestDoc::clusterise(	
			std::vector<int>* assignedClusters,
			const std::vector<cVector3int>* pointsBuffer, 
			const std::vector<cVector3int>* clusterBuffer )
{
	bool changed = false;
	// TODO : assert sizes

	int nbPoint = (int)pointsBuffer->size();

	for( int i = 0; i<nbPoint; i++ )
	{
		cVector3int point = (*pointsBuffer)[ i ];

		// Assign points to the closed cluster
		int previousCluster = (*assignedClusters)[i];
		(*assignedClusters)[i] = closerCluster( point, clusterBuffer, (int)clusterBuffer->size() );
		if( previousCluster != (*assignedClusters)[i] )
		{
			changed = true;
		}
	}

	return changed;
}



void CWiimoteTestDoc::kmean( const int k, std::vector<cVector3int>* clusterBuffer )
{
	//-------------
	// Take k cluster, assign them at equal distance on the AV

	clusterBuffer->resize( k );
	{
		int step = m_normalisedVectorSize/k;
		for( int i=0; i< k; i++ )
		{
			int id = step*i;
			if( id >= m_normalisedVectorSize )
			{
				id = m_normalisedVectorSize-1;
			}
			(*clusterBuffer)[i] = m_averageVector[ id ];
		}
	}

	//-------------
	// Do the Kmean algo

	// kmean loop
	bool finished = false;

	float* sumX = new float[ k ];
	float* sumY = new float[ k ];
	float* sumZ = new float[ k ];
	unsigned int* count = new unsigned int[ k ];
	
	unsigned int nbPoint = (unsigned int)m_pointsBuffer.size();
	std::vector<int> assignedCluster;
	assignedCluster.resize( nbPoint, 0 );

	int iteration = 0;
	static const int maxIteration = 50;
	
	while( !finished )
	{
		bool changed = clusterise( &assignedCluster, &m_pointsBuffer, clusterBuffer );
		finished = !changed;

		//-----------
		// Calculate mean value per cluster
		for( int i = 0; i<k; i++ )
		{
			sumX[ i ] = 0.0f;
			sumY[ i ] = 0.0f;
			sumZ[ i ] = 0.0f;
			count[ i ] = 0;
		}
		for( int i = 0; i<nbPoint; i++ )
		{
			cVector3int point = m_pointsBuffer[ i ];
			unsigned int cluster = assignedCluster[ i ];
			sumX[ cluster ] += point.x;
			sumY[ cluster ] += point.y;
			sumZ[ cluster ] += point.z;
			count[ cluster ]++;
		}

		int maxInACluster = 0;
		int biggestClusterId = 0;
		for( int i = 0; i<k; i++ )
		{
			sumX[ i ] /= count[ i ];
			sumY[ i ] /= count[ i ];
			sumZ[ i ] /= count[ i ];

			if( count[ i ]> maxInACluster )
			{
				maxInACluster = count[i];
				biggestClusterId = i;
			}
		}

		// Check that a cluster is empty
		for( int i = 0; i<k; i++ )
		{
			if( count[ i ] == 0 )
			{
				finished = false;
				sumX[ i ] = sumX[ biggestClusterId ] + (-1)*i; // Dirty
				sumY[ i ] = sumY[ biggestClusterId ] + (-1)*i;
				sumZ[ i ] = sumZ[ biggestClusterId ] + (-1)*i;
			}
		}

		//----------
		// assign mean value to cluster

		for( int i = 0; i<k; i++ )
		{
			(*clusterBuffer)[ i ].x = (int)( sumX[ i ] );
			(*clusterBuffer)[ i ].y = (int)( sumY[ i ] );
			(*clusterBuffer)[ i ].z = (int)( sumZ[ i ] );
		}

		if( iteration >= maxIteration )
			finished = true;
		iteration++;
	}

	delete [] sumX;
	delete [] sumY;
	delete [] sumZ;
	delete [] count;

}




void CWiimoteTestDoc::initialiseHMM()
{
	unsigned int nbRecords = (unsigned int)m_normalisedVectors.size();

	//------------------------------
	// PI : Initial probability

	m_HMM_PI.resize( m_HMM_StateNumber, 0 );

	// the movements vectors points
	for( unsigned int i=0; i<nbRecords; i++ )
	{
		cVector3int point = m_movementVector[i][0];
		int clusterId = closerCluster( point, &m_HMM_States, m_HMM_StateNumber );
		m_HMM_PI[ clusterId ]++;
	}
	
	// the normalised vectors points
	for( unsigned int i=0; i<nbRecords; i++ )
	{
		cVector3int point = m_normalisedVectors[i][0];
		int clusterId = closerCluster( point, &m_HMM_States, m_HMM_StateNumber );
		m_HMM_PI[ clusterId ]++;
	}

	// the average vector
	cVector3int point = m_averageVector[0];
	int clusterId = closerCluster( point, &m_HMM_States, m_HMM_StateNumber );
	m_HMM_PI[ clusterId ]++;

	for( unsigned int i=0; i<m_HMM_StateNumber; i++ )
	{
		m_HMM_PI[ i ] /= (nbRecords + nbRecords+1);
	//	m_HMM_PI[ i ] *= 1.5;	// TEST
	}


	//------------------------------
	// A : Transition probability matrix

	m_HMM_A.resize( m_HMM_StateNumber );
	// TODO : simplify the complexity of this algo
	for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
	{
		m_HMM_A[i].resize( m_HMM_StateNumber );
		for( unsigned int j=0; j<m_HMM_StateNumber; j++ )
		{
			float count = 0;
			float total_count = 0;

			for( unsigned int v=0; v<nbRecords; v++ ) // go throught each normalised vector
			{
				for( unsigned int p=0; p<m_normalisedVectorSize-1; p++ ) // go throught each point
				{
					cVector3int pointT0 = m_normalisedVectors[v][p];
					int clusterIdT0 = closerCluster( pointT0, &m_HMM_States, m_HMM_StateNumber );
					
					cVector3int pointT1 = m_normalisedVectors[v][p+1];
					int clusterIdT1 = closerCluster( pointT1, &m_HMM_States, m_HMM_StateNumber );

					if( clusterIdT0 == i && clusterIdT1 == j )
					{
						count++;
					}
					if( clusterIdT0 == i  )
					{
						total_count++;
					}
				} // for p (vector points)
			
				cVector3int point = m_normalisedVectors[v][p];
				int clusterId = closerCluster( point, &m_HMM_States, m_HMM_StateNumber );
				if( clusterId == i  )
				{
					total_count++;
				}

				if( total_count == 0 )
					m_HMM_A[i][j] = 0;
				else
					m_HMM_A[i][j] = count/total_count;

				
				if( m_HMM_A[i][j] == 0 )
					m_HMM_A[i][j] = 0.01; // TEST

	//			m_HMM_A[i][j] *= 1.5;	// TEST

			}	// for v (normalised vector)

		}	// for j
	}	//  for i

	
	//------------------------------
	// B : emmision probability

	int nbPoint = m_pointsBuffer.size();
	std::vector<int> assignedClusterVoc;	// vocabulary points are assigned to
	std::vector<int> assignedClusterHmm;	// Hmm state points are assigned to
	assignedClusterHmm.resize( nbPoint, 0 );
	assignedClusterVoc.resize( nbPoint, 0 );
	clusterise( &assignedClusterVoc, &m_pointsBuffer, &m_clusterVocabulary );
	clusterise( &assignedClusterHmm, &m_pointsBuffer, &m_HMM_States );

	m_HMM_B.resize( m_HMM_StateNumber );
	// TODO : simplify the complexity of this algo
	for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
	{
		m_HMM_B[i].resize( m_vocabularySize );
		for( int j=0; j<m_vocabularySize; j++ )
		{
			m_HMM_B[ i ][ j ] = 0;
		}
		float total_count = 0;

		// for each point
			// if the point is assigned to the current HMM state
				// find the vocabulary assigned and B[i][ assigned ]++ it
				// total_count ++;
		for( int p=0; p<nbPoint; p++ )
		{
			if( assignedClusterHmm[p] == i )
			{
				m_HMM_B[ i ][ (assignedClusterVoc[p]) ] ++;
				total_count ++;
			}
		}

		// for each vocabulary j
			// B[i][ j ] /= total_count
		for( int v=0; v<m_vocabularySize; v++ )
		{
			m_HMM_B[ i ][ v ] /= total_count;
	//		m_HMM_B[ i ][ v ] *= 1.5f;	// TEST
			if( m_HMM_B[ i ][ v ] == 0 )
				m_HMM_B[ i ][ v ] = 0.01; // TEST
		}

	}	//  for i

}


void CWiimoteTestDoc::computeObservationCluster( const std::vector<cVector3int>* observation, 
												std::vector<int>* observationClusters )
{
	std::vector<cVector3int> normalisedObs;
	normalisedObs.resize( m_normalisedVectorSize );
	normaliseVector( &normalisedObs, observation );

	for( int i=0; i<m_normalisedVectorSize; i++ )
	{
		cVector3int point = normalisedObs[ i ];
		(*observationClusters)[i] = closerCluster( point, &m_clusterVocabulary, m_vocabularySize );
	}
}



float CWiimoteTestDoc::forward( const std::vector<cVector3int>* observation )
{
	// 1) Convert the observation sequence to a normalised and vocabulary clusterised one
	// 2) Create the alpha buffer
	// 3) Compute the alpha buffer (forward algo)
	// 4) Compute the probability


	std::vector<int> observationClusters;
	std::vector< std::vector<float> > alpha; 

	// 1)
	// Normalised and clusterised observation sequence creation
	observationClusters.resize( m_normalisedVectorSize );
	computeObservationCluster( observation, &observationClusters );

	// 2)
	// float alpha [m_HMM_StateNumber][m_normalisedVectorSize];
	alpha.resize( m_HMM_StateNumber );
	for( int i=0; i<m_HMM_StateNumber; i++ )
	{
		alpha[i].resize( m_normalisedVectorSize );
	}


	// 3)
	computeAlpha( &alpha, &observationClusters );

	// 4)
	float prob = 0;
	for( int i=0; i<m_HMM_StateNumber; i++ )
	{
		prob += alpha[ i ][ m_normalisedVectorSize-1 ];
	}

	return prob;
    
}



void CWiimoteTestDoc::computeAlpha( std::vector< std::vector<float> >* alpha, const std::vector<int>* observation )
{
	// For time 0
	for( int i=0; i<m_HMM_StateNumber; i++ )
	{
		(*alpha)[i][0] = m_HMM_PI[i] * m_HMM_B[i][ (*observation)[0] ] ;
	}

	for( int t=1; t<m_normalisedVectorSize; t++ )
	{
		for( int i=0; i<m_HMM_StateNumber; i++ )
		{
			float sum = 0;
			for( int j=0; j<m_HMM_StateNumber; j++ )
			{
				sum += (*alpha)[j][t-1] * m_HMM_A[j][i];
			}
			float value = sum * m_HMM_B[i][ (*observation)[t] ];
			(*alpha)[i][t] = value;
		}
	}
}



void CWiimoteTestDoc::computeBeta( std::vector< std::vector<float> >* beta, const std::vector<int>* observation )
{
	// For time 0
	for( int i=0; i<m_HMM_StateNumber; i++ )
	{
		(*beta)[i][ m_normalisedVectorSize-1 ] = m_HMM_A[ i ][ m_HMM_StateNumber-1 ];	// previous value : 1.0f ;
	}

	for( int t=m_normalisedVectorSize-2; t>=0 ; t-- )
	{
		for( int i=0; i<m_HMM_StateNumber; i++ )
		{
			float sum = 0;
			for( int j=0; j<m_HMM_StateNumber; j++ )
			{
				sum += m_HMM_A[i][j] * m_HMM_B[j][ (*observation)[t+1] ] * (*beta)[j][t+1] ;
			}
			(*beta)[i][t] = sum;
		}
	}
}



float CWiimoteTestDoc::computeGamma( const int state, const int time, 
								    const std::vector< std::vector<float> >* alpha, 
									const std::vector< std::vector<float> >* beta )
{
	float numerator = (*alpha)[state][time] * (*beta)[state][time];
	float denominator = 0.0f;

	for( int i=0; i<m_HMM_StateNumber; i++ )
	{
		denominator += (*alpha)[i][time] * (*beta)[i][time];
	}

	if( denominator == 0 )
		return 0;

	return numerator / denominator;
}



float CWiimoteTestDoc::computeXi(	const int state1, const int state2, const int time, 
									const std::vector< std::vector<float> >* alpha, 
									const std::vector< std::vector<float> >* beta,
									const std::vector<int>* observation)
{
	float a = (*alpha)[state1][time];
	float b = m_HMM_A[state1][state2] ;
	float c = m_HMM_B[state2][(*observation)[time+1]];
	float d = (*beta)[state2][time+1];

	float numerator = (*alpha)[state1][time] * m_HMM_A[state1][state2] * m_HMM_B[state2][(*observation)[time+1]] * (*beta)[state2][time+1];
	float denominator = 0.0f;

	for( int i=0; i<m_HMM_StateNumber; i++ )
	{
		denominator += (*alpha)[i][time] * (*beta)[i][time];
	}

	if( denominator == 0 )
		return 0;

	return numerator / denominator;
}


void CWiimoteTestDoc::BaumWelchOptimisation( )
{	
	std::vector< std::vector<int> > observationClusters;

	int nbObs =  m_normalisedVectors.size() + 1;
	observationClusters.resize( nbObs );

	// normalised vectors
	for( int i=0; i<nbObs-1; i++ )
	{
		observationClusters[i].resize( m_normalisedVectorSize );
		computeObservationCluster( &(m_normalisedVectors[i]), &(observationClusters[i]) );
	}
	
	// Average vector
	observationClusters[nbObs-1].resize( m_normalisedVectorSize );
	computeObservationCluster( &m_averageVector, &(observationClusters[nbObs-1]) );

	BaumWelch( &observationClusters );

}


void CWiimoteTestDoc::BaumWelch( const std::vector< std::vector<int> >* observation  )
{
	std::vector< std::vector<float> >					alpha;
	std::vector< std::vector<float> >					beta;
	std::vector< std::vector< float > >					new_PIs;
	std::vector< std::vector< std::vector<float> > >	new_As;
	std::vector< std::vector< std::vector<float> > >	new_Bs;

	const int obsSize = observation->size();

	// initialise the differents buffers
	
	// float alpha [m_HMM_StateNumber][m_normalisedVectorSize];
	// float beta [m_HMM_StateNumber][m_normalisedVectorSize];
	alpha.resize( m_HMM_StateNumber );
	beta.resize( m_HMM_StateNumber );
	for( int i=0; i<m_HMM_StateNumber; i++ ){
		alpha[i].resize( m_normalisedVectorSize );
		beta[i].resize( m_normalisedVectorSize );
	}

	// float new_PI [ m_HMM_StateNumber ];
	new_PIs.resize( obsSize );
	for( int obs = 0; obs< obsSize; obs++ )
		new_PIs[obs].resize( m_HMM_StateNumber, 0 );

	// new_As , array of size observation->size() containing each
	// float new_A [ m_HMM_StateNumber ][ m_HMM_StateNumber ];
	new_As.resize( obsSize );
	for( int obs = 0; obs< obsSize; obs++ )
	{
		new_As[obs].resize( m_HMM_StateNumber );
		for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
			new_As[obs][i].resize( m_HMM_StateNumber );
	}
	
	// new_Bs , array of size observation->size() containing each
	// float new_B [ m_HMM_StateNumber ][ m_clusterVocabulary ];
	new_Bs.resize( obsSize );
	for( int obs = 0; obs< obsSize; obs++ )
	{
		new_Bs[obs].resize( m_HMM_StateNumber );
		for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
			new_Bs[obs][i].resize( m_vocabularySize );
	}

	bool finished = false;

	while( !finished )
	{
		for( int obs = 0; obs< obsSize; obs++ )
		{
			computeAlpha( &alpha, &(*observation)[obs] );
			computeBeta( &beta, &(*observation)[obs] );

			// Pi
			for( int i=0; i<m_HMM_StateNumber; i++) 
			{
				float tmp =  computeGamma( i, 0, &alpha, &beta );
				new_PIs[obs][ i ] = tmp;
			}

			// Transition
			for( int i=0; i<m_HMM_StateNumber; i++) 
			{
				for( int j=0; j<m_HMM_StateNumber; j++) 
				{
					float numerator = 0.0f;
					float denomerator = 0.0f;

					for( int t=0; t<m_normalisedVectorSize-2; t++ )
					{
						numerator += computeXi( i, j, t, &alpha, &beta, &(*observation)[obs] );

						for( int h=0; h<m_HMM_StateNumber; h++ )
						{
							denomerator += computeXi( i, h, t, &alpha, &beta, &(*observation)[obs] );
						}
					}

					float tmp ;
					if( denomerator == 0 )
						tmp = 0;
					else
						tmp = numerator/denomerator;
					new_As[obs][i][j] = tmp;
				} // for j
			} // for i

			//// Transition - old version
			//for( int i=0; i<m_HMM_StateNumber; i++) 
			//{
			//	for( int j=0; j<m_HMM_StateNumber; j++) 
			//	{
			//		float sum_gamma = 0.0f;
			//		float sum_xi = 0.0f;

			//		for( int t=0; t<m_normalisedVectorSize-1; t++ )
			//		{
			//			sum_gamma += computeGamma( i, t, &alpha, &beta );
			//			sum_xi += computeXi( i, j, t, &alpha, &beta, &(*observation)[obs] );
			//		}
			//		float tmp ;
			//		if( sum_gamma == 0 )
			//			tmp = 0;
			//		else
			//			tmp = sum_xi/sum_gamma;
			//		new_A[i][j] = tmp;
			//	} // for j
			//} // for i

			// Biases
			for( int i=0; i<m_HMM_StateNumber; i++)
			{
				for( int k=0; k<m_vocabularySize; k++) 
				{
					float num = 0.0f;
					float denom = 0.0f;

					for( int t=0; t<m_normalisedVectorSize; t++ )
					{
						float interDenom = computeGamma( i, t, &alpha, &beta );
						if(  (*observation)[obs][t] == k )
							num += interDenom;
						denom += interDenom;
					}
					float tmp ;
					if( denom == 0 )
						tmp = 0;
					else
						tmp = num / denom;
					new_Bs[obs][i][k] = tmp;

				} // for j
			} // for i

		} // for obs

		// zero values
		for( int i=0; i<m_HMM_StateNumber; i++) 
			m_HMM_PI[ i ] = 0;
			
		for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
			for( unsigned int j=0; j<m_HMM_StateNumber; j++ )
				m_HMM_A[i][j] = 0;

		for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
			for( unsigned int j=0; j<m_vocabularySize; j++ )
				m_HMM_B[i][j] = 0;

		// add values
		for( int obs = 0; obs< obsSize; obs++ )
		{
			for( int i=0; i<m_HMM_StateNumber; i++) 
				m_HMM_PI[ i ] += new_PIs[obs][ i ];
				
			for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
				for( unsigned int j=0; j<m_HMM_StateNumber; j++ )
					m_HMM_A[i][j] += new_As[obs][i][j];

			for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
				for( unsigned int j=0; j<m_vocabularySize; j++ )
					m_HMM_B[i][j] += new_Bs[obs][i][j];
		}

		for( int i=0; i<m_HMM_StateNumber; i++)
		{
        	m_HMM_PI[ i ] /= obsSize;
        	m_HMM_PI[ i ] *= 1.01f;
			if( m_HMM_PI[i] < 0.01f )
				m_HMM_PI[i] = 0.01f;
			if( m_HMM_PI[i] > 1 )
				m_HMM_PI[i] = 1;
		}
			
		for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
			for( unsigned int j=0; j<m_HMM_StateNumber; j++ )
			{
				m_HMM_A[i][j] /= obsSize;
				m_HMM_A[i][j] *= 1.01f;
				if( m_HMM_A[i][j] < 0.01f )
					m_HMM_A[i][j] = 0.01f;
				if( m_HMM_A[i][j] > 1 )
					m_HMM_A[i][j] = 1;
			}

		for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
			for( unsigned int j=0; j<m_vocabularySize; j++ )
			{
				m_HMM_B[i][j] /= obsSize;
				m_HMM_B[i][j] *= 1.01f;
				if( m_HMM_B[i][j] < 0.01f )
					m_HMM_B[i][j] = 0.01f;
				if( m_HMM_A[i][j] > 1 )
					m_HMM_A[i][j] = 1;
			}


		static int iteration = 0;
		if( iteration > 0 )
		{
			iteration --;
		}
		else
		{
			finished = true;
		}

	} // while !finished

}



void CWiimoteTestDoc::exportHMM()
{
	FILE* f = fopen( "Hmm.txt", "w" );

	fprintf( f, "HMM test\n" );
	fprintf( f, "\n" );
	fprintf( f, "Vocabulary size : %d\n", m_vocabularySize );
	fprintf( f, "Number of states : %d\n", m_HMM_StateNumber );
	fprintf( f, "Normalised size : %d\n", m_normalisedVectorSize );
	fprintf( f, "\n" );

	
	fprintf( f, "PI :\n" );
	fprintf( f, "\n" );
	for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
	{
		fprintf( f, " %f ", m_HMM_PI[i] );
	}
	fprintf( f, "\n" );
	fprintf( f, "\n" );

	fprintf( f, "A :\n" );
	fprintf( f, "\n" );
	for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
	{
		for( unsigned int j=0; j<m_HMM_StateNumber; j++ )
		{
		fprintf( f, " %f ", m_HMM_A[i][j] );
		}
		fprintf( f, "\n" );
	}
	fprintf( f, "\n" );

	
	fprintf( f, "B :\n" );
	fprintf( f, "\n" );
	for( unsigned int i=0; i<m_HMM_StateNumber; i++ ) 
	{
		for( unsigned int j=0; j<m_vocabularySize; j++ )
		{
		fprintf( f, " %f ", m_HMM_B[i][j] );
		}
		fprintf( f, "\n" );
	}
	fprintf( f, "\n" );

	fclose( f );
	MessageBox( NULL, "Saved", "File saved", MB_OK );
}
