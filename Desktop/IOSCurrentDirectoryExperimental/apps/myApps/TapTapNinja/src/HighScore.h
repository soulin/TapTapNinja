//
//  HighScore.h
//  TapTapNinja
//
//  Created by John Ribbins on 15/08/2013.
//
//this is a class to deal with the recording and the drawing of the high scores on the screen, Currently this class is incomplete, as the currently there is no way to save the scores out, to be referenced again in another play session, currently per game the scores are saved in ram and will work, Also currently the system does not take in to account artist names, needs to be implemented, two song with the same name will be an issue

#ifndef __TapTapNinja__HighScore__
#define __TapTapNinja__HighScore__

#include <iostream>
#include "ofMain.h"
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
//#include "ofxiPhone.h"
//#include "ofxiPhoneExtras.h"

class HighScore{
  
private:
    
    ofTrueTypeFont font;
    //holds all the high score data form the text file, when complete, currently will store all data give in the play session
    std :: vector<string> highScoreVector;
    
    //this saves only the song names to this vector, every 11 elements in the highScoreVector if worked, but will also save the names of new songs of that play session, checking on the names if there are already scored for that song
    std :: vector<string> songNames;
    
    //saves out all the score values for a particular song from the highScoreVector
    std :: vector<string> currentHighScores;
    
    //when the string values are converted it is saved here for comparasions
    std :: vector<int> currentIntScores;
    
    string convertToString(int);
    int convertToInt(string);
    
    //where in the highScore vector should the new high score he saved
    int whereInFile;
    bool isInFile;
    
    
    
public:
    
    HighScore();
    ~HighScore();
    void draw();
    void storeHighScores(string songName,string songArtist, int newScore);
    void reset();
    
    //incomplete
    void writeToFile();
    
    
    
    
};
#endif /* defined(__TapTapNinja__HighScore__) */
