//
//  CountIn.h
//  TapTapNinja
//
//  Created by John Ribbins on 06/08/2013.
//
//This class deals with the count down at the start of a song, in which the song is being processed

#ifndef __TapTapNinja__CountIn__
#define __TapTapNinja__CountIn__

#include <iostream>
#include "ofMain.h"

class CountIn{

public:
    
    CountIn();
    ~CountIn();
    void draw();
    void reset();
    
    
private:
    
    ofImage countDown;
    int counter;
    int xPos;
    int yPos;
    int xCut;
    int yCut;
    int xSize;
    int ySize;
    
};
#endif /* defined(__TapTapNinja__CountIn__) */
