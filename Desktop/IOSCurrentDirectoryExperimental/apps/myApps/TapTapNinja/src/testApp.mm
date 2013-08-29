#include "testApp.h"
#include "maximilian.h"/* include the lib */
#include "time.h"
#include "vectorUtils.h"


int mouseX=1;
int mouseY=1;


struct testSample {
	testSample(){};
	testSample(string a, string b):name(a),filename(b){};
	string name;
	string filename;
};


//--------------------------------------------------------------
void testApp::setup(){
    
    //determine if iPad or iPhone
    if ([([UIDevice currentDevice].model) isEqualToString:@"iPad"]){
        printf("Device detected is iPad\n");
        device = iPad;
    } else if ([([UIDevice currentDevice].model) isEqualToString:@"iPhone"]){
        printf("Device detected is iPhone\n");
        device = iPhone;
    } else {
        printf("Device detected is none of the above! Oh God, Panic!\n");
        device = other;
    }
    
    //You gotta swap around the height and the width. It's because talking straight to the UIScreen might be TOO accurate!
    
    screenW = (int)[[UIScreen mainScreen]bounds].size.height;
    screenH = (int)[[UIScreen mainScreen]bounds].size.width;
    
    printf ("Screen Width: %d, Screen Height: %d.\n", screenW, screenH);
    
    /* some standard setup stuff*/
	ofEnableAlphaBlending();
	//ofSetupScreen();
	ofBackground(255);
	ofSetWindowTitle("Beat Fighter Test");
	ofSetFrameRate(60);
    
    
    //load in the images for the game.
    menuScreen.loadImage("MAIN_MENU.png");
    pauseOverlay.loadImage("PAUSE_MENU.png");
    pauseButton.loadImage("PAUSE_BUTTON.png");
    difficultyImage.loadImage("SET_DIFFICULTY.png");
    resultsPage.loadImage("RESULTS.png");
    
    //Using the Menu Screen as a reference, we find out the scaling from iPad res to iPhone.
    scalarX = (float)menuScreen.width/(float)screenW;
    scalarY = (float)menuScreen.height/(float)screenH;
    
    //ofScale(scalarX, scalarY);
    
    /*
    //Resize all the stuff that's full screen.
    menuScreen.resize(screenW, screenH);
    pauseOverlay.resize(screenW, screenH);
    difficultyImage.resize(screenW, screenH);
    resultsPage.resize(screenW, screenH);
     */
    switch (device) {
        case iPad:
            
            break;
        case iPhone:
            
            break;
        default:
            break;
    }
    
    //Bracketed so I get get Mick's shiz out of the way.
    {
//-----------new Mick Code--------------------------
    
    // register touch events
	ofRegisterTouchEvents(this);
    
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
    
    //data.resize(maxiSettings::bufferSize, 0);
    
    audioFrame.resize(maxiSettings::bufferSize, 0);
    
    int queueSize = 100000;
	orgTargetQueue.resize(queueSize);
	
	crackle.load(ofToDataPath("crackle.wav"));
	
    //UInt32 x=1;
	ofSoundStreamSetup(2,0,this, maxiSettings::sampleRate, maxiSettings::bufferSize, 4);
	UInt32 audioCategory = kAudioSessionCategory_MediaPlayback; //for output audio //kAudioSessionCategory_PlayAndRecord; //both input and output
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory,sizeof(audioCategory),&audioCategory);
	UInt32 v = 1;
	AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryMixWithOthers, sizeof (UInt32), &v);
	
	appState = INIT;

    
