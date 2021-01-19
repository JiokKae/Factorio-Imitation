#pragma once
#include "Scene.h"

class Image;
class Button;
class TitleScene : public Scene
{
private:
	Image* img;
	Image* mainMenuImage;

	Button* button1;
	Button* gameStartButton;
	Button* TenCubeButton;
	Button* quitButton;
	Button* lightingButton;
public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;

	TitleScene(int width, int height)
		: Scene(width, height)
	{};
	virtual ~TitleScene() {};
};

