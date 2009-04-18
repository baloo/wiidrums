/**
 * \file AGRVersion.h
 * \brief Version information
 * \date 23/02/08
 */


#ifndef _AGRVERSION_
#define _AGRVERSION_


static const unsigned int AGR_VERSION		= 1;	/**< AGR version number */
static const unsigned int AGR_VERSION_MIN	= 0;	/**< AGR sub version number */

static const unsigned int AGR_HMM_ASCII_FILE_VERSION					= 2;	/**< Hmm ascii file version number */
static const unsigned int AGR_HMM_BINARY_FILE_VERSION					= 1;	/**< Hmm binary file version number */
static const unsigned int AGR_VOCABULARY_ASCII_FILE_VERSION				= 1;	/**< Vocabulary ascii file version number */
static const unsigned int AGR_VOCABULARY_BINARY_FILE_VERSION			= 1;	/**< Vocabulary binary file version number */
static const unsigned int AGR_GESTURE_RECOGNISER_ASCII_FILE_VERSION		= 2;	/**< GR ascii file version number */
static const unsigned int AGR_GESTURE_RECOGNISER_BINARY_FILE_VERSION	= 2;	/**< GR ascii binary version number */

static const char AGR_HMM_BIN_FILE_HEADER[3]				= {'A','G','H'};	/**< HMM binary file header*/
static const char AGR_VOCABULARY_BIN_FILE_HEADER[3]			= {'A','G','V'};	/**< Vocabulary binary file header*/
static const char AGR_GESTURE_RECOGNISER_BIN_FILE_HEADER[3]	= {'A','G','R'};	/**< GR binary file header*/

#endif // _AGRVERSION_

