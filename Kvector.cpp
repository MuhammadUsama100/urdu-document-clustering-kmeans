# include <iostream>
#include <algorithm>
#include <numeric>
# include <cstdlib>
# include <cstdio> 
# include <exception>

using namespace std;

class ExceptionOccur : public exception {
public:
	virtual const char* what() {
		return "Array out of Bound \nLowerBound exception ";
	}

};

class LowerBound : public  ExceptionOccur {
public:
	const char* what() {
		return "Array out of Bound \nLowerBound exception ";
	}

};

class UpperBound : public ExceptionOccur {
public:
	const char* what() {
		return "Array out of Bound \nUpperBound exception ";
	}

};

class SizeError : public ExceptionOccur {
public:
	const char* what() {
		return "InValid  Size";
	}

};


template <class T>
class Kvector {

private:
	T* data;
	int size;
	ExceptionOccur* except;
public:
	Kvector() :size(0), data(0) {}
	Kvector(int size) {
		if (size > 0) {
			this->size = size;
			this->data = new T[this->size];

		}
		else {
			//exception size 
			size = 0;
			data = 0;
			throw new SizeError;
		}

	}
	Kvector(T* start, T* end) :size(0), data(0) {
		while (start != end) {
			this->pushBack(*start);
			start++;
		}
	}
	Kvector(const  Kvector& obj) {
		this->size = obj.size;
		this->data = new T[this->size];
		memcpy(this->data, obj.data, sizeof(T) * this->size);
	}
	Kvector operator  =  (const Kvector& obj) {
		if (this != &obj) {
			this->size = obj.size;
			//	delete[] this->data;
			this->data = new T[this->size];
			memcpy(this->data, obj.data, sizeof(T) * this->size);
		}
		return *this;
	}
	
	void clear() {
		delete[] this->data;
		this->size = 0;
	}
	void insertData(T data, int position) {
		this->data[position] = data;
	}

	void reSize(int size) {
		if (size > this->size) {
			Kvector <T>  obj;
			obj = *this;
			this->size = size;
			
			this->data = new T[this->size];
			memcpy(this->data, obj.data, sizeof(T) * obj.size);

		}
		else {
			//exception  size
			throw new SizeError;
		}
	}
	T& operator [] (int position) {
		if (position > this->size) {
			//exception  bound 
			throw new UpperBound;
		}
		else if (position < 0) {
			throw new LowerBound;
		}
		else {
			return this->data[position];
		}
	}
	const T& operator [] (int position) const {
		if (position > this->size) {

			throw new UpperBound;
		}
		else if (position > 0) {
			throw new LowerBound;
		}
		else {
			return this->data[position];
		}
	}

	void pushBack(T  data) {
		reSize(this->size + 1);
		this->data[this->size - 1] = data;
	}
	int getSize() const {
		return this->size;
	}
	void insert(T* positiondis, T* positionSourceStart, T* positionSourceEnd) {
		int index = 0;
		while (this->data + index != positiondis) index++;
		this->size = index;
		while (positionSourceStart != positionSourceEnd) {
			this->pushBack(*positionSourceStart);
			//	index ++; 
			positionSourceStart++;
		}
	}
	T* begin() {
		return data;
	}
	T* end() {
		return (data + this->getSize());
	}
};
