#pragma once

#include <Tundra Engine/ui.h>

#include "item.h"

class InteractableItemSlot2D : public UIComponent
{
private:
	ItemStack stack;
	Item::EquipmentSlot slotType;
public:
	InteractableItemSlot2D(Item::EquipmentSlot slotType);

	void Insert(ItemStack& stack);
	bool RetrieveSingle(Item* item);

	void OnHover(const GameTime& deltaTime) override;
	void OnClick(int button) override;

	cant_copy(InteractableItemSlot2D);
};
