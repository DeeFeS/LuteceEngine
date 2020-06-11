#pragma once
#define F_PI 3.14159265358979323846f
#define D_PI 3.14159265358979323846
#define ToRadian(angle) (angle / 180.f * F_PI)
#define ToDegree(angle) (angle / F_PI * 180.f)
#define SafeDelete(T) if(T != nullptr) delete T;
#define UnreferencedParameter(T) (T)