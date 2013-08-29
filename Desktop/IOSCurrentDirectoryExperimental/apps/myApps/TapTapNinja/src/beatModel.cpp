/*
 *  beatModel.cpp
 *  bastardPop
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


/*
 ideas:  
 -flux as beat significance  - beat score is product of strength of itself and the beat being measured from
     --or use 1 + (proportion of strength)
 -compare flux signature  - indexes of bands that contributed to flux measurement
 -threshold adjustment with entropy  - http://ezbitz.com/2009/05/08/calculate-a-file-shannon-entropy-in-c/
 */

#include "beatModel.h"
#include <cmath>
#include "vectorUtils.h"

beatModel::beatModel() {
	decayFactor = 0.99;
	cout.precision(3);
	init();
};

void beatModel::init() {
	currentWinner = -1;
	framesSinceLastBeat = 0;	
	beatInfoBins.clear();
	beatInfoBins.resize((maxPeriod - minPeriod) / binSize, 0.0);
	currentTempo = 60;
	beatSize = medianFilter(8,30);
	timeSinceSigBeat = 0;
	strengthMF = medianFilter(5, 0);
}

//is val a multiple of factor, allowing for some leeway
//e.g. isApproximageMultiple(138.0, 70.0, 0.1) should be true
//e.g. isApproximageMultiple(138.0, 70.0, 0.1) should be true
inline bool isApproximateMultiple(const float val, const float factor, const float tolerance) {
	float v = val / factor;
	v = fmod(v, 1.0f);
	if (v > 0.5) v = 1.0 - v;
	return v < tolerance;
}


//todo: VDSP this (and maximilian version)
float spectralCentroid(btAlignedObjectArray<float> &bins) {
	float x=0, y=0;
//	for(int i=0; i < bins.size(); i++) { 
//		x += fabs(bins[i]) * (i+1);
//		y += fabs(bins[i]);
//	}
	//low end centroid
//	for(int i=0; i < bins.size() / 4; i++) {  
//		x += fabs(bins[i]) * (i+1);
//		y += fabs(bins[i]);
//	}
//	for(int i=bins.size() / 4; i < bins.size(); i++) { 
//		y += fabs(bins[i]);
//	}
//	return y != 0 ? x / y : 0;
	for(int i=0; i < bins.size() / 8; i++) { 
		x += fabs(bins[i]);
	}
	return x;
}

