#pragma once
#include "GameNode.h"

class Image;
class Button;
class TitleScene : public GameNode
{
private:
	Image* img;
	Button* button1;
	Button* quitButton;

public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;

	TitleScene() {};
	virtual ~TitleScene() {};
};

