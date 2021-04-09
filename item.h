#pragma once

#include "Tundra/engine.h"

typedef unsigned __int8 quantity_t;

class Item;

class ItemStack
{
public:
	Item* item;
	quantity_t quantity;
};

class Item
{
public:
	enum struct EquipmentSlot
	{
		NONE, // A regular slot, all other types will also fit here
		HAND, // HOLDING, QUICK1, and QUICK2, all types can fit in the hotbar but only HAND items can be selected (the last slot is always empty)
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

	std::string idName;
	bool isStackable; // If not stackable, ItemStacks will have InstanceData
	
	Item(const std::string& idName);

	virtual std::string GetName();

	virtual void OnSelect();
	virtual void OnDeselect();
	virtual void OnPrimaryBindingPress();
};
