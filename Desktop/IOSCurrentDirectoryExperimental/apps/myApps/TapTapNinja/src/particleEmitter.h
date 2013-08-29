//
//  particleEmitter.h
//  emptyExample
//
//  Created by John Ribbins on 14/06/2013.
//
//this class is a particle emitter class which is created in every enemy object. This adds particles to particle vector universal to all the enemy objects, the particles themselves are in a seperate class. 

#ifndef emptyExample_particleEmitter_h
#define emptyExample_particleEmitter_h
#include "ofMain.h"
#include "particles.h"


class ParticleEmitter{

public:
    ParticleEmitter();
    ParticleEmitter(ofImage* blood);
   
    ~ParticleEmitter();
    
    //void initialiseParticles(ofImage*);
    
    void updatePosition(int,int);
    void addMoreParticles(vector<Particle>&);
    void drawParticles();
    void moveParticles();
    int returnCounter();
    
    
    
private:
    
    //std:: vector<Particle*>particleVector;
    ofImage* bloodTexture;
    
    int xPos;
    int yPos;
    int particlesEmitted;
    int counter;
    
};
#endif
