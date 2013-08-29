#include "enemy.h"

Enemy :: Enemy(){

}

//initialise a bunch of variables --- constructor is quite large
Enemy :: Enemy(int heroWidthIn, int numberOfTextures, ofImage *blood){
    
    
	spriteWidth = 50;
	heroWidth = heroWidthIn;
//important for collision checks
	checkable = true;
//the perfect range, so that the middle can be found, which is the exact time of the beat
    perfect = 10;
//this judges the range
    hitValue = 0;
//where on the sprite sheet
    animateX = 0;
    animateY = 0;
    counter = 0;
//when to cull enemy
    killedCounter = 0;
    runOnce = false;
    state= walking;
//the size of each frame
    animationSpacing = 100;
    animationSpeed = 7;
    finishedAttack = false;
    sineMovement = 0.0f;
//starts the fly counter, so it knows when to kill the off the enemy after flight
    flyDeath = false;
    flyDeathCounter = 0;
    yPos = 400;
    bobbing = 0.0f;
    drawing = false;
    twohit = false;
//this is used for uberkilled
    goingToBeKilled = false;
    pauseAnimation = false;
//was used for uber killed
    pauseLength = 0;
    bloodTexture = blood;
//whether to show the arrow
    displayIndicator = false;
    red = 0;
    green = 0;
    blue = 0;
    specialKill = false;
    specialKillCounter = 0;
}

//check the objects are being removed a memory freed up 
 Enemy :: ~Enemy(){     
  
 }

// this assigns the enemy its origal texture, id(what side it is on), the type of enemy it is, and the speed of it
void Enemy :: assignTexture(ofImage *enemyImage, int givenId, int state, float speedOfEnemy){
    
        speed = speedOfEnemy;
        id = givenId;
        enemy = enemyImage;
        enemyTypeState = (type)state;

    
}

//this changes the texture on a boundary change., the speed and id has already been set so it shouldnt need to happen again
void Enemy :: assignNewTexture(ofImage *enemyImage, int newEnemyState){
    
    enemy = enemyImage;
    enemyTypeState = (type)newEnemyState;
    
}

//this is all the type specific implementation per enemy
void Enemy :: typeSpecific(type enemyType){
    
    switch(enemyType){
            
        case china:
            
            yPos = 400;
            if(state == missed)
                speed = 12;
            break;
        
        case pac:
            yPos = 400;
            if(state == missed)
                speed = 12;
            break;
        
        case bee:
            
            bobbing += 0.1;
            yPos += sin(bobbing);
            if(state == missed)
                speed = 12;
            break;
        
        case nyan:
            yPos = 400;
            if(state == missed)
                speed = 12;
            break;
        
        case pirate:
            if(state != missed){
                yPos = 400;
            }
            
            if(state == superKilled || state == killed){
                state = killedAir;
            }
            
            if(state == missed){
                if((id == 1  && xPos < 520) || (id == 2 && xPos > 460)){
                    
                    speed*=0;
                    yPos += 6;
                
                }else{
                    speed = 4;
                }
            }
            
            
            break;
    }

}

//void Enemy :: setSpeed(type enemyType){
//    
//    switch(enemyType){
//            
//        case china:
//            
//            speed = 4;
//            
//            break;
//            
//        case pac:
//            speed = 4;
//            break;
//            
//        case bee:
//            speed = 4;
//           
//            break;
//            
//        case nyan:
//                speed = 4;
//            break;
//            
//        case pirate:
//                speed = 4;
//            break;
//    }
//}


