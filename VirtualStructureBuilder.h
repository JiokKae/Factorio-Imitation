#pragma once
#include "GameNode.h"
class VirtualStructureBuilder : public GameNode
{

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	VirtualStructureBuilder() {};
	virtual ~VirtualStructureBuilder() {};
};

