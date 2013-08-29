/*LICENCE

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


#include "debugGraph.h"


debugGraph::debugGraph(int windowSize, float min, float max, int topx, int topy, int width, int height, bool lines) : lines(lines), windowSize(windowSize) {
	vals.resize(windowSize, 0);
	labels.resize(windowSize, "");
	x = topx; y = topy; w = width; h = height;
	ymin = min; ymax = max;
	xinc = width / float(windowSize);
	yrange = fabs(max) + fabs(min);
	markX = -1;
	r = b = a = 200;
	g = 255;
	resetMinMax();
	valsIdx = 0;

}

debugGraph::~debugGraph() {
	
}

void debugGraph::resetMinMax() {
	maxVal = FLT_MIN;
	minVal = FLT_MAX;
}

void debugGraph::mark() {
	markX = vals.size();	
}

void debugGraph::addVal(float val) {
	vals[valsIdx % windowSize] = val;
	labels[valsIdx % windowSize] = "";
	valsIdx++;
	markX --;
	if (val > maxVal) {
		maxVal = val;
	}
	if (val < minVal) {
		minVal = val;
	}
}

void debugGraph::addVal(float val, string label) {
	addVal(val);
	labels[(valsIdx-1) % windowSize] = label;
}


void debugGraph::draw() {
	//draw axis
	ofSetColor(200, 200, 200, a / 2);
	ofLine(x,y,x,y+h);
	float udpY = fabs(ymin) / (fabs(ymin) + fabs(ymax));
	udpY = y + h - (udpY * h);
	ofLine(x, udpY, x + w, udpY);
	ofSetColor(r,g,b, a);
	for(int i=0; i < windowSize; i++) {
		int idx = (valsIdx + i) % windowSize;
		float yh = vals[idx] / yrange * h;
		if (lines) {
			if (i>1) {
				int idxLast = (valsIdx + i - 1) % windowSize;
				float yhLast = vals[idxLast] / yrange * h;
				ofLine(x + ((i-1) * xinc), udpY - yhLast, x + (i * xinc), udpY - yh);
			}
		}
		else
		{
			ofRect(x + (i * xinc), udpY - yh, 1, 1);
		}
		if (labels[idx] != "") {
			font.drawString(labels[idx], x + (i * xinc), udpY - yh - 20);
		}
	}
	if (markX>0) {
		ofSetColor(255,0,0, a);
		float mx = x + (xinc * markX);
		ofLine(mx, y, mx, y + h);
	}
}

float debugGraph::getMaxVal() {
	return maxVal;
}

float debugGraph::getMinVal() {
	return minVal;
}

