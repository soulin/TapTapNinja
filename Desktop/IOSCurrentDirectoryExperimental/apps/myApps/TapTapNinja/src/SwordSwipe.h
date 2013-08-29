//
//  SwordSwipe.h
//  BeatFighter
//
//  Created by John Ribbins on 18/07/2013.
//
//this is a seperate class just for the sword swipes in the screen, linked heavily with the hero class, as originally the sword swipes were also dealt by the hero class itself. this structure allows the sword swipes to fad a move, fly out at multiple angles, which is functionality which is no longer used. 

#ifndef __BeatFighter__SwordSwipe__
#define __BeatFighter__SwordSwipe__

#include <iostream>
#include "ofMain.h"

class SwordSwipe{

public:
    
    SwordSwipe(ofImage*, int slash, int combo, int, int swordX, int swordY);
    ~SwordSwipe();
    void draw(bool);
    bool fadedOut();
    void move();


private:

    ofImage* swordImage;
    bool hasFaded;
    int randomSlash;
    int slashType;
    int swordX;
    int swordY;
    int enemyId;
    float alphaValue;
    float alphaChangeBy;
    int moveX;
    int moveY;
    bool runOnce;
    


};
#endif /* defined(__BeatFighter__SwordSwipe__) */


