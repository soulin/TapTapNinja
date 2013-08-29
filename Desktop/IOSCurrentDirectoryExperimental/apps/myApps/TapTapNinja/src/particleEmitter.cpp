//
//  particleEmitter.cpp
//  emptyExample
//
//  Created by John Ribbins on 14/06/2013.
//
//

#include "particleEmitter.h"

ParticleEmitter :: ParticleEmitter(){}

ParticleEmitter :: ParticleEmitter(ofImage* blood){
    
    xPos = 0;
    yPos = 0;
    particlesEmitted = 0;
    bloodTexture = blood;
    
    
}

ParticleEmitter :: ~ParticleEmitter(){

}

//the particle emitter moves with the enmy, and is in a relativly centralised position
void ParticleEmitter::updatePosition(int enemyXPos, int enemyYPos){
    
    xPos = enemyXPos + 10;
    yPos = enemyYPos + 10;
    
}
//the particles are given the location of the emitter and from that point the particles will move independantly from randomly chosen velocities.

void ParticleEmitter::addMoreParticles(vector<Particle>& particleVector){
    
    counter++;
//too much blood will make the game lag horrendously
    if(counter % 4 == 0){
        for(int i = 0; i < 3; i++){
            particleVector.push_back(Particle(xPos, yPos, bloodTexture));
        }
    }
}
