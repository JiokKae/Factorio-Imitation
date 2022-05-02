#pragma once
#include "../framework.h"
#include "Singleton.h"

// FMOD 관련
//#pragma comment(lib, "lib/fmodex_vc.lib")
#pragma comment(lib, "../../Librarys/fmod/fmodex64_vc.lib")
//#pragma comment(lib, "../../Librarys/fmod/fmodex64_vc.lib")

#include "fmod/inc/fmod.hpp"

using namespace FMOD;

#define SOUNDBUFFER		30

class SoundManager : public Singleton<SoundManager>
{
private:
	System* system;		// FMOD 라이브러리 제어
	Sound** sounds;		// 음원
	Channel** channels;	// 출력 사운드 관리

	std::map<string, Sound**> mapSoundData;

public:
	HRESULT Init();
	void Release();
	void Update();

	void AddSound(string key, string fileName, bool isBGM = false, bool isLoop = false);
	void Play(string key, float volume = 1.0f);		// 0.0f ~ 1.0f
	void Stop(string key);
	void Pause(string key);
	void Resume(string key);

	SoundManager();
	virtual ~SoundManager() {}
};

