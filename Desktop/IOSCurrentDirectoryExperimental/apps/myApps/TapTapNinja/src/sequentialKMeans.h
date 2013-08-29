/*
 *  sequentialKMeans.h
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


#include "accelerate/accelerate.h"
#include <stdio.h>
#include<string.h>
#include <stdlib.h>

class sequentialKMeans {
public:
	
	double alpha;
	unsigned int numCentroids;
	unsigned int dim;
	double **means;
	
	sequentialKMeans() : alpha(0.1), numCentroids(2), dim(2) {
		init();
	};

	sequentialKMeans(double alpha, unsigned int numCentroids, unsigned int dimensions) : alpha(alpha), numCentroids(numCentroids), dim(dimensions) {
		init();
	};
	
	inline void setCentroid(unsigned int index, double* val) {
		memcpy(means[index], val, sizeof(double) * dim);
	}
	
	inline void addSample(double* sample) {
		unsigned int lowestIdx=0;
		double lowestDistance = 9999999;
		for(int i=0; i < numCentroids; i++) {
			double dist = distBetween(sample, means[i], dim);
			if (dist < lowestDistance){ 
				lowestIdx = i;
				lowestDistance = dist;
			}
		}
		//m[~idx] = m[~idx] + (a * (x - m[~idx]));
		vDSP_vsubD(means[lowestIdx], 1, sample, 1, tmp, 1, dim);
		vDSP_vsmulD(tmp, 1, &alpha, tmp, 1, dim);
		vDSP_vaddD(tmp, 1, means[lowestIdx], 1, means[lowestIdx], 1, dim);
	}
	
	inline double distBetween(unsigned int centroidIdx1, unsigned int centroidIdx2) {
		return distBetween(means[centroidIdx1], means[centroidIdx2], dim);
	}

	~sequentialKMeans() {
		for(int i=0; i < numCentroids; i++) {
			delete[] means[i];
		}
		delete[] means, tmp;
	};
	
private:
	double *tmp;
	
	void init(){
		means = (double**)malloc(numCentroids * sizeof(double*));
		for(int i=0; i < numCentroids; i++) {
			means[i] = (double*) malloc(dim * sizeof(double));
		}
		tmp = (double*) malloc(dim * sizeof(double));
	}
	
	inline double distBetween(double *x, double *y, unsigned int count) {
		//t = y - x
		vDSP_vsubD(x, 1, y, 1, tmp, 1, count);
		//t.squared
		vDSP_vsqD(tmp, 1, tmp, 1, count);
		//t.sum
		double sum;
		vDSP_sveD(tmp, 1, &sum, count);
		return sqrt(sum);
	}
	
};