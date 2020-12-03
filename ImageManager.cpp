#include "ImageManager.h"
#include "Image.h"

HRESULT ImageManager::Init()
{
	return S_OK;
}

void ImageManager::Release()
{
	ReleaseSingleton();
}

Image* ImageManager::AddImage(string strKey, const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	Image* image = nullptr;
	image = FindImage(strKey);
	if (image)
	{
		return image;
	}
	image = new Image();
	if (FAILED(image->Init(fileName, width, height, isTrans, transColor)))
	{
		image->Release();
		delete image;

		return nullptr;
	}

	mapImageDatas.insert(make_pair(strKey, image));

	return image;
}

Image* ImageManager::AddImage(string strKey, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	Image* image = nullptr;
	image = FindImage(strKey);
	if (image)
	{
		return image;
	}
	image = new Image();
	if (FAILED(image->Init(fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor)))
	{
		image->Release();
		delete image;

		return nullptr;
	}

	mapImageDatas.insert(make_pair(strKey, image));

	return image;
}

void ImageManager::DeleteImage(string strKey)
{
	map<string, Image*>::iterator it = mapImageDatas.find(strKey);
	if (it != mapImageDatas.end())
	{
		it->second->Release();
		delete it->second;

		mapImageDatas.erase(it);
	}
}

Image* ImageManager::FindImage(string strKey)
{
	map<string, Image*>::iterator it = mapImageDatas.find(strKey);
	if (it != mapImageDatas.end())
	{
		return it->second;
	}
	
	return nullptr;
}
