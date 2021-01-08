#pragma once
#include "Structure.h"
class TransportBelt : public Structure
{
public:
	enum STATUS {
		DESTORY,
		STATUS_END
	};

	enum ImageIndex {
		TOP,
		BOTTOM,
		UP,
		LEFT_UP,
		RIGHT_UP,
		END
	};
	
private:
	GLImage* image;
	TransportBelt** aroundBelts;
	int imageFrameYByDirection[DIRECTION_END][ImageIndex::END];
	glm::ivec2 imageTopPosOffset[DIRECTION_END];
	ImageIndex renderState;

public:
	virtual HRESULT Init(int x, int y, DIRECTION direction, bool temp = false) override;
	virtual void Release();
	virtual void Update();
	virtual void FirstRender(Shader* lpShader) override;
	virtual void Render(Shader* lpShader) override;
	virtual void LateRender(Shader* lpShader) override;

	virtual void SetPosition(Vec2 position) override;
	void SetAroundBelts(DIRECTION direction, TransportBelt* aroundBelt);
	ImageIndex GetRenderState() { return renderState; }

};

