#pragma once

#include <Tundra Engine/ui.h>

#include "item.h"

class InteractableItemSlot2D : public UIComponent
{
private:
	ItemStack stack;
	Item::EquipmentSlot slotType;
	GameObject2* itemUI;
public:
	InteractableItemSlot2D(Item::EquipmentSlot slotType);

	// Takes from stack and puts into this
	void Transfer(ItemStack& stack);
	// Returns true if successful
	bool TransferSingle(ItemStack& stack);
	bool ConsumeSingle(Item* item);
	// Only works if the items are compatible and if there isn't already an item here
	bool Set(ItemStack&& stack);

	void OnHover(const GameTime& deltaTime) override;
	void OnClick(int button) override;
	void OnEnable() override;
	void OnDisable() override;

	cant_copy(InteractableItemSlot2D);
};
