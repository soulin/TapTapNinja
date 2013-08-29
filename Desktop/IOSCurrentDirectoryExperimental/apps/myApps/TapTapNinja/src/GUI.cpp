/*
 *  GUI.cpp
 *  bastardPop
 *
 *  Created by Chris on 04/11/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */

#include "GUI.h"

int matchCountX = 360;
int matchQualityX = 400;
int xFadeX = 440;

GUI::GUI() {
	guiObject = NONE;
	specPhase = UNDEFINED;
}

const int faderRadius = 20;
const int faderRange = 320 - (faderRadius * 2);

void GUI::draw() {
	//spectrogram
	ofEnableAlphaBlending();
	ofSetColor(255,255,255,255);
	spectroGramIm.fbo.draw(0,0,320,matchCountX);
	//draw subset?
	if (specPhase > UNDEFINED) {
		float hlStart = min(specSubsetEnd, specSubsetStart) * matchCountX;
		float hlEnd = max(specSubsetEnd, specSubsetStart) * matchCountX;
		if (DEFINING == specPhase)
			ofSetColor(255,255,0,100);
		else
			ofSetColor(0,255,0,100);
		ofRect(0, hlStart, 320, hlEnd - hlStart);
	}
	
	//sliders
	ofFill();
	ofSetColor(102, 102, 204, 100);
	ofRect(0,matchCountX,320,40);
//	ofNoFill();
//	ofSetColor(255,255,255,255);
//	ofRect(0,matchCountX,320,40);
	

	ofFill();
	ofSetColor(51, 204, 102, 100);
	ofRect(0,matchQualityX,320,40);
//	ofNoFill();
//	ofSetColor(255,255,255,255);
//	ofRect(0,matchQualityX,320,40);
	
	ofFill();
	ofSetColor(102, 0, 0, 100);
	ofRect(0,xFadeX,320,40);
//	ofNoFill();
//	ofSetColor(255,255,255,255);
//	ofRect(0,xFadeX,320,40);
	
	ofFill();
	ofSetColor(255,255,255,100);

	float pos = (faderRange * crossFader) + faderRadius;
	ofCircle(pos, xFadeX + faderRadius, faderRadius);
	pos = (faderRange * matchFactor) + faderRadius;
	ofCircle(pos, matchQualityX + faderRadius, faderRadius);
	pos = (faderRange * numMatches) + faderRadius;
	ofCircle(pos, matchCountX + faderRadius, faderRadius);
}

float getFaderVal(float x) {
	if (x < faderRadius) x = faderRadius;
	if (x > 320 - faderRadius) x = 320 - faderRadius;
	return (x - faderRadius) / (320 - (2 * faderRadius));
}

void GUI::processTouch(ofTouchEventArgs &touch) {
	switch(guiObject) {
		case SPECTROGRAM:
			specSubsetEnd = min(1.f, touch.y / matchQualityX);
			break;
		case MATCHQUANTITY:
			numMatches = getFaderVal(touch.x);
			break;
		case MATCHQUALITY:
			matchFactor = getFaderVal(touch.x);
			break;
		case XFADER:
			//cross fader
			crossFader = getFaderVal(touch.x);
			break;
	}
}

void GUI::touchDown(ofTouchEventArgs &touch){
	if (touch.y < matchCountX) {
		specPhase = DEFINING;
		specSubsetStart = touch.y / matchQualityX;
		guiObject = SPECTROGRAM;
	}else if (touch.y < matchQualityX) {
		guiObject = MATCHQUANTITY;
	}else if (touch.y < xFadeX) {
		guiObject = MATCHQUALITY;
	}else {
		guiObject = XFADER;
	}
	processTouch(touch);
}

//--------------------------------------------------------------
void GUI::touchMoved(ofTouchEventArgs &touch){
	processTouch(touch);
}

//--------------------------------------------------------------
void GUI::touchUp(ofTouchEventArgs &touch){
	if (SPECTROGRAM == guiObject) {
		specSubsetEnd = min(1.f, touch.y / matchQualityX);
		if (fabs(specSubsetEnd - specSubsetStart) < 0.01) {
			specPhase = UNDEFINED;
		}else{
			specPhase = DEFINED;
		}
	}
	guiObject = NONE;
}

