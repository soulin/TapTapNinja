//
//  particles.h
//  emptyExample
//
//  Created by John Ribbins on 14/06/2013.
//
//a particle class for the blood, may need to load multiple blood textures for the multiple enemies and the types of blood they would bleed 

#ifndef emptyExample_particles_h
#define emptyExample_particles_h
#include "ofMain.h"

class Particle{
    
public:
    
    Particle();
    Particle(int, int, ofImage*);
    ~Particle();
    
    void move();
    void draw(bool);
//used for the splating on screen
    void splat();
//decides when to cull the objects
    bool isOnScreen();
    
    
    
private:

    float xPos;
    float yPos;
//initial velocities
    float xMovement;
    float yMovement;
    float gravity;
//presplat
    int initialBloodSplat;
//the splat when splatted
    int finalBloodSplat;
    enum state{falling, splatted};
    state currentState;
//used to decide when it is able to splat. as otherwise it will not fly enough before splatting
    char splatTimer;
    bool runOnce;
    bool remove;
    ofImage* bloodTexture;
    int alphaChange;
//where on the sprite sheet
    int sheetX;

};


#endif
