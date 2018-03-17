#include "Scene.h"

bool Scene::Initialize() {
	if (!isDataInitialized) {
		InitializeResource();
		isDataInitialized = true;
	}
	return true;
}