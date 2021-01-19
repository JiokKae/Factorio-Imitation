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

// �̱��� �ʱ�ȭ
// static ���� ������ {} ���� �ۿ��� �ʱ�ȭ�Ѵ�.
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
