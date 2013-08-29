///*
// LICENCE
// 
// Earworm/Incus "The Software" © Goldsmiths College, University of London
// 
// The Software is and remains the property of Mick Grierson
// ("mgrierson") The Licensee will ensure that the Copyright Notice set
// out above appears prominently wherever the Software is used.
// 
// The Software is distributed under this Licence:
// 
// - on a exclusive basis,
// 
// - "AS-IS", mgrierson makes clear that no condition is made or to be
// implied, nor is any representation or warranty given or to be
// implied, as to (i) the quality, accuracy or reliability of the
// Software; (ii) the suitability of the Software for any particular
// use or for use under any specific conditions; and (iii) whether use
// of the Software will infringe third-party rights.
// 
// mgrierson disclaims:
// 
// - all responsibility for the use which is made of the Software; and
// 
// - any liability for the outcomes arising from using the Software.
// 
// The Licensee agrees to indemnify mgrierson and hold them
// harmless from and against any and all claims, damages and liabilities
// asserted by third parties (including claims for negligence) which
// arise directly or indirectly from the use of the Software or any
// derivative of it or the sale of any products based on the
// Software. The Licensee undertakes to make no liability claim against
// any employee, student, agent or appointee of mgrierson, in connection
// with this Licence or the Software.
// 
// No part of the Software may be reproduced, modified, transmitted or
// transferred in any form or by any means, electronic or mechanical,
// without the express permission of mgrierson. The conditions of this
// Licence are imposed upon the receiver of the product. You
// may be held legally responsible for any copyright infringement that is
// caused or encouraged by your failure to abide by these terms and
// conditions.	
// 
// You are not permitted under this Licence to use this Software
// commercially. Use for which any financial return is received shall be
// defined as commercial use, and includes (1) integration of all or part
// of the source code or the Software into a product for sale or license
// by or on behalf of Licensee to third parties or (2) use of the
// Software or any derivative of it for research with the final aim of
// developing software products for sale or license to a third party or
// (3) use of the Software or any derivative of it for research with the
// final aim of developing non-software products for sale or license to a
// third party, or (4) use of the Software to provide any service to an
// external organisation for which payment is received. If you are
// interested in using the Software commercially, please contact mgrierson to
// negotiate a licence. 
// */
/*
 LICENCE
 
 Earworm/Incus "The Software" © Goldsmiths College, University of London
 
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
 may be held legally responsible for any copyright infringement that is
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

#import "mediaStreamer.h"
#import <AVFoundation/AVFoundation.h>
#import <AVFoundation/AVAsset.h>
#include <unistd.h>


@implementation mediaStreamer

-(mediaStreamer*) init:(int)bufferSize withSampleRate:(float)rate withChannels:(int)channels {
	self = [super init];
	numChannels = channels;
	sampleRate = rate;
	bufSize = bufferSize;
	buffer = (float*)malloc(bufSize * sizeof(float));
    [self reset];
	return self;
}

- (void) reset {
	memset(buffer, 0, sizeof(float) * bufSize);
	readPos = writePos = 0;
    cancelled = NO;
	state = ABOUTTOSTREAM;
    restartTrig = NO;
}

- (void) stop {
    cancelled = YES;
}

- (void) restart {
    restartTrig = YES;
    [self stop];
}

- (void) onRestart {
    [self reset];
    [self initStream:url withTitle:tracktitle withArtist:trackartist withDuration:duration];
}

- (void)initStream:(NSURL*)assetURL withTitle:(NSString*)title withArtist:(NSString*)artist withDuration:(double)dur{
	state = ABOUTTOSTREAM;
	NSLog(@"InitStream: %@, %@, %@, %f\n", [assetURL absoluteString], title, artist, dur);
    if (title   != NULL) {
        tracktitle = [[NSString alloc] initWithString:title];
    }else{
        tracktitle = @"[no title specificed]";
    }
    if (artist != NULL) {
        trackartist = [[NSString alloc] initWithString:artist];
    }else{
        trackartist = @"[no artist specified]";
    }
	duration = dur;
	
	url = assetURL;
	[url retain];
    [tracktitle retain];
    [trackartist retain];
	
	//read file in new thread
	NSOperationQueue *queue = [NSOperationQueue new];
	NSInvocationOperation *operation = [[NSInvocationOperation alloc] 
										initWithTarget:self
										selector:@selector(bufferFile) 
										object:nil];
	[queue addOperation:operation]; 
	[operation release];
	
}

- (void)bufferFile {
	NSLog(@"Buffering File\n");
	NSDictionary *options = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:NO] forKey:AVURLAssetPreferPreciseDurationAndTimingKey];
	
	AVURLAsset * asset = [AVURLAsset URLAssetWithURL:url options:options];
	
	NSError *error = nil;
	AVAssetReader * filereader= [AVAssetReader assetReaderWithAsset:(AVAsset *)asset error:&error];
	
	[filereader retain];
	
	if (nil == error) {
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool required here, according to iphone forum posts
		//http://objective-audio.jp/2010/09/avassetreaderavassetwriter.html		
		NSDictionary *audioSetting = [NSDictionary dictionaryWithObjectsAndKeys:
									  [NSNumber numberWithFloat:sampleRate],AVSampleRateKey,
									  [NSNumber numberWithInt:numChannels],AVNumberOfChannelsKey,	//how many channels has original? 
									  [NSNumber numberWithInt:32],AVLinearPCMBitDepthKey, //was 16
									  [NSNumber numberWithInt:kAudioFormatLinearPCM], AVFormatIDKey,
									  [NSNumber numberWithBool:YES], AVLinearPCMIsFloatKey,  //was NO
									  [NSNumber numberWithBool:0], AVLinearPCMIsBigEndianKey,
									  [NSNumber numberWithBool:NO], AVLinearPCMIsNonInterleaved,
									  [NSData data], AVChannelLayoutKey, nil];	
		AVAssetReaderAudioMixOutput * readaudiofile = [AVAssetReaderAudioMixOutput assetReaderAudioMixOutputWithAudioTracks:(asset.tracks) audioSettings:audioSetting];
		//		AVAssetTrack * songTrack = [asset.tracks objectAtIndex:0];
		//		AVAssetReaderTrackOutput * readaudiofile = [[AVAssetReaderTrackOutput alloc] initWithTrack:songTrack outputSettings:audioSetting];
		[readaudiofile retain];  
		
		
		
		if([filereader canAddOutput:(AVAssetReaderOutput *)readaudiofile]) {
			[filereader addOutput:(AVAssetReaderOutput *)readaudiofile]; 
			if ([filereader startReading]) {
				
				while(!cancelled) {
					if (writePos - readPos < bufSize / 2) {
						CMSampleBufferRef ref = [readaudiofile copyNextSampleBuffer];
						
						if(NULL != ref) {
							//finished?
							if (CMSampleBufferDataIsReady(ref)) {
								
								CMItemCount countsamp= CMSampleBufferGetNumSamples(ref); 	
								UInt32 frameCount = countsamp; 
								CMBlockBufferRef blockBuffer;
								AudioBufferList audioBufferList;
								//allocates new buffer memory
								CMSampleBufferGetAudioBufferListWithRetainedBlockBuffer(ref, NULL, &audioBufferList, sizeof(audioBufferList),NULL, NULL, 0, &blockBuffer);
								
								float *filebuffer = (float *) audioBufferList.mBuffers[0].mData; 
								
								uint copyStart = (writePos % bufSize);
								uint copyLength = (frameCount * numChannels);
								uint copyEnd = copyStart + copyLength;
								if (copyEnd < bufSize) {
									memcpy(buffer + copyStart, filebuffer, sizeof(float) * copyLength);
								}else{
									uint firstChunk = copyLength - (copyEnd % bufSize);
									memcpy(buffer + copyStart, filebuffer, sizeof(float) * firstChunk);
									memcpy(buffer, filebuffer + firstChunk, sizeof(float) * (copyLength - firstChunk));
								}
								writePos += copyLength;
								//								NSLog(@"Read %d frames. Writepos: %llu, readpos: %llu.\n", frameCount, writePos, readPos);
								
								CFRelease(ref);
								CFRelease(blockBuffer);
								
								// If no frames were returned, conversion is finished
								if(0 == frameCount) {
									printf("Streamer: end, no more frames\n");
									//									state = ENDOFSTREAM;
									break;
									
								}
							} else {
								printf("Streamer: end, no more data\n");
								//								state = ENDOFSTREAM;
								break;
							}
						} 
						else {
							printf("Streamer: end\n");
							//							state = ENDOFSTREAM;
							break;
						}
					}else{
						state = STREAMING;
						usleep(100);
					}
					
				}
			} 	
		}
		
		[pool drain];
	}
	[filereader release];
    if (restartTrig) {
        restartTrig = NO;
        [self onRestart];
    }
}

- (float) getPosInSeconds {
    return readPos / (float) sampleRate / numChannels;
}

- (float) getTimeLeftInSeconds {
    return duration - [self getPosInSeconds];
}


- (BOOL)getNextBuffer:(float*)outBuffer outBufferSize:(uint)count {
	BOOL ok = NO;
	if (STREAMING == state) {
		//NSLog(@"w: %lld, r: %lld, c: %u \n", writePos, readPos, count);
		if (writePos - readPos > count) {
			ok = YES;
			uint copyStart = (readPos % bufSize);
			uint copyEnd = copyStart + count;
			if (copyEnd < bufSize) {
				memcpy(outBuffer, buffer + copyStart, sizeof(float) * count);
			}else{
				uint firstChunk = count - (copyEnd % bufSize);
				memcpy(outBuffer, buffer + copyStart, sizeof(float) * firstChunk);
				memcpy(outBuffer + firstChunk, buffer, sizeof(float) * (count - firstChunk));
			}
			readPos += count;
            //NSLog(@"%f", [self getTimeLeftInSeconds]);
		}else{
			memset(outBuffer, 0, sizeof(float) * count);
			if (readPos > 0) {
                state = ENDOFSTREAM;
                NSLog(@"Setting ENDOFSTREAM\n");
            }
		}
	}
	return ok;
}


@end
