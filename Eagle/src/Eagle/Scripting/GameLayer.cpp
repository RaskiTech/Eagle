#include <EaglePCH.h>
#include "GameLayer.h"

// This wouldn't be inside Eagle, but probably somewhere defined.
#include "Eagle/Scripting/ExampleScript.h"

namespace Egl {
	GameLayer::GameLayer()
	{
		FrameBufferDefenition defenition;
		defenition.width = 1280;
		defenition.height = 720;
		mFrameBuffer = FrameBuffer::Create(defenition);
	}
	void GameLayer::OnAttach()
	{
		mActiveScene = ApplicationStartup();
		mActiveScene->SceneBegin();
	}
	void GameLayer::OnDetach()
	{
		mActiveScene->SceneEnd();
	}
	void GameLayer::OnUpdate()
	{
		mActiveScene->OnUpdate();
	}
	void GameLayer::ResetApplication()
	{
		OnDetach();
		OnAttach();
	}
	void GameLayer::SetScale()
	{
		mActiveScene->SetViewportAspectRatio(1.6f / 0.9f);
	}
	void GameLayer::OnEvent(Event& event)
	{

	}
	void GameLayer::OnImGuiRender()
	{

	}
}