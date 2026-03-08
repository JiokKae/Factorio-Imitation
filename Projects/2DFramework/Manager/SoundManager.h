#pragma once
#include "../framework.h"
#include "Singleton.h"

// FMOD 관련
//#pragma comment(lib, "lib/fmodex_vc.lib")
#pragma comment(lib, "../../Librarys/fmod/fmodex64_vc.lib")

#include "fmod/inc/fmod.hpp"

constexpr int SOUNDBUFFER = 30;

class SoundManager : public Singleton<SoundManager>
{
public:
	HRESULT Init();
	void Release();
	void Update();

	void AddSound(const std::string& key, const std::string & fileName, bool isBGM = false, bool isLoop = false);
	void Play(const std::string& key, float volume = 1.0f);		// 0.0f ~ 1.0f
	void Stop(const std::string& key);
	void Pause(const std::string& key);
	void Resume(const std::string& key);

private:
	FMOD::System* system = nullptr;	// FMOD 라이브러리 제어
	FMOD::Sound** sounds = nullptr;	// 음원
	FMOD::Channel** channels = nullptr;	// 출력 사운드 관리

	std::map<std::string, FMOD::Sound**> mapSoundData;
};
