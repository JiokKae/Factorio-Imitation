#include "SoundManager.h"

HRESULT SoundManager::Init()
{
	// 사운드 시스템 생성
	System_Create(&system);

	// 채널 설정
	system->init(SOUNDBUFFER, FMOD_INIT_NORMAL, NULL);

	// 사운드, 채널 동적할당
	sounds = new Sound*[SOUNDBUFFER];
	channels = new Channel*[SOUNDBUFFER];

	memset(sounds, 0, sizeof(Sound*)* SOUNDBUFFER);
	// ZeroMemory(sounds, sizeof(Sound*)* SOUNDBUFFER);
	memset(channels, 0, sizeof(Sound*)* SOUNDBUFFER);
	
	return S_OK;
}

void SoundManager::Release()
{
	SAFE_ARR_DELETE(sounds);
	SAFE_ARR_DELETE(channels);

	system->close();
	system->release();
	ReleaseSingleton();
}

void SoundManager::Update()
{
	if (system)
		system->update();
}

void SoundManager::AddSound(string key, string fileName, bool isBGM, bool isLoop)
{
	if (isLoop)
	{
		if (isBGM)
		{
			system->createStream(fileName.c_str(), FMOD_LOOP_NORMAL, 0, &sounds[mapSoundData.size()]);
		}
		else 
		{
			system->createSound(fileName.c_str(), FMOD_LOOP_NORMAL, 0, &sounds[mapSoundData.size()]);
		}
	}
	else
	{
		system->createSound(fileName.c_str(), FMOD_DEFAULT, 0, &sounds[mapSoundData.size()]);
	}

	mapSoundData.insert(make_pair(key, &sounds[mapSoundData.size()]));
}

void SoundManager::Play(string key, float volume)
{
	map<string, Sound**>::iterator it;
	int count = 0;
	for (it = mapSoundData.begin(); it != mapSoundData.end(); it++, count++)
	{
		if (it->first == key)
		{
			system->playSound(FMOD_CHANNEL_FREE, (*mapSoundData[key]), false, &channels[count]);

			channels[count]->setVolume(volume);
			break;
		}
	}

}

void SoundManager::Stop(string key)
{
	map<string, Sound**>::iterator it;
	int count = 0;
	for (it = mapSoundData.begin(); it != mapSoundData.end(); it++, count++)
	{
		if (it->first == key)
		{
			channels[count]->stop();
			break;
		}
	}
}

void SoundManager::Pause(string key)
{
	map<string, Sound**>::iterator it;
	int count = 0;
	for (it = mapSoundData.begin(); it != mapSoundData.end(); it++, count++)
	{
		if (it->first == key)
		{
			channels[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(string key)
{
	map<string, Sound**>::iterator it;
	int count = 0;
	for (it = mapSoundData.begin(); it != mapSoundData.end(); it++, count++)
	{
		if (it->first == key)
		{
			channels[count]->setPaused(false);
			break;
		}
	}
}

SoundManager::SoundManager() :
	system(nullptr),
	sounds(nullptr),
	channels(nullptr)
{
}
