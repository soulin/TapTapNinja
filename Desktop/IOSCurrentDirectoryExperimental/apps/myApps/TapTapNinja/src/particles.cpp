//
//  particles.cpp
//  emptyExample
//
//  Created by John Ribbins on 14/06/2013.
//
//

#include "particles.h"

Particle :: Particle(){}

Particle :: Particle(int initialXPos, int initialYPos, ofImage* blood){
    
    xPos = initialXPos;
    yPos = initialYPos;
//randomise the intitial movement
    xMovement = ofRandom(-4,4);
    yMovement = ofRandom(-5,-9);
    gravity = 0.3;
    
    currentState = falling;
    splatTimer = 0;
    runOnce = false;
    remove = false;
    bloodTexture = blood;
    alphaChange = 255;
//random the splat 
    finalBloodSplat = ofRandom(1,8);
    sheetX = 0;
}

Particle :: ~Particle(){
}

//the movement of the particles
void Particle::move(){
    
    switch(currentState){
        case falling:
            
            xPos += xMovement;
            yPos +=yMovement;
            yMovement += gravity;
            break;
//splatted is stationary
        case splatted:
        
            xMovement = 0;
            yMovement = 0;
            gravity = 0;
            break;
    }
}


void Particle::draw(bool paused){
    splatTimer++;
    
    if(paused == false){
        
        switch(currentState){
                
                case falling:
//the image of the blob of blood
                    sheetX = 0;
                break; 
    
            case splatted:
            
//changes to the splat of blood
                    if(runOnce == false)
                    {
                        sheetX = finalBloodSplat*40;
                        runOnce = true; 
                    }
//starts lowering alpha. once invisible, remove
                    if(splatTimer % 4 == 0)
                    {
                        alphaChange -= (255/10);
                                      
                        if(alphaChange < 0){
                            remove = true;
                        }
                    }
             
                ofSetColor(255, 255, 255, alphaChange);
                break;
        }
    }
    bloodTexture->drawSubsection(xPos, yPos, 40, 40, sheetX, 0);
    ofSetColor(255, 255, 255, 255);
}

//the particles will also be removed if they more too low off the screen or too far to the left or right
bool Particle :: isOnScreen(){

    if((xPos > 1024 && xPos < -40)|| (yPos > 430) || remove == true)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//this is called when there should be the splat. atm it is set to when the player hits a perfect, great or okay.
void Particle :: splat()
{
    if(splatTimer > 10){
        currentState = splatted;
    }
}

