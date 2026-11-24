#include <iostream>
#include "Store.h"
#include <cassert>
#include <string>
#include <sstream>

void printValue(const Store& store, const std::string& key)
{
	auto result = store.get(key);
	if (!result.has_value())
	{
		std::cout << key << ": NOT FOUND\n";
		return;
	}
	const Value& value = result.value();
	if (const auto* text = std::get_if<std::string>(&value))
	{
		std::cout << key << ": " << *text << '\n';
	}
	else if (const auto* number = std::get_if<std::int64_t>(&value))
	{
		std::cout << key << ": " << *number << '\n';
	}

}
void runExample()
{
	Store store;
	std::cout << "Store created successfully.\n";
	std::cout << "Store size is " << store.size() << std::endl;
	store.set("username", "derrick");
	printValue(store, "username");
	store.set("username", "lee");
	printValue(store, "username");
	std::cout << "Store size is " << store.size() << std::endl;
	printValue(store, "missing");
	store.set("empty", "");
	printValue(store, "empty");

	//bool alpha prints true false instead of 0 and 1
	std::cout << std::boolalpha << store.erase("empty") << std::endl;
	std::cout << std::boolalpha << store.erase("empty") << std::endl;
}
void runTests()
{
	Store store;
	assert(store.size() == 0);
	store.set("username", "derrick");
	auto result = store.get("username");
	assert(result.has_value());
	
	store.set("score", std::int64_t{ 42 });
	auto score = store.get("score");
	assert(score.has_value());

	const auto* number = std::get_if<std::int64_t>(&score.value());
	assert(number != nullptr);
	assert(*number == 42);

	assert(store.size() == 2);
	store.set("score", "forty-two");
	auto scoreString = store.get("score");
	assert(scoreString.has_value());

	const auto* stringValue = std::get_if<std::string>(&scoreString.value());
	assert(stringValue != nullptr);
	assert(*stringValue == "forty-two");
	assert(store.size() == 2);

	const auto* text = std::get_if<std::string>(&result.value());
	assert(text != nullptr);
	assert(*text == "derrick");

	store.set("username", std::string{ "derrick" });
	store.set("score", std::int64_t{ 42 });

	printValue(store, "username");
	printValue(store, "score");
	printValue(store, "missing");
	std::cout << "All tests passed!\n";
}
bool hasExtraInput(std::istringstream& stream)
{
	std::string extra;
	return static_cast<bool>(stream >> extra);
}
int main()
{
	Store store;
	std::string input;
	store.set("username", "derrick");
	std::cout << "Welcome to Map Store! Type HELP for commands.\n";

	//this loop is perma true so ou would need to break out of it, doesnt actually check anything
	while (std::cout << "> " && std::getline(std::cin, input))
	{
		std::istringstream stream(input);
		std::string command;
		stream >> command;

		if (command == "EXIT")
		{
			break;
		}
		else if (command == "SIZE")
		{
			std::cout << store.size() << '\n';
		}
		else if (command == "HELP")
		{
			std::cout << "Commands: SIZE, HELP, GET <key>, SET <key> <value>, DEL <key>, EXIT\n";
		}
		else if (command == "SETINT")
		{
			std::string key;
			std::int64_t value;
			if ((stream >> key >> value) && !hasExtraInput(stream))
			{
				store.set(key, value);
			}
			else
			{
				std::cout << "ERROR: usage SETINT <key> <integer>" << std::endl;
			}
		}
		else if (command == "GET")
		{
			std::string key;
			if (!(stream >> key) || hasExtraInput(stream))
			{
				std::cout << "ERROR: usage GET <key>" << std::endl;
			}
			else
			{
				printValue(store, key);
			}

		}
		else if (command == "SET")
		{
			std::string key, value;
			//std::ws consumes whitespace before the value variable, translates into leaving the
			//rest of the text and getline stores it in value
			if ((stream >> key) && std::getline(stream >> std::ws, value)
				&& !value.empty())
			{
				store.set(key, value);
			}
			else
			{
				std::cout << "ERROR: usage SET <key> <value>" << std::endl;
			}

		}
		else if (command == "DEL")
		{
			std::string key;
			if (stream >> key)
			{
				bool removed = store.erase(key);
				if (removed)
				{
					std::cout << "Deleted" << std::endl;
				}
				else
				{
					std::cout << "NOT FOUND" << std::endl;
				}
			}
			else
			{
				std::cout << "ERROR: usage DEL <key>" << std::endl;
			}
		}
		else
		{
			std::cout << "UNKNOWN COMMAND\n";
		}
	}

	std::cout << "GOODBYE\n";

	return 0;
}