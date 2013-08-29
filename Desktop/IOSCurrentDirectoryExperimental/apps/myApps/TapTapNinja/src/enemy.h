#ifndef enemy_h
#define enemy_h

#include "ofMain.h"
#include "particleEmitter.h"
class Enemy{

public:
	
	Enemy();
	Enemy(int, int, ofImage*);//, ofImage*, ofImage* /*,int,int,int*/);
	~Enemy();
    
	void draw(vector<Particle>&, int, bool);
	void move(int);
    
//this return if and where in range to be killed
    int inRange(int, int, int, int, int, int);
//this checks whether the button has been hit, and from that where in range from the above funtion to set its animation state
	bool buttonHit(int);
    bool remove();
    
//required for the hero class;
    int getId();
    
//this is whether the enemy is checkable or not,this is because the enemy object animates itself, therefore is on screen for longer than required
    void noLongerCheckable();
//this return whether it has been checked/collided before. ie if missed or hit
    bool returnCheckable();
    
    enum currentStates{walking, killed, strike, killedAir, deathFloor, walkOff,missed,superKilled, frozen, secondHitBackSlide};
    currentStates state;
    int returnState();
    void reset();
    
//this sets up the enemy in the right position
    void drawOnScreen(int);
    void assignTexture(ofImage*, int, int, float);
    void assignNewTexture(ofImage*, int);
    
//this sets up the second hit timing, being set at the time the next offset is found
    void updateTheSecondHit();

//set up the autokill
    void uberKilled();
    void notUberKill();

//shows the arrow above the head of the to kill enemy
    void displayAttackable();
    
//this returns true when it has been super killed, so that the hero knows when to animate
    bool returnSpecialKilled();
    

private:
    
//sets the initiallocation of the special animation, such as lightening damage or fire damage, ice etc
    void superAnimateLocation(int);
//whether it shows up on the left or right, left = 2, right = 1
	int id;
	float xPos;
	float yPos;
//used for the second hit
    float secondXPos;

	int spriteWidth;
	int heroWidth;
    int perfect;
	bool checkable;
    float speed;
    ofImage *enemy;
//x and y location on the image themselves
    int animateX;
    int animateY;
//in what range the enemy is in when hit
    int hitValue;
    int counter;
//this decided when the object should be removed
    int killedCounter;
    bool runOnce;
    int animationSpacing;
    int animationSpeed;
    
    bool finishedAttack;
    float sineMovement;
    int randomHeight;
    int speedOfFly;
//this starts the counter so that the flying will animate, and cull itself
    bool flyDeath;
    int flyDeathCounter;
//the same vector
    ParticleEmitter emitter;
    ofImage* bloodTexture;
//this is where the animation will start, ie for electric attack, or fire attack, and where it ends 
    int startXPos;
    int endXPos;
//sine movement
    float bobbing;
    bool drawing;
    enum type{china, pac, bee, nyan, pirate};
    type enemyTypeState;
    void typeSpecific(type);
    
    bool twohit;
//sets up the killing after the second hit
    bool goingToBeKilled;
//used for uberkilled animation
    bool pauseAnimation;
//time it takes
    int pauseLength;
//whether a arrow should be drawn over head
    bool displayIndicator;
//colour the arrow
    int red;
    int green;
    int blue;
//allows the enemy to return its state for 1 frame, that it has been killed by the uber attack so that it can be scored. 
    bool specialKill;
    int specialKillCounter;
    
};
#endif