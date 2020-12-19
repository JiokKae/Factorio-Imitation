#pragma once
#include "Scene.h"
class LoadingScene1 : public Scene
{
private:
	Image* img;

public:
	virtual HRESULT Init()		 override;
	virtual void Release()		 override;
	virtual void Update()		 override;
	virtual void Render(HDC hdc) override;

	LoadingScene1(int width, int height)
		: Scene(width, height)
		, img(nullptr)
	{};
	virtual ~LoadingScene1() {};
};

