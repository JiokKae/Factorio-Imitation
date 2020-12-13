#pragma once
#include "framework.h"
#include "Singleton.h"

// FMOD ����
//#pragma comment(lib, "lib/fmodex_vc.lib")
#pragma comment(lib, "lib/fmodex64_vc.lib")
#include "inc/fmod.hpp"

using namespace FMOD;

#define SOUNDBUFFER		10

class SoundManager : public Singleton<SoundManager>
{
private:
	System* system;		// FMOD ���̺귯�� ����
	Sound** sounds;		// ����
	Channel** channels;	// ��� ���� ����

	map<string, Sound**> mapSoundData;

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

