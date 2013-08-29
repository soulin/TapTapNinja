//
//  TextOnSceen.cpp
//  BeatFighter
//
//  Created by John Ribbins on 01/08/2013.
//
//

#include "TextOnSceen.h"

TextOnScreen :: TextOnScreen(){
    
    //loads the same text but in different sizes for different purposes
    
    hitFont.loadFont("KraashBlack.ttf", 32);
    scoreFont.loadFont("KraashBlack.ttf", 32);
    scoringFont.loadFont("KraashBlack.ttf", 20);
    ratingFont.loadFont("KraashBlack.ttf", 150);
    songFont.loadFont("KraashBlack.ttf", 10);
    resultsFont.loadFont("KraashBlack.ttf", 20);
    
    //used for the pulsing effect. 
    hitSpacing = 1;
    scoreSpacing = 1;
    runOnce = false;
    
    //this loads the files    
    ofFile file("gamedata.txt");
    ofBuffer buffer(file);
    
    //this will then go though the file, saving the lines in the file to the string vector fileText and then conver the string to Int, and place them in to fileInt
    for(int i = 0; i < buffer.size(); i++){
        
        string line = buffer.getNextLine();
        fileText.push_back(line);
        fileInt.push_back(convertToInt(line));
    }
    
    
    
    
}

TextOnScreen :: ~TextOnScreen(){

}


void TextOnScreen :: draw(string theHit, int currentCombo, int currentScore){
   
    //this runOnce is here so that i dont need another reset function in another class, as this will no longer be set to false when the runOnce is actually changed to true, in the draw results method.
    
    runOnce = false;
    
    //the currentLetterSpacing changes when the pulse method is called.
    
    hitFont.setLetterSpacing(hitSpacing);
    scoreFont.setLetterSpacing(scoreSpacing);
    
    
    score = convertToString(currentScore);
    combo = convertToString(currentCombo);
    
                                        //keeps the text centralised even when the string lengthens
    hitFont.drawString("x" + combo, (ofGetWidth()-hitFont.stringWidth("x" + combo))/2, 570);
    hitFont.drawString(theHit, (ofGetWidth()-hitFont.stringWidth(theHit))/2, 520);
    scoreFont.drawString(score, (ofGetWidth()-scoreFont.stringWidth(score))/2, 80);

    //allows the pulsing effect. 
    hitSpacing -=0.2;
    
    if(hitSpacing < 1){
        hitSpacing = 1;
    }
    
    scoreSpacing -=0.2;
    
    if(scoreSpacing < 1){
        scoreSpacing = 1;
    }
}

void TextOnScreen :: pulseHit(){
    hitSpacing = 1.4;
}

void TextOnScreen :: pulseScore(){
    scoreSpacing = 1.4;
}


//string coversion, slow but again i donot know another method to complete this
string TextOnScreen :: convertToString(int intToConvert){
    
    string result = "";
    stringstream convert;
    convert << intToConvert;
    result = convert.str();
    
    return result;
    
}

int TextOnScreen :: convertToInt(string stats){
    
    string text = stats;
    int result;
    
    istringstream convert(text);
    
    if(!(convert >> result)){
        result = 0;
    }
    
    return result; 
}

void TextOnScreen :: drawResultsPage(float perfect, float great, float okay, float sloppy, float poor, float combo,float missed,  float finalScore, float totalBeats, float totalHits){

    
    ofSetColor(0);
    
    //convert the int to string only once.
    if(runOnce == false){
        perfectString = convertToString(perfect);
        greatString = convertToString(great);
        okayString = convertToString(okay);
        sloppyString  = convertToString(sloppy);
        poorString = convertToString(poor);
        comboString = convertToString(combo);
        finalScoreString = convertToString(finalScore);
        missedString = convertToString(missed);
        runOnce = true;
    }
    
    //this is for the ranking
    float percentageNumberHit = ((perfect + great + okay + sloppy + poor)/totalBeats)*100;
    float percentagePerfectHits = (perfect/totalBeats)*100;
    
    //this is the comparasion of the number of notes and the number of times the attack is used, be it greater than or less than. The number is multoplied by 200 so ever extra or less hit is more likely to change the ranking.
    float percentageMash = 100 - (((abs(totalHits - totalBeats))/totalBeats)*200);
    //average the two for finess rank
    float average = (percentageMash + percentagePerfectHits)/2;
    
    ofSetColor(255);
    
    
    resultsFont.drawString(songString, 50, 70);
    
    hitFont.drawString("Final Score: " + finalScoreString, 70, 180);
    scoringFont.drawString("MAX COMBO: " + comboString, 70, 260);


    
    scoringFont.drawString("PERFECT", 70, 360);
    scoringFont.drawString("GREAT  ", 70, 420);
    scoringFont.drawString("OKAY   ", 70, 480);
    scoringFont.drawString("SLOPPY ", 70, 540);
    scoringFont.drawString("POOR   ", 70, 600);
    scoringFont.drawString("MISSED ", 70, 660);
    
    scoringFont.drawString(" X " + perfectString, 270, 360);
    scoringFont.drawString(" X " + greatString, 270, 420);
    scoringFont.drawString(" X " + okayString, 270, 480);
    scoringFont.drawString(" X " + sloppyString, 270, 540);
    scoringFont.drawString(" X " + poorString, 270, 600);
    scoringFont.drawString(" X " + missedString, 270, 660);
    
    scoringFont.drawString("RATING:" , 580, 310);
    scoringFont.drawString("FINESS:" , 780, 310);
    ratingFont.drawString(returnRanking(percentageNumberHit), 600, 580);
    ratingFont.drawString(returnRanking(average), 780, 580);

    
    ofSetColor(255);
}


