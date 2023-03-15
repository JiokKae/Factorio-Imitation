#include "Scene.h"

bool Scene::IsUseBackBuffer()
{
	return useBackBuffer;
}

void Scene::SetUseBackBuffer(bool b)
{
	useBackBuffer = b;
}

void Scene::SetWidthHeight(int _width, int _height)
{
	width = _width;
	height = _height;
}

int Scene::GetWidth()
{
	return width;
}

int Scene::GetHeight()
{
	return height;
}