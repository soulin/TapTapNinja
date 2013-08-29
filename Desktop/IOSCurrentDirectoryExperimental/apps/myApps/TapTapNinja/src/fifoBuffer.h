/*
 *  fifoBuffer.h
 *  bastardPop
 *
 *  Created by Chris on 07/11/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */

#include "btAlignedObjectArray.h"
#include <iostream>
#include <vector>
using namespace std;

template<class T>
class fifoBuffer {
public:

	fifoBuffer() {size=0; cout << "Fifo create\n";};
	
	virtual void init(unsigned int _size, T initVal) {
		delete[] data;
		size = _size;
		cout << "Fifo: " << size << endl;
		data = (T*)malloc(sizeof(T) * size);
		for(int i=0; i < size; i++) data[i] = initVal;
		nextElement = 0;
	};
	
	virtual ~fifoBuffer() {
		delete[] data; 
		cout << "Fifo destroy\n";
	}
	
	void addsample(T v) {
		data[nextElement] = v;
		if (++nextElement == size) {
			nextElement = 0;
		};
	}

protected:
	unsigned int nextElement;
	T *data;
	int size;
};