//this class is given the particle vector(so that it can be filled), combo slash from hero(denotes the type of death animation, ie electricity)
void Enemy :: draw(vector<Particle> &particles, int comboSlash, bool paused){
    
    drawing = true;
    
//whenever uber killed, when in perfect range, id sent in to the air
    if(goingToBeKilled == true){
        //pauseLength++;
        
        if(hitValue == 1){
            state = killedAir;
            
            
//special kill is set to true, this allows the enemy to be scored and allows the hero to auto animate. however this causes a bug in which if the numberFound does not change, and special killed is set to true, the hero will attack with frame perfect attacks at nothing, but continue to increment the combo and scoreing for all the phantom hits
            
            specialKill = true;
            specialKillCounter++;
        }
        
//therefore this counter is set to 1, so that the enemy can be scored and attacked once, then set to noLongerCheckable. which has been added to the if statement in testApp.mm
        
        if(specialKillCounter > 1){
            noLongerCheckable();
            //cout<<"here"<<endl;
            
        }
    }
    
    
    //if(pauseLength > 20){
      //  pauseAnimation = false;
      //  state = killedAir;
    //}
    
    
    if(paused == false){
        if(pauseAnimation == false){
        
            typeSpecific(enemyTypeState);

                            
            if(counter % animationSpeed == 0){

                //animation going back and forth 
                if(state== walking){
                    animateX += animationSpacing;
                    if(animateX >100 || animateX < 100){
                        animationSpacing *=-1;
                    }       
            
                }
                
                //missed goes to fast attack animation
                if(state == missed){
            
                    if(finishedAttack == false){
                        animateX += animationSpacing;
                    }
                    if(animateX > 1300){
                
                        animateX = 0;
                        animationSpacing = 100;
                        finishedAttack = true;
                    }
            //then goes back to the walk animation but moves quickly 
                    if(finishedAttack == true){
                
                        animateX += animationSpacing;
                    
                        if(animateX >100 || animateX < 100){
                            animationSpacing *=-1;
                        }
                
                    }
          
            
                }
    
            }
    
            if(state == killed){
        
                if(runOnce == false){
                    //when killed initialises the emitter, the emitter adds particles to the particle vector
                    emitter = ParticleEmitter(bloodTexture);
                    
                    //moves enemy in the opposite direction
                    speed*=-2.0f;
                    speed*= 0.6f;
                    runOnce = true;
            
                }
                
                // only add particles when when on screen, too many enemies all adding particles lags the game
                if(enemyTypeState != pirate)
                {
                    if(xPos > -50 && xPos < 1024){
                        emitter.updatePosition(xPos, yPos);
                        emitter.addMoreParticles(particles);
                    }
                }
                
                //beings a counter
                killedCounter ++;
                
                //will be in this frame for 15 frames
                if(killedCounter < 15){
                        animateX = 700;
                
                //then afterwards this frame
                }else{
            
                    animateX = 800;
                    animateY = 0;
                }
            
            }
            
            //fired in to the air
            if(state == killedAir){
        
                if(runOnce == false){
                    
                    //add particle to emitter
                    emitter = ParticleEmitter(bloodTexture);
                    
                    //randomises the speed and then reverses the movement
                    speed*= ofRandom(2,4);
                    speed*= -1;
                    
                    runOnce = true;
                    
                    //chooses a height
                    randomHeight = ofRandom(100, 400);
                    //elongate time, for the higher flight, so it wont move super fast
                    speedOfFly = 30*(randomHeight/100);
            
                }
                
                //increments the counter so that it will cull the enemy
                killedCounter ++;
        
               
                //this is the values from zero to pi... so move up and down
                sineMovement += (PI/speedOfFly);
        
                // this moves the enemy up and down
                yPos = (400) - (sin(sineMovement)*randomHeight);
     
                //animate the enemy every 4 frames
                if(killedCounter % 4 == 0){
                    
                    animateX += 100;
                    
                    if(animateX > 600){
                        animateX = 200;
                    }
                }
            //when it comes back down and hits the "ground", movemnet stops, and so does the animation, flydeath = true, the counter increments and then from that will be culled
                if(yPos > 401){
            
                    yPos = 401;
                    speed*=0.0f;
                    animateX = 800;
                    animateY = 0;
                    flyDeath = true;
                }
                
                //adds particles to emitter
                if(enemyTypeState != pirate)
                {
                    if(xPos > -50 && xPos < 1024){
                        emitter.updatePosition(xPos, yPos);
                        emitter.addMoreParticles(particles);
                    }
                }
            }
    
            //begin the counter, so that it will be culled
            if(flyDeath == true){
                flyDeathCounter++;
            }
    
            // this is special animation, for lighting fire etc....
            if(state == superKilled){
                killedCounter ++;
                
                
                if(killedCounter % 2 == 0){
            
                    if(runOnce == false){
                        animateX = startXPos;
                        emitter = ParticleEmitter(bloodTexture);
                        
                        //give the right starting position for the elemental damage
                        superAnimateLocation(comboSlash);
                        
                        speed*=-2.0f;
                        speed*= 0.6f;
                        runOnce = true;
            
                    }
                
                    if(enemyTypeState != pirate)
                    {
                        //adds particles to the emitter
                        if(xPos > -50 && xPos < 1024){
                            emitter.updatePosition(xPos, yPos);
                            emitter.addMoreParticles(particles);
                        }
                    }
                    
                    //intrements and animates dependant on the element
                    if(killedCounter < 15){
                        animateX += 100;
                            
                        if(animateX > endXPos){
                            animateX = startXPos;
                        }
                
                    }else{
                        animateX = 800;
                        animateY = 0;
                    }
            
                }
            }
            
            //when frozen the enemy slides back until off the screen
            if(state == frozen)
            {
                if(runOnce == false){
                    
                    // goes bacwards off the screen quickly 
                    speed*=-2.0f;
                    speed*= 0.6f;
                    runOnce = true;
            
                }
        //it does not animate like all the other states
                animateX = 900;
        
            }
        
            //if missed runs off to the other side of the screen before being culled, attack animation
            if(state == missed){
        
                if(runOnce == false){
                    
                    animationSpacing = 100;
                    //checkable = false;
                    animateX = 900;
                    runOnce = true;
                    animationSpeed = 5;
                    speed *= 3;
                }

            }
    
            
            counter++;
            
            if(counter > 106){
                counter = 0;
            }
        }
    }
    
    //draw the triangle and changes the colour dependant on hitValue
    if(displayIndicator && checkable == true){
        
        if(hitValue == 0){
            red = 255;
            green = 255;
            blue = 255;
        
        }else if(hitValue == 1 || hitValue == 2 || hitValue == 3){
       
            red = 102;
            green = 255;
            blue = 102;
        }else{
            
            red = 255;
            green = 102;
            blue = 102;
            
        }
        
            ofSetColor(red, green, blue);
        
        
        if(id == 1){
            ofTriangle(xPos + 12, yPos-40, xPos + 12 + 30 ,yPos-40, xPos +12 + 15, yPos - 40 + 30);
        }
        
        if(id == 2){
            ofTriangle(xPos+8, yPos-40, xPos+8 + 30 , yPos -40, xPos +8+15, yPos -40+30);
        }
    }
    
    ofSetColor(255, 255, 255);
    
    //draws the enemy, -100 is to mirror on the x axis
    
    if(id == 1){
        enemy->drawSubsection(xPos+80, yPos -50, -100, 100, animateX, animateY, 100, 100);


    }else{

        enemy->drawSubsection(xPos-30, yPos -50, 100, 100, animateX, animateY, 100, 100);

    }
    
//    
//	if(id == 1){
//		ofLine(xPos, 0, xPos, ofGetHeight());
//	}
//
//	if(id == 2){
//		ofLine(xPos+spriteWidth, 0, xPos+spriteWidth, ofGetHeight());
//	}
    
    
}
    
