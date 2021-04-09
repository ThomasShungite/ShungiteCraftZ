#include "block.h"

std::vector<Block*> Block::blocks;
std::unordered_map<std::string, unsigned __int16> Block::blockMap;
Texture2DQuadAtlas Block::textureArray;

Block::Block(const std::string& idName, float hardness) :
	idName(idName),
	hardness(hardness),
	textureLocationInGrid(0.0F)
{ }

Block::~Block() { }

unsigned __int16 Block::Get(const std::string& idName)
{
	return blockMap[idName];
}

void Block::Register(Block* block)
{
	if (blockMap.find(block->idName) != blockMap.end())
		Console::Error("Error: " + block->idName + " is overriding an existing block");

	blocks.push_back(block);
	blockMap[block->idName] = static_cast<unsigned __int16>(blocks.size() - 1);

	block->textureLocationInGrid = textureArray.GetTexInfo("Blocks/" + block->idName);

	// TODO: Grab lang for displayName (for items)
}

void Block::FlushAll(bool init)
{
	for (Block* block : blocks)
		delete block;

	blocks.clear();
	blocks.push_back(nullptr);
	blockMap.clear();
	blockMap["air"] = 0;

	if (init)
		textureArray = Texture2DQuadAtlas(GL_SRGB8, 256, false); // gives 256 16x16 textures
}
