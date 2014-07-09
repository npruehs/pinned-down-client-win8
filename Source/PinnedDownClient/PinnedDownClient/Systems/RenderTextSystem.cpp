
#include "pch.h"
#include "Core\Event.h"
#include "Core\EventManager.h"
#include "Systems\RenderTextSystem.h"
#include "../Helpers/DirectXHelper.h"

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Events;

RenderTextSystem::RenderTextSystem()
{
}

void RenderTextSystem::InitSystem(std::shared_ptr<EventManager> eventManager)
{
	GameSystem::InitSystem(eventManager);

	eventManager->AddListener(std::shared_ptr<IEventListener>(this), RenderTargetChangedEvent::RenderTargetChangedEventType);

	DX::ThrowIfFailed(
		DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory2),
		&this->writeFactory)
		);

	// Create text format for the default font and size.
	DX::ThrowIfFailed(
		writeFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		12.0f,
		L"en-US",
		&this->textFormat)
		);
}

void RenderTextSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == RenderTargetChangedEvent::RenderTargetChangedEventType)
	{
		RenderTargetChangedEvent renderTargetChangedEvent = static_cast<RenderTargetChangedEvent&>(newEvent);
		this->OnRenderTargetChanged(renderTargetChangedEvent);
	}
}

void RenderTextSystem::OnRenderTargetChanged(RenderTargetChangedEvent renderTargetChangedEvent)
{
	this->d2dContext = renderTargetChangedEvent.d2dContext;

	// Create brush for font rendering.
	DX::ThrowIfFailed(
		this->d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&this->redBrush)
		);
}

void RenderTextSystem::Render()
{
	// Early out if no render target set.
	if (this->d2dContext == nullptr)
	{
		return;
	}

	// Actually draw the text at the origin.
	D2D1_RECT_F layoutRect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);

	this->d2dContext->BeginDraw();
	this->d2dContext->DrawText(
		L"Hello World",
		wcslen(L"Hello World"),
		textFormat.Get(),
		layoutRect,
		redBrush.Get());
	this->d2dContext->EndDraw();
}
