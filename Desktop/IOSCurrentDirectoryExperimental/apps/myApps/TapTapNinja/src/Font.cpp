//
//  Font.cpp
//  emptyExample
//
//  Created by John Ribbins on 11/06/2013.
//
//

#include "Font.h"


Font :: Font(){
    fonting.loadFont("verdana.ttf", 32, false, true, false);
    
}

Font :: ~Font(){}

void Font :: print(string words){
    
    fonting.drawString(words, 100,100);

}