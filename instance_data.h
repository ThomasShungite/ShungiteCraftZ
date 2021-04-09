#pragma once

#include <Tundra Engine/engine.h>

class InstanceData
{
private:
	/*struct Data
	{
		char* data;
		size_t size;

		// TODO: Override move operators!
		Data();
		Data(size_t size);
		~Data();
	};

	std::unordered_map<std::string, Data> data;*/
public:
	InstanceData() {}

	template<typename T>
	void Add(const std::string& name, T initial, size_t count = 1)
	{

	}

	template<typename T>
	void Set(const std::string& name, T vale, size_t index = 0)
	{

	}

	template<typename T>
	T Get(const std::string& name, size_t index = 0)
	{

	}

	template<typename T>
	void Remove(const std::string& name)
	{

	}
	//void AddI8(const std::string& name, __int8  size_t count = 1);
	//void SetI8(const std::string& name, size_t index = 0);
	//void RemoveI8(const std::string& name);
};
