/*
 *  boundarySegmentation.cpp
 *  mfccs
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


#include <iostream>
#include "boundarySegmentation.h"
#include "vectorUtils.h"
#include <Accelerate/Accelerate.h>

using namespace std;

void boundarySegments::init() {
	thisBeatAvg = (double*) malloc(sizeof(double) * mfccCount);
	beatMean = (double*) malloc(sizeof(double) * mfccCount);
	vDSP_vclrD(thisBeatAvg, 1, mfccCount);
	beatFrameCount=0;
	boundarySegWindowSize = 5;
	beatSizes.resize(boundarySegWindowSize, 0);	
	avgDist = lastAvgDist = 0.0;
	doubleVector mfccFrame;
	mfccFrame.resize(mfccCount, 0);
	mfccBeatAvgs = vectorDelay<doubleVector>(boundarySegWindowSize, mfccFrame);
	numAvgs = boundarySegWindowSize;
	distsToMean = (double*) malloc(sizeof(double) * boundarySegWindowSize);
	canTrigBoundary = true;
	framesAgo = 0;
	overThreshold = true;
	medianAvgDist = medianFilter(19,0);
}

boundarySegments::~boundarySegments() {
	delete[] thisBeatAvg, beatMean, distsToMean;
}

bool ascOrder(double i, double j) {return i < j;}

bool boundarySegments::processFrame(bool beat, double *mfccs) {
	bool onBoundary = false;
	if (beat) {
		if (beatFrameCount > minBeatSize) {
			beatSizes.push_back(beatFrameCount);
			beatSizes.pop_front();
			//			for(int k=0; k < beatSizes.size(); k++) cout << beatSizes[k] << ",";
			//			cout << endl;
			
			lastAvgDist = avgDist;
			
			//take average of beat
			double divisor = (double) beatFrameCount;
			vDSP_vsdivD(thisBeatAvg, 1, &divisor, thisBeatAvg, 1, mfccCount);
			
			//distAvgs = distBetween(thisBeatAvg, lastBeatAvg, mfccCount);
			//					cout << "New beat, dist from last = " << distAvgs << endl;
			//					
			//					memcpy(lastBeatAvg, thisBeatAvg, sizeof(double) * mfccCount);
			//					
			
			//add beat to queue
			doubleVector tmp;
			tmp.resize(mfccCount);
			std::copy(thisBeatAvg, thisBeatAvg + mfccCount, tmp.begin());
			mfccBeatAvgs.addFrame(tmp);
			
			//calc variance
			vDSP_vclrD(beatMean, 1, mfccCount);
			
			//sum averages
			for(int z=0; z < mfccBeatAvgs.values.size(); z++) {
				vDSP_vaddD(beatMean, 1, &(mfccBeatAvgs.getFrame(z)[0]), 1, beatMean, 1, mfccCount);
			}
			//get mean
			vDSP_vsdivD(beatMean, 1, &numAvgs, beatMean, 1, mfccCount);
			
			//get distances from mean
			for(int z=0; z < boundarySegWindowSize; z++) {
//				distsToMean[z] = distBetween(beatMean, &(mfccBeatAvgs.getFrame(z)[0]), mfccCount);
				distsToMean[z] = cosineDistance(beatMean, &(mfccBeatAvgs.getFrame(z)[0]), mfccCount);
			}
			
			//find nearest to mean
			double maxVal;
			vDSP_Length maxValIdx;
			vDSP_maxviD(distsToMean, 1, &maxVal, &maxValIdx, boundarySegWindowSize);

			for(int z=0; z < boundarySegWindowSize; z++) {
//				distsToMean[z] = distBetween(beatMean, &(mfccBeatAvgs.getFrame(maxValIdx)[0]), mfccCount);
				distsToMean[z] = cosineDistance(beatMean, &(mfccBeatAvgs.getFrame(maxValIdx)[0]), mfccCount);
			}
			
			//average dist
//			vDSP_meanvD(distsToMean, 1, &avgDist, boundarySegWindowSize);
			
			//median distance
			vDSP_vsortD(distsToMean, boundarySegWindowSize, 1);
//			vector<double> medDist(distsToMean, distsToMean + boundarySegWindowSize);
//			sort(medDist.begin(), medDist.end(), ascOrder);
			int medianIdx = floor(boundarySegWindowSize / 2.0);
			avgDist = distsToMean[medianIdx];
			
			avgDist *= avgDist;
			
			medianAvgDist.addsample(avgDist);
//			threshold = 0.095;
//			threshold = medianAvgDist.value() * 2.4;
			threshold = medianAvgDist.value() * 3;
			
			if (avgDist > threshold && canTrigBoundary) { 
				overThreshold = true;
			}
			//if descending from threshold
			if (avgDist < lastAvgDist && overThreshold && canTrigBoundary) {
				canTrigBoundary = false;
				bool beatSizesOK = true;
				
				//is there valid data to make decision?
				for(int k=0; k < boundarySegWindowSize; k++) if (beatSizes[k] > 700) {beatSizesOK = false; break;}
				if (beatSizesOK) {
					onBoundary = true;
					
					//when was it?
					framesAgo = 0;
					//get real trig point
					for(int k=floor((float)boundarySegWindowSize / 2.0); k < beatSizes.size(); k++) {
						framesAgo += beatSizes[k];
					}
				}
			}
			else{
//				if (avgDist < 0.055) {
				if (avgDist < threshold * 0.6) {
					canTrigBoundary = true;
					overThreshold = false;
					
				}
			}
			
			//reset average
			vDSP_vclrD(thisBeatAvg, 1, mfccCount);
			beatFrameCount = 0;
			
			//analyse averages
		}
		
	}
	vDSP_vaddD(thisBeatAvg, 1, mfccs, 1, thisBeatAvg, 1, mfccCount);	
	beatFrameCount++;
	return onBoundary;
}
