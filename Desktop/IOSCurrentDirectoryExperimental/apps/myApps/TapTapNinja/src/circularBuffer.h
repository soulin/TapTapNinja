/*
 *  circularBuffer.h
 *  bastardPop
 *
 *  Created by Chris on 16/09/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 
 //test
 
 //	circularBuffer<float> x;
 //	x.resize(12);
 //	btAlignedObjectArray<float> d;
 //	d.resize(10);
 //	for(int i=0; i < d.size(); i++) {
 //		d[i] = i;
 //	}
 //	x.push_back(d);
 //	d.resize(3);
 //	x.pop_front(d);
 //	for(int i=0; i < d.size(); i++) cout << d[i] << ","; cout << endl;
 //	x.pop_front(d);
 //	for(int i=0; i < d.size(); i++) cout << d[i] << ","; cout << endl;
 //	x.pop_front(d);
 //	for(int i=0; i < d.size(); i++) cout << d[i] << ","; cout << endl;
 //	d.resize(10);
 //	for(int i=0; i < d.size(); i++) {
 //		d[i] = i;
 //	}
 //	x.push_back(d);
 //	d.resize(3);
 //	x.pop_front(d);
 //	for(int i=0; i < d.size(); i++) cout << d[i] << ","; cout << endl;
 //	x.pop_front(d);
 //	for(int i=0; i < d.size(); i++) cout << d[i] << ","; cout << endl;
 //	x.pop_front(d);
 //	for(int i=0; i < d.size(); i++) cout << d[i] << ","; cout << endl;
 
 */

#pragma once
#include "btAlignedObjectArray.h"
#include <vector>

template<typename T>
class circularBuffer : public btAlignedObjectArray<T> {
public:
	unsigned int head, tail;
	circularBuffer() {
		head=0;
		tail=0;
	}
	
	void push_back(btAlignedObjectArray<T> &newBlock) {
		if (tail + newBlock.size() < this->size()) {
			memcpy(&(this->at(tail)), &(newBlock[0]), sizeof(T) * newBlock.size());
//			for(int i=0; i < newBlock.size(); i++) (*this)[tail+i] = newBlock[i];
			tail += newBlock.size();
		}else{
			const unsigned int firstSegSize = this->size() - tail;
			memcpy(&(this->at(tail)), &(newBlock[0]), sizeof(T) * firstSegSize);
//			for(int i=0; i < firstSegSize; i++) (*this)[tail+i] = newBlock[i];
			const unsigned int remainingSize = newBlock.size() - firstSegSize;
			memcpy(&(this->at(0)), &(newBlock[firstSegSize]), sizeof(T) * remainingSize);
//			for(int i=0; i < remainingSize; i++) (*this)[i] = newBlock[i + firstSegSize];
			tail = remainingSize;
		}
	}
	
	void pop_front(btAlignedObjectArray<T> &block) {
		if (head + block.size() < this->size()) {
			memcpy(&(block[0]), &(this->at(head)), sizeof(T) * block.size());
//			for(int i=0; i < block.size(); i++) block[i] = (*this)[head+i];
			head += block.size();
		}else{
			const unsigned int firstSegSize = this->size() - head;
			memcpy(&(block[0]), &(this->at(head)), sizeof(T) * firstSegSize);
//			for(int i=0; i < firstSegSize; i++) block[i] = (*this)[head+i];
			const unsigned int remainingSize = block.size() - firstSegSize;
			memcpy(&(block[firstSegSize]), &(this->at(0)), sizeof(T) * remainingSize);
//			for(int i=0; i < remainingSize; i++) block[i+firstSegSize] = (*this)[i];
			head = remainingSize;
		}
	}
	
	int bufferSize() {
		int result = 0;
		if (tail >= head)
			result = tail - head;
		else {
			result = (this->size() - head) + tail;
		}
		return result;

	}
};