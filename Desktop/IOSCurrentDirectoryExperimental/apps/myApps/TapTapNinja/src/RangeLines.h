#ifndef rangeLines_h
#define rangeLines_h
#include "ofMain.h"
//use this just to show the ranges of perfect, great, ok, sloppy and poor, just a balancing tool 

class RangeLines{

public:
	RangeLines();
	~RangeLines();
	void draw(int,int,int,int,int,int,int);

private:
    
    int screenWidth;
    int screenHeight;

};
#endif