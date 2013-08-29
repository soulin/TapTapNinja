//
//  slashScreen.cpp
//  BeatFighter
//
//  Created by John Ribbins on 05/07/2013.
//
//

#include "slashScreen.h"

SplashScreen :: SplashScreen(){
    
    
    numberLoaded = 9;
    ofEnableAlphaBlending();
    
    //load all the swipes in to memory. the folder is in order appearence. so electric, rainbow, fire, etc etc.
    for(int i = 0; i < numberLoaded; i ++){
        
        
        splashImages.push_back(ofImage());
        
        string result = "";
        stringstream currentCombo;
        currentCombo << i+1;
        result = currentCombo.str();
        
        splashImages[i].loadImage(ofToDataPath("swipe/" +result + ".png"));
    }
    
    //this is -1 as the draw adds one on an update, and i want to print the 0th element.. therefore it is -1
    indexDrawn = -1;
    currentAlpha = 0;
    
    //duration
    appearDuration = 40;
    frameCounter = 0;
    
    //fades up and down therefore the change to maximum fade is twice the change if it was just fading up/ 
    changeAlphaBy = 255/(appearDuration/2);
    currentCombo = 0;
    //lastCombo = 0;
    //resetTheSplash = false;
    missed = false;
    
}

SplashScreen :: ~SplashScreen(){}
                                        //this value is set to 7 atm, when the combo hits 7 then the sword will update and splash will play.
void SplashScreen :: draw(int combo, int slashThresh){
    
    switch(currentState){
    
        case checking:
            
        // check to see if the combo has hit the threshhold
           currentCombo = combo;
            
            if(combo % slashThresh == 0 && combo != 0){
            
                currentState = update;

            }
            
            //if the combo is dropped at anypoint during the drawig process the image to draw is then reset to mirror the sword type
            if(combo == 0 || currentCombo < lastCombo|| missed){

                indexDrawn = -1;
                missed = false;
            }
        
        break;
            
    
        case update:
            indexDrawn ++;
            
            // if the indexDrawn is greated than the number of images there are in the splash screen vector, then it will randomly pick from the top 3 images, the the cat, the unicorn and the laser cats
            
            if(indexDrawn > numberLoaded-1){
                //this finds numbers form 6 - 8, as the index is one less than the max, never reaching 9
                indexDrawn = ofRandom(6, 9);
            }
        
            currentState = drawing;
            
            // dont want to have the image reset half way though, instead the boolean is set to true so once bcak in a check state the images will be reset there. 
            if(combo == 0){
                missed = true;
            }
            
        break;
                   
        case drawing:
            
            //in the 40 frmaes 
            frameCounter++;
        
            ofSetColor(255, 255, 255, currentAlpha);
            
            // the images draw, 
            splashImages[indexDrawn].draw(0,0);
            //incrementing the alpha change
            currentAlpha += changeAlphaBy;
            
            //have the alpha reduce after reaching 255
            if(currentAlpha > 255){
                changeAlphaBy *= -1;
            }
            
            // saves the current combo at that pointand goes in to the reset state, this will allow the next splash screen to be drawn
            if(frameCounter == appearDuration){
                currentCombo = combo;
                currentState = reset;
            }
            
            
            if(combo == 0){
                missed = true;

            }
        
        break;
            
        case reset:
            
        //this checks if the combo has changed before starting to checking again
            
            if(currentCombo != combo){
                currentState = checking;
            }
            frameCounter = 0;
            currentAlpha = 0;
            changeAlphaBy = 255/(appearDuration/2);
            lastCombo = currentCombo;
            
            //incase the change is to 0 and therefore would not be recognise. this is in place to reset the value anyway
            if(combo == 0){
                missed = true;

            }
        
    
        break;
    }
}