//this returns the ranking letter from the percentages
string TextOnScreen :: returnRanking(float percentage){
    
    if(percentage > 90)
    {
        return "A";
    }else if(percentage > 75){
        
        return "B";
    }else if(percentage > 50){
        
        return "C";
    }else if(percentage > 25){
        
        return "D";
    }else if(percentage > 15){
        
        return "E";
    }else{
        return "F";
    }

}

// draws the artist and song name on the play screen.
void TextOnScreen :: titleOnScreen(string artist, string songTitle, string difficulty){
    
    artistString = artist;
    songString = songTitle;
    
    ofSetColor(255, 255, 255, 255);
    songFont.drawString(artistString, 0, 20);
                                            //draw the song name after the artst name
    songFont.drawString(" - " + songString, songFont.stringWidth(artist) , 20);
    songFont.drawString(difficulty + " difficulty", 0, 40);
    
}

//this draws everything required on the stat screen
void TextOnScreen :: statScreen(){
    ofSetColor(0,0,0,255);
    
//this is all the stat titles, ranking is not implemented, maybe later, but just a thought
    scoringFont.drawString("Dis Da Stats", 0 , 50);
    scoringFont.drawString("Ranking : Lord Ninja", 0 , 150);
    scoringFont.drawString("Total Perfects", 50 , 200);            
    scoringFont.drawString("Total Greats", 50 , 250);              
    scoringFont.drawString("Total Okays", 50 , 300);               
    scoringFont.drawString("Total Sloppy", 50 , 350);              
    scoringFont.drawString("Total Poor", 50 , 400);               
    scoringFont.drawString("BODY COUNT", 50 , 500);                
    scoringFont.drawString("Longest Combo", 50 , 550);
    scoringFont.drawString("Number Of Plays", 50 , 600);           
    scoringFont.drawString("Most Perfects", 50 , 650);             
    
    //this is just used to seperate the totals and the other stats
    int extraSpacing = 0;
    
    //draws the 9 lines
    for(int i = 0; i < 9; i++){
        
        //after the 5th line it seperates the total form the others 
        if(i > 4){
            extraSpacing = 50;
        }else{
            extraSpacing = 0;
        }
        
        //this then draws the numbers for each of hte totals
        scoringFont.drawString("X "+ fileText[i], 500 , 200 + (i*50)+extraSpacing);
        
    }

}

void TextOnScreen :: addValues(int perfect, int great, int okay, int sloppy, int poor, int longestCombo){
    
    //this takes the values from file, int type, and adds the specific values to be saved and drawn for the stats screen 
    
    fileInt[0] += perfect;
    fileInt[1] += great;
    fileInt[2] += okay;
    fileInt[3] +=sloppy;
    fileInt[4] += poor;
    
    //this used for body count 
    fileInt[5] += (perfect + great + okay + okay + sloppy + poor);
    
    //looks for the longest combo for any song 
    if(longestCombo > fileInt[6]){
        fileInt[6] = longestCombo;
    }
    
    //number of plays, just increments
    fileInt[7]++;
    
    //looks for the most perfects in a song ever
    if(perfect > fileInt[8]){
        fileInt[8] = perfect;
    }
    
    //converts the values back to strings and save them in the vectors, so that they can be drawn again the on the screen on the stat screen. 
    for(int i = 0; i < 9; i++){
        fileText[i] = convertToString(fileInt[i]);
    }

}


