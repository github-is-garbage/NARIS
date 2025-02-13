#pragma once

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

class BaseFeature
{
public:
	virtual void Setup() = 0;
	virtual void Destroy() = 0;
};

class Features
{
private:
	std::unordered_map<std::type_index, BaseFeature*> mapFeatures;

public:
	Features();

	void Setup();
	void Destroy();

	template <typename T>
	void AddFeature(T* pFeature)
	{
		this->mapFeatures.insert({ std::type_index(typeid(T)), pFeature });
	}

	template <typename T>
	T* FindFeature()
	{
		auto Found = this->mapFeatures.find(std::type_index(typeid(T)));

		if (Found == this->mapFeatures.end())
			return nullptr;

		return dynamic_cast<T*>(Found->second);
	}
};
