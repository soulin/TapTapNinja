//
//  uberKilled.cpp
//  BeatFighter
//
//  Created by John Ribbins on 29/07/2013.
//
//

#include "uberKilled.h"

UberKilled :: UberKilled(float scalarX, float scalarY){
    
    sX = scalarX;
    sY = scalarY;
    
    //load in all the elements of the tap tap bar
    tapTapBar.loadImage("TAPTAPBAR_BG.png");
    tapTapBar.resize(tapTapBar.width*sX, tapTapBar.height*sY);
    
    tapTapBarBar.loadImage("TAPTAPBAR_BAR.png");
    tapTapBarBar.resize(tapTapBar.width*sX, tapTapBarBar.height*sY);
    
    theTaps.loadImage("TAPTAPBAR_HINT.png");
    theTaps.resize(theTaps.width*sX, theTaps.height*sY);
    
    //drawing = false;
    
    xPos = ofGetWidth()/2;
    yPos = 600 * sY;
    
    meterFull = false;
    
    //current amount of meter
    amountOfMeter = 0;
    
    //the amount of meter for full meter bar
    sizeOfMeter = 545 * sX;
    meterHeight = 101 * sY;
    
    //number of steps to fill the bar.
    numberToBuildMeter= 40*sX;
    
    //draw the two buttons.
    drawTheTapTaps = false;
    xSizeOfTheTapTaps = 167 * sX;
    ySizeOfTheTapTaps = 162 * sY;
    
    //position of each tap button
    tapOneXPos = ((ofGetWidth()/2) - (sizeOfMeter/2)) - (xSizeOfTheTapTaps/2);
    tapOneYPos = (yPos+(meterHeight/2)) - (ySizeOfTheTapTaps/2);
    tapTwoXPos = ((ofGetWidth()/2) + (sizeOfMeter/2)) - (xSizeOfTheTapTaps/2);
    tapTwoYPos = (yPos+(meterHeight/2)) - (ySizeOfTheTapTaps/2);
    
    counter = 0;
    
    //these cause the uber attack
    hasHitLeft = false;
    hasHitRight = false;
    depleteingMeter = false;
    
}

UberKilled :: ~UberKilled(){
}


void UberKilled :: draw(bool paused){
    
    //this counter is used so that the input for hitleft, hitright donot have to be input on the same frame. But they must also stay true for the duration of the meter deplete therefore if depletemeter is true it no longer counts. 
    if((hasHitLeft || hasHitRight) && depleteingMeter == false){
        counter ++;
    }
    //this stop accidental input, so you have to have both inputs for the Uberkill mode to fire. 
    if(counter > 4){
        hasHitRight = false;
        hasHitLeft = false;
    }
    
    ofSetColor(255, 255, 255, 255);
    
    //draws the bar from the center of the meter outwards
    tapTapBar.draw((ofGetWidth()/2)-((560/2)*sX), yPos);
    
    tapTapBarBar.drawSubsection((ofGetWidth()/2)-((0.5f*amountOfMeter)), (yPos+(10*sY)),
                                amountOfMeter, 80*sY,
                                0, 0);
    
    //draw buttons when the meter is full, updated by the update meter method.
    if(drawTheTapTaps){
    
        theTaps.draw(tapOneXPos, tapOneYPos);
        theTaps.draw(tapTwoXPos, tapTwoYPos);
    }
    ofSetColor(255);
    
}

void UberKilled :: updateMeter(){
    //dont gain meter while uberkilling
        
    if(depleteingMeter == false){
        
        //makes meter larger
        amountOfMeter+= sizeOfMeter/numberToBuildMeter;
        
        //does not grow larger than the meter bar
        if(amountOfMeter > sizeOfMeter){
            amountOfMeter = sizeOfMeter;
        }
        
        // when full draw in the taptap buttons
        if(amountOfMeter == sizeOfMeter){
            drawTheTapTaps = true;
            meterFull = true;
        }
    }
}

void UberKilled :: reduceMeter(){
    
    //lose meter when the method is called, in the missed state. 
    
    if(depleteingMeter == false){
        amountOfMeter-= sizeOfMeter/numberToBuildMeter;
    
        if(amountOfMeter < 0){
            amountOfMeter = 0;
        }

        drawTheTapTaps = false;
        meterFull = false;
    }
}


//this is used so regulate when hasHitLeft and hasHitRight can be checked. 
bool UberKilled::returnMeterFull(){
    
    return meterFull;
}

void UberKilled :: resetMeter(){

    
    amountOfMeter = 0;
    drawTheTapTaps = false;
    meterFull = false;
    hasHitLeft = false;
    hasHitRight = false;
    depleteingMeter = false;
}

//returns the coordinates of the taptap buttons for hitbox usage. 1-4 for the first button, 5-8 for the second button. 
int UberKilled :: returnPositions(int askedFor){
    
    switch(askedFor){
    
        case 1: return tapOneXPos;
            break;
            
        case 2: return tapOneXPos + xSizeOfTheTapTaps;
            break;
            
        case 3: return tapOneYPos;
            break;
        case 4: return tapOneYPos+ySizeOfTheTapTaps;
            break;
        
        case 5: return tapTwoXPos;
            break;
            
        case 6: return tapTwoXPos+xSizeOfTheTapTaps;
            break;
            
        case 7: return tapTwoYPos;
            break;
            
        case 8: return tapTwoYPos+ySizeOfTheTapTaps;
            break;
            
        default:
            return 1;
            break;
    }
    
}

//allow uber killed to happen when both are hit similaneously
void UberKilled :: hitLeft(){
    hasHitLeft = true;
    counter =0;

}

void UberKilled :: hitRight(){
    hasHitRight = true;
    counter = 0;

}

//if both hit uber kill, this is needed so that other classes can go in to their uber killed state.
bool UberKilled:: shouldUberKill(){
    
    if(hasHitLeft && hasHitRight){
        return true;
    }else{
        return false; 
    }
}

// when "shouldUberKill" is true, start draining the meter when at zero reset variables to build the meter once again. 
void UberKilled :: depleteMeter(){
    
    depleteingMeter = true;
    
    if(depleteingMeter){
        amountOfMeter-= 2;
        
        if(amountOfMeter < 0){
            amountOfMeter = 0;
            depleteingMeter = false;
        }
        
        drawTheTapTaps = false;
        meterFull = false;
    }
    
}

