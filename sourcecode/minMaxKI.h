/*********************************************************************\
	minMaxKI.h													  
 	Copyright (c) Thomas Weber. All rights reserved.				
	Licensed under the MIT License.
	https://github.com/madweasel/madweasels-cpp
\*********************************************************************/
#ifndef MINIMAXKI_H
#define MINIMAXKI_H

#include <iostream>
#include <cstdio>
#include <math.h>
#include "muehleKI.h"
#include <miniMax\\miniMax.h>

//using namespace std;

#define VALUE_GAME_LOST		-1000.0f
#define VALUE_GAME_WON	     1000.0f

/*** Klassen *********************************************************/
class minMaxKI : public muehleKI, miniMax
{
protected: 

	// structs
	struct possibilityStruct
	{
		unsigned int from[MAX_NUM_POS_MOVES];
		unsigned int to  [MAX_NUM_POS_MOVES];
	};

	struct backupStruct
	{
		float			value;
		bool			gameHasFinished;
		bool			settingPhase;
		int				fieldFrom, fieldTo;				// value of field
		unsigned int	from, to;						// index of field
		unsigned int	curNumStones, oppNumStones;		
		unsigned int	curPosMoves,  oppPosMoves;
		unsigned int	curMissStones, oppMissStones;
		unsigned int	stonesSet;
		unsigned int	stoneMustBeRemoved;
		unsigned int	stonePartOfMill[fieldStruct::size];
		unsigned int	warnings[fieldStruct::size];
		playerStruct	*curPlayer, *oppPlayer;
	};

	// Variables
	fieldStruct		*field;							// pointer of the current field [changed by move()]
	float			currentValue;					// value of current situation for field->currentPlayer
	bool			gameHasFinished;				// someone has won or current field is full

	int				ownId;							// id of the player who called the play()-function
	unsigned int	curSearchDepth;					// current level
	unsigned int	depthOfFullTree;				// search depth where the whole tree is explored
	unsigned int	*idPossibilities;				// returned pointer of getPossibilities()-function
	backupStruct	*oldStates;						// for undo()-function	
	possibilityStruct *possibilities;				// for getPossNormalMove()-function
			
	// Functions
	unsigned int *	getPossSettingPhase				(unsigned int *numPossibilities, void **pPossibilities);
	unsigned int *	getPossNormalMove				(unsigned int *numPossibilities, void **pPossibilities);
	unsigned int *	getPossStoneRemove				(unsigned int *numPossibilities, void **pPossibilities);

	// move functions
	inline void		updatePossibleMoves				(unsigned int stone, playerStruct *stoneOwner, bool stoneRemoved, unsigned int ignoreStone);
	inline void		updateWarning					(unsigned int firstStone, unsigned int secondStone);
	inline void		setWarning						(unsigned int stoneOne, unsigned int stoneTwo, unsigned int stoneThree);
	inline void		removeStone						(unsigned int from, backupStruct *backup);
	inline void		setStone						(unsigned int to, backupStruct *backup);
	inline void		normalMove						(unsigned int from, unsigned int to, backupStruct *backup);

	// Virtual Functions
	void			prepareBestChoiceCalculation	();
	unsigned int *	getPossibilities				(unsigned int threadNo, unsigned int *numPossibilities, bool *opponentsMove, void **pPossibilities);
	void			deletePossibilities				(unsigned int threadNo, void *pPossibilities);
	void			move							(unsigned int threadNo, unsigned int idPossibility, bool opponentsMove, void **pBackup,  void  *pPossibilities);
	void			undo							(unsigned int threadNo, unsigned int idPossibility, bool opponentsMove, void  *pBackup,  void  *pPossibilities);
	void			getValueOfSituation				(unsigned int threadNo, float &floatValue, twoBit &shortValue);
	void			printMoveInformation			(unsigned int threadNo, unsigned int idPossibility, void *pPossibilities);

	unsigned int	getNumberOfLayers				()																							{ return 0;		};
	unsigned int	getNumberOfKnotsInLayer			(unsigned int layerNum)																		{ return 0;		};
    void            getSuccLayers               	(unsigned int layerNum, unsigned int *amountOfSuccLayers, unsigned int *succLayers)         { 				};
	unsigned int	getPartnerLayer					(unsigned int layerNum)																		{ return 0;		};
	string			getOutputInformation			(unsigned int layerNum)																		{ return string("");};
	void			setOpponentLevel				(unsigned int threadNo, bool isOpponentLevel)												{ 				};
	bool			setSituation					(unsigned int threadNo, unsigned int layerNum, unsigned int stateNumber)					{ return false;	};
	bool			getOpponentLevel				(unsigned int threadNo)																		{ return false;	};
	unsigned int	getLayerAndStateNumber			(unsigned int threadNo, unsigned int &layerNum, unsigned int &stateNumber)					{ return 0;		};
	unsigned int	getLayerNumber					(unsigned int threadNo)																		{ return 0;		};
	void			getSymStateNumWithDoubles		(unsigned int threadNo, unsigned int *numSymmetricStates, unsigned int **symStateNumbers)	{ 				};
    void            getPredecessors             	(unsigned int threadNo, unsigned int *amountOfPred, retroAnalysisPredVars *predVars)		{ 				};
	void			printField						(unsigned int threadNo, unsigned char value)												{ 				};
	void			prepareDatabaseCalculation		()																							{ 				};						
	void			wrapUpDatabaseCalculation		(bool calculationAborted)																	{ 				};

public:
    // Constructor / destructor
    minMaxKI();
    ~minMaxKI();

	// Functions
	void			play							(fieldStruct *theField, unsigned int *pushFrom, unsigned int *pushTo);
	void			setSearchDepth					(unsigned int depth);
};

#endif