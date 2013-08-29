//
//  MyGuiView.m
//  iPhone Empty Example
//
//  Created by theo on 26/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MediaPickerView.h"
//#include "ofxiPhoneExtras.h"


@implementation MediaPickerView

// called automatically after the view is loaded, can be treated like the constructor or setup() of this class
-(void)viewDidLoad 
{
//	myApp = (testApp*)ofGetAppPtr();
//    self.view.transform = CGAffineTransformMakeRotation(0.5 * M_PI);
}

//mediaPicker code by tapsquare
- (void)mediaPicker:(MPMediaPickerController *)mediaPicker 
  didPickMediaItems:(MPMediaItemCollection *)mediaItemCollection 
{
	[self dismissModalViewControllerAnimated:YES];
	for (MPMediaItem* item in mediaItemCollection.items) 
    {
		NSString* title = [item valueForProperty:MPMediaItemPropertyTitle];
		NSString* artist = [item valueForProperty:MPMediaItemPropertyArtist];
		NSNumber* dur = [item valueForProperty:MPMediaItemPropertyPlaybackDuration]; 
		//NSTimeInterval is a double
		//		duration_ = [dur doubleValue]; 
		
		//MPMediaItemPropertyArtist
		NSURL* assetURL = [item valueForProperty:MPMediaItemPropertyAssetURL];
		if (nil == assetURL)
        {
			/**
			 * !!!: When MPMediaItemPropertyAssetURL is nil, it typically means the file
			 * in question is protected by DRM. (old m4p files)
			 */
			return;
		}
		[streamer initStream:assetURL withTitle:title withArtist:artist withDuration:[dur doubleValue]];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"streamCued" object:nil];
	}
    [self view].hidden = YES;
}

- (void)mediaPickerDidCancel:(MPMediaPickerController *)mediaPicker 
{
	[self dismissModalViewControllerAnimated:YES];	
    [self view].hidden = YES;
//    [self.view removeFromSuperview];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"mediaPickerCancelled" object:nil];
}



- (void)showMediaPicker:(mediaStreamer*) stream {
	streamer = stream;
	[self view].hidden = NO;

	MPMediaPickerController* mediaPicker = [[[MPMediaPickerController alloc] initWithMediaTypes:MPMediaTypeMusic] autorelease];
	mediaPicker.delegate = self;
	[mediaPicker shouldAutorotateToInterfaceOrientation:UIInterfaceOrientationLandscapeLeft];
	[self presentModalViewController:mediaPicker animated:YES];
}
@end
