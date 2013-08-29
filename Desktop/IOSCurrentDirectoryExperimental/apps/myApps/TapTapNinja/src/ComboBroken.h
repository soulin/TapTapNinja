//
//  ComboBroken.h
//  BeatFighter
//
//  Created by John Ribbins on 01/08/2013.
//
//this class and the splash screen classes are very similar, could use a parent class for both and inherit, but it is how it is atm. 

#ifndef __BeatFighter__ComboBroken__
#define __BeatFighter__ComboBroken__

#include <iostream>
#include "ofMain.h"

class ComboBreak{

    
public:
    
    ComboBreak(int w, int h);
    ~ComboBreak();
    void draw(int, bool);
    void resetTheCounter();
    
private:
    enum state{checking, update, drawing, reset};
    state currentState;
    ofImage comboBreak;
    int currentCombo;
    int lastCombo;
    int frameCounter;
    int currentAlpha;
    int changeAlphaBy;
    int appearDuration;
    int resetCounter;
    

};
#endif /* defined(__BeatFighter__ComboBroken__) */
