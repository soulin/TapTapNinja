#include "ofMain.h"
#include "testApp.h"

int main(){
	ofSetupOpenGL(960 ,640, OF_FULLSCREEN);			// <-------- setup the GL context
    
    //iPhoneGetOFWindow() -> setOrientation(OF_ORIENTATION_90_RIGHT);
	ofRunApp(new testApp);
}
