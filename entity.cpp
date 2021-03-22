#include "entity.h"

std::unordered_map<std::string, GameObject3*> Entity::entityMap;

Entity::Entity(const std::string& idName, float maxHealth) :
	idName(idName),
	health(maxHealth),
	maxHealth(maxHealth),
	healthCapacity(maxHealth)
{ }

Entity::Entity(const Entity& other) :
	idName(other.idName),
	health(other.health),
	maxHealth(other.maxHealth),
	healthCapacity(other.healthCapacity)
{ }

void Entity::Damage(float health)
{
	float actual = fminf(health, this->health);
	this->health -= actual;
	OnDamage(actual);
	if (this->health <= 0.0F)
	{
		this->health = 0.0F;
		OnDeath();
	}
}

void Entity::OnDamage(float health)
{

}

void Entity::OnDeath()
{

}

Attackable::Attackable(double radius, float subHealth, bool isGroup) :
	radius(radius),
	subHealth(subHealth),
	isGroup(isGroup),
	connected(nullptr)
{ }

Attackable::Attackable(const Attackable& other) :
	radius(other.radius),
	subHealth(other.subHealth),
	isGroup(other.isGroup),
	connected(nullptr)
{ }

namespace
{
	RequestList<Attackable*> attackables;
}

void Attackable::OnEnable()
{
	GameObject3* next = gameObject;
	while (true)
	{
		if (Entity* entity = next->GetComponent<Entity>())
		{
			connected = entity;

			break;
		}
		next = next->transform.GetParent()->GetOwner();
	}

	if (isGroup)
		attackables.Add(this);
}

void Attackable::OnDisable()
{
	if (isGroup)
		attackables.Remove(this);
}

void Attackable::Attack(float damage)
{
	connected->Damage(damage);
	float actual = fminf(damage, this->subHealth);
	subHealth -= actual;
	OnAttack(actual);
	if (subHealth <= 0.0F)
	{
		OnSubDeath();
		subHealth = 0.0F;
	}
}

void Attackable::OnAttack(float damage) { }

void Attackable::OnSubDeath() { }

namespace World
{
	//double maxStep = 0.5;

	bool SphereIntersect(const DVector3& rayStart, const DVector3& rayDir, double range, double sphereRadius)
	{
		double sphereRadius2 = sphereRadius * sphereRadius;
		double tca = -glm::dot(rayStart, rayDir);
		double d2 = glm::dot(rayStart, rayStart) - tca * tca;
		if (d2 > sphereRadius2)
			return false;
		double thc = sqrt(sphereRadius2 - d2);

		// Furthest is in front of start and closest is within range.
		return tca + thc >= 0.0 && tca - thc <= range;
	}

	// if t0Min != nullptr, will only be true if t0 is smaller than t0Min
	bool RotatedBoxIntersect(const DVector3& rayStart, const DVector3& rayDir, Transform3* box, double range, double* t0Min)
	{
		const double tolerance = 0.001;

		DVector4 posVertex4 = box->GetMatrix() * DVector4(1.0, 1.0, 1.0, 1.0);
		DVector4 negVertex4 = box->GetMatrix() * DVector4(0.0, 0.0, 0.0, 1.0);
		DVector3 posVertex(posVertex4.x, posVertex4.y, posVertex4.z);
		DVector3 negVertex(negVertex4.x, negVertex4.y, negVertex4.z);
		
		// TODO: May be transpose the way I am using it, can just switch operations also 
		DMatrix3x3 posNormals = box->GetWorldRotationMatrix3x3(); // Positive cube normals equal local coord axes
		DMatrix3x3 negNormals = -posNormals; // TODO: May be able to take the negative result instead of storing this whole matrix!
		
		// Allowed because they are treated as row vectors.
		DVector3 posC = (posVertex - rayStart) * posNormals;
		DVector3 negC = (negVertex - rayStart) * negNormals;

		DVector3 post = posC / (rayDir * posNormals);
		DVector3 negt = negC / (rayDir * negNormals);

		// Init to large values, well out of any reasonable range.
		double t0 = 1000.0;
		double t1 = 1000.0;

		DVector3 lowerBound = DVector3(-tolerance);
		DVector3 upperBound = DVector3(box->GetWorldScale()) + DVector3(tolerance);
		for (int i = 0; i < 6; ++i)
		{
			double t = i < 3 ? (&post.x)[i] : (&negt.x)[i - 3];
			if (isfinite(t)) // t for parallel planes will not be finite (0 dot product from perpendicular normal).
			{
				DVector3 pos = rayStart + rayDir * t;
				DVector3 posInBox = (pos - negVertex) * posNormals;

				if (posInBox.x >= lowerBound.x && posInBox.y >= lowerBound.y && posInBox.z >= lowerBound.z && posInBox.x <= upperBound.x && posInBox.y <= upperBound.y && posInBox.z <= upperBound.z)
				{
					if (t0 == 1000.0)
						t0 = t;
					else if (t != t0)
					{
						t1 = t;
						if (t0 > t1)
							std::swap(t0, t1);

						// Furthest is in front of start and closest is within range.
						if (t1 >= 0.0 && t0 <= range)
						{
							if (t0Min != nullptr)
							{
								if (fmax(t0, 0.0) < *t0Min)
								{
									*t0Min = fmax(t0, 0.0);
									return true;
								}
								else
									return false;
							}
							else
								return true;
						}
					}
				}
			}
		}

		return false;
	}

	void AttackPathRecusionElement(GameObject3* based, float rawDamage, const DVector3& start, const DVector3& dir, double range, double* t0Min, Attackable** target)
	{
		for (Transform3* childTransform : based->transform.GetChildren())
		{
			//std::cerr << "has a child" << std::endl;
			if (Attackable* subAttackable = childTransform->GetOwner()->GetComponent<Attackable>())
			{
				//std::cerr << "has a good child" << std::endl;
				// TODO: 
				if (SphereIntersect(start - subAttackable->gameObject->transform.GetWorldPosition(), dir, range, subAttackable->radius))
				{
					//std::cerr << "pre-intersect" << std::endl;
					if (RotatedBoxIntersect(start, dir, childTransform, range, t0Min))
					{
						std::cerr << "intersect" << std::endl;
						*target = subAttackable;
					}
				}
			}
			else
			{
				AttackPathRecusionElement(childTransform->GetOwner(), rawDamage, start, dir, range, t0Min, target);
			}
		}
	}

	void AttackPath(float rawDamage, const DVector3& start, const DVector3& dir, double range)
	{
		attackables.ProcessRequests();

		double t0Min = 1000.0;
		Attackable* target = nullptr;

		// TODO: Remember, go through all and select the closest!
		for (Attackable* attackable : attackables)
			if (SphereIntersect(start - attackable->gameObject->transform.GetWorldPosition(), dir, range, attackable->radius))
			{
				//std::cerr << "group intersect" << std::endl;
				AttackPathRecusionElement(attackable->gameObject, rawDamage, start, dir, range, &t0Min, &target);
			}

		if (target != nullptr)
			target->Attack(rawDamage);

		/*double distance = glm::distance(start, end);
		int steps = ceil(distance / maxStep);
		for (int i = 0; i <= steps; ++i)
		{
			double s = static_cast<double>(i) / steps;
			DVector3 pos = 
		}*/
	}
}
