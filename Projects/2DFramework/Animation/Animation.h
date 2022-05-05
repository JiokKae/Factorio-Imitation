#pragma once
#include "../framework.h"

class Animation
{
private:
	std::vector<POINT> vecFrameList;		// key Frame 위치 리스트
	std::vector<int> vecPlayList;		// 플레이 리스트

	int frameCount;		// 전체 프레임 수
	int frameWidth;		// 프레임 가로 크기
	int frameHeight;	// 프레임 세로 크기
	
	bool isLoop;		// 반복 되는지 여부
	int nowPlayIdx;		// 현재 플레이 중인 프레임 인덱스
	bool isPlaying;		// 현재 플레이 중인지 여부

	float keyFrameUpdateTime;	// 업데이트 기준 시간
	float accumulateTime;		// 누적 시간

public:
	HRESULT Init(int totalWidth, int totalHeight, int frameWidth, int frameHeight);
	void SetPlayFrame(bool _isLoop = false, bool isReverse = false);
	void SetPlayFrame(int startFrame, int endFrame, bool _isLoop = false, bool isReverse = false);
	void SetPlayFrame(int* arr, int arrLength, bool _isLoop = false, bool isReverse = false);
	void UpdateFrmae();
	
	void Start();
	void Stop();
	void Pause();
	void Resume();

	POINT GetFramePoint() { return vecFrameList[vecPlayList[nowPlayIdx]]; }

	void SetKeyFrameUpdateTime(float time) { keyFrameUpdateTime = time; }

	Animation();
	~Animation();

};

