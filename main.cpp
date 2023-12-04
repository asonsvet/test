
//#include "ContainerD.h"

// Реализация с использованием std::vector и std::pair
// Данные хранятся в отсортированном виде без дубликатов

#include "dictionary.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

// Вспомогательные функции сравнения искомого ключа с первым элементом пары
template<class Key, class Value>
struct compare {
	bool operator()(const std::pair<Key, Value> &pair, const Key &key)
	{
		return (pair.first < key);
	}
	bool operator()(const Key &key, const std::pair<Key, Value> &pair)
	{
		return (key < pair.first);
	}
};

template<class Key, class Value>
class ContainerD : public dictionary<Key, Value>
{
private:
	std::vector<std::pair<Key, Value>> vec;
public:
	const Value& get(const Key& key) const
	{
		std::pair<Key, Value> temp = { key, std::numeric_limits<Value>::min() };

		auto iter = std::lower_bound(vec.begin(), vec.end(), temp);

		if ((iter == vec.end()) || (vec[iter - vec.begin()].first != key))
			throw ExceptionKeyNotFound<Key>(key);

		return vec[iter - vec.begin()].second;
	}

	void set(const Key& key, const Value& value)
	{
		// Если ключ уже есть в контейнере, обновляется его значение
		if (std::binary_search(vec.begin(), vec.end(), key, compare<Key, Value>()))
		{
			std::pair<Key, Value> temp = { key, std::numeric_limits<Value>::min() };
			auto iter = std::lower_bound(vec.begin(), vec.end(), temp);
			vec[iter - vec.begin()].second = value;
		}

		vec.push_back(std::pair<Key, Value>(key, value));
		std::sort(vec.begin(), vec.end());
	}

	bool is_set(const Key& key) const
	{
		return std::binary_search(vec.begin(), vec.end(), key, compare<Key, Value>());
	}

	ContainerD() {}
	~ContainerD() {}
};

template<class Key>
class ExceptionKeyNotFound : public not_found_exception<Key>
{
private:
	Key k;
public:
	const Key& get_key() const noexcept
	{
		return k;
	}

	ExceptionKeyNotFound(const Key key) : k(key) {}
};

int main()
{
	ContainerD<int, char> test;
	test.set(3, 'c');
	test.set(2, 'b');
	test.set(4, 'd');
	test.set(1, 'a');

	//test.set(4, 'k');

	if (test.is_set(3))
		std::cout << "The key is set.\n";
	else 
		std::cout << "The key is not set.\n";

	try {
		std::cout << "Value: " << test.get(4) << std::endl;
		std::cout << "Value: " << test.get(0) << std::endl;
		std::cout << "Value: " << test.get(2) << std::endl;
	}
	catch (const ExceptionKeyNotFound<int>& e) {
		std::cout << "\nException thrown. Key " << e.get_key() << " not found.\n";
	}

	return 0;
}
