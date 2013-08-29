/*
 *  beats.cpp
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
#include "beats.h"
#include "vectorUtils.h"
#include <Accelerate/Accelerate.h>


using namespace std;

//void medianTrigger::init() {
//	phase = WAITING;
//	sigMF = medianFilter(filterSize, 0);
//	holdCount = 99999;
//}
//
//bool medianTrigger::newFrame(float sig) {
//	bool isBeat = false;
//	sigMF.addsample(sig);
//	holdCount++;
//	switch(phase) {
//		case WAITING:
//			if (holdCount > holdTime) {
//				threshold = sigMF.value() + upperBoundScale;
//				if (sig > threshold) {
//					isBeat = true;
//					phase = INBEAT;
//					holdCount = 0;
//				}
//			}
//			break;
//		case INBEAT:
//			threshold = sigMF.value() + lowerBoundScale;
//			if (sig < threshold) {
//				phase = WAITING;
//			}
//			break;
//	}
//	return isBeat;
//}


//beatTimeModel::beatTimeModel() {
//	beatInfoBins.resize((maxPeriod - minPeriod) / binSize);
//	framesSinceLastBeat = 0;
//	decayFactor = 0.99;
//	//cout.setf(0,ios::floatfield);  
//	cout.precision(2);
//	currentWinner = -1;
//	totalBeatAmp=0;
//	mtrig = medianTrigger(100, 0.8, 1.3, 10);
//	timeSincePulse = 0;
//};
//
//bool beatTimeModel::newFrame(bool trig, double *melBands) {
//	bool sigBeat = false;
//	framesSinceLastBeat++;
//	timeSincePulse++;
//	timeSinceTrig++;
//	bool kick = mtrig.newFrame(melBands[0] + 8.0);
//	if (kick) {
//		cout << "Kick: " << timeSinceTrig << endl;
//		if (timeSinceTrig < 15) {
//			sigBeat = true;
//			timeSinceTrig = 9999;
//		}else{
//			timeSincePulse = 0;
//		}
//	}
//	if (trig) {
//		if (framesSinceLastBeat >= minPeriod && framesSinceLastBeat < maxPeriod) {
//
//			//reduce all scores by alpha
//			for( int i=0; i < beatInfoBins.size(); i++) {
//				beatInfoBins[i] *= decayFactor;
//			}
//			
//			int whichBin = floor((framesSinceLastBeat - minPeriod) / binSize);
//			beatInfoBins[whichBin] += totalBeatAmp;
//			//add in time to prev beats
//			list<uint>::iterator it;
//			uint totalTime=framesSinceLastBeat;
//			for(it = prevBeatTimes.begin(); it != prevBeatTimes.end(); it++) {
//				totalTime += *it;
//				if (totalTime >= minPeriod && totalTime < maxPeriod) {
//					int whichBin = floor((totalTime - minPeriod) / binSize);
//					beatInfoBins[whichBin] += 1.0;
//					if (whichBin > currentWinner - 4 && whichBin < currentWinner + 4) {
//						timeSinceTrig = 0;
//						if (timeSincePulse < 10) {
//							sigBeat = true;
//							timeSincePulse = 9999;
//						}
//					}					
//				}
//			}
//			
//			//trim times from end of list
//			if (totalTime > maxPeriod) {
//				int timeToDelete = totalTime - maxPeriod;
//				it = prevBeatTimes.end();
//				it--;
//				while(timeToDelete > 0) {
//					timeToDelete -= *it;
//					it--;
//				}
//			}
//			prevBeatTimes.push_front(framesSinceLastBeat);
//			
//			calcScores();
//			framesSinceLastBeat = 0;
//		}else{
//			if (framesSinceLastBeat > 100) {
//				framesSinceLastBeat = 0;
//			}
//		}
//	}
//	return sigBeat;
//}
//
//void beatTimeModel::calcScores() {
//	float maxVal=0; int maxIdx=0;
//	for(int i=floor(maxPeriod / 3.0) - minPeriod; i < beatInfoBins.size(); i++) {
//		if (beatInfoBins[i] > maxVal) {
//			maxVal = beatInfoBins[i];
//			maxIdx = i;
//		}
////		float binTotal=0;
////		for(int j=0; j < beatInfoBins[i].size(); j++) {
////			binTotal += beatInfoBins[i][j].score;
////		}
////		if (beatInfoBins[i] > 0.5) {
////			cout << (i + minPeriod) << ":" << beatInfoBins[i] << ",";
////		}
//	}
//	cout << (maxIdx + minPeriod) << endl;
//	currentWinner = maxIdx;
//	
//	//cout << endl;
//	//		float maxVal=0; int maxIdx=0;
//	//		for(int i=0; i < beatInfoBins.size(); i++) {
//	//			float binTotal=0.0;
//	//			for(int j=0; j < beatInfoBins[i].size(); j++) {
//	//				binTotal += beatInfoBins[i][j].score;
//	//			}
//	//			cout << binTotal << ",";
//	//			if (binTotal > maxVal) {
//	//				maxIdx = i;
//	//				maxVal = binTotal;
//	//			}
//	//		}
//	//		cout << endl;
//	//		int winningPeriod = minPeriod + (maxIdx * binSize);
//	//		cout << "Winning bin: " << maxIdx << ", " << winningPeriod << " - " << (winningPeriod + binSize) << endl;
//	//		
//	//		//estimate bpm
//	//		if (beatInfoBins[maxIdx].size() > 0) {
//	//			float avgFrames;
//	//			for(int i=0; i < beatInfoBins[maxIdx].size(); i++) {
//	//				avgFrames += beatInfoBins[maxIdx][i].framesSinceLastBeat;
//	//			}
//	//			avgFrames /= beatInfoBins[maxIdx].size();
//	//			float approxBPM = (framesPerSec / (avgFrames * 4.0)) * 60;
//	//			cout << "Approx bpm: " << approxBPM << endl;
//	//		}
//	
//}



void beatDetector::init() {
	avgAmpMA = movingAverage(maSize);
//	avgMFCCVarMA = movingAverage(100);
	avgMFCCVarMA = medianFilter(100);
	beatSpaceSize = 9999;
	phase = WAITING;
	significantBeat = false;
	sigFramesAgo = 0;
	beatMaxFrames = 10;
	beatSeqLen = beatMaxFrames * mfccCount;
	newBeat = doubleVector(beatSeqLen, 0);
}

const unsigned int maxSigBeats = 3;
inline bool compareScoreDesc(beatDataItem a, beatDataItem b) {
	return a.first > b.first;
}



medianFilter ampMF = medianFilter(10,10);
int beatFrame = 0;

bool beatDetector::isSigBeat(bool isBeat, doubleVector beatMFCC) {
	bool isSignificant = false;
	if (!isBeat) {
		//add to beat sequence
		if (beatFrame < beatMaxFrames) {
			int startOffset = mfccCount * beatFrame;
			for(int i=startOffset; i < startOffset + mfccCount; i++) {
				newBeat[i] = beatMFCC[i - startOffset];
			}
		}
		beatFrame++;
	}else{
		sigFramesAgo = beatFrame;
		if (sigBeats.size() == 0) {
			sigBeats.push_back(beatDataItem(1, newBeat));
		}else{
			//get dist from current beats
			float closestValue = 999;
			int closestIdx = -1;
			beatData::iterator it, closestBeat;
			int i=0;
			float scoresMean=0;
			for ( it=sigBeats.begin() ; it != sigBeats.end(); it++ ){
				float dist = cosineDistance(&(newBeat[0]), &((*it).second[0]), beatSeqLen);
				if (dist < closestValue) {
					closestValue = dist;
					closestBeat = it;
					closestIdx = i;
				}
//				cout << (*it).first << ",";
				scoresMean += (*it).first;
				i++;
			}
			scoresMean /= sigBeats.size();
			
//			cout << "\nClosest: " << closestValue << endl;

			if (closestValue < 0.15) {
				//increase score of beat
				(*closestBeat).first++;
//				vDSP_vaddD(&(newBeat[0]), 1, &((*closestBeat).second[0]), 1, &((*closestBeat).second[0]),  1, mfccCount);
//				double mult = 0.5;
//				vDSP_vsmulD(&((*closestBeat).second[0]), 1, &mult, &((*closestBeat).second[0]), 1, mfccCount);
//				isSignificant = closestIdx > sigBeats.size() * 0.2 && closestIdx < sigBeats.size() * 0.5 ; //&& amp>0;
				//find mean idx
				int meanIdx=0;
				for ( it=sigBeats.begin() ; it != sigBeats.end(); it++ ){
					if ((*it).first < scoresMean) break;
					meanIdx++;
				}
				//cout << "MeanIdx: " << meanIdx << endl;
				
//				isSignificant = closestIdx > sigBeats.size() * 0.1 ; //&& amp>0;
				isSignificant = closestIdx > 0 ; //&& amp>0;
				if (isSignificant) {
					sigBeatLabel = closestIdx;
				}
//				isSignificant = closestIdx > meanIdx ; //&& amp>0;
//				isSignificant = true ; //&& amp>0;
				//sigBeats.push_back(beatDataItem((*closestBeat).first+1, newBeat));
				
			}
			else{
				//if not near any beats, then add in
				sigBeats.push_back(beatDataItem((*sigBeats.begin()).first, newBeat));
			}
//			for ( it=sigBeats.begin() ; it != sigBeats.end(); it++ ){
//				(*it).first *= 0.95;
//			}
			//sort
			sigBeats.sort(compareScoreDesc);


			//trim end
			int maxLength = 50;
			if (sigBeats.size() > maxLength) {
				it = sigBeats.begin();
				advance(it, maxLength);
				sigBeats.erase(it, sigBeats.end());
			}
		}
		beatFrame=0;

	}
	return isSignificant;
}

void beatDetector::clearSigBeats() {
	sigBeats.clear();
}

double beatDetector::processFrame(doubleVectorStream &mfccStream, double *melBands) {
//	static int i=0;
//
////	cout << "ica" << icadata.rows << "," << icadata.columns << endl;
//	for (int m=0; m < 5; m++) {
//		icadata( m, i) = melBands[m];
//	}
//	
//	i++;
//
//	if (i == 10) {
//		cout << "Doing ica...\n";
//		FastICASettings<double> settings;
//		settings.convergenceTolerance = 0.01;
//		settings.maximumIterations = 1;
//		settings.contrastFunction = ContrastFunction<double>::GAUSSIAN;
//		settings.initializationType = FastICASettings<double>::RANDOM;
//		FastICANew<double> fastIca(settings, &icadata, 123456UL, false);
//		fastIca.runFastICA();
//		NicMatrix<double> mixing;
//		NicMatrix<double> invMixing;
//
//		fastIca.mixingMatrix( mixing );
//		invert( mixing, invMixing );
//		
//		cout << "ICA: rows: " << invMixing.rows << ", cols: " << invMixing.columns << endl;
//		
//		i = 0;
//	}
	
	
	
	significantBeat = false;
	//mfcc diff
//	mfccFrameDifference = distBetween(&(mfccStream.getMostRecentFrame()[0]), &(mfccStream.getLastFrame()[0]), mfccCount) * 1.0;
	mfccFrameDifference = cosineDistance(&(mfccStream.getMostRecentFrame()[0]), &(mfccStream.getLastFrame()[0]), mfccCount) * 2;
	mfccFrameDifference = exp(mfccFrameDifference * 4.0) / exp(4.0);
	avgMFCCVarMA.addsample(mfccFrameDifference);

	//avg amp
	vDSP_meanvD(melBands, 1, &avgAmp, melBandCount);
	avgAmp += 10.0; avgAmp /= 5.0;
	
	expAvgAmp = exp(avgAmp * 4.0) / exp(4.0);
//	avgAmpMA.addsample(avgAmp);
	avgAmpMA.addsample(expAvgAmp);
	onsetThreshold = (avgAmpMA.value() * 1.2);

	static int trigType=-1;
	double trig=0;

	mfccVarThreshold = avgMFCCVarMA.value() * 2;
	trig = mfccFrameDifference - mfccVarThreshold;
	//cout << "M: " << mfccFrameDifference << endl;
	double ampTrig = expAvgAmp - onsetThreshold;
	bool trigBeat = false;
	if (WAITING == phase && beatSpaceSize > 5) {
		if (trig > 0) {
			trigBeat = true;
			trigType = 0;
		}
		if (ampTrig > 0) {
			trigBeat = true;
			trigType = 1;
		}
		if (trigBeat) {
			phase = INBEAT;
			beatSpaceSize=0;
		}
	}else {
		trig=0;
		ampTrig = 0;
	}
	if (INBEAT == phase) {
		if  (
			 (trigType == 0) && (mfccFrameDifference < mfccVarThreshold * 0.8)
			 ||
			 (trigType == 1) && (expAvgAmp < onsetThreshold * 0.8)
			){
			phase = WAITING;
		}
	}

//	significantBeat = isSigBeat(trigBeat, mfccStream.getMostRecentFrame());
//	significantBeat = btm.newFrame(trigBeat, avgAmp, melBands);
	
	beatSpaceSize++;
	
	return trigBeat ? 1.0 : 0.0;
}	
