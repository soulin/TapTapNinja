//
//  enemyTextureAssign.cpp
//  BeatFighter
//
//  Created by John Ribbins on 11/07/2013.
//
//

#include "enemyTextureAssign.h"

enemyTextureAssign :: enemyTextureAssign(int numberOfTextures){
    
    ofEnableAlphaBlending();
    
    //load the images to memeory. they are in the same order as the background images , therefore the same number will call the correct BG and enemies for the stage on a boundary change. also may be more effecitve a less memory intensive if loaded from a thread when a boundary is found rather than having all loaded in at once.
    
    for(int i = 0; i < numberOfTextures; i ++){
        
        enemyTextures.push_back(ofImage());
        
        string result = "";
        stringstream currentCombo;
        currentCombo << i;
        result = currentCombo.str();
        
        enemyTextures[i].loadImage(ofToDataPath("enemies/" +result + ".png"));
    }
    
    currentEnemy = 0;


}

enemyTextureAssign :: ~enemyTextureAssign(){}

//update the current texture to be returned
void enemyTextureAssign :: nextBoundary(int type){
  
        currentEnemy = type;

}

//returns the memory location of the enemy texture so that i can be draw in the enemy class
ofImage* enemyTextureAssign :: currentImage(){
    
    return &enemyTextures[currentEnemy];
}

//return the enemy number
int enemyTextureAssign :: returnEnemyType(){
    return currentEnemy;
}

//reset to china stage when called. 
void enemyTextureAssign :: reset(){
    currentEnemy = 0; 
}