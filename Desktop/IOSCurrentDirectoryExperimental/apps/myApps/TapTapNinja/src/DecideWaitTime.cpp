//
//  DecideWaitTime.cpp
//  TapTapNinja
//
//  Created by John Ribbins on 06/08/2013.
//
//

#include "DecideWaitTime.h"

//slashThresh is the amount of consecutive, slashes required to level up the players sword, this is also the moment in which the game gets harder. 

DecideWaitTime :: DecideWaitTime(int slashThresh){
    threshHold = slashThresh;
    currentCombo = 0;
    runOnce = false;
    minimumWaitTime = 0;
    maximumWaitTime = 0;
}

DecideWaitTime :: ~DecideWaitTime(){

}

int DecideWaitTime :: returnWaitTime(int combo){
    
    //checks the current combo, makes sure its not 0 so that it wont immediately become as hard as the difficulty allows, and then checks the combo against the threshhold to see if it has been hit and should be updated, and runOnce is a boolean to have it only increment and increase in difficulty by one step.
    
    if(combo != 0 && combo % threshHold== 0 && runOnce == false){
        
        currentWaitTime -=5;
        
        if(currentWaitTime < minimumWaitTime){
            currentWaitTime = minimumWaitTime;
        }
        
        
        //stores the last combo, so that misses can be checked. 
        currentCombo = combo;
        runOnce = true;
        
    }else if(combo == 0 && runOnce == false){
        
        //makes the game easier when the player drops the combo. 
        
        currentWaitTime += 5;
        
        if(currentWaitTime > maximumWaitTime){
            currentWaitTime = maximumWaitTime;
        }
        
        runOnce = true;
        currentCombo = combo;
    }
    
    //resumes checking once the player has starting hitting enemies again. 
    
    if(currentCombo != combo){
        runOnce = false;
    }
    return currentWaitTime;
    
}


void DecideWaitTime :: setWaitTime(int difficultyChosen){
    
    //initial difficulty assignments 
    
    difficulty = (difficultySetting)difficultyChosen;
    
    switch(difficulty){
            
        case easy: currentWaitTime = 50;
                   minimumWaitTime = 30;
                   maximumWaitTime = 55;
            break;
        case medium: currentWaitTime = 30;
                     minimumWaitTime = 20;
                     maximumWaitTime = 35;
            break;
        case hard: currentWaitTime = 20;
                   minimumWaitTime = 10;
                   maximumWaitTime = 25;
            break;
        case nightmare: currentWaitTime = 5;
                        minimumWaitTime = 5;
                        maximumWaitTime = 10;
            break;
            
    }

}