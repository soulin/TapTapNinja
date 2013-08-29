//
//  ComboBroken.cpp
//  BeatFighter
//
//  Created by John Ribbins on 01/08/2013.
//
//

#include "ComboBroken.h"

ComboBreak :: ComboBreak(){
    
    comboBreak.loadImage("combobreak.png");
    frameCounter = 0;
    currentAlpha = 0;
    //no of frames that the splash screen will show up for
    appearDuration = 20;
    //as the alpha value goes up then back down in the 20 frames the change is 255/half the number of overall frames
    changeAlphaBy = 255/(appearDuration/2);;
    lastCombo = 0;
    currentCombo = 0;
    currentState = checking;
    resetCounter = 0;
    
    
}

ComboBreak :: ~ComboBreak(){

}

void ComboBreak :: draw(int combo, bool paused){
    
    //makes sure that the splash screen does not show up in the reset, as the combo number is reset to 0, which is the same condition as a dropped combo
    resetCounter++;
    
    switch (currentState){
            
        case checking:

            currentCombo = combo;
            
            //to show when the combo is zero, but not multiple times when the combo counter is zero
            if(currentCombo == 0 && lastCombo !=0){
                
                if(resetCounter > 10){
                    currentState = drawing;
                }
            }
            
            lastCombo = currentCombo;

            break;
            
        case drawing:

            frameCounter++;
            
            ofSetColor(255, 255, 255, currentAlpha);
            
            comboBreak.draw(0,0);
            
            if(paused == false){
            
                //fades in and out 
                currentAlpha += changeAlphaBy;
            
                if(currentAlpha > 255){
                    changeAlphaBy *= -1;
                }
            
                if(frameCounter == appearDuration){
                    currentState = reset;
                }
                            
                break;
            }
        case reset:
            //when the combo is no longer 0, then begin checking for dropped combos again. 
            if(currentCombo != combo){
                currentState = checking;
            }
            frameCounter = 0;
            currentAlpha = 0;
            changeAlphaBy = 255/(appearDuration/2);
            lastCombo = currentCombo;

        default:
            break;
    }
    
}

//rest the variables 
void ComboBreak :: resetTheCounter(){
    resetCounter = 0;
    currentState = reset;
    lastCombo = 0; 
}

