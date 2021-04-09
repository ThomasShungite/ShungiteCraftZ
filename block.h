#pragma once

#include "Tundra/engine.h"

#include <string>
#include <unordered_map>
// TODO: Map names to IDs in world file also

class Block
{
	friend class ChunkRenderingProcess;
public:
	static std::vector<Block*> blocks;
private:
	static std::unordered_map<std::string, unsigned __int16> blockMap;
	static Texture2DQuadAtlas textureArray;
public:
	std::string idName;
	float hardness; // Seconds it takes to mine using the base pickaxe
	FVector4 textureLocationInGrid;

	Block(const std::string& idName, float hardness);
	virtual ~Block();

	static unsigned __int16 Get(const std::string& idName);
	static void Register(Block* block);
	static void FlushAll(bool init); // Is also used for init
};

#define register_block(blockInfo) Block::Register(new blockInfo)
