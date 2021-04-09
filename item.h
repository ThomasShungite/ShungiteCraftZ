#pragma once

#include "Tundra Engine/engine.h"
#include "instance_data.h"

typedef unsigned __int32 quantity_t;

class Item;

class ItemStack
{
public:
	Item* item;
	InstanceData* instanceData;
	quantity_t quantity; // Durability if the item is not stackable
	// Have 32 extra bits here

	ItemStack();
	ItemStack(const ItemStack& other);
	ItemStack(ItemStack&& other);
	ItemStack(Item* item);
	ItemStack(Item* item, quantity_t quantity);
	~ItemStack();

	ItemStack& operator=(const ItemStack& other) override;
	ItemStack& operator=(ItemStack&& other) override;
};

class Item
{
private:
	static std::unordered_map<std::string, Item*> itemMap;
public:
	enum struct EquipmentSlot
	{
		NONE, // A regular slot, all other types will also fit here
		HOLDING, // HOLDING, QUICK1, and QUICK2, all types can fit in the hotbar but only HAND items can be selected (the last slot is always empty)
		CHARM, // All CHARMN
		RESERVED, // Primarily the last slot, means it is compatible with nothing

		NECK,   HEAD,   BACK,
		WRIST,  CHEST,  HANDS,
		WAIST,  LEGS,   FEET,

/*		          CHARM4, CHARM5, CHARM6,
		
		CHARM3,   NECK,   HEAD,   BACK,   CHARM7,
		CHARM2,   WRIST,  CHEST,  HANDS,  CHARM8,
		CHARM1,   WAIST,  LEGS,   FEET,   CHARM9,

				  QUICK1  HOLDING QUICK2
*/
// R F for quick
	} compatibleSlot;

	bool isStackable; // If not stackable, ItemStacks will have InstanceData
	std::string idName;
	std::string texture;
	
	Item(const std::string& idName, const std::string& texture, bool isStackable, EquipmentSlot compatibleSlot);

	bool IsCompatible(EquipmentSlot slotType);

	virtual std::string GetName();

	virtual void OnSelect(const ItemStack& stack);
	virtual void OnHold(const ItemStack& stack); // TODO: Use to make a dynamic torch
	virtual void OnDeselect(const ItemStack& stack);
	virtual void OnPrimaryBindingPress(const ItemStack& stack);
	virtual void OnPrimaryBindingHold(const ItemStack& stack);
	virtual void OnPrimaryBindingRelease(const ItemStack& stack);
	virtual void OnSecondaryBindingPress(const ItemStack& stack);
	virtual void OnSecondaryBindingHold(const ItemStack& stack);
	virtual void OnSecondaryBindingRelease(const ItemStack& stack);

	static Item* Get(const std::string& idName);
	static void Register(Item* item);
	static void FlushAll();
};

#define register_item(itemInfo) Item::Register(new itemInfo)
