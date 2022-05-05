#pragma once
#include "../GLFramework/UI/UI.h"

struct ItemInfo;
class Structure;
class HandUI : public UI
{
	ItemInfo* handItem;

	GLImage* allItemImage;

	GLImage* background;
	GLImage* backgroundName;

	GLImage* dismantleBar;
	GLImage* dismantleEmptyBar;
	float dismantlePercent;
	Structure* dismantleStructure;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(Shader* shader) override;

	ItemInfo* GetHandItem() { return handItem; }
	bool DismantleStructure(Structure* structure);
	bool IsEmpty();
};

