#include "GameEnginePCH.h"
#include "CameraComponent.h"

LuteceEngine::CameraComponent::CameraComponent()
	: Component((int)eEngineComponentType::Camera)
{}

//glm::vec2 LuteceEngine::CameraComponent::GetInversePosition() const
//{
//	auto window = GameEngine::GetWindow();
//	auto ret = -GetTransform()->GetWorldPosition();
//	ret.x += window.width / 2.f;
//	ret.y += window.height / 2.f;
//	return ret;
//}
