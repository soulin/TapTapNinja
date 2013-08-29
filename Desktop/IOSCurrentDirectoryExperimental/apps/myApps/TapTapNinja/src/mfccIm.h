/*
 *  mfccIm.h
 *  bastardPop
 *
 *  Created by Chris on 28/10/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */
#include "ofMain.h"
#include "btAlignedObjectArray.h"
class mfccIm {
public:
	void setup(int numFrames, int dataSize);
	void addLine(float *data);
	void addMags(float *data);
	void update();
	void render(vector<btAlignedObjectArray<float> > data, int mfccCount);
	ofFbo fbo;
	vector<btAlignedObjectArray<float> > fftMags;
private:
	int frames;
	int texSize;
	int currentLine;
	int fftMagPos;
	ofImage heatMapScheme;
	unsigned char* pix;
	float coefWidth, coefHeight;
	int dataSize;
};