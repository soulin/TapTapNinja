//
//  ObjectPool.cpp
//  BeatFighter
//
//  Created by John Ribbins on 03/07/2013.
//
//

#include "enemyObjectPool.h"

enemyObjectPool* enemyObjectPool :: instance = 0;


enemyObjectPool* enemyObjectPool :: getInstance(){
    if(instance == 0){
        instance = new enemyObjectPool;
    }
    return instance;
}

Enemy* enemyObjectPool :: getResource(){
    if(resource.empty()){
        //cout << "Creating new" << endl;
        return new Enemy(50, noTextures, sX, sY, &blood);
    }else{
        //cout << "reuse existing" << endl;
        Enemy* enemy = resource.front();
        resource.pop_front();
        return enemy;
    }
}

void enemyObjectPool :: returnResource(Enemy* enemy){
    enemy -> reset();
    resource.push_back(enemy);
}

void enemyObjectPool:: deleteObjects(){
    while (resource.size()> 0){
        //delete[] resource;
        resource.pop_front();
        
    }
}

void enemyObjectPool:: init(){
    for(int i = 0; i < resource.size(); i ++){
        resource.push_back(new Enemy(50, sX, sY, noTextures, &blood));
    }
    blood.loadImage("bloodsplats.png");
}
void enemyObjectPool::setInfo(float scalarX, float scalarY){
    sX = scalarX;
    sY = scalarY;
}

void enemyObjectPool :: numberOfTextures(int numberTex){
    noTextures = numberTex;
}



