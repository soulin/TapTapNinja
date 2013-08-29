//
//  boundaryChangeFlash.h
//  BeatFighter
//
//  Created by John Ribbins on 17/07/2013.
//
//this flashed the blue flash when the background and the enemies need to change.

#ifndef __BeatFighter__boundaryChangeFlash__
#define __BeatFighter__boundaryChangeFlash__

#include <iostream>
#include "ofMain.h"

class BoundaryChangeFlash{
    
public:
    
    BoundaryChangeFlash();
    ~BoundaryChangeFlash();
    void draw(int);
    
private:
    
    ofImage flash;
    int currentBoundary;
    enum state{checking, drawing, reset};
    state currentState; 
    
    int counter;
    int limit;
    int currentAlpha;
    int changeAlphaBy; 
    
};

#endif /* defined(__BeatFighter__boundaryChangeFlash__) */
