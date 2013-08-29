/*
 LICENCE
 
 The Software is and remains the property of Mick Grierson
 ("mgrierson") The Licensee will ensure that the Copyright Notice set
 out above appears prominently wherever the Software is used.
 
 The Software is distributed under this Licence:
 
 - on a exclusive basis,
 
 - "AS-IS", mgrierson makes clear that no condition is made or to be
 implied, nor is any representation or warranty given or to be
 implied, as to (i) the quality, accuracy or reliability of the
 Software; (ii) the suitability of the Software for any particular
 use or for use under any specific conditions; and (iii) whether use
 of the Software will infringe third-party rights.
 
 mgrierson disclaims:
 
 - all responsibility for the use which is made of the Software; and
 
 - any liability for the outcomes arising from using the Software.
 
 The Licensee agrees to indemnify mgrierson and hold them
 harmless from and against any and all claims, damages and liabilities
 asserted by third parties (including claims for negligence) which
 arise directly or indirectly from the use of the Software or any
 derivative of it or the sale of any products based on the
 Software. The Licensee undertakes to make no liability claim against
 any employee, student, agent or appointee of mgrierson, in connection
 with this Licence or the Software.
 
 No part of the Software may be reproduced, modified, transmitted or
 transferred in any form or by any means, electronic or mechanical,
 without the express permission of mgrierson. The conditions of this
 Licence are imposed upon the receiver of the product. You
 may be held legally responsie for any copyright infringement that is
 caused or encouraged by your failure to abide by these terms and
 conditions.
 
 You are not permitted under this Licence to use this Software
 commercially. Use for which any financial return is received shall be
 defined as commercial use, and includes (1) integration of all or part
 of the source code or the Software into a product for sale or license
 by or on behalf of Licensee to third parties or (2) use of the
 Software or any derivative of it for research with the final aim of
 developing software products for sale or license to a third party or
 (3) use of the Software or any derivative of it for research with the
 final aim of developing non-software products for sale or license to a
 third party, or (4) use of the Software to provide any service to an
 external organisation for which payment is received. If you are
 interested in using the Software commercially, please contact mgrierson to
 negotiate a licence.
 */


#ifndef _TEST_APP
#define _TEST_APP

//#define DLIB_USE_BLAS
//#define DLIB_USE_LAPACK

#include "ofMain.h"
#include "ofxMaxim.h"
#include <sys/time.h>

#include "maxiMFCC.h"
#include "vectorDelay.h"
#include "debugGraph.h"
#include "movingAverage.h"
#include "movingRMS.h"
//#include <deque>
#include "beats.h"
#include "boundarySegmentation.h"
#include "beatModel.h"
#include "medianTrigger.h"
#include "basicTrigger.h"
#include "pyOSCDebug.h"
#include "btAlignedObjectArray.h"
#include "Hero.h"
#include "enemy.h"
#include <vector>
#include "RangeLines.h"
//#include <sstream>
//#include <string>
//#include <locale>
//#include <iomanip>
#include "Font.h"

#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "circularBuffer.h"
#include "TargetConditionals.h"
#include <sstream>

#include "GUI.h"


#import "mediaStreamer.h"
#import "simulatorMediaStreamer.h"

#import <MediaPlayer/MPMusicPlayerController.h>
#import <MediaPlayer/MPMediaPickerController.h>

#include "MediaPickerView.h"
#include "enemyObjectPool.h"
#include "slashScreen.h"
#include "enemyTextureAssign.h"
#include "background.h"

#include "boundaryChangeFlash.h" 
#include "uberKilled.h"
#include "ComboBroken.h"
#include "TextOnSceen.h"
#include "DecideWaitTime.h"
#include "CountIn.h"
#include "HighScore.h"
//ObjectPool* ObjectPool :: instance = 0;


using namespace std;
//using namespace dlib;

static const uint mfccCount = 13;


namespace bastardConstants {
	const string audioFileName = "tmp.raw";
};



class testApp : public ofxiPhoneApp{
	
    public:
    
    //~testApp();/* deconsructor is very useful */
        void setup();
        void update();
        void draw();
        void exit();
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
    
