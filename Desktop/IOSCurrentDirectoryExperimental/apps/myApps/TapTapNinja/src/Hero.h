#ifndef Hero_h
#define Hero_h

#include "ofMain.h"
#include "SwordSwipe.h" 

// this is the hero class, which completes all the animation and the sword swipes, the enemy also looks at this class to know how to animate dependant on the type of sword the hero is using. 

class Hero{

public:

	Hero();
	~Hero();
    
    
	void loadSpriteSheet(string, int, int , int);
	void drawHero(bool paused);
    void drawSword();
	void animate(int);
	void keyRelease();
	
    void heroAnimate(int);
    void chooseSwordSwipe(int, int, int);
    
    int returnComboSlash();
    
    void uberKill();
    void reset();
    
    //void assignTextures(ofImage*, ofImage*, ofImage*, ofImage*);
    

private:
    //images, where they should go
    ofImage* swordImage;
    ofImage* heroImage;
    
	ofImage hero;
    ofImage sword;
   
	int xPos; 
	int yPos;
    
	int spriteWidth;
	int spriteHeight;
    
    //how sprites animate
	int animateX;
    int animateY;
    
    int counter;
    
    bool facingLeft;
    
    bool animationOver; 
    int animationCounter;
    int currentAnimationLimit;
    void howToAnimate(int);
    int comboSlash;
    int swordX;
    int swordY;

    int randomSlash;
    bool slashOnce;
    
    int animationSpeed;
    int fastAni;
    int slowAni;
    
    enum state{stationary, attacking, recovering};
    state currentState;
    int loadSet;
    
    vector<SwordSwipe> swordImages;
    
    bool gamePaused;
    bool fillWithSwords;
    bool once;
    int uberKillCounter;
    bool uberKilling;
    
    

};
#endif