//----------------------------------------------------
    
    waitTime = 0;
	
    //	oscd.setup("127.0.0.1", 1234);
    //	oscd.clear();
	
	sampleRate 			= 44100; /* Sampling Rate */
	initialBufferSize	= 1024;	/* Buffer Size. you have to fill this buffer with sound*/
	lAudioOut			= new float[initialBufferSize];/* outputs */
	rAudioOut			= new float[initialBufferSize];
	lAudioIn			= new float[initialBufferSize];/* inputs */
	rAudioIn			= new float[initialBufferSize];
    
	fftSize = 1024;
    
	uint hopSize = 256;
	uint framesPerBuffer = initialBufferSize / hopSize;
    
	bufferCount = ceil(((float)readAheadTime * sampleRate) / (float)initialBufferSize);
	//cout << "Buffer count: " << bufferCount << endl;
	
	//	buffers.resize(bufferCount);
	floatVector fftFrame;
	fftFrame.resize(fftSize/2, 0);
	fftFrameDelay = vectorDelay<floatVector>(bufferCount * framesPerBuffer, fftFrame);
	doubleVector melFrame;
	melFrame.resize(42, 0.0);
	melBandsDelay = vectorDelay<doubleVector>(bufferCount * framesPerBuffer, melFrame);
	doubleVector mfccFrame;
	mfccFrame.resize(mfccCount, 0);
	mfccDelay = vectorDelay<doubleVector>(bufferCount * framesPerBuffer, mfccFrame);
	
	//fluxTrigger = medianTrigger(10, 0.1, 0.4, 10);
	fluxTrigger = basicTrigger(10, 0.15, 0.075, 10, 0.2);
	fluxSignature.resize(fftSize/2, 0);
	
	flatnessStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	trigStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	avgAmpStream = vectorDelay<double>(bufferCount * framesPerBuffer, 0.0);
	mfccDistStream = vectorDelay<double>(bufferCount * framesPerBuffer, 0.0);
	onsetThreshStream = vectorDelay<double>(bufferCount * framesPerBuffer, 0.0);
	//beatVarianceStream = vectorDelay<double>(bufferCount * framesPerBuffer, 0.0);
	trackVarianceStream = vectorDelay<double>(bufferCount * framesPerBuffer, 0.0);
    // 	boundaryStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
 	actualBoundaryStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	avgMFCCVarStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	boundarySegThreshStream = vectorDelay<double>(bufferCount * framesPerBuffer, 0.0);
	sigBeatStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	sigBeatLabelStream = vectorDelay<string>(bufferCount * framesPerBuffer, "");
	spectralFluxStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	anythingStream = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	anythingStream2 = vectorDelay<float>(bufferCount * framesPerBuffer, 0.0);
	
    
    
	for(unsigned int i=0; i < bufferCount; i++) {
		buffers.push_back((float*) malloc(sizeof(float) * initialBufferSize * 2));
	}
	currentBuffer = 0;
	playBackStarted = false;
    
	float range = 1.0;
	thresholdGraph = debugGraph(300, 0, range, 550, 50, 450, 300, true);
	distGraph2 = debugGraph(300, 0, 0.5, 550, 50, 450, 300, true);
	distGraph2.setColor(255,0,0,255);
	onsetGraph = debugGraph(300, 0, 1, 550, 50, 450, 300, true);
	onsetGraph.setColor(0,0,255,200);
	flatnessGraph = debugGraph(300, 0, 1, 550, 50, 450, 300, true);
	flatnessGraph.setColor(255,0,255,255);
	ampGraph = debugGraph(300, 0, 1, 550, 50, 450, 300, true);
	ampGraph.setColor(0,255,255,150);
    //	beatVarianceGraph = debugGraph(300, 0, 1, 550, 50, 450, 300, true);
    //	beatVarianceGraph.setColor(255,50,200,255);
	trackVarianceGraph = debugGraph(300, 0, 0.3, 550, 50, 450, 300, true);
	trackVarianceGraph.setColor(100,255,50,200);
    //	boundariesGraph = debugGraph(300, 0, 0.8, 550, 50, 450, 300, true);
    //	boundariesGraph.setColor(238,238,0,200);
	actualBoundariesGraph = debugGraph(300, 0, 1.0, 550, 50, 450, 300, true);
	actualBoundariesGraph.setColor(255,255,255,255);
	avgMFCCVarGraph = debugGraph(300, 0, 0.5, 550, 50, 450, 300, true);
	avgMFCCVarGraph.setColor(255,50,200,255);
	boundarySegThreshGraph = debugGraph(300, 0, 0.3, 550, 50, 450, 300, true);
	boundarySegThreshGraph.setColor(255,255,50,200);
	sigBeatGraph = debugGraph(300, 0, 1.0, 550, 50, 450, 300, true);
	sigBeatGraph.setColor(255,0,0,255);
	sigBeatGraph.font = font;
	fluxGraph = debugGraph(300, 0, range, 550, 50, 450, 300, true);
	fluxGraph.setColor(255,255,255,150);
	anythingGraph = debugGraph(300, 0, range, 550, 50, 450, 300, true);
	anythingGraph.setColor(255,255,255,255);
	anythingGraph2 = debugGraph(300, 0, range, 550, 50, 450, 300, true);
	anythingGraph2.setColor(0,255,0,255);
	
	
	/* This is a nice safe piece of code */
	memset(lAudioOut, 0, initialBufferSize * sizeof(float));
	memset(rAudioOut, 0, initialBufferSize * sizeof(float));
	
	memset(lAudioIn, 0, initialBufferSize * sizeof(float));
	memset(rAudioIn, 0, initialBufferSize * sizeof(float));
	
	/* Now you can put anything you would normally put in maximilian's 'setup' method in here. */
	
    //HERE IS THE FILE LOAD---------------------------------------------------------------------
   // samp.load(ofToDataPath("20 Pastel Flags.wav"));
    
    
    
	samp.getLength();
	
	
	
	sampleLen = samp.myDataSize / 2.0 / 44100.0;
	
	envf.setAttack(1);
	envf.setRelease(100);
    envf2.setAttack(1);
	envf2.setRelease(10);
	
	mfft.setup(fftSize, 512, hopSize);
	ifft.setup(fftSize, 512, hopSize);
	
	
	mfccs = (double*) malloc(sizeof(double) * mfccCount);
	mfcc.setup(512, 42, mfccCount, 20, 20000, sampleRate);
	
	mfccVarMA = new movingAverage(80);
    //	avgAmpMA = new movingAverage(320);
	flatnessMA = new movingAverage(160);
	ampRMS = new movingRMS(1000);
	fluxMA = movingAverage(40,0);
	fluxLTMA = movingAverage(200,0);
    
	
	beats = beatDetector(mfccCount, 42, 160);
	boundaries = new boundarySegments(mfccCount, floor(sampleRate / hopSize / 1.0));
	
	melBandAverage.resize(42,0.0);
	melBandTmp.resize(42,0.0);
	melBandDiff.resize(42,0.0);
	
	lastFFT.resize(fftSize/2,0);
	lastMel.resize(42,0);
    
    //-------------------PAused here not 100% sure why it was here in the first place. 
    
    
	//paused = false;
	
    
    
    
    
	ofxMaxiSettings::setup(sampleRate, 2, initialBufferSize);
	ofSoundStreamSetup(2,2, this, sampleRate, initialBufferSize, 4);/* Call this last ! */
	
	ofSetVerticalSync(true);
	
    
    //MAXIMILIAN CODE ABOVE------------------------------------------------------------------
    
    }
    //my stuff here--- initialisation of stuff, this is a combination of mine and the Micks code from maximilian sound library. i will try to differenciate the code.
    
    //these are the lines to show where perfect, great, okay, sloppy, and poor ranges are 
    rangeLines = new RangeLines();
    
    //set to true when the screen is tapped, variable name from when the old build
    buttonPressed = false;
    
    
    counter = 1;
	singleFrameCounter = 0;
    
    //which side of the screen is hit
    buttonId = 0;
    
    combo = 0;
    score = 0;
    
    boundaryChange = false;
    
    //more like a framecounter
    noBoundaryChange = 0;
    nextMode = true;
    
    //this sets up the screen orientation. This is a bit of a mess, as the ipad is in portrait orientation(the reason why the media picker is stuck in portrait) with openframeworks rotating the window 90 degrees for the landscape orientation. When you change the ipad orientation to landscape, and then rotate the OF to landscape only rotates all the assest but not the window itself so the whole thing is broken and will need to reposition everything in the original landscape orientation to have it work.
    
    //iPhoneGetOFWindow() -> setOrientation(OF_ORIENTATION_90_LEFT);
    
    

       ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
     //ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_PORTRAIT);
    
    //CHANGE THE BELOW SETTINGS -------------------------------------------------------------
    
    speedOfEnemyMovement = 4.0f;
    initialSpace = 0;
    perfectRange = 30;
    greatRange = 10;
    okRange = 10;
    sloppyRange = 20;
    poorRange = 20;
    
    
    //the initialisation of the object pool;
    pool->init();
        
    hero = new Hero();
    
    red = 0;
    green = 0;
    blue = 0;
    
    startXPos = 312;
    startYPos = 370;
    startWidth = 390;
    startHeight = 140;
    
    statsXPos = 295;
    statsYPos = 590;
    statsWidth = 420;
    statsHeight = 130;
    
    statBackX = 50;
    statBackY = 700;
    statBackWidth = 100;
    statBackHeight = 50;
    
    currentState = startScreen;
    
    //number of consecutive hits for an upgrade
    slashThresh = 7;
    
    //currently loading 5 textures
    numberOfTextures = 5;
    
    splash = new SplashScreen();
    assign = new enemyTextureAssign(numberOfTextures);
    background = new Background(numberOfTextures);
    boundaryFlash = new BoundaryChangeFlash();
    comboBroken = new ComboBreak();
    text = new TextOnScreen();
    
    //dont know what this does
    bufferCount = ceil(((float)readAheadTime * sampleRate) / (float)frameSize);
    buffersLeft = bufferCount;
    
    
    randomBackGround = 0;
    lastBackGround = 0;
    shouldAddRight = 0;
    shouldAddLeft = 0;
    
    pool->numberOfTextures(numberOfTextures - 1);
    
    //default difficulty for now .. means nothing as it changes due to decide wait time. 
    waitingTime = 20;
    
    
    uberkilled = new UberKilled();
    songselect = false;
    hasRunOnce = false;
    
    
    perfectCounter = 0;
    greatCounter = 0;
    okayCounter = 0;
    sloppyCounter = 0;
    poorCounter = 0;
    missedCounter = 0;
    maxComboCounter = 0;
    selectDifficulty = false;
    decideWaitTime = new DecideWaitTime(slashThresh);
    countIn = new CountIn();
    indicatorOn = false;
    bt.init();
    boundaries->init();
    shouldScore = false;
    chosen = false;
    ableToAttack = true;
    highScores = new HighScore();
    updateHighScore = false;
    
    
    
    NSFileManager *filemgr;
    NSString *currentPath;
    
    filemgr = [NSFileManager defaultManager];
    currentPath = [filemgr currentDirectoryPath];
    string directory = *new std::string([currentPath UTF8String]);
    cout<< directory << endl;
    
    [filemgr release];
    
}

