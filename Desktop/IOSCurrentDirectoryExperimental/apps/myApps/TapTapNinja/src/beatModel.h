/*
 *  beatModel.h
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

#include "sys/types.h"
#include <iostream>
#include <vector>
#include <list>
#include "medianFilter.h"
#include "btAlignedObjectArray.h"

using namespace std;

class beatModel {
public:
	beatModel();
	void init();
	bool newFrame(bool trig, float strength, btAlignedObjectArray<float> &fluxSignature);
	float getAvgBeatSize() { return beatSize.value();}
	bool wasLongBeat() {return longBeatFlag;}
	bool wasSignificantBeat() {return sigBeatFlag;}
	int currentTempo;
private:
	struct beatInfo {
		uint framesSinceLastBeat;
		btAlignedObjectArray<float> fluxSignature;
		float strength;
		bool strongBeat;
		float centroid;
	};
	typedef list<beatInfo> beatList;
	struct sigBeatCandidate {
		btAlignedObjectArray<float> fluxSignature;
		float centroid;
		float age;
//		float score;
	};
	typedef list<sigBeatCandidate> sigBeatCandidateList;
//	typedef vector<beatInfo> beatInfoBin;
	static const uint maxPeriod = 90;
	static const uint maxPrevBeats = maxPeriod * 4;
	static const uint minPeriod = 14;
	static const uint binSize = 1;
	float decayFactor;
	vector<float> beatInfoBins;
	beatList prevBeatTimes;
	uint framesSinceLastBeat;
	int currentWinner;
	void calcScores();
	medianFilter beatSize;
	bool longBeatFlag;
	bool sigBeatFlag;
	sigBeatCandidateList sigBeatCandidates;
	int timeSinceSigBeat;
	bool lastBeatWasPotentialSigBeat;
	
	medianFilter strengthMF;
};