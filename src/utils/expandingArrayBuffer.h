#pragma once

#include "../glm/vec3.hpp"
#include "../glm/vec2.hpp"
#include "../logger.h"
#include <cstring>

template <class T>
class ExpandingArrayBuffer {
private:
	T* buffer;
	int size;
	int count = 0;
	void expand(int size);
public:
	ExpandingArrayBuffer(int initialSize);
	~ExpandingArrayBuffer();
	inline const T* getBuffer() const { return (const T*)this->buffer;}
	inline int getSize() const { return this->size; }
	inline int getCount() const { return this->count; }
	void push(T value);
};


template <class T>
ExpandingArrayBuffer<T>::ExpandingArrayBuffer(int initialSize) : size(initialSize) {
	this->buffer = new T[initialSize];
}

template <class T>
ExpandingArrayBuffer<T>::~ExpandingArrayBuffer() {
	delete[] this->buffer;
}

template <class T>
void ExpandingArrayBuffer<T>::expand(int newSize) {
	if(newSize < this->size) {
		throw "Failed to expand buffer: Size is smaller than buffer size!";
	}
	logMessage(INFO, "Expanding array buffer from " + std::to_string(this->size) + " to " + std::to_string(newSize));
	T* newBuffer = new T[newSize];
	
	memcpy(newBuffer, this->buffer, count*sizeof(T));

	this->size = newSize;

	delete[] this->buffer;
	this->buffer = newBuffer;

}

template <class T>
void ExpandingArrayBuffer<T>::push(T value) {
	if(this->count+1 >= this->size) {
		expand(this->size > 0x10000 ? this->size+0x10000 : this->size*2);
	}

	this->buffer[this->count] = value;
	count++;
}
