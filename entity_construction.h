#pragma once

#include "Tundra/engine.h"

#include "entity.h"

class Disjointable : public Attackable
{ 
public:
	Disjointable(double radius, float subHealth);
	Disjointable(const Disjointable& other);

	void OnSubDeath() override;

	can_copy(Disjointable);
};

namespace EntityConstruction
{
	// All arguments are in pixels
	// pos is relative to parent and pivot is relative to pos
	GameObject3* AddBodyPiece(GameObject3* parent, const IVector3& size, const DVector3& pos, const DVector3& pivot);
}
