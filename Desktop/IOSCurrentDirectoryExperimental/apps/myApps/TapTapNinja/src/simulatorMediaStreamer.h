//
//  simulatorMediaStreamer.h
//  bastardPop
//
//  Created by Chris on 14/10/2011.
//  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "mediaStreamer.h"
#include <AudioToolbox/ExtendedAudioFile.h>
#include "ofxMaxim.h"

@interface simulatorMediaStreamer : mediaStreamer {
@private
//	ExtAudioFileRef					mSoundID;
	maxiSample sample;

//	unsigned int					mFrameRate,
//	mNumChannels,
//	mNumSamples,
//	mBytesPerSample;	
	
	
}

- (void)initStream:(NSString*)path withTitle:(NSString*)title withArtist:(NSString*)artist withDuration:(double)dur;
- (BOOL)getNextBuffer:(float*)outBuffer outBufferSize:(uint)count;
//- (BOOL)open:(NSString*) mPath;

@end
