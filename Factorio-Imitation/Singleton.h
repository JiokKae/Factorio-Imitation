// Singleton.h

#pragma once

template <typename T>
class Singleton
{
protected:
	static T* instance;

	Singleton() {};
	virtual ~Singleton() {};

public:
	static T* GetSingleton();
	void ReleaseSingleton();
};

// 싱글톤 초기화
// static 변수 문법상 {} 영역 밖에서 초기화한다.
template <typename T>
T* Singleton<T>::instance = nullptr;

template <typename T>
T* Singleton<T>::GetSingleton()
{
	if (!instance)
	{
		instance = new T;
	}

	return instance;
}

template<typename T>
inline void Singleton<T>::ReleaseSingleton()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}
