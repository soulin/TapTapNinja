//
//  CountIn.cpp
//  TapTapNinja
//
//  Created by John Ribbins on 06/08/2013.
//
//

#include "CountIn.h"

CountIn :: CountIn(){
    
    countDown.loadImage("COUNT.png");
    counter = 0;
    xPos = 0;
    yPos = 0;
    xCut = 0;
    yCut = 0;
    xSize = 0;
    ySize = 0;
}

CountIn :: ~CountIn(){

}

void CountIn :: draw(){
    
    //this counter is making sure that the drawing stops after 160 frames,  as each number and "fight" only lasts 40frames
    counter ++;

    if(counter < 160){
        
        
        if(counter < 40){
        
            //cutting ans centralising the image
            xSize = 147;
            ySize = 190;
            xPos = (ofGetWidth()/2) - (xSize/2);
            
            //being too central obsured the Hero so it has been moved up by 70
            
            yPos = (ofGetHeight()/2) - (ySize/2) - 70;
            xCut = 0;
            yCut = 0;
        
        
        }else if(counter < 80 && counter > 40){
        
            xSize = 147;
            ySize = 190;
            xPos = (ofGetWidth()/2) - (xSize/2);
            yPos = (ofGetHeight()/2) - (ySize/2)-70;
            xCut = 147;
            yCut = 0;
        
        }else if(counter < 120 && counter > 80){
            xSize = 147;
            ySize = 190;
            xPos = (ofGetWidth()/2) - (xSize/2);
            yPos = (ofGetHeight()/2) - (ySize/2)-70;
            xCut = 294;
            yCut = 0;
            
        }else if(counter < 160 && counter > 120){
        
            xSize = 430;
            ySize = 220;
            xPos = (ofGetWidth()/2) - (xSize/2);
            yPos = (ofGetHeight()/2) - (ySize/2)-70;
            xCut = 0;
            yCut = 200;
        
        }
        ofSetColor(255,255,255,255);
        
        countDown.drawSubsection(xPos, yPos, xSize , ySize, xCut, yCut, xSize, ySize);

    }
    
   

    
    
}

void CountIn :: reset(){
    counter = 0;
}