//--------------------------------------------------------------

//-- Mick Newly added function ------------------

void testApp::startStream(mediaStreamer *st) {
#if !TARGET_IPHONE_SIMULATOR
	MediaPickerView *myGuiViewController;
	myGuiViewController	= [[MediaPickerView alloc] initWithNibName:@"mediaPickerView" bundle:nil];
	[ofxiPhoneGetUIWindow() addSubview:myGuiViewController.view];
	[myGuiViewController showMediaPicker:st];
    
#else
#endif
}
//---------------------------------------------
void testApp::update(){
    
//-----intergrate Micks code-----------------------
    
    //this loads up the media picker
    if(songselect == true){
        switch(appState) {
            case INIT: {
                //[targetStream stop];
                
                targetStream = [[mediaStreamer alloc] init:480000 withSampleRate:44100.0 withChannels:1];

                //get target
                appState = CHOOSINGTARGET;
                
                break;
            case CHOOSINGTARGET:
                
                startStream(targetStream);
                ofSoundStreamStart();
                appState = MASHUP; //(!)
                
                break;
            case MASHUP:
                hasRunOnce = true;
                break;
                
            }
        }
    }
    
    //starts the song ... converts the track and artist name so that i can be printed in the game playing screen 
    
    if (hasRunOnce == true && STREAMING == targetStream->state && paused == false){
        artist = targetStream->trackartist;
        convertArtist = new std::string([artist UTF8String]);
        
        song = targetStream->tracktitle;
        convertSong = new std::string([song UTF8String]);
        
        //set state to play 
            currentState = playing;
    }
    
//----------------------------------------------
    if(currentState == playing){
        
        //assigns the wait time, even if its the same
            waitingTime = decideWaitTime->returnWaitTime(combo);
        
//Boundary Change Stuff---------------------------------------------------------------------------------------------------------------------
        
            //this is from the audiorequest methos
        if(boundaryChange == true){
            
                //this is a framecounter, which is reset when splash screen finishes
            if(noBoundaryChange == 0){
                
                        //if the same BG is found it finds another
                while(randomBackGround == lastBackGround){
                    randomBackGround = ofRandom(0, numberOfTextures);
                }
                
                lastBackGround = randomBackGround;
                
                    //assigns the new enemy and the new background
                assign->nextBoundary(randomBackGround);
                background->update(randomBackGround);
    
                
                
                    //looks at every enemy in the vector, Culls all those that are in a missed state state : (6)
                for(int i = 0; i < enemyPointerVector.size(); i++)
                {
                   
                    if(enemyPointerVector[i]->returnState() == 6 || enemyPointerVector[i]->returnCheckable()== false || enemyPointerVector[i]->returnCheckable() == false)
                    {
                        pool->returnResource(enemyPointerVector[i]);
                        enemyPointerVector.erase(enemyPointerVector.begin()+i);
                    }
                    
                    
                }
                
                    //assigns a new texture, for some reason the first element would not change, therefore i made it explicit change for that
                for(int i = 0 ; i < enemyPointerVector.size(); i ++){
                    enemyPointerVector[0]->assignNewTexture(assign->currentImage(), randomBackGround);
                    enemyPointerVector[i]->assignNewTexture(assign->currentImage(), randomBackGround);
                }
            }
            
            
            noBoundaryChange ++;
        }
        
        //this is needed as the boundary change appears of 10 frames. I dont what there to be 10 scene changes in that time
        if(noBoundaryChange > 10){
            boundaryChange = false;
            noBoundaryChange = 0;
        }
        
//Addition of the enemies to the vector------------------------------------------------------------------------------------------

        //the timing vector is filled with a number of the enemy id in the audio request method
    if(timingVector.size() > 0){
        
        //grabs on object from the pool and moves it to the drawing vectos. 
        enemyPointerVector.push_back(pool->getResource());
        
        //assign it the texture, ID(aka timing vector), returnEnemyType is the int id for the specific enemy so it knows how to act. and speed of enemy
        enemyPointerVector[enemyPointerVector.size()-1] -> assignTexture(assign->currentImage(), timingVector[0], assign->returnEnemyType(), speedOfEnemyMovement);
        
        //set the object to drawOnScreen
        enemyPointerVector[enemyPointerVector.size()-1] -> drawOnScreen(timingVector[0]);
        
        //clear the vector, so another enemy is not drawn on that beat
        timingVector.clear();
        
        //used for the ranking later. 
        totalBeats ++;
    }
    
        
//the beginning of the actual game logic----------------------------------------------------------------------------------------------------
        
    //used to find the current checkable object
        
    foundIt = false;
    numberFound = 0;
        
if(paused == false){
    
    
    for(int h = 0; h < enemyPointerVector.size(); h++){
        
        //find the first enemy in the vector which is checkable.... for collision checks to be made on. 
        if(enemyPointerVector[h]->returnCheckable() == true && foundIt ==false){
            numberFound = h;
            
            //this will draw an arrow above the head of the currently Checkable if in medium and easy difficulty. 
            if(indicatorOn){
                enemyPointerVector[numberFound]->displayAttackable();
            }
            
            break;
        }
        
        
    }
        //move the enemy position.
            for(int j = 0; j < enemyPointerVector.size(); j++){
        
                enemyPointerVector[j]->move(ofGetFrameRate());
            }
        
    
    ofSetColor(0);
    
    
    //the below set of code just runs whenever a button is pressed, the single framecounter just makes sure that the buttong press only lasts one frame, and will allow for another button press action as soon as possible
    
    if(buttonPressed == true && singleFrameCounter < 1){
        
            hero->animate(buttonId);
        
        //if the player is just mashing they will lose there combo especitially if the the vector is empty
        if(enemyPointerVector.empty()){
            combo = 0;
            theHit = "missed";
            missedCounter ++;    
            text->pulseHit();
            uberkilled->reduceMeter();
        }

    }
    
    
    
    if(enemyPointerVector.size() > 0){
    
        //this does an immediate check on the closest enemy. if it is out of range it is just a miss, but once in range will make the object not checkable until it is culled and reset
        
    typeOfHit = enemyPointerVector[numberFound]->inRange(initialSpace,perfectRange, greatRange, okRange, sloppyRange, poorRange);
        
    //if(((typeOfHit != 0) && (enemyPointerVector[numberFound]->buttonHit(buttonId) == true) && buttonPressed == true && singleFrameCounter < 1)){
        
        // if a button is pressed                           //this is bypassed when in UberKilled mode.
    if((buttonPressed == true && singleFrameCounter < 1) || shouldScore == true){
        
        //when uber killed mode enemie defeat themselves, animates and scores a perfect.
        if(shouldScore == true){
            typeOfHit = 1;
            hero->animate(enemyPointerVector[numberFound]->getId());
        }
        
        // for the players hit the accuracy is as below, dependant on the type of hit return
        
        if(typeOfHit == 1){
            theHit = "PERFECT";
            perfectCounter++;
            combo++;
            score += 10*combo;
            uberkilled->updateMeter();
            
        }else if(typeOfHit== 2){
            theHit = "Great";
            greatCounter++;
            combo++;
            score += 8*combo;
            uberkilled->updateMeter();

            
        }else if(typeOfHit==3){
            theHit = "Okay";
            okayCounter++;
            combo++;
            score += 6*combo;
            uberkilled->updateMeter();

            
        }else if(typeOfHit==4){
            theHit = "Sloppy";
            sloppyCounter++;
            combo = 0;
            score+=2;
            //uberkilled->reduceMeter();

            
        }else if(typeOfHit==5){
            theHit = "Poor";
            poorCounter++;
            combo = 0;
            score+=1;
            //uberkilled->reduceMeter();

        }
        
        
        ofSetColor(255);
        
        //as long as it is not a miss, an animation is sent to the enemy object
        
        if(typeOfHit != 0 && enemyPointerVector[numberFound]->returnCheckable() == true){
            
            text->pulseHit();
            text->pulseScore();
            enemyPointerVector[numberFound]->buttonHit(buttonId);           
            enemyPointerVector[numberFound]->noLongerCheckable();

        }
        
        //hero checks its slash state.
        hero->chooseSwordSwipe(combo, typeOfHit, slashThresh);
        
        //set the enemy to no longer checkable, until it is reset
        enemyPointerVector[numberFound]->noLongerCheckable();
        
        //this is only used when in the uber killed mode
        shouldScore = false;

        if(typeOfHit == 0){
            combo = 0;
            theHit = "missed";
            missedCounter ++;
            hero->chooseSwordSwipe(combo, typeOfHit, slashThresh);
            text->pulseHit();
            uberkilled->reduceMeter();
        }
        
        
                
    }else if((enemyPointerVector[numberFound]->returnCheckable() == true && enemyPointerVector[numberFound]->returnState()== 6)){
        //all this runs when the player completely missed and they have pass through them and run off to the other side
        
        combo = 0;
        theHit = "missed";
        missedCounter ++;
        hero->chooseSwordSwipe(combo, typeOfHit, slashThresh);
        enemyPointerVector[numberFound]->noLongerCheckable();
        text->pulseHit();
        uberkilled->reduceMeter();
        
    }
            
}
    
//------------------------------------------------------------------------------------------------------------------------------------------------
    
    //keep a track of the maximum combo so far
    if(combo > maxComboCounter){
        maxComboCounter = combo;
    }
    
    // remove the enemies that have set themselve to remove
    
    for(int t = 0; t < enemyPointerVector.size(); t++){
        
        if(enemyPointerVector[t]->remove() == true){
         
            pool->returnResource(enemyPointerVector[t]);
            enemyPointerVector.erase(enemyPointerVector.begin()+t);
            
        }
    }
    
}
        
//stop all input after 1 frame so that they cant just hold down the button/touch screen and beat the game
    
	if(buttonPressed == true){
		singleFrameCounter ++;
	}
    
    if(singleFrameCounter > 1){
        
        buttonPressed = false;
        buttonId = 0;
        
    }
        
//remove the particles off the screen
    
    for(int y = 0; y < particleVector.size(); y++){
        if(particleVector[y].isOnScreen() == false){
                        
            swap(particleVector[y], particleVector.back());
            particleVector.pop_back();
            
            
        }
    }

//splats whenever the hero hits a perfect, great or okay.
    
    for(int y = 0; y < particleVector.size(); y++){
        

        if(typeOfHit !=0 && typeOfHit < 3){

           
            particleVector[y].splat();
            
        }
    }
        
//move all the particles that are on the screen
    
    for(int y = 0; y < particleVector.size(); y++){
        
        if(paused == false){
            particleVector[y].move();
        }
        
    }

}
    
}