void Enemy:: updateTheSecondHit(){
    //when updateing the second hit, it forms a second beat position which move at the same pace, so that when the enemy starts moving back it will move back until its position and the second hits postion are teh same before starting to move forward once again
    
    if(id == 1){
         secondXPos =  (1024/2)+(50/2) + (speed*180)+ (perfect/2);
        
    }else if(id == 2){
         secondXPos =   (1024/2)-(50/2)-spriteWidth-(speed*180) - ((perfect/2));
    }
    
    
    twohit = true;
   
}

void Enemy :: move(int theFrameRate){
    
    //move the xposition dependant on the id, + or - dependant on the xPos
    if(pauseAnimation== false){
    if(state == walking){
        
        checkable = true;
        
        if(id == 1){
            xPos -= speed;
        
        
        }else if(id == 2){
            xPos +=speed;
        }
    }
    
    
    if(state != secondHitBackSlide && state != walking){
        if(id == 1){
            xPos -= speed;
            
            
        }else if(id == 2){
            xPos +=speed;
        }
    }
    
        //slides backwards until the position, is the same as the movement or the second beat... and then acts as though it is a normal beat
    if(state == secondHitBackSlide){
        
        checkable = false;
        
        if(id == 1){
            xPos+=2;
            if(xPos > secondXPos){
                //sets itself to walking
                state = walking;
                //and does not have a second hit in the enemy 
                twohit = false;
            }
            
            
        }else if(id == 2){
            xPos-=2;
            
            if(xPos < secondXPos){
                state = walking;
                twohit = false;
            }
        }
    }
    
    
    
    if(twohit == true){
        if(id == 2){
            secondXPos += speed;
        }else{
            secondXPos -= speed;
        }
    }
    
    
    
    //will set itself to missed, if it walks past the half way point
    
        if(id == 1 && xPos < ofGetWidth()/2 + spriteWidth/2 && state == walking){
            
            if(twohit == false){
                state = missed;
            }else{
                state = secondHitBackSlide;
            }
            
        }else if(id == 2 && xPos+spriteWidth > ofGetWidth()/2 - spriteWidth/2 && state == walking){
            
            if(twohit == false){
                state = missed;
            }else{
                state = secondHitBackSlide;
            }
            
        }
    }
   
}


