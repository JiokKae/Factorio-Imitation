#pragma once
#include "Structure.h"
class TransportBelt : public Structure
{
public:
	enum STATUS {
		DESTORY,
		END
	};
	
private:
	GLImage* image;
	TransportBelt** aroundBelts;
public:
	virtual HRESULT Init(int x, int y) override;
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

	void SetAroundBelts(DIRECTION direction, TransportBelt* aroundBelt);
};

