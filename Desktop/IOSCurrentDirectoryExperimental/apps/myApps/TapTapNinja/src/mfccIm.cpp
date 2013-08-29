/*
 *  mfccIm.cpp
 *  bastardPop
 *
 *  Created by Chris on 28/10/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */

#include "mfccIm.h"
#include <Accelerate/Accelerate.h>

void mfccIm::setup(int _numFrames, int _dataSize) {
	heatMapScheme.loadImage(ofToDataPath("classic.png"));
	pix = heatMapScheme.getPixels();
  	cout << _numFrames << endl;
	frames = _numFrames;
	dataSize = _dataSize;
	ofFbo::Settings settings;  
	texSize = 1024;
	settings.width = texSize;  
	settings.height = texSize;  
	settings.internalformat = GL_RGB;  
	settings.numSamples = 0;
	settings.useDepth = false;  
	settings.useStencil = false;
	fftMagPos = 0;
	coefWidth = texSize / (float) dataSize;
	coefHeight = max(1.f, texSize / (float) frames);
	fbo.allocate(settings);
	currentLine=0;
	fbo.begin();
	ofSetColor(255,255,255);
	ofFill();
	ofRect(0,0,texSize,texSize);
	fbo.end();
}

void mfccIm::update() {
	fbo.begin();
	if (coefHeight > 1.0) {
		//draw all
		while(fftMagPos < fftMags.size() - 1) {
			addLine(&(fftMags[fftMagPos][0]));
			fftMagPos++;
		}						
	}else{
		//draw one for each line, skip rest
		float u = frames / texSize;
		for(int i=0; i < texSize; i++) {
			int idx = floor(u*i);
			addLine(&(fftMags[idx][0]));			
		}
	}
	fbo.end();
	fftMags.clear();
}

void mfccIm::addMags(float *data) { 
	btAlignedObjectArray<float> newMags;
	fftMags.push_back(newMags);
	fftMags.back().resize(dataSize);
	memcpy(&(fftMags.back()[0]), data, dataSize * sizeof(float));
}

void mfccIm::addLine(float *data) {
	static const int speedMult = 4.0;
	for(int i=0; i < dataSize; i+=speedMult) {		
		int idx = 255 - min(255.f, max(0.f, data[i] / 20.f * 255.f));
		int pos = heatMapScheme.getWidth() * 3 * idx;
		ofSetColor(pix[pos], pix[pos+1], pix[pos+2]);
		ofRect(i * coefWidth, currentLine * coefHeight, coefWidth * speedMult, coefHeight);
	}	
	currentLine++;
	
}

void mfccIm::render(vector<btAlignedObjectArray<float> > data, int mfccCount) {
	ofFbo::Settings settings;  
	const int texsize = 1024;
	settings.width = texsize;  
	settings.height = texsize;  
	settings.internalformat = GL_RGB;  
	settings.numSamples = 0;  
	settings.useDepth = false;  
	settings.useStencil = false;
	fbo.allocate(settings);
	fbo.begin();

	const float coefWidth = texsize / (float) mfccCount;
	const float coefHeight = texsize / (float) data.size();
	for(int i=0; i < data.size(); i++) {
		btAlignedObjectArray<float> mfcc;
		mfcc.resize(mfccCount,0);
		//get average of mfccs for this beat
		for(int j=0; j < data[i].size(); j+=mfccCount) {
			vDSP_vadd(&(mfcc[0]), 1, &(data[i][j]), 1, &(mfcc[0]), 1, mfccCount);
		}
		float count = data.size();
		vDSP_vsdiv(&(mfcc[0]), 1, &count, &(mfcc[0]), 1, mfccCount);
		
		for(int j=0; j < mfccCount; j++) {
			cout << mfcc[j] << ",";
			ofSetColor(0, max(0.0, mfcc[j] * 40.0 * 255.0), 0);
			ofRect(j * coefWidth, i * coefHeight, coefWidth, coefHeight);
		}
		cout << endl;
		
	}
//	ofSetColor(255,0,0);
//	ofFill();
//	ofRect(0,0,512,512);
	fbo.end();
}