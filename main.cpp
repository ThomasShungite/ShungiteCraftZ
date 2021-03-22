#include <window.h>
#include <events_state.h>
#include <input.h>
#include <ui.h>
#include <rendering.h>
#include <pbr.h>

#include "block.h"
#include "chunk.h"
#include "renderers.h"
#include "mesh_factories.h"
#include "entity.h"
#include "entity_construction.h"

class ArrayUIContainer : public Component2
{
private:
	FVector2 componentSize;
	float borderSize;
	std::shared_ptr<ScaleConstraint> widthConstraint, heightConstraint;
public:
	ArrayUIContainer(const FVector2& componentSize, float borderSize) :
		componentSize(componentSize),
		borderSize(borderSize),
		widthConstraint(nullptr),
		heightConstraint(nullptr)
	{ }

	void OnEnable()
	{
		int xCountMax = static_cast<int>(floorf((gameObject->transform.GetScale().x * EventsState::windowWidth) / (componentSize.x + borderSize)));
		int yCountMax = static_cast<int>(floorf((gameObject->transform.GetScale().y * EventsState::windowHeight) / (componentSize.y + borderSize)));

		if (xCountMax == 0 || yCountMax == 0)
			return;

		widthConstraint = scale_constraint(CenterScaleConstraint(componentSize.x));
		heightConstraint = scale_constraint(CenterScaleConstraint(componentSize.y));

		int x = 0;
		int y = 0;
		for (Transform2* transform : gameObject->transform.GetChildren())
		{
			if (x >= xCountMax)
			{
				++y;
				if (y >= yCountMax)
					return;
				x = 0;
			}
			transform->WithConstraints(pos_constraint(LesserEdgePosConstraint(x * (componentSize.x + borderSize) + borderSize)), pos_constraint(GreaterEdgePosConstraint(y * (componentSize.y + borderSize) + borderSize)), widthConstraint, heightConstraint);
			++x;
		}
	}

	can_copy(ArrayUIContainer);
};

class Controller : public UpdateableComponent3
{
private:
	double currentSpeed;
	const double adjustmentConstant;
	float pitch;
	float yaw;
public:
	Controller(double speed, double adjustmentConstant) :
		currentSpeed(speed),
		adjustmentConstant(adjustmentConstant),
		pitch(0.0F),
		yaw(0.0F)
	{ }

	void OnCreate() override
	{
		float gap = 2.0F;
		float slotSize = 64.0F;
		int hotbarSlots = 12;
		{
			GameObject2* hotbar = Instantiate<GameObject2>(Transform2(nullptr, pos_constraint(LesserEdgePosConstraint(gap)), scale_constraint(CenterScaleConstraint(gap + (slotSize + gap) * hotbarSlots)), scale_constraint(CenterScaleConstraint(slotSize + gap * 2))));
			//hotbar->AddComponent<ArrayUIContainer>();
			for (int x = 0; x < hotbarSlots; ++x)
			{
				GameObject2* slot = Instantiate<GameObject2>(Transform2(pos_constraint(LesserEdgePosConstraint(gap + (slotSize + gap) * x)), nullptr, scale_constraint(CenterScaleConstraint(slotSize)), scale_constraint(AspectRatioScaleConstraint(1.0F))));
				slot->AddComponent<RenderComponent2>("UI/slot");
				slot->SetParent(*hotbar);
			}
			hotbar->Enable();
		}
	}

	void Update(const GameTime& time) override
	{
		if (Input::IsButtonDown(Input::RIGHT_MOUSE_BUTTON))
		{
			pitch -= 0.3F * EventsState::mouseDY;
			yaw -= 0.3F * EventsState::mouseDX;

			if (pitch > 89.9F)
				pitch = 89.9F;
			else if (pitch < -89.9F)
				pitch = -89.9F;

			yaw = fmodf(yaw, 360.0F);

			gameObject->transform.SetRotation(glm::quat(FVector3(pitch / 180.0F * PI, yaw / 180.0F * PI, 0.0F)));
		}

		if (Input::IsKeyDown(SDL_SCANCODE_Q))
		{
			currentSpeed += currentSpeed * adjustmentConstant * time.deltaTime;
		}
		if (Input::IsKeyDown(SDL_SCANCODE_E))
		{
			currentSpeed -= currentSpeed * adjustmentConstant * time.deltaTime;
		}

		FMatrix3x3 viewRotationMatrix = CreateMatrix::Rotation3x3(gameObject->transform.GetRotation());
		gameObject->GetComponent<Camera3>()->SetViewRotationMatrix(glm::lookAt(FVector3(0.0F, 0.0F, 0.0F), glm::normalize(FVector3(-viewRotationMatrix[2])), glm::normalize(FVector3(viewRotationMatrix[1]))));
		DVector2 forwardWalkVec = glm::normalize(FVector2(-viewRotationMatrix[2].x, -viewRotationMatrix[2].z)) * currentSpeed * time.deltaTime;
		DVector2 sideWalkVec = glm::normalize(FVector2(viewRotationMatrix[0].x, viewRotationMatrix[0].z)) * currentSpeed * time.deltaTime;

		DVector3 change(0.0, 0.0, 0.0);
		if (EventsState::keyStates[SDL_SCANCODE_W])
		{
			change.x += forwardWalkVec.x;
			change.z += forwardWalkVec.y;
		}
		if (EventsState::keyStates[SDL_SCANCODE_S])
		{
			change.x -= forwardWalkVec.x;
			change.z -= forwardWalkVec.y;
		}

		if (EventsState::keyStates[SDL_SCANCODE_A])
		{
			change.x -= sideWalkVec.x;
			change.z -= sideWalkVec.y;
		}
		if (EventsState::keyStates[SDL_SCANCODE_D])
		{
			change.x += sideWalkVec.x;
			change.z += sideWalkVec.y;
		}

		if (EventsState::keyStates[SDL_SCANCODE_SPACE])
		{
			change.y += currentSpeed * time.deltaTime;
		}
		if (EventsState::keyStates[SDL_SCANCODE_LSHIFT])
		{
			change.y -= currentSpeed * time.deltaTime;
		}

		gameObject->transform.ChangePosition(change);

		if (Input::WasButtonPressed(Input::LEFT_MOUSE_BUTTON))
			World::AttackPath(1.0F, gameObject->transform.GetWorldPosition(), gameObject->transform.GetWorldForward(), 10.0);
	}

