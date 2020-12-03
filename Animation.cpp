#include "Animation.h"

HRESULT Animation::Init(int totalWidth, int totalHeight, int frameWidth, int frameHeight)
{
	// 가로 프레임 수
	int frameWidthCount = totalWidth / frameWidth;

	// 세로 프레임 수
	int frameHeightCount = totalHeight / frameHeight;

	frameCount = frameWidthCount * frameHeightCount;

	vecFrameList.clear();
	vecFrameList.resize(frameCount);

	// 각 프레임의 시작 좌표 설정
	for (int i = 0; i < frameHeightCount; i++)
	{
		for (int j = 0; j < frameWidthCount; j++)
		{
			vecFrameList[i * frameWidthCount + j].x = j * frameWidth;
			vecFrameList[i * frameWidthCount + j].y = i * frameHeight;
		}
	}

	return S_OK;
}

void Animation::SetPlayFrame(bool isLoop, bool isReverse)
{
	// 루프 여부 결정
	this->isLoop = isLoop;

	// 리버스 여부 결정

	vecPlayList.clear();
	if (isReverse)
	{
		// 정방향
		for (int i = 0; i < frameCount; i++)
		{
			vecPlayList.push_back(i);
		}

		// 역방향
		for (int i = frameCount - 1; i >= 0; i--)
		{
			vecPlayList.push_back(i);
		}
	}
	else
	{
		for (int i = 0; i < frameCount; i++)
		{
			vecPlayList.push_back(i);
		}
	}

}

void Animation::SetPlayFrame(int startFrame, int endFrame, bool isLoop, bool isReverse)
{
	// 루프 여부 결정
	this->isLoop = isLoop;

	vecPlayList.clear();

	if (startFrame > endFrame)
	{
		if (isReverse)
		{
			// 정방향
			for (int i = startFrame; i < frameCount; i++)
			{
				vecPlayList.push_back(i);
			}
			for (int i = 0; i <= endFrame; i++)
			{
				vecPlayList.push_back(i);
			}

			// 역방향
			for (int i = endFrame; i >= 0; i--)
			{
				vecPlayList.push_back(i);
			}
			for (int i = frameCount - 1; i >= startFrame; i--)
			{
				vecPlayList.push_back(i);                                   
			}              
		}
		else
		{
			for (int i = startFrame; i < frameCount; i++)
			{
				vecPlayList.push_back(i);
			}
			for (int i = 0; i <= endFrame; i++)
			{
				vecPlayList.push_back(i);
			}
		}
	}
	else 
	{
		if (isReverse)
		{
			// 정방향
			for (int i = startFrame; i <= endFrame; i++)
			{
				vecPlayList.push_back(i);
			}

			// 역방향
			for (int i = endFrame; i >= startFrame; i--)
			{
				vecPlayList.push_back(i);
			}
		}
		else
		{
			for (int i = startFrame; i <= endFrame; i++)
			{
				vecPlayList.push_back(i);
			}
		}
	}
}

void Animation::SetPlayFrame(int * arr, int arrLength, bool isLoop, bool isReverse)
{
	// 루프 여부 결정
	this->isLoop = isLoop;

	// 리버스 여부 결정

	vecPlayList.clear();
	if (isReverse)
	{
		// 정방향
		for (int i = 0; i < arrLength; i++)
		{
			vecPlayList.push_back(arr[i]);
		}

		// 역방향
		for (int i = arrLength - 1; i >= 0; i--)
		{
			vecPlayList.push_back(arr[i]);
		}
	}
	else
	{
		for (int i = 0; i < arrLength; i++)
		{
			vecPlayList.push_back(arr[i]);
		}
	}
}

void Animation::UpdateFrmae()
{
	if (isPlaying)
	{
		accumulateTime += TimerManager::GetSingleton()->GetTimeElapsed();

		if (accumulateTime >= keyFrameUpdateTime)
		{
			nowPlayIdx++;
			                     
			if (nowPlayIdx >= vecPlayList.size())
			{
				// 루프일 때
				if (isLoop)
				{
					nowPlayIdx = 0;
				}
				// 아닐 때
				else
				{
					nowPlayIdx--;
					isPlaying = false;
				}
			}

			accumulateTime -= keyFrameUpdateTime;
		}
	}
}

void Animation::Start()
{
	nowPlayIdx = 0;
	isPlaying = true;
}

void Animation::Stop()
{
	nowPlayIdx = 0;
	isPlaying = false;
}

void Animation::Pause()
{
	isPlaying = false;
}

void Animation::Resume()
{
	isPlaying = true;
}

Animation::Animation() :
	frameCount(0),
	frameHeight(0),
	frameWitdh(0),
	isLoop(false),
	nowPlayIdx(0),
	isPlaying(false),
	keyFrameUpdateTime(1.0f),
	accumulateTime(0.0f)
{
}

Animation::~Animation()
{
}
