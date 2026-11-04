#include "Store.h"
#include <string>
#include <unordered_map>
#include <optional>

std::size_t Store::size() const
{
	return entries_.size();
}

void Store::set(const std::string& key, const std::string& value)
{
	entries_[key] = value;
}

//lets you return a no value through optional, not a nullptr or null (special marker)
std::optional<std::string> Store::get(const std::string& key) const
{
	auto it = entries_.find(key);
	if (it != entries_.end())
	{
		return it->second;
	}

	return std::nullopt;
}
bool Store::erase(const std::string& key)
{
	auto it = entries_.find(key);
	if (it != entries_.end())
	{
		entries_.erase(it);
		return true;
	}

	return false;
}