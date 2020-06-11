#include "GameEnginePCH.h"
#include "Service.h"

template<class T>
T* LuteceEngine::Service<T>::m_pService = nullptr;
template<class T>
T* LuteceEngine::Service<T>::m_pInitialService = nullptr;
