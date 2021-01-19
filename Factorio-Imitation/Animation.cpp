#include "Animation.h"

HRESULT Animation::Init(int totalWidth, int totalHeight, int frameWidth, int frameHeight)
{
	// ���� ������ ��
	int frameWidthCount = totalWidth / frameWidth;

	// ���� ������ ��
	int frameHeightCount = totalHeight / frameHeight;

	frameCount = frameWidthCount * frameHeightCount;

	vecFrameList.clear();
	vecFrameList.resize(frameCount);

	// �� �������� ���� ��ǥ ����
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
	// ���� ���� ����
	this->isLoop = isLoop;

	// ������ ���� ����

	vecPlayList.clear();
	if (isReverse)
	{
		// ������
		for (int i = 0; i < frameCount; i++)
		{
			vecPlayList.push_back(i);
		}

		// ������
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
	// ���� ���� ����
	this->isLoop = isLoop;

	vecPlayList.clear();

	if (startFrame > endFrame)
	{
		if (isReverse)
		{
			// ������
			for (int i = startFrame; i < frameCount; i++)
			{
				vecPlayList.push_back(i);
			}
			for (int i = 0; i <= endFrame; i++)
			{
				vecPlayList.push_back(i);
			}

			// ������
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
			// ������
			for (int i = startFrame; i <= endFrame; i++)
			{
				vecPlayList.push_back(i);
			}

			// ������
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
	// ���� ���� ����
	this->isLoop = isLoop;

	// ������ ���� ����

	vecPlayList.clear();
	if (isReverse)
	{
		// ������
		for (int i = 0; i < arrLength; i++)
		{
			vecPlayList.push_back(arr[i]);
		}

		// ������
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
				// ������ ��
				if (isLoop)
				{
					nowPlayIdx = 0;
				}
				// �ƴ� ��
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
