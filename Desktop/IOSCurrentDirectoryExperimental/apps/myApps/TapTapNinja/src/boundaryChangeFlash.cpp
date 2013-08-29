//
//  boundaryChangeFlash.cpp
//  BeatFighter
//
//  Created by John Ribbins on 17/07/2013.
//
//

#include "boundaryChangeFlash.h"

BoundaryChangeFlash :: BoundaryChangeFlash(){
    flash.loadImage("boundary1.png");
    currentBoundary = 0;
    currentState = checking;

    currentAlpha = 0;
    //flash is up for 40 frames
    limit = 40;
    counter = 0;
    
    //change by this amount every frame of the 40 frames as the flash fades in and then back out.
    changeAlphaBy = 255/(limit/2);
}

BoundaryChangeFlash :: ~BoundaryChangeFlash(){

}

//nextBoundary is just the number for the background/theme, so when the number passed by next boundary is not the same as the last on saved in current Boundary, then the flash will draw. 

void BoundaryChangeFlash :: draw(int nextBoundary){
    
    ofSetColor(255, 255, 255, 255);

    switch (currentState) {
        case checking:
            
            
            if(currentBoundary != nextBoundary){

                currentBoundary = nextBoundary;
                currentState = drawing;
            }
            
            break;
            
        case drawing:
           
            //in the drawing state the alpha increases and then decreases rapidly
            counter++;
            ofSetColor(255, 255, 255, currentAlpha);

            flash.draw(0,0);
            currentAlpha += changeAlphaBy;
            
            if(currentAlpha > 255)
            {
                //lowers the alpha once again
                changeAlphaBy *= -1;
            }
            
            if(counter == limit)
            {
                //when complete reset state to reset te variables involved
                currentState = reset;
            }
            
            break;
            
        case reset:
            
            counter = 0;
            currentAlpha = 0;
            changeAlphaBy = 255/(limit/2);
            currentState = checking;
            
            break;
    }
    
    
}