//--------------------------------------------------------------
void testApp::draw(){
    
    switch (currentState){
            
            
        case startScreen:
        
            //Draws in the main menu when the game starts
            menuScreen.draw(0,0);
        
            if(selectDifficulty == true){
            
                //when start is hit, the difficulty image draws over the top. When this boolean is true, the hit boxes on screen also change
                difficultyImage.draw(0,0);
                ofSetColor(255, 255, 255);
            }
        
            //this is just a timer so that when a difficulty is chosen and the media picker is moving up the screen the difficulty select will not immediately diasappear.
        
            if(begintimer){
                difficultyCounter++;
            
                if(difficultyCounter > 40){
                    begintimer = false;
                    difficultyCounter = 0;
                    selectDifficulty = false;
                    chosen = false; 
                }
            }
            
            break;
    
            // on the stats screen just draws in the back square, where the hitbox back is present
        case stats:
            
            text->statScreen();
            ofSetColor(red, green, blue);
            ofFill();
            ofRect(statBackX, statBackY, statBackWidth, statBackHeight);
            ofNoFill();
            ofSetColor(255, 255, 255, 255);
        
            break;
    
    
        case playing:
        
            ofFill();
            ofSetColor(255, 255, 255, 255);
        
            //draws the currently chosen background which changes from the update
            background-> drawBackground();
        
            //this draws in the enemy in its updated position, this function also completes all the animation
            for(int h = 0; h < enemyPointerVector.size(); h++)
            {
                enemyPointerVector[h]->draw(particleVector,hero->returnComboSlash(), paused);
            }
        
            //this checks if the user had hit the two ends of the uberbar, when full
            if(paused == false){
                if(uberkilled->shouldUberKill()){
            
                    uberkilled->depleteMeter();
        
                    //deletes all the missed enemies
                    for(int i = 0; i < enemyPointerVector.size(); i++){
                
                        if(enemyPointerVector[i]->returnState() == 6)
                        {
                            pool->returnResource(enemyPointerVector[i]);
                            enemyPointerVector.erase(enemyPointerVector.begin()+i);
                        }
                    }
        
                    //this flips a bool in the enemy that will make the enemy kill itself when in perfect range.
                    enemyPointerVector[numberFound]->uberKilled();
            
                    //when the enemy is autokilled this is set to true           this checkable is needed to stop the hero from autoattacking the       same enemy scoring point and imcrementing the combo counter. 
                    if(enemyPointerVector[numberFound]->returnSpecialKilled() && enemyPointerVector[numberFound]->returnCheckable()){
            
                        //should score is changed to true so that it will score with out needing the player to hit the screen, and then automatically animate the hero to look as though the hero has killed the enemy
                        
                        shouldScore = true;
                        hero->animate(enemyPointerVector[numberFound]->getId());
                        ableToAttack = false;
                        
                    }
                    
                }else{
                
                    if(enemyPointerVector.size() > 0)
                    {
                        //this stops uber kill enemy after the meter runs out if it has been assigned to uber killed while there was still meter
                        enemyPointerVector[numberFound]->notUberKill();
                    }
                
                    ableToAttack = true;

                }
            }
            
    
            //draw all the particles
            for(int y = 0; y < particleVector.size(); y++){
        
                particleVector[y].draw(paused);
            }
            
            //the draw order
            hero->drawHero(paused);
            background->drawForeground();
            background->drawClouds();
            hero->drawSword();
//          rangeLines->draw(50, 0, perfectRange, greatRange, okRange, poorRange, sloppyRange);
            uberkilled->draw(paused);
            splash->draw(combo, slashThresh);
            boundaryFlash-> draw(randomBackGround);
            comboBroken->draw(combo, paused);
            countIn->draw();
            ofSetColor(255);
            text->draw(theHit,combo, score);
            pauseButton.draw(920, 0);
            text->titleOnScreen(*convertArtist, *convertSong, difficulty);
            //highScores->draw();
            
            break;
            
        case highScoreDisplay:
            
            if(updateHighScore == false){
                highScores-> storeHighScores(*convertSong, *convertArtist, score);
                updateHighScore = true; 
            }
        
                highScores->draw();
            
            ofSetColor(0, 0, 0,255);
            ofRect(850, 700, 174, 768);
            ofSetColor(255, 255, 255,255);

            
                break;
            
    //resets the game so that it can return to the main menu
        case resetted:
            mainMenuReset();
            currentState = startScreen;
            break;
    
    // draws the results on screen
        case results:
        
            resultsPage.draw(0,0);
            ofSetColor(255);
            text->drawResultsPage(perfectCounter, greatCounter, okayCounter, sloppyCounter, poorCounter, maxComboCounter,missedCounter, score, totalBeats, totalNumberOfButtonPresses);

            ofSetColor(255);
            break;
            
    }
       
    //below is the original graph showing beats and boundaries
    //actualBoundariesGraph.draw();
    //ofSetColor(255, 255, 255,255);
    
    //then paused draw the overlay
    if(paused == true){
        pauseOverlay.draw(0,0);
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    
    //delete objects then the game is out of focus
    
    //highScores->writeToFile();
    
    pool->deleteObjects();
    delete pool;
    delete hero;
    delete rangeLines;
    delete assign;
    delete background;
    delete boundaryFlash;
    delete uberkilled;
    delete comboBroken;
    delete text;
    delete decideWaitTime;
    delete countIn;
    delete convertArtist;
    delete convertSong;
    delete highScores;
    
}

void testApp::mainMenuReset() {
    
    text->addValues(perfectCounter, greatCounter, okayCounter, sloppyCounter, poorCounter, maxComboCounter);

    //reset is the generic rest function
    reset();
    
    //required to reset to the main menu
    [targetStream stop];
    ofSoundStreamStop();
    appState = CHOOSINGTARGET;
    indicatorOn = false;
    
}

void testApp::restart() {
    
    reset();
    //just estarts the song
    [targetStream restart];
    
}

void testApp:: reset(){
    
    if(paused){
        paused = false;
    }
    //not 100% sure what this does
    for(int i=0; i < lastFFT.size(); i++) lastFFT[i] = 0.0;
    
    
    //clear out buffers
	for(unsigned int i=0; i < bufferCount; i++)
    {
        for(unsigned int j=0; j < frameSize * 2; j++)
        {
            buffers[i][j] = 0.0;
        }
    }
    
    highScores->reset();
    updateHighScore = false;
    
    boundaryChange = false;
    noBoundaryChange = 0;
    currentBuffer = 0;
    
    //counter for enemiy sides
    counter = 1;
    
    score = 0;
    
    buffersLeft = bufferCount;
    bt.init();
    boundaries->init();
    
    //used for ranking
    totalNumberOfButtonPresses = 0;
    
    //for checking collision
    numberFound = 0;
    
    singleFrameCounter = false;
    buttonPressed = false;
    countIn->reset();
    hero->reset();
    comboBroken -> resetTheCounter();
    uberkilled->resetMeter();

    particleVector.clear();
    
    //further ranking stuff
    totalBeats =0;
    
    //results page stuff
    maxComboCounter = 0;
    perfectCounter = 0;
    greatCounter = 0;
    okayCounter = 0;
    sloppyCounter = 0;
    poorCounter = 0;
    missedCounter = 0;
    
    //return all the enemey objects
    for(int t = 0; t < enemyPointerVector.size(); t++)
    {
        pool->returnResource(enemyPointerVector[t]);
    }
    
    enemyPointerVector.clear();
    
    //reset the type of hit text, otherwise it will be drawn on a reset 
    theHit = "";
    
    nextMode = false;
    songselect = false;
    hasRunOnce = false;
    
    //reset the enemy texture and the BG texture;
    assign->reset();
    background->reset();
    currentFlux = 0;
    lastFlux = 0;
    combo = 0;
    ableToAttack = true;


}





//--------------------------------------------------------------
//this is micks code mostly, there have been some addition , most of them not mine,

void testApp::audioRequested (float* output, int bufferSize, int nChannels){
    
    if(currentState == playing && (targetStream->state == STREAMING || targetStream->state == READINGOUTBUFFER)){
    
	static int sampleIdx=0;
	if (!paused) {
        
        float *dataFromSong = buffers[currentBuffer % bufferCount];
        
        if (STREAMING == targetStream->state)
        {
            //grab some new data and analyse it
            [targetStream getNextBuffer:dataFromSong outBufferSize:bufferSize];
            //                timeInto += (bufferSize / nChannels / (float)sampleRate);
            //                cout << timeInto << endl;
        }
        else
        {
            
            vDSP_vclr(dataFromSong, 1, bufferSize * nChannels);
            buffersLeft--;
            if (0==buffersLeft) {
                [[NSNotificationCenter defaultCenter] postNotificationName:@"endOfStream" object:nil];
            }
        }
//this was is added ---------------------------------------------------------------------------------------------------------

        if (ENDOFSTREAM == targetStream->state){
            currentState =results;
        }
        
//---------------------------------------------------------------------------------------------------------------------------
        
		static float normalisation = 1.0;
		static float wavemax=0.0;
        float absWaveAvg = 0.;
        
        //	static double tm;
        
        
		for (int i = 0; i < bufferSize; i++){
            //		wave = lAudioIn[i];
	//		wave = samp.play(1.);
            wave = dataFromSong[i];
            
			float waveabs = fabs(wave);
            
			if (waveabs > wavemax) {
				wavemax = waveabs;
				normalisation = min(1.0 / wavemax, 4.0);
			}
			float normalisedWave = wave * normalisation;
            
            absWaveAvg+=waveabs;
			
			//get fft
			if (mfft.process(normalisedWave)) {
                
				//---FFT
				mfft.magsToDB();
				//store frame
				floatVector frame;
				frame.resize(mfft.bins, 0);
				std::copy(mfft.magnitudesDB, mfft.magnitudesDB + mfft.bins,frame.begin());
				fftFrameDelay.addFrame(frame);
				
				//work out spectral flux
				float hwDiff;
				flux = 0.0;
				for( int j=0; j < frame.size(); j++) {
					hwDiff = mfft.magnitudes[j] - lastFFT[j];
					if (hwDiff > 0) {
						flux += hwDiff;
						fluxSignature[j] = hwDiff;
					}else{
						fluxSignature[j] = 0;
					}
				}
				//oscd.send(debug);
				
				flux /= frame.size();
                //			cout << "F: " << flux << endl;
				spectralFluxStream.addFrame(flux);
                currentFlux=flux;
				fluxMA.addsample(flux);
				fluxLTMA.addsample(flux);
				float envfValue = envf.play(normalisedWave);
				anythingStream.addFrame(fluxLTMA.value());
				anythingStream2.addFrame(envfValue);
                
				fluxTrigger.peakSizeThreshold = 0.02;
				bool trig = fluxTrigger.newFrame(flux);
				
				//store for next frame
				std::copy(mfft.magnitudes, mfft.magnitudes + mfft.bins, lastFFT.begin());
				
				flatnessMA->addsample(mfft.spectralFlatness());
				flatnessStream.addFrame(flatnessMA->value());
				flatnessGraph.addVal(flatnessStream.getFrame());
				
				//--MFCCs
				mfcc.mfcc(mfft.magnitudes, mfccs);
				//add mfcc to stream
				doubleVector mfccFrame;
				mfccFrame.resize(mfccCount);
				std::copy(mfccs, mfccs + mfccCount, mfccFrame.begin());
				mfccDelay.addFrame(mfccFrame);
				
				//mel bands frame
				doubleVector melBandsFrame;
				melBandsFrame.resize(42);
				std::copy(mfcc.melBands, mfcc.melBands + 42, melBandsFrame.begin());
				melBandsDelay.addFrame(melBandsFrame);
				
				std::copy(mfcc.melBands, mfcc.melBands + 42, lastMel.begin());
				
				//mel band averaging for new features
				//exponential moving average
				double avgAlpha = 0.9; double avgAlphaInv = 1.0 - avgAlpha;
				vDSP_vsmulD(mfcc.melBands, 1, &avgAlpha, &(melBandTmp[0]), 1, 42);
				vDSP_vsmulD(&(melBandAverage[0]), 1, &avgAlphaInv, &(melBandAverage[0]), 1, 42);
				vDSP_vaddD(&(melBandTmp[0]), 1, &(melBandAverage[0]), 1, &(melBandAverage[0]), 1, 42);
				//get diff
				vDSP_vsubD(&(melBandAverage[0]), 1, mfcc.melBands, 1, &(melBandDiff[0]), 1, 42);
				
                
				//static double distAvgs = 0;
				static float actualBoundaryVal = 0.0;
                
                
				double trigGraphVal;
				float sigBeatValue = 0;
				static float lastEnv = 0;
				static float threshFloor = 0.02;
				//float envDiff = envfValue - lastEnv;
				lastEnv = envfValue;
                envf.setRelease(envf2.play(flux)*10);
				if (trig) {
                    
                    
                    
					avgBeatSize = bt.getAvgBeatSize();
                    
                    
                    if (avgBeatSize < 28) {
                        
                        
						float shift = 0.02;
                        fluxTrigger.onThreshold += shift;

                        
					} else if (avgBeatSize > 90) {
                        
						float shift = 0.02f;
                        
						fluxTrigger.onThreshold -= shift;
                        
					}
				}
                
                //------------THIS IS MY CODE HERE---------------------------------------------
                //this in here as it it in another thread and can be inconsistent 
                //works out the average flux of the song so far

                //this is the onset/beat detection if statement ....DONT TOUCH(not my code);
                
                if (currentFlux-lastFlux>(fluxMA.value()) && currentFlux>(fluxMA.getStDev()*3) && currentFlux-lastFlux>sqrt(envfValue) && waitTime == 0) {
                    
                    beatTrig2=true;

//these if statements adds a number to a vector.if the vector has anything in it the object is created, and then the number in the timing vector is removed 

                        if(counter % 2 == 1){
                            
                            //this commented out code is used in conjuction with the enemy object to allow two hits 
                            
//                            shouldAddLeft ++;
                            
//                    this basically skips the one appearing on the opposite side of the hero before useing the next enemies location
                            
//                            if(shouldAddLeft % 2 == 0){
//                                addLeft = true;
//                            }else{
//                                addLeft = false;
//                            }
//                            
//                            if(enemyPointerVector.size() > 1 && addLeft){
//                                enemyPointerVector[enemyPointerVector.size()-2]->updateTheSecondHit();
//                            }else{
                                timingVector.push_back(2);
//                            }
                            
                            
                        }else if(counter % 2 == 0){
                            
//                            shouldAddRight++;
//                            
//                            if(shouldAddRight % 2 == 0){
//                                addRight = true;
//                            }else{
//                                addRight = false;
//                            }
//                            
//                            if(enemyPointerVector.size() > 1 && addRight){
//                                //cout << "here" << endl;
//                                enemyPointerVector[enemyPointerVector.size()-1]->updateTheSecondHit();
//
//                            }else{
                                timingVector.push_back(1);
//                            }

                        }
                    
                    //this is the counter adding enemies to the vector with alternating ids 
                        if(counter > 101){
                            counter = 0;
                        }
                        counter++;
                        
                        
                        
                    //}
                    
                    
                    
                    //this is the important variable---------------------
                    //this is the wait time between beats found... make this smaller if you want more to be found
                    //usually 5, 5 is impossible, 20 is hard but good, 50 is too slow and therefore boring
                    
                    waitTime=waitingTime;
                    
//I dont know what follows ---------------------------------------------------------------------------------------------------------
                    
                } else {
                    beatTrig2=false;
                    
                    if (waitTime>0) waitTime--;
                }
                
                lastFlux=currentFlux;
				//recheck trigger
				bool beattrig = bt.newFrame(trig, flux / fluxTrigger.onThreshold, fluxSignature);
                
                
                
                //this probably decided how for back the boundary was found .... change this for timeing purpses.
                
				if (boundaries->processFrame(beattrig, mfccs)) {
					//limit to size of window
					int framesAgo = boundaries->framesAgo;
					framesAgo = min((int) actualBoundaryStream.values.size() - 2, framesAgo);
                    //					cout << "Frames ago: " << framesAgo << endl;
					for(int k=framesAgo; k > framesAgo - 10; k--)
						actualBoundaryStream.setFrameValue(k, 0.9);
                        
				}
                
                if (beattrig)
                {
                    float avgBeatSize = bt.getAvgBeatSize();
                    if (avgBeatSize < 28)
                    {
                        fluxTrigger.onThreshold += 0.02;
                        fluxTrigger.offThreshold += 0.02;
                    }
                    else if (avgBeatSize > 87)
                    {
                        fluxTrigger.onThreshold -= 0.02;
                        fluxTrigger.offThreshold -= 0.02;
                    }
                }
                
                
				if (beatTrig2) {
					trigGraphVal = 0.8;
					//cout << "beat: " << (sampleIdx / (float)maxiSettings::sampleRate) << "s\n";
					beatTimes.push_back((sampleIdx / (float)maxiSettings::sampleRate));
                    beepAmp=1.0;
					
				}else{
					trigGraphVal = 0;
                    beepAmp=0.0;
				}
				if (bt.wasLongBeat()) {  //received no information for > 1/4 beat
                    
                    
                    fluxTrigger.onThreshold = max(fluxMA.value()*2.0f, threshFloor);
                    fluxTrigger.offThreshold = max(fluxTrigger.onThreshold * 0.5, 0.01);
                    fluxTrigger.reset();
                    //
                    //                    }
					//cout << "\nlong\n";
				}
                
                //				}
				trigStream.addFrame(trigGraphVal);
				onsetGraph.addVal(trigStream.getFrame());
				
                
                
				
				sigBeatStream.addFrame(sigBeatValue);
				sigBeatLabelStream.addFrame("");
				sigBeatGraph.addVal(sigBeatStream.getFrame(), sigBeatLabelStream.getFrame());
                
				actualBoundaryStream.addFrame(actualBoundaryVal);
				actualBoundariesGraph.addVal(actualBoundaryStream.getFrame());
                
//MORE OF MY CODE HERE FOR A LITTLE BIT PROBABLY MOVE LATER---------------------------------------
                
//check that the boundary had changed
                
                //actualBoundaryStream.getFrame() is set to 0.9 when a boundary is found and lasts of 10 frames , therefore changes were made in the upadate fuction
                            
                if(actualBoundaryStream.getFrame() != 0 && boundaryChange ==false){
                    
                    boundaryChange = true;
                    
                }
                
                
//------------------------------------------------------------------------------------------------
				trackVarianceStream.addFrame(boundaries->avgDist);
				trackVarianceGraph.addVal(trackVarianceStream.getFrame());
				
				mfccDistStream.addFrame(beats.mfccFrameDifference);
                //				distGraph2.addVal(mfcsvncDistStream.getFrame());
				avgAmpStream.addFrame(beats.expAvgAmp);
				ampGraph.addVal(avgAmpStream.getFrame());
				onsetThreshStream.addFrame(fluxTrigger.onThreshold);
				thresholdGraph.addVal(onsetThreshStream.getFrame());
				avgMFCCVarStream.addFrame(beats.mfccVarThreshold);
				avgMFCCVarGraph.addVal(avgMFCCVarStream.getFrame());
				boundarySegThreshStream.addFrame(boundaries->threshold);
				boundarySegThreshGraph.addVal(boundarySegThreshStream.getFrame());
				fluxGraph.addVal(spectralFluxStream.getFrame());
				anythingGraph.addVal(anythingStream.getFrame());
				anythingGraph2.addVal(anythingStream2.getFrame());
				
			}
            
            
			//inverse fft
			gettimeofday(&callTS,NULL);
			gettimeofday(&callEndTS,NULL);
			callTime = (float)(callEndTS.tv_usec - callTS.tv_usec) / 1000000.0;
			callTime += (float)(callEndTS.tv_sec - callTS.tv_sec);
			//play result
            
            //wave = delay.dl(samp.play(1.), 60000);
            
			mymix.stereo(wave, outputs, 0.5);
            
            beepOut = beep.sinebuf(200)*beepAmp;
			lAudioOut[i] = output[i*nChannels    ] = outputs[0] + beepOut; /* You may end up with lots of outputs. add them here */
			rAudioOut[i] = output[i*nChannels + 1] = outputs[1] + beepOut;
			
			sampleIdx++;
            
		}
        // beepAmp = 0.0;
        
		
		//delayed buffering
		//float *data = buffers[currentBuffer % bufferCount];
		//memmove(data, output, sizeof(float) * bufferSize * 2);
           
        
        if (currentBuffer >= bufferCount)
        {
            playBackStarted = true;
                       // if (!offPath)
            
            //float *data = buffers[currentBuffer % bufferCount];
      //      memmove(output, buffers[(currentBuffer+1) % bufferCount], sizeof(float) * bufferSize * 2);
            int bufIdx = (currentBuffer+1) % bufferCount;
            for(int j=0; j < bufferSize; j++) {
                output[2*j] = output[2*j+1] = buffers[bufIdx][j];
            }
            
        }
        else
        {
            vDSP_vclr(output, 1, bufferSize * nChannels);
        }
        currentBuffer++;
    } else{
        vDSP_vclr(output, 1, bufferSize * nChannels);
    }
        
    }else{
        //paused, so output silence
        vDSP_vclr(output, 1, bufferSize * nChannels);
    }



    


}

// i have no clue what this does --------------------------------------------------------------
void testApp::audioReceived 	(float* input, int bufferSize, int nChannels){
    
    if(currentState == playing){
        
        
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
		
		lAudioIn[i] = input[i*2];
		rAudioIn[i] = input[i*2+1];
	}
    
    }
	
}


