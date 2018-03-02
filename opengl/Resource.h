#pragma once
#include <iostream>
#include <memory>
#include <list>
using namespace std;
class Resource;
typedef shared_ptr<Resource> StrongResourcePtr;
typedef weak_ptr<Resource> WeakResourcePtr;

class Resource
{

public:
	Resource();
	~Resource();


	// interface; these functions should be overridden by the subclass as needed
	virtual void VOnInit(const char * resourceString) { }  // called during the first update; responsible for setting the initial state (typically RUNNING)
	virtual void VOnUpdate(unsigned long deltaMs) = 0;  // called every frame

	WeakResourcePtr AttachResource(StrongResourcePtr p);
};

