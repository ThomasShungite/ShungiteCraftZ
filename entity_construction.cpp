#include "entity_construction.h"

#include <Tundra Engine/asset_loader.h>

Disjointable::Disjointable(double radius, float subHealth) :
	Attackable(radius, subHealth, false)
{ }

Disjointable::Disjointable(const Disjointable& other) :
	Attackable(other)
{ }

void Disjointable::OnSubDeath()
{
	std::cerr << "limb removed!" << std::endl;
	// TODO: Needs to adjust position to be old global!
	gameObject->transform.GetParent()->SetParent(nullptr);
}

namespace EntityConstruction
{
	GameObject3* AddBodyPiece(GameObject3* parent, const IVector3& size, const DVector3& pos, const DVector3& pivot)
	{
		static std::shared_ptr<Material3> material(new Material3("Entities"));
		static Mesh mesh = Load::OBJFile("Entities/box.obj");

		GameObject3* piece = Instantiate<GameObject3>(Transform3((pos + pivot) * (1.0 / 16.0)));

		GameObject3* renderBox = Instantiate<GameObject3>(Transform3(-pivot * (1.0 / 16.0), FVector3(size) * (1.0F / 16.0F)));
		renderBox->AddComponent<RenderComponent3>(mesh, material);
		renderBox->SetParent(piece);

		piece->SetParent(parent);

		return piece;
	}
}