// below are the hit boxes and interactions associated with the game--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){
    printf("TouchX = %f, TouchY = %f\n", touch.x, touch.y);
    printf("ScalarX = %f, ScalarY = %f\n", scalarX, scalarY);
    touch.x/=scalarX;
    touch.y/=scalarY;
    printf("TouchX = %f, TouchY = %f\n", touch.x, touch.y);
    
    nextMode = true;
    
    if(currentState == stats){
        
        //this just changes the colour of the rectangle in the bottom corner of the page to go back, "nextMode" just makes sure that the user does not advance multiple time if the hitboxes from one screen to the next are in the same place
        
        if(currentState == stats && nextMode == true && touch.x < statBackX + statBackWidth && touch.x > statBackX && touch.y > statBackY  && touch.y < statBackY + statBackHeight){
            
            red = 200;
            green = 200;
            blue = 200;
            
        }
    }

    
    if(currentState == playing && nextMode == true){
        
        
        // this is used to pause the game by touching the top corner.
        if(touch.x > 935 && touch.x < 1015 && touch.y > 10 && touch.y < 90)
        {
            paused = true;
        }
        
//this check hitting the right side of the screen ------------------------------------------------------------------------------------------
        
        if(paused == false){
        if(ableToAttack == true){
        if(touch.x > 512)
        {
            //check if the uberkilled buttons are there, if so check the uberkill status over attacking and missing.
            if(uberkilled->returnMeterFull() && uberkilled -> returnMeterFull() && (touch.x > uberkilled->returnPositions(5) && touch.x < uberkilled->returnPositions(6) && touch.y > uberkilled->returnPositions(7) && touch.y < uberkilled->returnPositions(8))){
                
                uberkilled->hitRight();

            }else{
                
                //number of times pressed for ranking purposes
                totalNumberOfButtonPresses++;
            
                //returns a button id of 1, this is the same as the id of the enemies on the right, being the same number, they are compared and therefore a correct hit is found and the enemy is put in a killed state
                buttonId = 1;
            
                //true for a frame, and on the frame collision checks are done.
                buttonPressed = true;
                //cout<<touch.x<<endl;
            }
            
        }
//hit the left side of the screen ------------------------------------------------------------------------------------------------------------
        else if (touch.x < 512)
        {
            
            if(uberkilled -> returnMeterFull() && (touch.x > uberkilled->returnPositions(1) && touch.x < uberkilled->returnPositions(2) && touch.y > uberkilled->returnPositions(3) && touch.y < uberkilled->returnPositions(4)))
            {
                uberkilled->hitLeft();

            }else{
                totalNumberOfButtonPresses++;
                buttonId = 2;
                buttonPressed = true;
                //cout<<touch.x<<endl;

            }
        }
        }
        }
        
        
    //this is the pause menu hitboxes---------------------------------------------------------------------------------------------------
        if(paused == true){
            if(touch.x > 360 && touch.x < 660 && touch.y > 300 && touch.y < 420){
                paused = false;
            }
            if(touch.x > 360 && touch.x < 660 && touch.y > 435 && touch.y < 555){
                paused = false;
                restart();
       
            }
            if(touch.x > 360 && touch.x < 660 && touch.y > 575 && touch.y < 695){
                currentState = resetted;
            }
        }
    
    }

