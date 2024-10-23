#include <iostream>
#include "Item.h"

void Example001(std::shared_ptr<Item> shared)
{
	std::cout << "Main::Example001()\n";
}

void Example002()
{
	std::cout << "Item 을 SharedPtr 로만 만들어서 했을 때,\n";
	std::shared_ptr<Item> sharedItem = std::make_shared<Item>();
}

void Example003()
{
	std::cout << "Item 을 UniquePtr 로 만들고 SharedPtr 로 넣었을 때,\n";
	std::shared_ptr<Item> sharedItem3 = Item::New();
}

int main()
{
	Example002();

	Example003();
	
	//std::shared_ptr <Item, void(*)(Item*)> sharedItem2;	// error

	//std::unique_ptr <Item, void(*)(Item*)> 
	// uniqueItem(new Item, [](Item* obj){});	// no error
	
	//Example001(sharedItem3);
}