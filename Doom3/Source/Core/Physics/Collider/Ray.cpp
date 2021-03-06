#include "Ray.h"
#include "../Graphics/DebugGraphics.h"
doom::physics::Ray::Ray()
	: mOrigin{}, mNormal{0, 0, -1}
{
}
doom::physics::Ray::Ray(const math::Vector3 & origin, const math::Vector3& normal)
	: mOrigin{ origin }, mNormal{ normal.normalized() }
{

}

void doom::physics::Ray::Render(eColor color, bool drawInstantly /*= false*/)
{
#ifdef DEBUG_MODE
	graphics::DebugGraphics::GetSingleton()->DebugDraw3DLine(this->mOrigin, this->mOrigin + this->mNormal * 100000000.0f, color, drawInstantly);
#endif
}

math::Vector3 doom::physics::Ray::GetNormal() const
{
	return this->mNormal;
}

void doom::physics::Ray::SetNomral(const math::Vector3& normal)
{
	this->mNormal = normal.normalized();
}

void doom::physics::Ray::SetNomral_Unsafe(const math::Vector3& normal)
{
	this->mNormal = normal;
}

bool doom::physics::Ray::IsPointOnRay(const math::Vector3& point)
{
	return math::dot((point - this->mOrigin).normalized(), this->mNormal) < math::epsilon<float>();
}

math::Vector3 doom::physics::Ray::GetClosestPoint(const Ray& ray, const math::Vector3& point)
{
	auto vecToPoint = point - ray.mOrigin;
	float t = math::dot(ray.mNormal, vecToPoint);
	if (t <= 0)
	{
		return ray.mOrigin;
	}
	else
	{
		return ray.mOrigin + ray.mNormal * t;
	}
}

doom::physics::ColliderType doom::physics::Ray::GetColliderType() const
{
	return doom::physics::ColliderType::Ray;
}
