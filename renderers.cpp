#include "renderers.h"

#include "block.h"

ChunkRenderingProcess::ChunkRenderingProcess() :
	shader("Shaders/chunks")
{ } 

void ChunkRenderingProcess::AddObject(const RenderComponent3& object)
{
	chunks.push_back(object.gameObject->GetComponent<Chunk>());
}

void ChunkRenderingProcess::RemoveObject(const RenderComponent3& object)
{
	RemoveVectorComponent(chunks, object.gameObject->GetComponent<Chunk>());
}

void ChunkRenderingProcess::Run(const DMatrix4x4& projectionView, const Camera3* camera)
{
	shader.Bind();
	if (Internal::Storage::RenderingObj::directionalLight != nullptr)
	{
		Shader::LoadVector3(2, Internal::Storage::RenderingObj::directionalLight->gameObject->transform.GetWorldForward());
		Shader::LoadVector3(3, &Internal::Storage::RenderingObj::directionalLight->color.r);
	}
	else
	{
		Shader::LoadVector3(2, FVector3(0.0F, -1.0F, 0.0F));
		Shader::LoadVector3(3, FVector3(0.0F, 0.0F, 0.0F));
	}

	RVector3 camPos = camera->gameObject->transform.GetWorldPosition();

	Block::textureArray.Bind(0);

	for (Chunk* chunk : chunks)
		if (chunk->isMeshReady)
		{
			chunk->mesh.Bind();
			Shader::LoadMatrix4x4(0, static_cast<FMatrix4x4>(projectionView * chunk->gameObject->transform.GetMatrix()));
			Shader::LoadVector3(1, static_cast<FVector3>(camPos - chunk->gameObject->transform.GetWorldPosition()));
			chunk->mesh.DrawElementsLong();
		}

	// TODO: Do further away from time from rendering?
	for (Chunk* chunk : chunks)
		if (chunk->nextReload != nullptr)
		{
			//std::cerr << chunk->nextReload->indices.size() << " " << chunk->nextReload->positions.size() << " " << chunk->nextReload->normals.size() << " " << chunk->nextReload->texCoords.size() << std::endl;
			chunk->mesh.Rebuild(Mesh::Descriptor(static_cast<unsigned __int32>(chunk->nextReload->positions.size())).AddIndices(&chunk->nextReload->indices[0], static_cast<unsigned __int32>(chunk->nextReload->indices.size()), false).AddArray(&chunk->nextReload->positions[0]).AddArray(&chunk->nextReload->normals[0]).AddArray(&chunk->nextReload->texCoords[0]));
			chunk->nextReload = nullptr;
			chunk->isMeshReady = true;
		}
}

EntityRenderingProcess::EntityRenderingProcess() :
	shader("Shaders/entities")
{ }

void EntityRenderingProcess::AddObject(const RenderComponent3& object)
{
	objects[object.GetMesh()].push_back(&object.gameObject->transform);
}

void EntityRenderingProcess::RemoveObject(const RenderComponent3& object)
{
	RemoveVectorComponent(objects[object.GetMesh()], &object.gameObject->transform);
}

void EntityRenderingProcess::Run(const DMatrix4x4& projectionView, const Camera3* camera)
{
	shader.Bind();
	if (Internal::Storage::RenderingObj::directionalLight != nullptr)
	{
		Shader::LoadVector3(2, Internal::Storage::RenderingObj::directionalLight->gameObject->transform.GetWorldForward());
		Shader::LoadVector3(3, &Internal::Storage::RenderingObj::directionalLight->color.r);
	}
	else
	{
		Shader::LoadVector3(2, FVector3(0.0F, -1.0F, 0.0F));
		Shader::LoadVector3(3, FVector3(0.0F, 0.0F, 0.0F));
	}

	//Block::textureArray.Bind(0);

	for (auto& meshPair : objects)
	{
		meshPair.first.Bind();
		for (Transform3* transform : meshPair.second)
		{
			Shader::LoadMatrix4x4(0, static_cast<FMatrix4x4>(projectionView * transform->GetMatrix()));
			Shader::LoadMatrix3x3(1, transform->GetWorldRotationMatrix3x3());
			meshPair.first.DrawElements();
		}
	}
}
