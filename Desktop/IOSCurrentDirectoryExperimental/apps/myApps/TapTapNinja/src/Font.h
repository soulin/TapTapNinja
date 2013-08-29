//
//  Font.h
//  emptyExample
//
//  Created by John Ribbins on 11/06/2013.
//
//

#ifndef emptyExample_Font_h
#define emptyExample_Font_h
#include "ofMain.h"


class Font{

public:
    
    Font();
    ~Font();
    void print(string);
    

private:
    int counter;
    ofTrueTypeFont fonting;
    
};


#endif
