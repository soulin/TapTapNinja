#include "Hero.h"
//constructor
Hero :: Hero(){
    
    //the size of the sprite on the image
	spriteWidth = 50;
	spriteHeight = 50;
        
    //loading initial images
    hero.loadImage("player.png");
    //hero.resize(hero.width*sW, hero.height*sH);
    sword.loadImage("SWIPES.png");
    //sword.resize(sword.width*sW, sword.height*sH);
    
    
    frameSizeX = 150;
    frameSizeY = 150;
    
    //the hero begins by looking right. 
    loadSet = 1;
  
    //where form the image to draw from, comboSlash*theHeightOfEachSlash
    comboSlash = 0;
    
    //this does the animating through the images
    animateX = 0;
    animateY = 0;
    
    facingLeft = false;

    //counting the number of frames for the animation
    animationCounter = 0;
    
    //the number of animation frames the hero should go through before looping
    currentAnimationLimit = 0;
    
    //where the sword is/position on screen
    swordX = 0;
    swordY = 0;
    
    //the animation speed is defines by the "%" of the number of frames, so fast animation changes frames every 2 ingame frames, slow is every8 
    fastAni = 2;
    slowAni = 8;
    animationSpeed = fastAni;
    
    //so that only one slash image is found per button press, and not have it change over the single attack
    slashOnce = false;
    randomSlash = 0;
    
    
    currentState = stationary;
    gamePaused = false;
    
    //this used to be used to fill the sword vector will a bunch of moving sword to have them be emitted
    fillWithSwords = false;
    
    //this is used to have the hero spin constantly when in uberkill mode. 
    uberKillCounter = 0;
    uberKilling = false;

}

//deconstructor
Hero :: ~Hero(){}

void Hero::setPos(float newXPos, float newYPos){
    xPos = newXPos - (frameSizeX/2);
    yPos = newYPos - (frameSizeY);
}

//drawing the sowrd seperately from the hero so that the sword can be drawn over the enemies, whil haveing the enemies move over the hero sprite
void Hero:: drawSword(float sX, float sY){
    
    if(currentState == attacking){
        
        if(slashOnce == false){
            //just increments the slash instead of randomising it, ofRandom kept giveing the same sword
            randomSlash++;
            
            if(randomSlash > 2){
                randomSlash = 0;
            }
            
            slashOnce = true;
        }
        
        //adds a sword slash to the vector, when the sword fades out it is removed
        swordImages.push_back(SwordSwipe(&sword, randomSlash, comboSlash, loadSet, swordX, swordY));
        
    }
/*
    //this was the last uberkill, random adding swords with random texturesto the draw vector
    
//    if(fillWithSwords){
//        
//        for(int i = 0; i < 1; i++){
//            
//            randomSlash++;
//            
//            if(randomSlash > 2){
//                randomSlash = 0;
//            }
//            
//            comboSlash = ofRandom(8);
//            
//            loadSet = ofRandom(1,3);
//            
//            if(loadSet == 2){
//                swordX = xPos - 100;
//                swordY = yPos +17;
//            }
//            else if(loadSet ==1)
//            {
//                swordX = xPos - 20;
//                swordY = yPos + 17;
//            
//            }
//            
//            //adds a load of swords to the vector
//            swordImages.push_back(SwordSwipe(&sword, randomSlash, comboSlash, loadSet, swordX, swordY));
//            
//            //the images are then set to move, this makes make the move variables to a number that actually moves the slashes
//            swordImages[swordImages.size()-1].move();
//        }
//        
//
//    }
*/    
   //when the sword fades to invisible it is removed from the vector. 
        for(int i = 0; i < swordImages.size(); i++){
        
            if(swordImages[i].fadedOut())
            {
                swordImages.erase(swordImages.begin()+i);
            }
            else
            {
                swordImages[i].draw(gamePaused, sX, sY);
            }
        }
}


