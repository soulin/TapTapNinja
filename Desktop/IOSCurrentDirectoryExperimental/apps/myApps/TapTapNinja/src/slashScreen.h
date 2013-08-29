//
//  slashScreen.h
//  BeatFighter
//
//  Created by John Ribbins on 05/07/2013.
//
//this class deals with the splash screen whenever the player hits the threshHold for the sword upgrade. very similar to boundaryChangeFlash adn combo broken

#ifndef __BeatFighter__slashScreen__
#define __BeatFighter__slashScreen__

#include <iostream>
#include "ofMain.h"

class SplashScreen{

public:
  
    SplashScreen();
    void draw(int, int);
    ~SplashScreen();
    void drawOnBoundary(int);
    
    
private:
    std:: vector <ofImage> splashImages;
    int numberLoaded;
    // the position in the vector to draw from
    int indexDrawn;
    bool drawOnScreen;
    enum state{checking, update, drawing, reset};
    state currentState = checking;
    
    int currentAlpha;
    int changeAlphaBy;
    int hasAppearedFor; 
    int appearDuration;
    int frameCounter;
    int currentCombo;
    int lastCombo;
    int currentBoundary;
    bool resetTheSplash;
    bool missed;
    
    
    bool done; 
};

#endif /* defined(__BeatFighter__slashScreen__) */
