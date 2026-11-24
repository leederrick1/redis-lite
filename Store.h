// tells compiler to process header onlyonce per .cpp file being compiled even if its multiple paths
#pragma once
#include <unordered_map>
#include <optional>
//provides basic types like size_t
#include <cstddef>
#include <string>
#include <variant>
#include <cstdint>

using Value = std::variant<std::string, std::int64_t>;

class Store
{
	public:
		// updated way to create default object
		Store() = default;

		//trailing const will not let the method make changes to the members of the object
		std::optional<Value> get(const std::string& key) const;
		void set(const std::string& key, const Value& value);
		std::size_t size() const;
		bool erase(const std::string& key);
	private:
		//private member variables should include _ at end of name
		std::unordered_map<std::string, Value> entries_;

};