#include "SoundManager.h"

HRESULT SoundManager::Init()
{
	// 사운드 시스템 생성
	FMOD::System_Create(&system);

	// 채널 설정
	if (system)
	{
		system->init(SOUNDBUFFER, FMOD_INIT_NORMAL, NULL);
	}

	// 사운드, 채널 동적할당
	sounds = new FMOD::Sound*[SOUNDBUFFER];
	channels = new FMOD::Channel*[SOUNDBUFFER];

	::memset(sounds, 0, sizeof(FMOD::Sound*)* SOUNDBUFFER);
	// ZeroMemory(sounds, sizeof(FMOD::Sound*)* SOUNDBUFFER);
	::memset(channels, 0, sizeof(FMOD::Sound*)* SOUNDBUFFER);
	
	return S_OK;
}

void SoundManager::Release()
{
	SAFE_ARR_DELETE(sounds);
	SAFE_ARR_DELETE(channels);

	if (system)
	{
		system->close();
		system->release();
	}

	ReleaseSingleton();
}

void SoundManager::Update()
{
	if (system)
	{
		system->update();
	}
}

void SoundManager::AddSound(const std::string& key, const std::string& fileName, bool isBGM, bool isLoop)
{
	if (system == nullptr)
	{
		return;
	}

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

	mapSoundData.emplace(key, &sounds[mapSoundData.size()]);
}

void SoundManager::Play(const std::string& key, float volume)
{
	if (system == nullptr)
	{
		return;
	}

	int count{ 0 };

	for (const std::pair<std::string, FMOD::Sound**>& pair : mapSoundData)
	{
		if (pair.first == key)
		{
			system->playSound(FMOD_CHANNEL_FREE, *pair.second, false, &channels[count]);
			if (channels[count])
			{
				channels[count]->setVolume(volume);
			}
			break;
		}
		count++;
	}
}

void SoundManager::Stop(const std::string& key)
{
	int count{ 0 };

	for (const std::pair<std::string, FMOD::Sound**>& pair : mapSoundData)
	{
		if (pair.first == key)
		{
			if (channels[count])
			{
				channels[count]->stop();
			}
			break;
		}
		count++;
	}
}

void SoundManager::Pause(const std::string& key)
{
	int count{ 0 };

	for (const std::pair<std::string, FMOD::Sound**>& pair : mapSoundData)
	{
		if (pair.first == key)
		{
			if (channels[count])
			{
				channels[count]->setPaused(true);
			}
			break;
		}
		count++;
	}
}

void SoundManager::Resume(const std::string& key)
{
	int count{ 0 };

	for (const std::pair<std::string, FMOD::Sound**>& pair : mapSoundData)
	{
		if (pair.first == key)
		{
			if (channels[count])
			{
				channels[count]->setPaused(false);
			}
			break;
		}
		count++;
	}
}
