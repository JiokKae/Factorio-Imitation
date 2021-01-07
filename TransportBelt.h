#pragma once
#include "Structure.h"
class TransportBelt : public Structure
{
public:
	enum STATUS {
		NOPOWER,
		WAITING_SPACE,
		DESTORY,
		END
	};

private:
	GLImage* image;

public:
	virtual HRESULT Init(int x, int y) override;
	virtual void Release();
	virtual void Update();
	virtual void Render(Shader* lpShader);

};