//run on every enemy if in time the missed value becomes false so that it can be counted for points later. 
bool Enemy :: buttonHit(int buttonId){
    
	
	if(id == buttonId && hitValue != 0){
        
       //sets the state of the enemy so that it will animate in the corrent way when checked
            if(hitValue == 1){
                if(twohit == false){
                    state = superKilled;
                }else{
                    state = secondHitBackSlide;
                }
            }else if(hitValue == 2){
                if(twohit == false){
                    state = killedAir;
                    animateX = 300;
                }else{
                    state = secondHitBackSlide;
                }
            }else{
                if(twohit == false){
                    state = killed;
                }else{
                    state = secondHitBackSlide;
                }
            }
        
        //this bascially say that the player has pressed the button in the right direction
		return true;
	}else{
        return false;
    }
	
}

bool Enemy :: remove(){
   
    //this tells the vector in the testapp.mm to remove the enemy objects from the vector, each is a different state and a different conditions before the it is culled
    
    if(state == killed && killedCounter > 30){
       
        return true;
    }else if(flyDeathCounter > 20){
        
        return true;
        
    }else if(state == superKilled && killedCounter > 30){
            
            return true;
        
    }else if(state == frozen &&  (xPos < -100 || xPos > 1100)){
        
        return true;
        
    }else if((id == 1 && xPos < -100) || yPos > 764){
        return true;
        
    }else if ((id == 2 && xPos > 1100) || yPos > 764 ){
        return true;
    }else{
        return false;
        
    }
    
}


