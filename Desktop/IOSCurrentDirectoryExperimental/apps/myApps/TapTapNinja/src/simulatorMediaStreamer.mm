//
//  simulatorMediaStreamer.mm
//  bastardPop
//
//  Created by Chris on 14/10/2011.
//  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
//

#import "simulatorMediaStreamer.h"
#include <string>
#include <libgen.h>
using namespace std;

@implementation simulatorMediaStreamer

- (void)initStream:(NSString*)path withTitle:(NSString*)title withArtist:(NSString*)artist withDuration:(double)dur{
	NSLog(@"InitStream: %@, %@, %@, %f\n", path, title, artist, dur);
	tracktitle = [[NSString alloc] initWithString:title];
	trackartist = [[NSString alloc] initWithString:artist];
	duration = dur;

	string mPath = [path UTF8String];
	
	sample.load(mPath);
//	OSStatus err = noErr;
//	const char *filename = basename((char *) mPath.c_str());
//	
//	CFURLRef mURL = CFURLCreateFromFileSystemRepresentation(NULL,  
//															(const UInt8 *) mPath.c_str(), 
//															mPath.size(), 
//															false);
//	
//	err = ExtAudioFileOpenURL(mURL, &mSoundID);
//	CFRelease(mURL); mURL = NULL;
//	if (err != noErr)
//	{
//		printf("[pkmEXTAudioFileReader]: ExtAudioFileOpenURL %s: err %d (file: '%s')\n", filename, (int) err, mPath.c_str());
//	}
//	
//	SInt64 fileSize;	
//	UInt32 propertySize = sizeof(fileSize);
//	
//	err = ExtAudioFileGetProperty(mSoundID, 
//								  kExtAudioFileProperty_FileLengthFrames , 
//								  &propertySize, 
//								  &fileSize);
//	
//	if (err != noErr)
//	{ 
//		printf("[pkmEXTAudioFileReader]: ExtAudioFileGetProperty FileLengthFrames: err %d (file: '%s')\n", (int) err, mPath.c_str());
//	}
//	
//	// get the input file format
//	AudioStreamBasicDescription	format;
//	propertySize = sizeof(format);
//	err = ExtAudioFileGetProperty(mSoundID, 
//								  kExtAudioFileProperty_FileDataFormat, 
//								  &propertySize, 
//								  &format);
//	if (err != noErr)
//	{ 
//		printf("[pkmEXTAudioFileReader]: ExtAudioFileGetProperty FileDataFormat: err %d (file: '%s')\n", (int) err, mPath.c_str());
//	}
//	
//	mFrameRate = (unsigned) format.mSampleRate;
//	mNumChannels = (unsigned) format.mChannelsPerFrame;
//	mNumSamples = (unsigned) fileSize * (44100.0 / format.mSampleRate);
//	mBytesPerSample = (unsigned) format.mBitsPerChannel / 8;
//	
//	printf("Opened %s (%d hz, %d ch, %d samples, %d bps)\n", mPath.c_str(), mFrameRate, mNumChannels, mNumSamples, mBytesPerSample);
//	
//	AudioStreamBasicDescription dstFormat;
//	memset(&dstFormat, 0, sizeof(AudioStreamBasicDescription));
//	UInt32 propSize;
//	
//	propSize = sizeof(dstFormat);
//	ExtAudioFileGetProperty(mSoundID, 
//							kExtAudioFileProperty_FileDataFormat, 
//							&propSize, 
//							&dstFormat);
//	
//	dstFormat.mSampleRate			= 44100.0;
//	dstFormat.mFormatID				= kAudioFormatLinearPCM;
//	dstFormat.mBitsPerChannel		= sizeof(float) * 4;
//	dstFormat.mChannelsPerFrame		= 2; // set this to 2 for stereo
//	dstFormat.mFramesPerPacket		= 2;
//	//dstFormat.mBytesPerPacket		= dstFormat.mFramesPerPacket * dstFormat.mBytesPerFrame;
//	//dstFormat.mBytesPerFrame		= sizeof(Float32) * dstFormat.mChannelsPerFrame;
//	dstFormat.mFormatFlags			= kAudioFormatFlagsNativeFloatPacked;//kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked | kAudioFormatFlagsNativeEndian;
//	
//	dstFormat.mBytesPerFrame		= 4;//dstFormat.mBitsPerChannel * dstFormat.mChannelsPerFrame / 8;
//	dstFormat.mBytesPerPacket		= 4;//dstFormat.mFramesPerPacket * dstFormat.mBytesPerFrame;
//	dstFormat.mReserved				= 0;
//	
//	err = ExtAudioFileSetProperty(mSoundID, 
//								  kExtAudioFileProperty_ClientDataFormat, 
//								  propSize, 
//								  &dstFormat);
//	if (err != noErr)
//	{
//		printf("[pkmEXTAudioFileReader]: ExtAudioFileSetProperty failed: %d\n", (int)err);
//	}
	
	state = STREAMING;
	
}

- (BOOL)getNextBuffer:(float*)outBuffer outBufferSize:(uint)count {
	bool ok = YES;
	if (readPos + count > sample.myDataSize/2.0) {
		state = ENDOFSTREAM;
	}else{
		for(int i=0; i < count; i++) {
			outBuffer[i] = sample.play();
		}
		readPos += count;
	}
//	if (writePos - readPos > count) {
//		ok = YES;
//		uint copyStart = (readPos % bufSize);
//		uint copyEnd = copyStart + count;
//		if (copyEnd < bufSize) {
//			memcpy(outBuffer, buffer + copyStart, sizeof(float) * count);
//		}else{
//			uint firstChunk = count - (copyEnd % bufSize);
//			memcpy(outBuffer, buffer + copyStart, sizeof(float) * firstChunk);
//			memcpy(outBuffer + firstChunk, buffer, sizeof(float) * (count - firstChunk));
//		}
//		readPos += count;
//	}else{
//		memset(outBuffer, 0, sizeof(float) * count);
//	}
	
	return ok;
}


@end
