//
//  HighScore.cpp
//  TapTapNinja
//
//  Created by John Ribbins on 15/08/2013.
//
// to have this high score system work probably need to have open frameworks to create the txt file and place it in the directory, and then just check eveytime the game is loaded if the file exists, and then load the high scores then on in. current working directory seems to be root which is odd, maybe useful

#include "HighScore.h"

HighScore :: HighScore(){
    
    font.loadFont("KraashBlack.ttf", 20);

    //tries to load the file, pretty sure its not doing that atm
    ofFile highScoreFile("highScores.txt");
    ofBuffer bufferHighScore(highScoreFile);
    
//save every line to highScoreVector
    for(int i = 0; i < bufferHighScore.size(); i++){
        
        string line = bufferHighScore.getNextLine();
        highScoreVector.push_back(line);
        
//every 11 lines is the song name so save those out as an index to be referenced later
        if(i % 11 == 0)
        {
            songNames.push_back(line);
        }
    }
    isInFile = false;
    whereInFile = 0;
    
}

HighScore :: ~HighScore(){}

//string coversion, slow but again i donot know another method to complete this
string HighScore :: convertToString(int intToConvert)
{
    
    string result = "";
    stringstream convert;
    convert << intToConvert;
    result = convert.str();
    
    return result;
    
}

//converts the string to int, again slow but will have to do for now
int HighScore :: convertToInt(string stats)
{
    
    string text = stats;
    int result;
    istringstream convert(text);
    if(!(convert >> result))
    {
        result = 0;
    }
    return result;
}

//this stored the new high score in a the high score vector, in order, this can then be used to save to a text file, but this functionality have not been worked out
void HighScore :: storeHighScores(string songName, string songArtist, int newScore)
{
    for(int i = 0; i < songNames.size(); i++)
    {
        //compared the played song name to those in the song name vector, aim to also compare artist name, probably pulled out from the single title with a delimiter, and compare both before finding the position in the vector
        if (songNames[i].compare(songName) == 0)
        {
            // as the song names are in the same order as those in the highScoreVector, grab the songTitle in vector line *11
            whereInFile = i*11;
            isInFile = true;
            break;
        }
    }
    
    //if the song title is already saved in the file, or atleast the score vector
    if(isInFile == true)
    {
        for(int i = 0; i < 10; i++)
        {
            // grabs the 10 values from the vector, but not the top line being the title of the song, coverted to int as begins as a string
            currentIntScores.push_back(convertToInt(highScoreVector[(whereInFile+1)+i]));
        }
        
        //looked for where in the list of scores it should go, or if it should be in the scores at all 
        for(int i = 0; i < 10; i ++)
        {
            if(newScore >= currentIntScores[i])
            {
                //if found the result is inserted in to the vector of 10 element, the rest is shifted down, and then the last element is deleted to keep only 10 scores and keep the formate the same. 
                currentIntScores.insert(currentIntScores.begin()+i, newScore);
                currentIntScores.erase(currentIntScores.end()-1);
                break;
            }
        }
      
        //reinsert the values in to the file 
        
        for(int i = 0; i < 10; i++)
        {
            // then reinsert the values in to the position in the highScore vector;
            highScoreVector[(whereInFile+1)+i] = convertToString(currentIntScores[i]);
        }
        
        
        
        //failed save to file attempt, 
//        ofFile highScoreFile("highScores.txt");
//        
//        for(int i = 0; i < highScoreVector.size(); i++){
//            highScoreFile<< highScoreVector[i]<<endl;
//        }
//        
//        highScoreFile.close();
        
        
        
        //clear out the temp scores vector just incase; 
        currentIntScores.clear();
    }
    
//if the song title does not exist in the text file or highScoreVector
    if(isInFile == false)
    {
        //sets the read position to the current end of the vector, which will then be filled with the values of the new high score, 
        whereInFile = highScoreVector.size();

        //add the new score at the beginning of the temp high scores vector
        currentIntScores.push_back(newScore);
        
        //add 9 zeros to the vector 
        for(int i = 0; i < 9; i++)
        {
            currentIntScores.push_back(0);
        }
        
        //then to the vector storing all the values from the txt file, initially add the name of the song, as this is the identifier later
        highScoreVector.push_back(songName);
        
        //then convert the temp vector to stings and add them to the highScores vecotr
        for(int i = 0; i < 10; i++)
        {
            highScoreVector.push_back(convertToString(currentIntScores[i]));
        }
        
        //adds the new song name ot the songNames vector, as an index for it later if the same song is played in the same play session 
        songNames.push_back(songName);
        
        
        
        //failed save to file attempt
//        ofFile highScoreFile("highScores.txt");
//        
//        for(int i = 0; i < highScoreVector.size(); i++){
//            highScoreFile<< highScoreVector[i]<<endl;
//        }
//        highScoreFile.close();
        
        
        
        
        currentIntScores.clear();
    }
}

//this is drawn before a reset to the use of whereInFile is fine
void HighScore :: draw()
{
    ofSetColor(0,0,0,255);
    if(highScoreVector.size()>0)
    {
        for(int i = 0; i < 11; i++)
        {
            //draws the song name not indented
            if(i < 1)
            {
                font.drawString(highScoreVector[whereInFile + i], 0, 100 + (i*50));
            }
            //draws all the results indented
            else
            {
                font.drawString(highScoreVector[whereInFile + i], 100, 100 + (i*50));
            }
        }
    }
    
     ofSetColor(255,255,255,255);
}

//reset so that it can be used again for another song
void HighScore :: reset()
{
    //would then write to the file when done looking at the high scores, but not working
    writeToFile();
    isInFile = false;
    currentIntScores.clear();
    whereInFile = 0;
}

//could write to file on the exit of the game which would be the best way to implement this but i havent got it working so no good for now. 
void HighScore :: writeToFile(){
    //this needs to be written 
}
