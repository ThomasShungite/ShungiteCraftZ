#pragma once

#include <engine.h>

#include "chunk.h"

class ChunkRenderingProcess : public RenderingOpaqueProcess3
{
public:
	ChunkRenderingProcess();

	void AddObject(const RenderComponent3& object) override;
	void RemoveObject(const RenderComponent3& object) override;

	void Run(const DMatrix4x4& projectionView, const Camera3* camera) override;
private:
	std::vector<Chunk*> chunks;

	const Shader shader;
};

class EntityRenderingProcess : public RenderingOpaqueProcess3
{
public:
	EntityRenderingProcess();

	void AddObject(const RenderComponent3& object) override;
	void RemoveObject(const RenderComponent3& object) override;

	void Run(const DMatrix4x4& projectionView, const Camera3* camera) override;
private:
	std::unordered_map<Mesh, std::vector<Transform3*>> objects;

	const Shader shader;
};

