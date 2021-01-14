#pragma once
#include "Structure.h"
#include <deque>
class TransportBelt : public Structure
{
private:
	enum ImageIndex {
		TOP,
		BOTTOM,
		UP,
		LEFT_UP,
		RIGHT_UP,
		END
	};
	
private:
	// render vars
	static int imageFrameYByDirection[DIRECTION_END][ImageIndex::END];
	static glm::ivec2 imageTopPosOffset[DIRECTION_END];
	GLImage* image;
	TransportBelt** aroundBelts;
	ImageIndex renderState;

	// function vars
	deque<ItemOnGround*> leftline;
	deque<ItemOnGround*> rightline;


public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;
	virtual void LateRender(Shader* lpShader) override;
	virtual void Render(Shader* shader, float posX, float posY) override;
	void FlowItem(ItemOnGround* item);
	virtual bool InputItem(ItemInfo* item, glm::vec2 pos);

	virtual void SetPosition(Vec2 position) override;
	void SetAroundBelts(DIRECTION direction, TransportBelt* aroundBelt);
	ImageIndex GetRenderState() { return renderState; }

};

