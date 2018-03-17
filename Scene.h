#pragma once
#include "MyPG.h"

//
class Scene {

	bool isDataInitialized = false;

public:
	explicit Scene() {}
	virtual ~Scene() {}

	virtual void InitializeResource() = 0;
	virtual bool Initialize();
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Finalize() = 0;

};