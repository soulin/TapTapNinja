//
//  SwordSwipe.cpp
//  BeatFighter
//
//  Created by John Ribbins on 18/07/2013.
//
//

#include "SwordSwipe.h"

SwordSwipe :: SwordSwipe(ofImage* givenSwordImage, int slash, int combo, int loadSet, int swordXPos, int swordYPos){
    
    //gets the memory address of the sword image in memory, loaded in the hero once. to save on memory.
    swordImage = givenSwordImage;
    
    // where on the sprite sheet to cut from,  this is no longer random though
    randomSlash = slash;
    
    //the slash dependant on combo
    slashType = combo;
    
    //this denotes which way to draw the sword. to the left or right
    enemyId = loadSet;
    
    alphaValue = 180;
    swordX = swordXPos;
    swordY = swordYPos;
    moveX = 0;
    moveY = 0;
    runOnce = false;
    alphaChangeBy = 15;

}

SwordSwipe :: ~SwordSwipe(){

}

void SwordSwipe :: draw(bool paused)
{
    
    
    ofSetColor(255, 255, 255, alphaValue);

    //how to draw the sword, if the enemy is on the left or right. 
    
    if(enemyId == 1)
    {
        swordImage->drawSubsection(swordX,swordY,300 ,150, randomSlash*300 ,slashType*150);
    }
    else
    {
        swordImage->drawSubsection(swordX+275,swordY,-300 ,150, randomSlash*300 ,slashType*150, 300, 150);
    }
    
    
    if(paused == false)
    {
        
        // have the alpha value fade out slower when below 70 to have thswipe linger a little longer. 
        if(alphaValue > 70 && runOnce ==false)
        {
            alphaChangeBy = 15;
           
        }
        else if(runOnce == false)
        {
            alphaChangeBy = 5;
        }
        
        alphaValue -=alphaChangeBy;
        //if the swipe has been set to move, this would be used to move them , but not used atm ... maybe used later. 
        swordX += moveX;
        swordY += moveY;
    }
}

//return true when it has faded out and not longer visable, to be culled. 
bool SwordSwipe :: fadedOut(){
    if(alphaValue < 0){
        return true;
    }else{
        return false;
    }
    
}

//called to move the swipe if required to move, again not used atm
void SwordSwipe :: move()
{
    
    if(runOnce == false){
        
        //randomise the Id, therefore choosing which side the swipe should face, as well as the direction in which the swipe should move
        alphaChangeBy = 2;
        enemyId = ofRandom(2);
        
        if(enemyId == 1){
            moveX = ofRandom(5,10);
            moveY = ofRandom(0,10);
            
        //invert the y axis movement
            int upOrDown = ofRandom(2);
            
            if(upOrDown >= 1){
                moveY*= -1;
            }
            
            
        }else{
            
            moveX = ofRandom(-5,-10);
            moveY = ofRandom(0,7);
            
            
            int upOrDown = ofRandom(2);
            
            if(upOrDown >= 1){
                moveY*= -1;
            }
            
        
        }
        
        runOnce = true;
    }
    
}

