//
//  TextOnSceen.h
//  BeatFighter
//
//  Created by John Ribbins on 01/08/2013.
//
//this class print the text that needs to change. ie the score, the combo, the hit, and the results. 

#ifndef __BeatFighter__TextOnSceen__
#define __BeatFighter__TextOnSceen__

#include <iostream>
#include "ofMain.h"

class TextOnScreen{
  
public:
    
    TextOnScreen();
    ~TextOnScreen();
    void draw(string, int , int);
    void pulseHit();
    void pulseScore();
    void drawResultsPage(float perfect, float great, float okay, float sloppy, float poor, float combo, float missed, float finalScore, float totalBeats, float totalHits);
    void titleOnScreen(string, string, string);
    void statScreen();
    void addValues(int perfect, int great, int okay, int sloppy, int poor, int longestCombo);
    void highScores();
    void storeHighScores(int);
    
private:
    
    // all these for differennt sized text though out the game
    ofTrueTypeFont hitFont;
    ofTrueTypeFont scoreFont;
    ofTrueTypeFont scoringFont;
    ofTrueTypeFont ratingFont;
    ofTrueTypeFont songFont;
    ofTrueTypeFont resultsFont;
    
    // i know strings are slow but i dont know another way to do this. 
    string convertToString(int);
    int convertToInt(string);
    
    string score;
    string combo;
    string perfectString;
    string greatString;
    string okayString;
    string sloppyString;
    string poorString;
    string comboString;
    string finalScoreString;
    string missedString;
    string artistString;
    string songString;
    
    //stores the data form the file in 2 seperate types, ints and strings, string to draw on screen, Int to modify and increment the values, before being converted back in to string to draw again. 
    std :: vector<string> fileText;
    std :: vector<int> fileInt;
    

    bool runOnce;
    float hitSpacing;
    float scoreSpacing;
    
    string returnRanking(float);

    
    
};
#endif /* defined(__BeatFighter__TextOnSceen__) */