//menu button in the bottom corner ------------------------------------------------------------------------------------------------------------
    if(currentState == results)
    {
        if(touch.x > 850 && touch.x < 1024 && touch.y > 700 && touch.y < 768 && nextMode == true)
        {
            currentState = highScoreDisplay;
            nextMode = false;
            //cout<< "here"<<endl;
        }
    }
    
    if(currentState == highScoreDisplay)
    {
        if(touch.x > 850 && touch.x < 1024 && touch.y > 700 && touch.y < 768 && nextMode == true)
        {
            currentState = resetted;
            nextMode = false;

        }
    }
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){
    touch.x/=scalarX;
    touch.y/=scalarX;
    
// this just changes the colour of the stats back button if you hover over it ---------------------------------------------------------------

    if(currentState == stats && nextMode == true && touch.x < statBackX + statBackWidth && touch.x > statBackX && touch.y > statBackY  && touch.y < statBackY + statBackHeight){
                
                red = 200;
                green = 200;
                blue = 200;
                
        
            }else{
                red = 0;
                green = 0;
                blue = 0;
        
            }
        
    
    
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){
    
    touch.x/=scalarX;
    touch.y/=scalarX;
    
//used to select difficulty---------------------------------------------------------------------------------------------------------------------
    
    if(selectDifficulty == true){
        
        if(touch.x < 700 && touch.x > 310 && touch.y > 310  && touch.y < 410){
            
            chosen = true;
            indicatorOn = true;
            decideWaitTime->setWaitTime(0);
            difficulty = "Easy";
            
        }
        
        if(touch.x < 700 && touch.x > 310 && touch.y > 410  && touch.y < 510){
            
            chosen = true;
            indicatorOn = true;
            decideWaitTime->setWaitTime(1);
            difficulty = "Medium";


        }
        
        if(touch.x < 700 && touch.x > 310 && touch.y > 510  && touch.y < 610){
            
            chosen = true;
            indicatorOn = false;
            decideWaitTime->setWaitTime(2);
            difficulty = "Hard";

        }
        
        if(touch.x < 700 && touch.x > 310 && touch.y > 610  && touch.y < 710){
            
            chosen = true;
            indicatorOn = false;
            decideWaitTime->setWaitTime(3);
            difficulty = "Nightmare";

        }
        
        if(chosen == true){
            
            begintimer = true;
            songselect = true;
            appState = INIT;
            nextMode = false;
        }
        
    }
    
//just hitting the start screen------------------------------------------------------------------------------------------------------------------
    if(currentState == startScreen && nextMode == true && touch.x < startXPos + startWidth && touch.x > startXPos && touch.y > startYPos  && touch.y < startYPos + startHeight && selectDifficulty == false){
        
        selectDifficulty = true;
        nextMode = false;
        
    }
    
//if hit the stats button, go to the stat screen ------------------------------------------------------------------------------------------------
    if(currentState == startScreen && nextMode == true && touch.x < statsXPos + statsWidth && touch.x > statsXPos && touch.y > statsYPos  && touch.y < statsYPos + statsHeight){
        
       
        currentState = stats;
        nextMode = false;
        
    }
    
// going back to the main menu from the stats screen ------------------------------------------------------------------------------------------
   if(currentState == stats && nextMode == true && touch.x < statBackX + statBackWidth && touch.x > statBackX && touch.y > statBackY  && touch.y < statBackY + statBackHeight){
        
        currentState = startScreen;
        nextMode = false;
        
    }
    
    
    
    
// reset the colour of the button ------------------------------------------------------------------------------------------------------------
    red = 0;
    green = 0;
    blue = 0;
    
    
//let go of the left and right buttons of the uberkilled, to make sure that the user holds down the buttons for uber killed ------------------
    if(currentState == playing){
        runOnce = false;
        buttonId = 0;
        singleFrameCounter = 0;
        
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){
}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

    //this changes the orientation of game. 

    if(newOrientation == 4){
        iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
    } else if(newOrientation == 3){
        iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);
    }
}

