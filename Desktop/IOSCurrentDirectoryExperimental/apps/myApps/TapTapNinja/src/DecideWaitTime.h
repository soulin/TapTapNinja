//
//  DecideWaitTime.h
//  TapTapNinja
//
//  Created by John Ribbins on 06/08/2013.
//
//this class deals with the difficulty of game, wait time is the amount of time before the system looks for another beat, the lower it is harder the game

#ifndef __TapTapNinja__DecideWaitTime__
#define __TapTapNinja__DecideWaitTime__

#include <iostream>
#include "ofMain.h"

class DecideWaitTime{

public:
    
    DecideWaitTime(int slashThresh);
    ~DecideWaitTime();
    
    
    int returnWaitTime(int combo);
    void updateWaitTime(int);
    void setWaitTime(int difficultyChosen);
    
private:
    
    int currentWaitTime;
    int threshHold;
    bool runOnce;
    int currentCombo;
    int minimumWaitTime;
    int maximumWaitTime; 
    enum difficultySetting{easy, medium, hard, nightmare};
    difficultySetting difficulty;

};
#endif /* defined(__TapTapNinja__DecideWaitTime__) */
