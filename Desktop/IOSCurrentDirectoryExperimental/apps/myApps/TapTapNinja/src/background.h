//
//  background.h
//  BeatFighter
//
//  Created by John Ribbins on 11/07/2013.
//
// this loads in all the backgrounds, foregrounds and cloud images to memory, and to draw the right stage for the right enemies when called for. 

#ifndef __BeatFighter__background__
#define __BeatFighter__background__

#include <iostream>
#include "ofMain.h"

class Background{

public:
    Background(int numberOfTextures, int w, int h);
    ~Background();
    void update(int type);
    void drawBackground();
    void drawForeground();
    void drawClouds();
    void reset();
    
private:
    
    vector<ofImage> backgroundImages;
    vector<ofImage> foregroundImages;
    vector<ofImage> cloudImages;
    int currentBGType;
    int numberLoaded;

};

#endif /* defined(__BeatFighter__background__) */
