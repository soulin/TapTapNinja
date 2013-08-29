//
//  background.cpp
//  BeatFighter
//
//  Created by John Ribbins on 11/07/2013.
//
//

#include "background.h"

Background :: Background(int numberOfTextures){
    
    ofEnableAlphaBlending();
    
    //loads all the background, foreground and cloud images to memory in each vector. the folders are arranges in the same way for each image and have the same file names for each type of BG. 
    for(int i = 0; i < numberOfTextures; i ++){
        
        
        backgroundImages.push_back(ofImage());
        foregroundImages.push_back(ofImage());
        cloudImages.push_back(ofImage());
        
        string result = "";
        stringstream currentCombo;
        currentCombo << i;
        result = currentCombo.str();
        
        backgroundImages[i].loadImage(ofToDataPath("BG/" +result + ".png"));
        foregroundImages[i].loadImage(ofToDataPath("GRASS/" +result + ".png"));
        cloudImages[i].loadImage(ofToDataPath("CLOUDS/" +result + ".png"));

    }
    
    currentBGType = 0;
    
}

Background :: ~Background(){}

void Background :: update(int type){
        //update to draw another background when called. 
        currentBGType = type;

}

void Background :: drawBackground(){
    backgroundImages[currentBGType].draw(0,0);
}

void Background :: drawForeground(){
    foregroundImages[currentBGType].draw(0,0);
}

void Background :: drawClouds(){
    cloudImages[currentBGType].draw(0,0);
}

void Background :: reset(){
    //reset back to the china level. 
    currentBGType = 0;
}