//draws the hero
void Hero :: drawHero(bool paused, float sX, float sY){
    
	ofFill();
	ofSetColor(255);
    gamePaused = paused;

    //stops all animation when paused. 
    if(gamePaused == false){
        
        counter ++;
        if(counter > 100){
            counter=0;
        }
    
   
    
        if(counter % animationSpeed == 0){
            
            //when the hero is attacking or recovering does actually animate. when stationary it is a single frame displyed over and over again, this value is reset to 0 everytime it is needed.
            
            if(currentState == attacking || currentState == recovering){
                animationCounter++;
            }
            
            switch(currentState){
            //when animate is called, another method, the state is changes to attacking
                    
                case attacking: 
                
                    // the animationLimit is 3, so animation counts 3 frames before moving to the next state. the start position of the attack animation is randomised between three initial values, therefore it just animates increases its animationX position unless off the sprite sheet
                
                    if(animationCounter < currentAnimationLimit){
                
                        animateX +=150;
                
                        if(animateX > 1350){
                            animateX = 0;
                            animateY = 150;
                        }
                    }
                    
                    
            
                    // after the initial attack animation, reposition the drawn subsection, change the number of animation frames, and then change the state to recovering
                
                    if(animationCounter == currentAnimationLimit){
                        
                        currentState = recovering;
                        animationCounter = 0;
                        currentAnimationLimit = 5;
                        counter = 0;
                        animateX = 0;
                        animateY = 0;
                        animationSpeed = slowAni;
                    }
                    
                    break; 
                
                //animates 5 frames of spin recovery. 
                case recovering:
            
                    if(animationCounter < currentAnimationLimit){
                
                        animateX +=150;
                    }
                    
                    //once finished moves to station state and just hold still on the one standing sprite image.
                    if(animationCounter == currentAnimationLimit){
                
                        currentState = stationary;
                        animationSpeed = 1;
                    }
                
                    break;
            
                case stationary:
            
                animateX = 750;
                animateY = 0;
                //animationSpeed = 1;
                    break;
            }
        }
/*
//this becomes true when the uberkilled function is called. and will have then hero spin, but is not longeer used
//    if(uberKilling){
//            
//        uberKillCounter++;
//
//        if(once == false){
//                
//            once = true;
//            animateX = 0;
//            animateY = 0;
//        }
//            
//        animateX += 150;
//        if(animateX > 450){
//            animateX = 0;
//        }
//    }
//                    
//        if(uberKillCounter > 25){
//            once = false;
//            uberKilling = false;
//            currentState = stationary;
//            uberKillCounter = 0;
//            fillWithSwords = false;
//        }
*/

    }

//loadSet is an old variable name when there used to be two sets of images for the left and right side. Now just used one image and mirrors it 
    //float frameSizeX = sX * 150;
    //animateX *= sX;
    //animateY *= sY;
    
    //sY -= 0.05;
    
    if(loadSet == 1){ 
        hero.drawSubsection(((float)xPos),            ((float)yPos),
                            
                            frameSizeX, frameSizeY,
                            animateX, animateY,
                            frameSizeX, frameSizeY);
    }else{
        hero.drawSubsection(((float)xPos)+frameSizeX, ((float)yPos),
                            
                           -frameSizeX, frameSizeY,
                            animateX, animateY,
                            frameSizeX, frameSizeY);
    }
    //heroImage->drawSubsection(xPos, yPos , 300, 300, animateX, animateY, 150, 150);
}


void Hero :: animate(int enemy){
   
    // this prepares the variables for animating though attack and recovery, dependant on the side the enemy is on 
	        
        currentState = attacking;
        animationSpeed = fastAni;
        counter = 0;
        animationCounter = 0;
        currentAnimationLimit = 3;
      
        if(enemy == 2){
            
            slashOnce = false;
            howToAnimate(enemy);

        }else{ 
            slashOnce = false;
            howToAnimate(enemy);
           
           
		}
	
}

//this changes the sword that is drawn at the combo level
void Hero :: chooseSwordSwipe(int currentCombo, int hitType, int slashThresh){
    
    if(currentCombo >= slashThresh * 1 && currentCombo < slashThresh *2 && hitType == 1){
        comboSlash = 1;
    }else if(currentCombo >=slashThresh * 2 && currentCombo < slashThresh * 3&& hitType == 1){
        comboSlash = 2;
    }else if(currentCombo >=slashThresh * 3 && currentCombo < slashThresh * 4&& hitType == 1){
        comboSlash = 3;
    }else if(currentCombo >= slashThresh * 4 && currentCombo < slashThresh * 5&& hitType == 1){
        comboSlash = 4;
    }else if(currentCombo >= slashThresh * 5 && currentCombo < slashThresh * 6 && hitType == 1){
        comboSlash = 5;
    }else if(currentCombo >= slashThresh * 6 && currentCombo < slashThresh*7 && hitType == 1){
        comboSlash = 6;
    }else if(currentCombo >= slashThresh * 7){
        
        comboSlash = ofRandom(1, 7);
    
    }else{
        comboSlash = 0;
    }
    
}

//this is returned so that the enemy knows how to animate
int Hero::returnComboSlash(){
    
    return comboSlash;
}

//this function decides when how the Hero animates dependant on the side the enemy is on
void Hero :: howToAnimate(int enemy){
    
    //works out the type of sword and on what side
    if(enemy == 2){
       
        //if(fillWithSwords == false){
            swordX = xPos - 100;
            swordY = yPos +17;
            loadSet = 2;
        //}
    }
    
    if(enemy == 1){
        
        //if(fillWithSwords == false){
            
            swordX = xPos - 20;
            swordY = yPos + 17;
            loadSet = 1;
        //}
        
    }
    //randoms the attackanimation starting postion
        float random = ofRandom(-1,3);
            
        if(random < 1){
            animateX = 900;
            animateY = 0;
            
        }else if(random >= 1 && random < 2){
            animateX = 1350;
            animateY = 0;
        
        }else if(random <= 2){
            animateX = 300;
            animateY = 150;
        
        }
    }

//void Hero :: uberKill(){
//    uberKilling = true;
//    fillWithSwords = true; 
//}

//reset for when the song is restarted or to the main menu 

void Hero :: reset(){
    
    animationCounter = 0;
    currentAnimationLimit = 0;
    swordX = 0;
    swordY = 0;
    fastAni = 2;
    slowAni = 8;
    animationSpeed = fastAni;
    slashOnce = false;
    randomSlash = 0;
    comboSlash = 0; 
    
    currentState = stationary;
    gamePaused = false;
    fillWithSwords = false;
    uberKillCounter = 0;
    swordImages.clear();
    currentState = stationary;
    
    
}

