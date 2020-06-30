#pragma once
#include "GameObject.h"
#include "Vector3D.h"

class CollisionDetection
{
private:
	static CollisionDetection* _instance;

public:
	static CollisionDetection* Instance() { static CollisionDetection* _instance = new CollisionDetection; return _instance;	}

	bool CheckSphereAndPlaneCollisionDetection(GameObject* sphere, GameObject* plane);

	bool CheckSphereAndSphereCollisionDetection(GameObject* sphere, GameObject* sphere2);

};