#pragma once
#include "../GLFramework/UI/UI.h"
#include <memory>

struct ItemInfo;
class Structure;
class HandUI : public UI
{
public:
	HandUI();
	~HandUI();

	virtual void Update() override;
	virtual void Render(ShaderProgram* shader) override;

	ItemInfo* GetHandItem();

private:
	void PipetteTool(Structure* structure);
	void ClearCursor();
	void DropItem();
	void DismantleStructure(Structure* structure);

	void RenderStructureInfoUI(ShaderProgram* shader, Structure* structure);
	void RenderDismantleProgressBar(ShaderProgram* shader);

	ItemInfo* handItem;

	std::unique_ptr<GLImage> allItemsImage;

	std::unique_ptr<GLImage> background;
	std::unique_ptr<GLImage> backgroundName;

	std::unique_ptr<GLImage> dismantleBar;
	std::unique_ptr<GLImage> dismantleEmptyBar;

	float dismantleTime;
	Structure* dismantleStructure;

	static constexpr float DISMANTLE_POSITION_Y{ 150.0f };
};
