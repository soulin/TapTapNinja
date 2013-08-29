#include "RangeLines.h"

RangeLines :: RangeLines(){
    screenHeight = 768;
    screenWidth = 1024;
}


RangeLines :: ~RangeLines(){

}

void RangeLines :: draw(int spriteWidth, int initialSpace, int perfect , int great, int ok, int sloppy, int poor){
	ofSetColor(0);

//initial Spacing Lines--------------------------------------------------------------------------
    
    ofLine((screenWidth/2+(spriteWidth/2)+initialSpace), 0, (screenWidth/2 + (spriteWidth/2)+initialSpace), screenHeight);
    
	ofLine((screenWidth/2-(spriteWidth/2)-initialSpace), 0, (screenWidth/2 - (spriteWidth/2)-initialSpace), screenHeight);
    
//Perfect Range Lines--------------------------------------------------------------------------- 
    
	ofLine((screenWidth/2+(spriteWidth/2)+(perfect+initialSpace)), 0, (screenWidth/2 + (spriteWidth/2)+(perfect+initialSpace)), screenHeight);
    
	ofLine((screenWidth/2-(spriteWidth/2)-(perfect+initialSpace)), 0, (screenWidth/2 - (spriteWidth/2)-(perfect+initialSpace)), screenHeight);
//The Exact Perfect Line --------------------------------------------------------------------------
    
    ofSetColor(255,0,0);
    
    ofLine((screenWidth/2+(spriteWidth/2)+(perfect/2+initialSpace)), 0, (screenWidth/2 + (spriteWidth/2)+(perfect/2+initialSpace)), screenHeight);
    
	ofLine((screenWidth/2-(spriteWidth/2)-(perfect/2+initialSpace)), 0, (screenWidth/2 - (spriteWidth/2)-(perfect/2+initialSpace)), screenHeight);
    
    ofSetColor(0);
    
//Great Range Lines-----------------------------------------------------------------------------

	ofLine((screenWidth/2+(spriteWidth/2)+perfect+great+initialSpace), 0, (screenWidth/2 + (spriteWidth/2)+perfect + great+initialSpace), screenHeight);
    
	ofLine((screenWidth/2-(spriteWidth/2)-(perfect + great+initialSpace)), 0, (screenWidth/2 - (spriteWidth/2)-(perfect + great+initialSpace)), screenHeight);

    
//OK Range Lines -------------------------------------------------------------------------------
    
	ofLine((screenWidth/2+(spriteWidth/2)+perfect + great+ok+initialSpace), 0, (screenWidth/2 + (spriteWidth/2)+perfect + great+ok+initialSpace), screenHeight);
    
	ofLine((screenWidth/2-(spriteWidth/2)-(perfect+ great+ok+initialSpace)), 0, (screenWidth/2 - (spriteWidth/2)-(perfect + great+ok+initialSpace)), screenHeight);

//Sloppy Range Lines --------------------------------------------------------------------------
    
    ofLine((screenWidth/2+(spriteWidth/2)+perfect + great+ok+initialSpace+sloppy), 0, (screenWidth/2 + (spriteWidth/2)+perfect + great+ok+initialSpace+sloppy), screenHeight);
    
	ofLine((screenWidth/2-(spriteWidth/2)-(perfect+ great+ok+initialSpace+sloppy)), 0, (screenWidth/2 - (spriteWidth/2)-(perfect + great+ok+initialSpace+sloppy)), screenHeight);

//Poor Range Lines ----------------------------------------------------------------------------
    
    ofLine((screenWidth/2+(spriteWidth/2)+perfect + great+ok+initialSpace+sloppy + poor), 0, (screenWidth/2 + (spriteWidth/2)+perfect + great+ok+initialSpace+ sloppy + poor), screenHeight);
    
	ofLine((screenWidth/2-(spriteWidth/2)-(perfect+ great+ok+initialSpace+sloppy+poor)), 0, (screenWidth/2 - (spriteWidth/2)-(perfect + great+ok+initialSpace+sloppy+poor)), screenHeight);
    
    
	//ofLine((ofGetWidth()/2+(spriteWidth/2)+40), 0, (ofGetWidth()/2 + (spriteWidth/2)+40), ofGetHeight());
	//ofLine((ofGetWidth()/2-(spriteWidth/2)-40), 0, (ofGetWidth()/2 - (spriteWidth/2)-40), ofGetHeight());
	//ofLine((ofGetWidth()/2+(spriteWidth/2)+50), 0, (ofGetWidth()/2 + (spriteWidth/2)+50), ofGetHeight());
	//ofLine((ofGetWidth()/2-(spriteWidth/2)-50), 0, (ofGetWidth()/2 - (spriteWidth/2)-50), ofGetHeight());
	ofSetColor(255);
}