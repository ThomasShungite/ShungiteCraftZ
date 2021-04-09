#include "item_ui.h"

const quantity_t MAX_STACK_SIZE = 256;

InteractableItemSlot2D::InteractableItemSlot2D(Item::EquipmentSlot slotType) :
	slotType(slotType)
{ }

void InteractableItemSlot2D::Insert(ItemStack& stack)
{
	if (this->stack.item == stack.item)
	{
		quantity_t q = stack.quantity;
		if (this->stack.quantity + q > MAX_STACK_SIZE)
			q = MAX_STACK_SIZE - this->stack.quantity;

		stack.quantity -= q;
		this->stack.quantity += q;
	}
}

bool InteractableItemSlot2D::RetrieveSingle(Item* item)
{
	if (stack.item == item)
	{

		return;
	}
	else
		return false;
}

void InteractableItemSlot2D::OnHover(const GameTime& deltaTime)
{

}

void InteractableItemSlot2D::OnClick(int button)
{

}