bool beatModel::newFrame(bool trig, float strength, btAlignedObjectArray<float> &fluxSignature) {
	framesSinceLastBeat++;
	timeSinceSigBeat++;
	longBeatFlag = false;
	//use largest poss tempo period for comparison, with 1/4 beat range
	int tempoFactor = currentTempo;
	if (tempoFactor * 2 < maxPeriod) tempoFactor *= 2;
	if (trig) {
		sigBeatFlag = false;
		if (framesSinceLastBeat >= minPeriod && framesSinceLastBeat < maxPeriod) {
			
			
			//reduce all scores by alpha
			for( int i=0; i < beatInfoBins.size(); i++) {
				beatInfoBins[i] *= decayFactor;
			}
			
			vector<float> sortedBins = beatInfoBins;
			sort(sortedBins.begin(), sortedBins.end());
			
			beatInfo newBeat;
			newBeat.framesSinceLastBeat = framesSinceLastBeat;
			newBeat.fluxSignature.copyFromArray(fluxSignature);
			newBeat.strength = strength;
			newBeat.centroid = spectralCentroid(fluxSignature);
			newBeat.strongBeat = newBeat.centroid <= (strengthMF.value() * 1.2);
			prevBeatTimes.push_front(newBeat);
			strengthMF.addsample(newBeat.centroid);
			
			
			beatSize.addsample(framesSinceLastBeat);

			//add in time to prev beats
			beatList::iterator it;
			uint totalTime=0;
//			cout << "Dists: ";
			float potentialSigBeat = false;
			for(it = prevBeatTimes.begin(); it != prevBeatTimes.end(); it++) {
				totalTime += (*it).framesSinceLastBeat;
				if (totalTime >= minPeriod && totalTime < maxPeriod) {
					int whichBin = floor((totalTime - minPeriod) / binSize);
					int targetBin = whichBin;
					//check bins either side for clamping
					if (whichBin > 0) {
						if (beatInfoBins[whichBin-1] > beatInfoBins[whichBin]) {
							if (beatInfoBins[whichBin-1] > sortedBins[sortedBins.size() * 0.9]) {
								targetBin = whichBin-1;
							}
						}
					}
					if (whichBin < beatInfoBins.size()-1) {
						if (beatInfoBins[whichBin+1] > beatInfoBins[whichBin]) {
							if (beatInfoBins[whichBin+1] > sortedBins[sortedBins.size() * 0.9]) {
								targetBin = whichBin+1;
							}
						}
					}
					beatInfoBins[targetBin] += (1.0);
					

				}
				//compare to beat
				if (it != prevBeatTimes.begin()) {
					if ((*it).strongBeat && newBeat.strongBeat) {
						int thisTime = (totalTime - (*it).framesSinceLastBeat);
						if (isApproximateMultiple(thisTime, tempoFactor, 0.1)) {
//							cout << "*";
							potentialSigBeat = true;
						}
					}
//					float dist = cosineDistance(&(fluxSignature[0]), &((*it).fluxSignature[0]), fluxSignature.size());
//					int thisTime = (totalTime - (*it).framesSinceLastBeat);
//					cout << dist << "[" << thisTime << "]";
//					if (dist < 0.2) {
//						cout << "*" ;
//						if (isApproximateMultiple(thisTime, tempoFactor, 0.1)) {
//							cout << "*";
//							potentialSigBeat = true;
//						}
//					}
//					cout << ",";
				}
				
			}
//			cout << endl;
			
			
			if (potentialSigBeat) {
//				//is it close to any current sigbeats?
//				sigBeatCandidate *closestCandidate = NULL;
//				float minV = 99999;
//				sigBeatCandidateList::iterator sigIt;
//				vector<float> centroids;
//				for(sigIt = sigBeatCandidates.begin(); sigIt != sigBeatCandidates.end(); sigIt++) {
//					float dist = cosineDistance(&(fluxSignature[0]), &((*sigIt).fluxSignature[0]), fluxSignature.size());
//					if (dist < minV) {
//						minV = dist;
//						closestCandidate = &(*sigIt);
//					}
//					centroids.push_back((*sigIt).centroid);
//				}
//				//found a match
//				if (minV < 0.5) {
//					cout << "MinV: " << minV << ", centroid: " << closestCandidate->centroid << endl;
//					//is it the bassiest?
//					sort(centroids.begin(), centroids.end());
//					for(int i=0; i < centroids.size(); i++) cout << centroids[i] << ",";
//					cout << endl;
//					int centroidMargin = round(centroids.size() * 0.4);
//					if (closestCandidate->centroid <= centroids[centroidMargin]) {
//						sigBeatFlag = true;
//						timeSinceSigBeat = 0;
//					}
//					//replace flux signature with this one
//					closestCandidate->fluxSignature.copyFromArray(fluxSignature);
//					closestCandidate->centroid = spectralCentroid(fluxSignature);
//				}else{
//					sigBeatCandidate newCandidate;
//					newCandidate.fluxSignature.copyFromArray(fluxSignature);
//					newCandidate.centroid = spectralCentroid(fluxSignature);
//					newCandidate.age = 0;
//					sigBeatCandidates.push_back(newCandidate);
//				}
//				cout << "Candidates: " << sigBeatCandidates.size() << endl;

			}
			sigBeatCandidateList::iterator sigIt = sigBeatCandidates.begin();
			while(sigIt != sigBeatCandidates.end()) {
				(*sigIt).age += framesSinceLastBeat;
				if ((*sigIt).age > maxPrevBeats) {
					//trim old candidate
					sigBeatCandidates.erase(sigIt);
				}
				sigIt++;
			}
			//			if (isApproximateMultiple(timeSinceSigBeat, tempoFactor, 0.1)) {
//				sigBeatFlag = true;
//			}
			
			//trim times from end of list
			if (totalTime > maxPrevBeats) {
				int timeToDelete = totalTime - maxPrevBeats;
				it = prevBeatTimes.end();
				it--;
				while(timeToDelete > 0) {
					timeToDelete -= (*it).framesSinceLastBeat;
					prevBeatTimes.pop_back();
					it--;
				}
			}
			
			if (newBeat.strongBeat && potentialSigBeat) {
				sigBeatFlag = true;
				timeSinceSigBeat = 0;
			}else{
				sigBeatFlag = false;
			}
			
			calcScores();
			framesSinceLastBeat = 0;
		}else{
			if (framesSinceLastBeat <= minPeriod) 
				trig = false;
		}
	}else{
		if (framesSinceLastBeat >= maxPeriod) {
			framesSinceLastBeat = 0;
			longBeatFlag = true;
		}
	}
	return trig;
}

void beatModel::calcScores() {
	float maxVal=0; int maxIdx=-1;
	//search for quarter beats, 60->180 bpm
	for(int i=28 - minPeriod; i < 87 - minPeriod; i++) {
//		for(int i=56 - minPeriod; i < beatInfoBins.size(); i++) {
		if (beatInfoBins[i] > 1) {
//			cout << (i + minPeriod) << ":" << beatInfoBins[i] << ",";
		}
		if (beatInfoBins[i] > maxVal) {
			maxVal = beatInfoBins[i];
			maxIdx = i;
		}
	}
	//cout << endl;
	currentTempo = maxIdx + minPeriod;
	currentWinner = maxIdx;
	//cout << "Beat time: " << currentTempo << endl;
	
}

