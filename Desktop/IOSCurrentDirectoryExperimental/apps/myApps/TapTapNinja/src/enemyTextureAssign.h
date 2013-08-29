//
//  enemyTextureAssign.h
//  BeatFighter
//
//  Created by John Ribbins on 11/07/2013.
//
//assign the enemies with the right texture for the stage when called for . 

#ifndef __BeatFighter__enemyTextureAssign__
#define __BeatFighter__enemyTextureAssign__

#include <iostream>
#include "ofMain.h"

class enemyTextureAssign{
    
public:
    enemyTextureAssign(int);
    ~enemyTextureAssign();
    
    ofImage* currentImage();
    //check there is a boundary and the number found so that the enemy can be changed to the appropriate type.
    void nextBoundary(int);
    int returnEnemyType();
    void reset();

private:
    
    vector<ofImage> enemyTextures;
    int numberLoaded;
    int currentEnemy;
    
};

#endif /* defined(__BeatFighter__enemyTextureAssign__) */