	cant_copy(Controller);
};

#include <Windows.h>
// Makes NVidia devices use the NVidia card instead of the integrated graphics card.
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(int argc, char** argv)
{
	// Engine Init
	EventsState::Init(1450, 790);
	Window window("GanjaCraft Z");

	Internal::Rendering::Init(1450, 790);
	Internal::GameObjects2::Init(1024);
	//Internal::PBR::Init();

	glDisable(GL_MULTISAMPLE);

	Internal::Storage::GameObjects2::texture.SetTexInfoRegion("white", "white", FVector4(0.5F, 0.5F, 0.0F, 0.0F));

	std::vector<ObjectRenderingProcess3*> chunkProcess;
	chunkProcess.push_back(new ChunkRenderingProcess());
	Internal::Rendering::RegisterProcess("Chunks", chunkProcess);
	std::vector<ObjectRenderingProcess3*> entityProcess;
	entityProcess.push_back(new EntityRenderingProcess());
	Internal::Rendering::RegisterProcess("Entities", entityProcess);

	GameObject3* sun = Instantiate<GameObject3>(Transform3());
	float intensity = 1.0F;
	sun->AddComponent<DirectionalLightComponent3>(FColor(intensity, intensity, intensity));
	// LookDir FVector3(1.0F, -1.0F, 1.0F)
	sun->Enable();

	Cubemap skyboxCubemap(Cubemap::Descriptor(Internal::Storage::Rendering::ENVIRONMENT_MAP_RESOLUTION, Internal::Storage::Rendering::ENVIRONMENT_MAP_RESOLUTION, GL_RGB16F, true, false));

	float sunAngle = PI;
	sun->transform.SetRotation(FQuaternion(FVector3(sunAngle, 0.0F, 0.0F)));

	std::vector<ObjectRenderingProcess3*> skyboxProcess;
	SkyboxRenderingProcess3* skyProcess;
	skyboxProcess.push_back(skyProcess = new SkyboxRenderingProcess3(skyboxCubemap));
	Internal::Rendering::RegisterProcess("Skybox", skyboxProcess);

	Block::FlushAll(true);
	register_block(Block("ganja", 1.0F));
	register_block(Block("cobblestone", 4.0F));

	// Green Zombie
	{
		GameObject3* zombie = Instantiate<GameObject3>(Transform3());
		zombie->AddComponent<Entity>("green_zombie", 10.0F);
		// TODO: Auto-generate AABB (AddBodyPiece should record this along with tex prep data).
		zombie->AddComponent<Attackable>(sqrt((6 + 12) * (6 + 12) + 2 * 2 + (4 + 4) * (4 + 4)) / 16.0, 0.0F, true);
		GameObject3* body = EntityConstruction::AddBodyPiece(zombie, IVector3(8, 12, 4), IVector3(-4, -6, -2), DVector3(0.0, 0.0, 0.0));
		GameObject3* head = EntityConstruction::AddBodyPiece(body, IVector3(8, 8, 8), IVector3(0, 12, -2), DVector3(6.0, 0.0, 6.0));
		GameObject3* leftArm = EntityConstruction::AddBodyPiece(body, IVector3(4, 12, 4), IVector3(-4, 0, 0), DVector3(4.0, 11.0, 2.0));
		{
			// Remember, this is not regular double-extended scale!
			GameObject3* joint = Instantiate<GameObject3>(Transform3(DVector3(-1.0, -1.0, -2.0) / 16.0, FVector3(2.0F, 2.0F, 4.0F) / 16.0F));
			joint->AddComponent<Disjointable>(sqrt(2 * 2 + 2 * 2 + 4 * 4) / 16.0, 1.0F);
			joint->SetParent(leftArm);
		}
		//(*leftArm->transform.GetChildren().begin())->GetOwner()->AddComponent<Attackable>(sqrt(4 * 4 + 12 * 12 + 4 * 4) / 16.0, 0.0F, false);
		GameObject3* leftLeg = EntityConstruction::AddBodyPiece(body, IVector3(4, 12, 4), IVector3(0, -12, 0), DVector3(2.0, 12.0, 2.0));

		// TODO: Make mirror function (mirror top then -1 the rest)
		GameObject3* rightArm = EntityConstruction::AddBodyPiece(body, IVector3(4, 12, 4), DVector3(8.0, 0.0, 0.0), DVector3(0.0, 11.0, 2.0));
		GameObject3* rightLeg = EntityConstruction::AddBodyPiece(body, IVector3(4, 12, 4), DVector3(4.0, -12.0, 0.0), DVector3(2.0, 12.0, 2.0));

		// TODO: Based on performance, may only want to rebuild tex coords in debug mode.

		register_entity(zombie);
	}

	GameObject3* testZombie = Instantiate<GameObject3>(Entity::entityMap["green_zombie"], Transform3(DVector3(0.0, 3.0, 0.0)));
	testZombie->Enable();

	GameObject3* player = Instantiate<GameObject3>(Transform3(DVector3(8.0F, 16.0F, 8.0F)));
	Camera3* camera = player->AddComponent<Camera3>(90.0, static_cast<double>(EventsState::GetAspectRatio()), 0.01, 1000.0);
	player->AddComponent<Controller>(2.0, 0.9);
	player->Enable();

	// Debugging crosshair
	GameObject2* crosshair = Instantiate<GameObject2>(Transform2(FVector2(0.0F), FVector2(0.01F, 0.01F)).WithYConstraint(scale_constraint(AspectRatioScaleConstraint(1.0F))));
	crosshair->AddComponent<RenderComponent2>("white", FColor(1.0F, 1.0F, 1.0F, 0.5F));
	crosshair->Enable();

	GameObject3* testChunkObj = Instantiate<GameObject3>(Transform3());
	Chunk* testChunk = testChunkObj->AddComponent<Chunk>();
	for (int z = 0; z < Chunk::CHUNK_SIZE; ++z)
		for (int x = 0; x < Chunk::CHUNK_SIZE; ++x)
		{
			int surfaceHeight = 3;
			for (int y = 0; y <= surfaceHeight; ++y)
			{
				if (y == surfaceHeight)
					testChunk->SetBlock(x, y, z, Block::Get("ganja"));
				else
					testChunk->SetBlock(x, y, z, Block::Get("cobblestone"));
			}
		} 
	testChunkObj->Enable();

	ChunkMeshFactory chunkMeshFactory;
	chunkMeshFactory.GenerateMesh(testChunk, nullptr, nullptr, nullptr, nullptr);

	GameTime time;
	Internal::Update(time);
	Scene::Update<Transform3>();

	while (EventsState::isProgramActive)
	{
		testZombie->transform.ChangePosition(DVector3(0.0, 0.0, time.deltaTime * 0.5));

		EventsState::Update();

		if (Input::WasKeyPressed(SDL_SCANCODE_P))
		{
			bool setToTrueAfter = false;
			if (SDL_GetRelativeMouseMode() == SDL_TRUE)
			{
				SDL_SetRelativeMouseMode(SDL_FALSE);
				setToTrueAfter = true;
			}

			while (true)
			{
				EventsState::Update();
				if (Input::WasKeyPressed(SDL_SCANCODE_P))
					break;
			}

			if (setToTrueAfter)
				SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		time.Update();
		float spinSpeed = 0.125F;
		if (Input::IsKeyDown(SDL_SCANCODE_LSHIFT))
			spinSpeed *= 4.0F;
		if (Input::IsKeyDown(SDL_SCANCODE_UP))
		{
			sunAngle += PI * time.deltaTime * spinSpeed;

			sun->transform.SetRotation(FQuaternion(FVector3(sunAngle, 0.0F, 0.0F)));
			//RefreshScatteredSkybox();
		}
		else if (Input::IsKeyDown(SDL_SCANCODE_DOWN))
		{
			sunAngle -= PI * time.deltaTime * spinSpeed;

			sun->transform.SetRotation(FQuaternion(FVector3(sunAngle, 0.0F, 0.0F)));
			//RefreshScatteredSkybox();
		}

		Internal::GameObjects2::Update(time);

		if (Input::WasKeyPressed(SDL_SCANCODE_O))
		{
			if (SDL_GetRelativeMouseMode() == SDL_TRUE)
				SDL_SetRelativeMouseMode(SDL_FALSE);
			else
				SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		Internal::Update(time);
		Scene::Update<Transform2>();
		Scene::Update<Transform3>();

		Scene::Render(camera, skyboxCubemap);

		Internal::GameObjects2::Render();

		window.Update();
	}

	Block::FlushAll(false);

	// Engine Destructor
	Scene::Clear<Transform3>();
	Scene::Clear<Transform2>();
	Internal::Rendering::Clear();
	Internal::GameObjects2::Clear();
	EventsState::Clear();

	return 0;
}
