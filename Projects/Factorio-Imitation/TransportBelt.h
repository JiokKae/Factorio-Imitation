#pragma once
#include "Structure.h"
#include <deque>
class TransportBelt : public Structure
{
protected:
	enum ImageIndex {
		TOP,
		BOTTOM,
		UP,
		LEFT_UP,
		RIGHT_UP,
		END
	};
	
protected:
	// render vars
	static int imageFrameYByDirection[DIRECTION_END][ImageIndex::END];
	static glm::ivec2 imageTopPosOffset[DIRECTION_END];
	GLImage* image;
	TransportBelt** aroundBelts;
	ImageIndex renderState;

	float speed;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(ShaderProgram* lpShader) override;
	virtual void Render(ShaderProgram* lpShader) override;
	virtual void LateRender(ShaderProgram* lpShader) override;
	virtual void RenderInScreen(ShaderProgram* shader, float posX, float posY) override;

	// 주변의 벨트들과 연결하는 함수
	void LinkAroundBelts();

	// 주변의 벨트들과 연결을 끊는 함수
	void UnlinkAroundBelts();

	void FlowItem(Entity* item, bool isItem);
	virtual bool InputItem(ItemInfo* item, glm::vec2 pos);

	virtual void SetPosition(Vec2 position) override;
	void SetAroundBelts(DIRECTION direction, TransportBelt* aroundBelt);
	ImageIndex GetRenderState() { return renderState; }
	static bool IsTransportBelt(int itemEnum);
};