int Enemy :: inRange(int initialSpace, int perfect, int great, int ok, int sloppy, int poor){

    //this works out where the the enemy is on screen, if in range. and the hit value is worked
	int distance;

	if(id == 1){
		distance = xPos-(ofGetWidth()/2+(heroWidth/2));
	}else if(id == 2){
		distance = (ofGetWidth()/2-(heroWidth/2))-(xPos + 50);
	}

    
	if(distance <= perfect+initialSpace && distance > initialSpace){
        hitValue = 1;
	}else if(distance <= great+perfect+initialSpace && distance > perfect+initialSpace){
		hitValue = 2;
	}else if(distance <= ok + great + perfect +initialSpace && distance > great+perfect+initialSpace){
		hitValue = 3;
	}else if(distance <= sloppy +ok+ great + perfect +initialSpace && distance > ok + great+perfect+initialSpace){
		hitValue = 4;
	}else if(distance <= (poor-3) + sloppy +ok + great + perfect + initialSpace && distance > sloppy + ok+ great+perfect+initialSpace){
		hitValue = 5;
	}else{
        //if not in range or missed
        hitValue = 0;
    }
    
    return hitValue;
	
}
//if on the left or right of the screen
int Enemy :: getId(){
    return id;
}

//whether the enemy should check collision
bool Enemy :: returnCheckable(){

    return checkable;
}

//this is set to false, so no longer checkable
void Enemy :: noLongerCheckable(){
    if(twohit ==false){
        checkable = false;
    }
}

// return the type of enemy

int Enemy :: returnState(){
    return (int)state;
}

//resets everything
void Enemy :: reset(){
    
    id = 0;
    state = walking;
	checkable = true;
    hitValue = 0;
    animateX = 0;
    animateY = 0;
    counter = 0;
    killedCounter = 0;
    runOnce = false;
    animationSpacing = 100;
    animationSpeed = 7;
    finishedAttack = false;
    sineMovement = 0;
    flyDeath = false;
    flyDeathCounter = 0;
    yPos = 400;
    
    bobbing = 0; 
    speed = 4;
    drawing = false;
    pauseAnimation = false;
    pauseLength = 0;
    goingToBeKilled = false;
    displayIndicator = false;
    specialKill = false;
    specialKillCounter = 0;
}


//draws the enemy in the right position on the left or the right side of the screen, 
void Enemy ::drawOnScreen(int givenId){
    
    id = givenId;
    
    
    if(id == 1){
        xPos = (1024/2)+(50/2) + (speed*180)+ (perfect/2);
        
    }else if(id == 2){
        xPos =  (1024/2)-(50/2)-spriteWidth-(speed*180) - ((perfect/2));
    }
    
}

//this finds the type of elemental damage that the enemy takes and decides how it should animate, dependant on the comboslash, from the hero class. comboslash worked out by the division of the current combo by the slashthrest (the amount of kills needed upgrade a sword)

void Enemy :: superAnimateLocation(int comboSlash)
{
    if(comboSlash != 0){
        
        animateY = 100;
    
        if(comboSlash == 1)
        {
            

            startXPos = 0;
            animateX = startXPos;
            endXPos = 100;
        }
        else if(comboSlash == 2)
        {
            
            startXPos = 500;
            animateX = startXPos;
            endXPos = 600;
        }
        else if(comboSlash == 3)
        {
            
            startXPos = 200;
            animateX = startXPos;
            endXPos = 300;
        }
        else if(comboSlash == 4)
        {
            
            startXPos = 700;
            animateX = startXPos;
            endXPos = 800;
        }
        else if(comboSlash == 5)
        {
            
            state = frozen;
            
            
        }
        else if(comboSlash == 6)
        {
            
            startXPos = 1000;
            animateX = startXPos;
            endXPos = 1100;
        }
    }
    else
    {
        state = killed;
        
    }
    
}

//returns whether it is a special kill and whether the score should be counted
bool Enemy:: returnSpecialKilled(){
        return specialKill;
}

//uberkilled called to have the enemy auto kill 
void Enemy :: uberKilled(){
    
    goingToBeKilled = true;
    typeSpecific(enemyTypeState);

}

void Enemy :: notUberKill(){
    goingToBeKilled = false;
}

// set whether the arrow should be drawn overhead
void Enemy :: displayAttackable()
{
    displayIndicator = true;
    
}



