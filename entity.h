#pragma once

#include <engine.h>

class Entity : public Component3
{
protected:
	float health;
	float maxHealth; // One bar
	float healthCapacity; // Actual max
public:
	static std::unordered_map<std::string, GameObject3*> entityMap;

	const std::string idName;

	Entity(const std::string& idName, float maxHealth);
	Entity(const Entity& other);

	void Damage(float health);
	virtual void OnDamage(float health);
	virtual void OnDeath();

	can_copy(Entity);
};

// For things damaged by weaponry and such
// TODO: Need origin filter to avoid self
class Attackable : public Component3
{
	// The prism uses gameObject's data, assuming that the visual representation exists in the first octant
public:
	const double radius;
private:
	float subHealth; // Can be 0, does not change entity's base health. Damage will decrease both.
	bool isGroup; // Groups are for pre-intersection only. Groups can not be recursive (as of yet). A group must always be used (at the moment).
	Entity* connected;
public:
	Attackable(double radius, float subHealth, bool isGroup);
	Attackable(const Attackable& other);

	//void OnCreate() override;
	void OnEnable() override;
	void OnDisable() override;

	void Attack(float damage);

	virtual void OnAttack(float damage);
	virtual void OnSubDeath(); // When subHealth gets to 0.

	can_copy(Attackable);
};

namespace World
{
	void AttackPath(float rawDamage, const DVector3& start, const DVector3& dir, double range);
}

#define register_entity(gameObject) { GameObject3* gobj_macro = gameObject; Entity::entityMap[gameObject->GetComponent<Entity>()->idName] = gobj_macro; }
