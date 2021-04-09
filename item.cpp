#include "item.h"

std::unordered_map<std::string, Item*> Item::itemMap;

ItemStack::ItemStack() :
	item(nullptr),
	instanceData(nullptr),
	quantity(0)
{ }

ItemStack::ItemStack(const ItemStack& other) :
	item(other.item),
	quantity(other.quantity)
{
	// TODO: Copy instanceData!
}

ItemStack::ItemStack(ItemStack&& other) :
	item(other.item),
	instanceData(other.instanceData),
	quantity(other.quantity)
{
	other.item = nullptr;
	other.instanceData = nullptr;
}

ItemStack::ItemStack(Item* item) :
	item(item),
	instanceData(nullptr),
	quantity(1)
{
	// TODO: New instance data if it isn't stackable
}

ItemStack::ItemStack(Item* item, quantity_t quantity) :
	item(item),
	instanceData(nullptr),
	quantity(quantity)
{
	// TODO: New instance data if it isn't stackable
}

ItemStack::~ItemStack()
{
	if (instanceData != nullptr)
		delete instanceData;
}

ItemStack& ItemStack::operator=(const ItemStack& other)
{
	item = other.item;
	// TODO: New instance data! (copy!)
	quantity = other.quantity;

	return *this;
}

ItemStack& ItemStack::operator=(ItemStack&& other)
{
	item = other.item;
	instanceData = other.instanceData;
	quantity = other.quantity;

	other.item = nullptr;
	other.instanceData = nullptr;

	return *this;
}

Item::Item(const std::string& idName, const std::string& texture, bool isStackable, EquipmentSlot compatibleSlot) :
	idName(idName),
	texture(texture),
	isStackable(isStackable),
	compatibleSlot(compatibleSlot)
{ }

bool Item::IsCompatible(EquipmentSlot slotType)
{
	if (compatibleSlot == EquipmentSlot::RESERVED || slotType == EquipmentSlot::RESERVED)
		return false;

	if (slotType == EquipmentSlot::NONE)
		return true;
	else if (slotType == compatibleSlot)
		return true;
	else
		return false;
}

std::string Item::GetName()
{
	return idName;
}

void Item::OnSelect(const ItemStack& stack) { }
void Item::OnHold(const ItemStack& stack) { }
void Item::OnDeselect(const ItemStack& stack) { }
void Item::OnPrimaryBindingPress(const ItemStack& stack) { }
void Item::OnPrimaryBindingHold(const ItemStack& stack) { }
void Item::OnPrimaryBindingRelease(const ItemStack& stack) { }
void Item::OnSecondaryBindingPress(const ItemStack& stack) { }
void Item::OnSecondaryBindingHold(const ItemStack& stack) { }
void Item::OnSecondaryBindingRelease(const ItemStack& stack) { }

Item* Item::Get(const std::string& idName)
{
	return itemMap[idName];
}

void Item::Register(Item* item)
{
	if (itemMap.find(item->idName) != itemMap.end())
		Console::Error("Error: " + item->idName + " is overriding an existing block");

	itemMap[item->idName] = item;
}

void Item::FlushAll()
{
	for (auto pair : itemMap)
		delete pair.second;

	itemMap.clear();
}
