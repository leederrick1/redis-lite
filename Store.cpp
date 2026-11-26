#include "Store.h"
#include <string>
#include <unordered_map>
#include <optional>

std::size_t Store::size() const
{
	std::size_t size = 0;
	for (const auto& [key, entry] : entries_)
	{
		if (!isExpired(entry))
		{
			size += 1;
		}
	}
	return size;
}

void Store::set(const std::string& key, const Value& value)
{
	entries_[key] = Entry{ value, std::nullopt };
}

//lets you return a no value through optional, not a nullptr or null (special marker)
std::optional<Value> Store::get(const std::string& key) const
{
	auto it = entries_.find(key);
	if (it != entries_.end() && !isExpired(it->second))
	{
		return it->second.value;
	}

	return std::nullopt;
}
bool Store::erase(const std::string& key)
{
	auto it = entries_.find(key);
	if (it == entries_.end())
	{
		return false;
	}
	bool wasLive = !isExpired(it->second);
	entries_.erase(it);
	return wasLive;
}

bool Store::isExpired(const Entry& entry) const
{
	if (!entry.expiresAt.has_value())
	{
		return false;
	}

	return std::chrono::steady_clock::now() >= entry.expiresAt.value();
}

bool Store::expire(const std::string& key, std::chrono::seconds ttl)
{
	auto it = entries_.find(key);
	if (it == entries_.end())
	{
		return false;
	}
	if (isExpired(it->second))
	{
		entries_.erase(it);
		return false;
	}
	if (ttl <= std::chrono::seconds{ 0 })
	{
		entries_.erase(it);
		return true;
	}
	it->second.expiresAt = std::chrono::steady_clock::now() + ttl;
	return true;
}