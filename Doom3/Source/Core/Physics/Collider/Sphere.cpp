#include "Sphere.h"
#include "../Graphics/DebugGraphics.h"

#include "AABB.h"

void doom::physics::Sphere::Render(eColor color, bool drawInstantly /*= false*/)
{

#ifdef DEBUG_MODE
	auto debugGraphics = graphics::DebugGraphics::GetSingleton();

	const float intervalRadian = math::PI * 2 / 72.0f;


	float deltaTheta = math::PI / 12;
	float deltaPhi = 2 * math::PI / 10;

	

	math::Vector3 exVertex{ };
	math::Vector3 currentVertex{};
	
	float theta{ 0 };
	for (int ring = 0 ; ring < 11; ring++) { //move to a new z - offset 
		float phi{ 0 };

		theta += deltaTheta;
		for (int point = 0 ; point < 11; point++) { // draw a ring
			phi += deltaPhi;
			currentVertex.x = sin(theta) * cos(phi) * this->mRadius;
			currentVertex.y = sin(theta) * sin(phi) * this->mRadius;
			currentVertex.z = cos(theta) * this->mRadius;
			currentVertex += this->mCenter;

			if (point != 0)
			{
				debugGraphics->DebugDraw3DLine(exVertex, currentVertex, color, drawInstantly);
			}
			exVertex = currentVertex;
		}
	}

	theta = 0;
	for (int ring = 0; ring < 11; ring++) { //move to a new z - offset 
		float phi{ 0 };

		theta += deltaTheta;
		for (int point = 0; point < 11; point++) { // draw a ring
			phi += deltaPhi;
			currentVertex.z = sin(theta) * cos(phi) * this->mRadius;
			currentVertex.y = sin(theta) * sin(phi) * this->mRadius;
			currentVertex.x = cos(theta) * this->mRadius;
			currentVertex += this->mCenter;

			if (point != 0)
			{
				debugGraphics->DebugDraw3DLine(exVertex, currentVertex, color, drawInstantly);
			}
			exVertex = currentVertex;
		}
	}
#endif

}



doom::physics::ColliderType doom::physics::Sphere::GetColliderType() const
{
	return doom::physics::ColliderType::Sphere;
}

