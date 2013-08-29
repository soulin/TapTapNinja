//
//  MyGuiView.h
//  iPhone Empty Example
//
//  Created by theo on 26/01/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#pragma once

#import <UIKit/UIKit.h>

//#include "testApp.h"
#import <MediaPlayer/MediaPlayer.h>
#import "mediaStreamer.h"


@interface MediaPickerView : UIViewController <MPMediaPickerControllerDelegate>
{
	IBOutlet UILabel *displayText;
	
    // points to our instance of testApp
    //testApp *myApp;		
	mediaStreamer *streamer;
}

-(void)showMediaPicker:(mediaStreamer*) stream;
@end