    //these two had * after the float...just to make sure i remember
    void audioRequested 	(float* input, int bufferSize, int nChannels); /* output method */
	void audioReceived 	(float* input, int bufferSize, int nChannels); /* input method */
	//void audioRequested(float*, int, int);
    
    float 	* lAudioOut; /* outputs */
	float   * rAudioOut;
    
	float * lAudioIn; /* inputs */
	float * rAudioIn;
	
	int		initialBufferSize; /* buffer size */
	int		sampleRate;
	
    /* stick you maximilian stuff below */
	
	double wave,sample,outputs[2], ifftVal;
	maxiMix mymix;
	maxiOsc osc;
	
	float *ifftOutput;
	int ifftSize;
	
	ofxMaxiIFFT ifft;
	ofxMaxiFFT mfft;
	int fftSize;
	int bins, dataSize;
	
	float callTime;
	timeval callTS, callEndTS;
	
	maxiMFCC mfcc;
	double *mfccs;
    
	maxiSample samp;
    
    std::vector<float*> buffers;
    //	std::vector<double*> mfccFrames;
    //	typedef std::vector<float> floatVector;
    //	typedef std::vector<double> doubleVector;
	std::vector<floatVector> fftFrames;
	vectorDelay<floatVector> fftFrameDelay;
	vectorDelay<doubleVector> mfccDelay, melBandsDelay;
	vectorDelay<float> flatnessStream, trigStream, avgMFCCVarStream, sigBeatStream;
	vectorDelay<string> sigBeatLabelStream;
	vectorDelay<double> avgAmpStream, mfccDistStream, onsetThreshStream, /*beatVarianceStream,*/ trackVarianceStream, boundarySegThreshStream;
    //	vectorDelay<doubleVector> mfccBeatAvgs;
	vectorDelay<float> /*boundaryStream,*/ actualBoundaryStream, spectralFluxStream;
	unsigned int bufferCount;
	unsigned long currentBuffer;
    
    
    //Use this to change the buffer length ------------------------------/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/----------/-/-/-/-/-/-/-/--
	static const float readAheadTime = 3.0; //seconds
    
    
	bool playBackStarted;
	vectorDelay<float> anythingStream, anythingStream2;
    //	deque<int> beatSizes;
    //	int boundarySegWindowSize;
	
	doubleVector melBandAverage, melBandTmp, melBandDiff;
	
	//clustering
	
    //    std::vector<sample_type> samples;
    //    std::vector<sample_type> initial_centers;
    //
    //    sample_type m;
	debugGraph thresholdGraph, distGraph2, onsetGraph, flatnessGraph, ampGraph, /*beatVarianceGraph, */
    trackVarianceGraph, /*boundariesGraph,*/ actualBoundariesGraph, avgMFCCVarGraph, boundarySegThreshGraph, sigBeatGraph, fluxGraph,
    anythingGraph, anythingGraph2;
	
    //	sequentialKMeans *km;
	movingAverage *mfccVarMA, *flatnessMA;
    //	movingAverage *avgAmpMA;
	movingRMS *ampRMS;
	movingAverage fluxMA, fluxLTMA;
	
    //	double /**lastBeatAvg,*/ *thisBeatAvg;
    //	unsigned int beatFrameCount;
	
	beatDetector beats;
	boundarySegments *boundaries;
	
	ofTrueTypeFont font;
	vector<float> lastFFT, lastMel;
	
	bool paused;
	
	basicTrigger fluxTrigger;
	beatModel bt;
	
    
	
	pyOSCDebug oscd;
	
	btAlignedObjectArray<float> fluxSignature;
	
	vector<float> beatTimes;
	float sampleLen;
	
	float currentFlux, lastFlux,waitTime;
    maxiEnvelopeFollower envf,envf2;
    
    double beepAmp,beepOut;
    maxiOsc beep;
    maxiDelayline myDelay;
    
    
//-------More of Micks code from another example that he sent
    
    void startStream(mediaStreamer *stream);
	
