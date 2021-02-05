#pragma once
#include "framework.h"

class DebugHelper : public Singleton<DebugHelper>
{
	class Property
	{
	protected:
		enum Type 
		{
			INT,
			FLOAT,
			DOUBLE,
		};

		union Data
		{
			int _int;
			float _float;
			double _double;
		};

	public:
		Data data;
		Type type;

	public:
		Property(int data);
		Property(float data);
		Property(double data);

		string ToString();
	};

	map<string, Property> Values;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(float x, float y);

	void	SetFloat(string key, float value);
	void	SetInt(string key, int value);

	float	GetFloat(string key);
	int	GetInt(string key);
};


