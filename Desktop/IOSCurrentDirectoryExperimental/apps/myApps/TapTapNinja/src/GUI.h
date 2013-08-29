/*
 *  GUI.h
 *  bastardPop
 *
 *  Created by Chris on 04/11/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */

#include "ofMain.h"
#include "mfccIm.h"

class GUI {
public:
	GUI();
	void draw();
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void processTouch(ofTouchEventArgs &touch);
	
	mfccIm spectroGramIm;
	float crossFader, matchFactor, numMatches;
	enum ACTIVEOBJECTS {
		NONE, SPECTROGRAM, MATCHQUANTITY, MATCHQUALITY, XFADER
	} guiObject;
	
	enum specSubsetPhases {UNDEFINED, DEFINING, DEFINED} specPhase;
	float specSubsetStart, specSubsetEnd;
};