	enum appStates {
		INIT,CHOOSINGTARGET, MASHUP
        
        
	} appState;
    
    
    //audio
#if !TARGET_IPHONE_SIMULATOR
	mediaStreamer *targetStream;
#else
	simulatorMediaStreamer *targetStream;
#endif
	btAlignedObjectArray<float> audioFrame;
	btAlignedObjectArray<float> data;
	circularBuffer<float> orgTargetQueue;
	ifstream sourceFile;
	uint sourceFileSize;
	string sourceFileName;
    
    //for testing
	maxiSample crackle;
	maxiFilter hip;
    
    static const uint frameSize = 1024;


    
private://---------this is all the stuff i added , the rest of it i have no clue Sorry
    
    float scalarX, scalarY;
   
    //to reset and return to the mainmenu
    void mainMenuReset();
    //to instant restart the song in the pause menu
    void restart();
    //generic reset stuff
    void reset();
	Hero* hero;
    
    
   enemyObjectPool* pool = enemyObjectPool::getInstance();
    
    
    std :: vector<Enemy*>enemyPointerVector;
    std :: vector<ofImage*> enemyImages;
    std::vector<Particle> particleVector;
    std::vector<int> timingVector;
    
    
	int counter;
	bool buttonPressed;
    
	RangeLines *rangeLines;
    
	int singleFrameCounter;
	int buttonId;
	bool runOnce;
    
    float speedOfEnemyMovement;
    bool beatTrig2;
    
    int initialSpace;
    int perfectRange;
    int greatRange;
    int okRange;
    int sloppyRange;
    int poorRange;
    
    float avgBeatSize;
    float flux;
        
    int combo;
    string theHit; // the printing of string on to the screen, ie  perfect, great, okay, etc, 
   
    double score;
    bool boundaryChange;
    
    //frame count for the boundary change
    int noBoundaryChange;
    int enemyState;
    
    //to find the next enemy to hit
    bool foundIt;
    int numberFound;
    
    enum deviceTypes{iPad, iPhone, other};
    deviceTypes device;
    int screenW, screenH;
    float percW, percH;
    enum state{startScreen, songSelect, playing, highScore, resetted, stats, results, highScoreDisplay};
    state currentState;
    bool nextMode;
    int red;
    int green;
    int blue;

    
    //this size of the hitboxes for the stats and the start buttons
    int startXPos;
    int startYPos;
    int startWidth;
    int startHeight;
    
    int statsXPos;
    int statsYPos;
    int statsWidth;
    int statsHeight;
    
    int statBackX;
    int statBackY;
    int statBackWidth;
    int statBackHeight;
    int slashThresh;
    
    SplashScreen* splash;
    
    //all the images required
    ofImage menuScreen;
    ofImage pauseOverlay;
    ofImage pauseButton;
    ofImage difficultyImage;
    ofImage resultsPage; 
    
    int currentBoundary;
    int buffersLeft;
    enemyTextureAssign * assign;
    Background * background;
    int numberOfTextures;
    int randomBackGround;
    BoundaryChangeFlash *boundaryFlash;
    int lastBackGround;
    int waitingTime;
    int shouldAddLeft;
    int shouldAddRight;
    
    bool addLeft;
    bool addRight;
    
    int missedCounter;
    
    UberKilled* uberkilled;
    bool songselect;
    bool hasRunOnce;
    ComboBreak* comboBroken;
    TextOnScreen* text;
    
    float perfectCounter;
    float greatCounter;
    float okayCounter;
    float sloppyCounter;
    float poorCounter;
    float maxComboCounter;
    float totalBeats;
    
    bool selectDifficulty;
    bool begintimer;
    int difficultyCounter;
    DecideWaitTime* decideWaitTime;
    CountIn *countIn;
    bool indicatorOn;
    int totalNumberOfButtonPresses;
    bool shouldScore;
    bool chosen;
    
    NSString *artist;
    std::string *convertArtist;
    
    NSString *song;
    std::string *convertSong;
    
    int typeOfHit;
    bool ableToAttack;
    string difficulty;
    int lastSpecialKilled;
    HighScore *highScores;
    bool updateHighScore;
     std::string *convertDir;
    
};
#endif

