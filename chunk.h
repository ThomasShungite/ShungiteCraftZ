#pragma once

#include <engine.h>

#include <vector>

class Chunk : public Component3
{
	friend class ChunkRenderingProcess;
	friend class ChunkMeshFactory;
private:
	Mesh mesh;

	struct ReloadData
	{
		std::vector<FVector3> positions;
		std::vector<FVector3> normals;
		std::vector<FVector2> texCoords;
		std::vector<unsigned __int32> indices;

		ReloadData();
	};

	bool isDirty;
	unsigned __int16* blocks;
	std::shared_ptr<ReloadData> nextReload; // Data to reload between renders
public:
	static const int CHUNK_SIZE = 16;
	static const int CHUNK_HEIGHT = 16; // To make any larger than 16, Mesh would have to support 32 bit indices (technically, should basically work, just for robustness of dense scenarios)

	bool isMeshReady; // If the mesh is ready after chunk creation;

	Chunk();
	~Chunk();

	void OnCreate() override;

	inline void SetBlock(int x, int y, int z, unsigned __int16 block)
	{
		blocks[x + z * CHUNK_SIZE + y * (CHUNK_SIZE * CHUNK_SIZE)] = block;
		isDirty = true;
	}

	unsigned __int16 GetBlock(int x, int y, int z)
	{
		return blocks[x + z * CHUNK_SIZE + y * (CHUNK_SIZE * CHUNK_SIZE)];
	}

	unsigned __int16 GetBlockOrMonoDirectionalTouchingNeighborSlabBlock(int x, int y, int z, Chunk* posX, Chunk* negX, Chunk* posZ, Chunk* negZ)
	{
		Chunk* chunk = this;

		if (x == CHUNK_SIZE)
		{
			chunk = posX;
			x = 0;
		}
		else if (x == -1)
		{
			chunk = negX;
			x = CHUNK_SIZE - 1;
		}
		else if (z == CHUNK_SIZE)
		{
			chunk = posZ;
			z = 0;
		}
		else if (z == -1)
		{
			chunk = negZ;
			z = CHUNK_SIZE - 1;
		}
		else if (y == CHUNK_HEIGHT || y == -1)
			chunk = nullptr;

		if (chunk != nullptr)
			return chunk->GetBlock(x, y, z);
		else
			return 0;
	}

	cant_copy(Chunk);
};
