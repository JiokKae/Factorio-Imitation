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
	string buffer;
	auto it = Values.begin();

	while (it != Values.end())
	{
		buffer += it->first + ":\t210\t" +it->second.ToString() + "\n";
		++it;
	}
	TextRenderer::GetSingleton()->RenderText(buffer, x, y, 0.7f);
}

void DebugHelper::SetFloat(string key, float value)
{
	auto it = Values.find(key);

	if (it == Values.end())
		Values.insert(make_pair(key, Property(value)));
	else
		it->second.data._float = value;
}

void DebugHelper::SetInt(string key, int value)
{
	auto it = Values.find(key);

	if (it == Values.end())
		Values.insert(make_pair(key, Property(value)));
	else
		it->second.data._int = value;
}

float DebugHelper::GetFloat(string key)
{
	auto it = Values.find(key);

	if (it == Values.end())
		return it->second.data._float;
	else
		return 0.0f;
}

int DebugHelper::GetInt(string key)
{
	auto it = Values.find(key);

	if (it == Values.end())
		return it->second.data._int;
	else
		return 0;
}

DebugHelper::Property::Property(int data)
{
	this->type = Type::INT;
	this->data._int = data;
}

DebugHelper::Property::Property(float data)
{
	this->type = Type::FLOAT;
	this->data._float = data;
}

DebugHelper::Property::Property(double data)
{
	this->type = Type::DOUBLE;
	this->data._double = data;
}

string DebugHelper::Property::ToString()
{
	switch (type)
	{
	case DebugHelper::Property::INT:
		return to_string(data._int);
	case DebugHelper::Property::FLOAT:
		return to_string(data._float);
	case DebugHelper::Property::DOUBLE:
		return to_string(data._double);
	}

	return string();
}
