//
//  uberKilled.h
//  BeatFighter
//
//  Created by John Ribbins on 29/07/2013.
//
//this class deals with the meter management of the uberkill attack. other classes check this for activation and for the length of the time the uberkill is active for. 

#ifndef __BeatFighter__uberKilled__
#define __BeatFighter__uberKilled__

#include <iostream>
# include "ofMain.h"

class UberKilled{
    
    
public:
  
    UberKilled(float xScalar, float scalarY);
    ~UberKilled();
    void draw(bool);
    void updateMeter();
    bool returnMeterFull();
    void resetMeter();
    //returns the coordinates and the size of the taps, so that the hit boxes can be produced in testapp
    int returnPositions(int);
    //returns if active or not
    bool shouldUberKill();
    void hitLeft();
    void hitRight();
    void reduceMeter();
    void upLeft();
    void upRight();
    void depleteMeter();
    

private:
    
    ofImage tapTapBar;
    ofImage tapTapBarBar;
    ofImage theTaps;
    
    bool drawing;
    int xPos;
    int yPos;
    float sX;
    float sY;
    bool meterFull;
    int amountOfMeter;
    int sizeOfMeter;
    int numberToBuildMeter;
    bool drawTheTapTaps;
    int meterHeight;
    int xSizeOfTheTapTaps;
    int ySizeOfTheTapTaps;
    
    int tapOneXPos;
    int tapOneYPos;
    int tapTwoXPos;
    int tapTwoYPos;
    int counter;
    
    bool hasHitLeft;
    bool hasHitRight;
    
    bool depleteingMeter;

    
};

#endif /* defined(__BeatFighter__uberKilled__) */
