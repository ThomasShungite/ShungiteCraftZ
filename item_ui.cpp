#include "item_ui.h"

const quantity_t MAX_STACK_SIZE = 256;

InteractableItemSlot2D::InteractableItemSlot2D(Item::EquipmentSlot slotType) :
	slotType(slotType)
{ }

void InteractableItemSlot2D::Transfer(ItemStack& stack)
{
	if (this->stack.item == stack.item)
	{
		quantity_t q = stack.quantity;
		if (this->stack.quantity + q > MAX_STACK_SIZE)
			q = MAX_STACK_SIZE - this->stack.quantity;

		stack.Decrement(q);
		this->stack.Increment(q);
	}
}

bool InteractableItemSlot2D::TransferSingle(ItemStack& stack)
{
	if (this->stack.item == stack.item)
		if (this->stack.quantity + 1 <= MAX_STACK_SIZE)
		{
			stack.Decrement(1);
			this->stack.Increment(1);

			return true;
		}

	return false;
}

bool InteractableItemSlot2D::ConsumeSingle(Item* item)
{
	if (stack.item == item)
	{
		stack.Decrement(1);

		return true;
	}
	else
		return false;
}

bool InteractableItemSlot2D::Set(ItemStack&& stack)
{
	if (this->stack.item == nullptr && stack.item->IsCompatible(slotType))
	{
		this->stack = std::move(stack);
		if (gameObject->IsEnabled())
		{
			itemUI = this->stack.item->Create2DDisplay(this->stack, &itemUI);
			itemUI->SetParent(*gameObject);
		}

		return true;
	}

	return false;
}

void InteractableItemSlot2D::OnHover(const GameTime& deltaTime)
{

}

void InteractableItemSlot2D::OnClick(int button)
{

}

void InteractableItemSlot2D::OnEnable()
{
	if (stack.item != nullptr)
	{
		itemUI = this->stack.item->Create2DDisplay(this->stack, &itemUI);
		itemUI->SetParent(*gameObject);
	}
}

void InteractableItemSlot2D::OnDisable()
{
	if (itemUI != nullptr)
		Destroy(itemUI);
}
