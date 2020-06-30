#include "CollisionDetection.h"

bool CollisionDetection::CheckSphereAndPlaneCollisionDetection(GameObject* sphere, GameObject* plane)
{
	if (sphere->GetSphere()._type != "Sphere")
		return false;

	Vector3D _spherePos = sphere->GetTransform()->GetPosition();

	float ballDistance = plane->GetPlane()._direction * _spherePos - sphere->GetSphere()._radius - plane->GetPlane()._offset;

	if (ballDistance < 5.0f)
	{
		sphere->GetParticle()->_isGravityOn = false;
	}

	if (ballDistance <= 0.0f)
	{
		return true;
	}
	else
		return false;
}

bool CollisionDetection::CheckSphereAndSphereCollisionDetection(GameObject* sphere, GameObject* sphere2)
{
	if (sphere->GetSphere()._type != "Sphere" || sphere2->GetSphere()._type != "Sphere")
	{
		return false;
	}
	//sphere->GetCollisionPrimitive()->GetType();
	float distance = sphere->GetTransform()->GetPosition().Distance(sphere2->GetTransform()->GetPosition());

	return (distance < sphere->GetSphere()._radius + sphere2->GetSphere()._radius);
}
