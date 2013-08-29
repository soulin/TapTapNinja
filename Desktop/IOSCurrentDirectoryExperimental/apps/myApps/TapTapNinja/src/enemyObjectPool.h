//
//  ObjectPool.h
//  BeatFighter
//
//  Created by John Ribbins on 03/07/2013.
//
//

//code for this found here https://gist.github.com/astro-/1124832
//object pool which procreates 10 object to be reused, when more are required it will create them as necessary

#ifndef __BeatFighter__enemyObjectPool__
#define __BeatFighter__enemyObjectPool__

#include <iostream>
#include <string>
#include <list>
#include "enemy.h"

class enemyObjectPool{
    
    private:
        list <Enemy*> resource;
        static enemyObjectPool* instance;
        enemyObjectPool(){}
        ofImage blood;
    
    
    public:
    
        //std::vector<ofImage>enemyImages;
        static enemyObjectPool* getInstance();
        Enemy* getResource();
        void returnResource(Enemy* enemy);
        void deleteObjects();
        void init();
        int noTextures;
        void numberOfTextures(int);

    
};

#endif /* defined(__BeatFighter__ObjectPool__) */
