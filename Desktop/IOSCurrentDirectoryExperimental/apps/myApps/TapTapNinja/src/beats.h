/*
 *  beats.h
 *  mfccs
 *
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


#pragma once

#include "vectorDelay.h"
#include "movingAverage.h"
#include "movingRMS.h"
#include "medianFilter.h"
#include <list>

enum beatPhases {
	WAITING, INBEAT
};

typedef pair<float, doubleVector> beatDataItem;
typedef list<beatDataItem> beatData;

using namespace std;

//class medianTrigger {
//public:
//	medianTrigger():filterSize(40), lowerBoundScale(0.0), upperBoundScale(0.0), holdTime(2) {init();}
//	medianTrigger(uint filterSize, float lowerBoundScale, float upperBoundScale, uint holdTime)
//	:filterSize(filterSize), lowerBoundScale(lowerBoundScale), upperBoundScale(upperBoundScale), holdTime(holdTime) {init();}
//	bool newFrame(float sig);
//	medianFilter sigMF;
//	float threshold;
//protected:
//	void init();
//	beatPhases phase;
//	uint filterSize;
//	uint holdTime;
//	float lowerBoundScale, upperBoundScale; //percentage of median filter
//	uint holdCount;
//};

//class beatTimeModel {
//public:
//	beatTimeModel();
//	bool newFrame(bool trig, double *melBands);
//	medianTrigger mtrig;
//private:
//	struct beatInfo {
//		float score;
//		uint framesSinceLastBeat;
//	};
//	typedef vector<beatInfo> beatInfoBin;
//	//hardcode for now
//	static const uint framesize = 1024;
//	static const uint hopsize = 256;
//	static const float framesPerSec = 44100.0 / hopsize;
//	
//	static const uint maxPeriod = 173;
//	static const uint minPeriod = 14;
//	static const uint binSize = 1;
//	float decayFactor;
////	vector<beatInfoBin> beatInfoBins;
//	vector<float> beatInfoBins;
//	list<uint> prevBeatTimes;
//	uint framesSinceLastBeat;
//	int currentWinner;
//	void calcScores();
//	medianFilter sigBeatAmpsMF;
//	float totalBeatAmp;
//	uint timeSincePulse, timeSinceTrig;
//};

class beatDetector {
public:
	int mfccCount, melBandCount;
	double mfccFrameDifference;
	double avgAmp, expAvgAmp;
	double onsetThreshold;
	movingAverage avgAmpMA;
	medianFilter avgMFCCVarMA;
	unsigned int maSize;
	float mfccVarThreshold;
	int beatSpaceSize;
	beatPhases phase;
	beatDetector(){mfccCount=13;melBandCount=42; init();}
	beatDetector(int mfccCount, int melBandCount, int maSize=320):mfccCount(mfccCount), melBandCount(melBandCount), maSize(maSize) {init();};
	
	void init();
	
	double processFrame(doubleVectorStream &mfccStream, double *melBands);
	
	//finding major beats
	beatData sigBeats;
	bool isSigBeat(bool isBeat, doubleVector beatMFCC);
	bool significantBeat;
	void clearSigBeats();
	int sigFramesAgo;
	int sigBeatLabel;
	
//	beatTimeModel btm;
	bool pause;
	
private:
	
	int beatSeqLen;
	int beatMaxFrames;
	doubleVector newBeat;

};