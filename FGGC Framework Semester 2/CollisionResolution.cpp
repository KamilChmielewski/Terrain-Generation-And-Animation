#include "CollisionResolution.h"

void CollisionResolution::SphereAndPlaneCollisionResolution(GameObject* sphere, GameObject* plane)
{
	sphere->GetTransform()->SetPosition(sphere->GetTransform()->GetPosition().x, plane->GetTransform()->GetPosition().y + 0.5f, sphere->GetTransform()->GetPosition().z);
}

void CollisionResolution::SphereAndSphereCollisionResolution(GameObject* sphere, GameObject* sphere2)
{
	if (sphere->GetSphere()._type != "Sphere" || sphere2->GetSphere()._type != "Sphere")
	{
		return;
	}

	float totalRadius = sphere->GetSphere()._radius + sphere2->GetSphere()._radius;
	Vector3D firstPosition = sphere->GetTransform()->GetPosition();
	Vector3D secondPosition = sphere2->GetTransform()->GetPosition();
	Vector3D forDistance = firstPosition - secondPosition;

	float distance = forDistance.Magnitude();
	float depth = totalRadius - distance;

	Vector3D direction = firstPosition - secondPosition;

	direction.Normalise();

	firstPosition = sphere->GetTransform()->GetPosition() + direction * depth;
	secondPosition = sphere2->GetTransform()->GetPosition() - direction * depth;

	sphere->GetTransform()->SetPosition(firstPosition);
	sphere2->GetTransform()->SetPosition(secondPosition);
	CalculateCoefficientResolution(sphere, sphere2);
}

void CollisionResolution::CalculateCoefficientResolution(GameObject* object1, GameObject* object2)
{
	float e = object1->GetCoefficient() + object2->GetCoefficient() / 2;

	float Object1Mass = object1->GetParticle()->GetMass();
	float Object2Mass = object2->GetParticle()->GetMass();

	Vector3D Object1InitialVelocity = object1->GetParticle()->GetVelocity();
	Vector3D Object2InitialVelocity = object2->GetParticle()->GetVelocity();

	Vector3D temp1 = Object1InitialVelocity * (Object1Mass - (Object2Mass * e));
	Vector3D temp2 = Object2InitialVelocity * Object2Mass * (1.0f + e);

	Vector3D Object1FinalVelocity = (temp1 + temp2) / (Object1Mass + Object2Mass);

	temp1 = Object1InitialVelocity * Object1Mass;
	temp2 = Object2InitialVelocity * Object2Mass;

	Vector3D temp3 = Object1FinalVelocity * Object1Mass;
	Vector3D Object2FinalVelocity = (temp1 + temp2 - temp3) / Object2Mass;

	Vector3D Object1Normal = object1->GetTransform()->GetPosition() - object2->GetTransform()->GetPosition();
	Object1Normal.Normalise();

	Object1Normal.Invert();
	Vector3D Object2Normal = Object1Normal;

	Object1Normal.Invert();

	Vector3D Object1RelativeVelocity = Object1Normal * Vector3D::DotProduct(Object1Normal, Object1FinalVelocity);
	Vector3D Object2RelativeVelocity = Object2Normal * Vector3D::DotProduct(Object2Normal, Object2FinalVelocity);


	object1->GetParticle()->SetVelocity(Object1RelativeVelocity);
	object2->GetParticle()->SetVelocity(Object2RelativeVelocity);


}
