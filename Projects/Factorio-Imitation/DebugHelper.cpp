#include "DebugHelper.h"

HRESULT DebugHelper::Init()
{
	return S_OK;
}

void DebugHelper::Release()
{
	ReleaseSingleton();
}

void DebugHelper::Update()
{
}

void DebugHelper::Render(float x, float y)
{
	if (g_debuggingMode == false)
		return;

	string buffer;

	for (auto it = Values.cbegin(); it != Values.cend(); ++it)
		buffer += std::format("{}:\t210\t{}\n", it->first, std::string{ it->second });

	TextRenderer::GetSingleton()->RenderText(buffer, x, y, 0.7f);
}

void DebugHelper::SetFloat(const std::string& key, float value)
{
	auto it = Values.find(key);

	if (it == Values.end())
		Values.emplace(key, Property(value));
	else
		it->second.data._float = value;
}

void DebugHelper::SetInt(const std::string& key, int value)
{
	auto it = Values.find(key);

	if (it == Values.end())
		Values.emplace(key, Property(value));
	else
		it->second.data._int = value;
}

float DebugHelper::GetFloat(const std::string& key) const
{
	auto it = Values.find(key);

	return (it == Values.end()) ? 0.0f : it->second.data._float;
}

int DebugHelper::GetInt(const std::string& key) const
{
	auto it = Values.find(key);

	return (it == Values.end()) ? 0 : it->second.data._int;
}

DebugHelper::Property::Property(int data)
	: type{ Type::INT }
	, data{ ._int = data }
{
}

DebugHelper::Property::Property(float data)
	: type{ Type::FLOAT }
	, data{ ._float = data }
{
}

DebugHelper::Property::Property(double data)
	: type{ Type::DOUBLE }
	, data{ ._double = data }
{
}

DebugHelper::Property::operator std::string() const
{
	switch (type)
	{
	case DebugHelper::Property::INT:
		return std::to_string(data._int);
	case DebugHelper::Property::FLOAT:
		return std::to_string(data._float);
	case DebugHelper::Property::DOUBLE:
		return std::to_string(data._double);
	}
	return std::string{};
}
