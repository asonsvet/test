#pragma once
#include <exception>

template<class Key, class Value>
class dictionary
{
public:
    virtual ~dictionary() = default;

    virtual const Value& get(const Key& key) const = 0; // возвращает значение в зависимости от ключа
    virtual void set(const Key& key, const Value& value) = 0; // добавляет пару ключ-значение
    virtual bool is_set(const Key& key) const = 0; // проверяет добавлен ли ключ key. возвращает true/false
};
    
template<class Key>
class not_found_exception : public std::exception
{
public:
    virtual const Key& get_key() const noexcept = 0;
};

