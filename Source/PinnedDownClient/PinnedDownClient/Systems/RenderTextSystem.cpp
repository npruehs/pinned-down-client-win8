
#include "pch.h"
#include "Core\Event.h"
#include "Core\EventManager.h"
#include "Systems\RenderTextSystem.h"

using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Events;

RenderTextSystem::RenderTextSystem()
{
}

void RenderTextSystem::InitSystem(std::shared_ptr<EventManager> eventManager)
{
	GameSystem::InitSystem(eventManager);

	eventManager->AddListener(std::shared_ptr<IEventListener>(this), RenderTargetChangedEvent::RenderTargetChangedEventType);
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
}

void RenderTextSystem::Render()
{
	// Early out if no render target set.
	if (this->d2dContext == nullptr)
	{
		return;
	}

	this->d2dContext->BeginDraw();

	HRESULT hr = S_OK;
	ID2D1SolidColorBrush* pIRedBrush = NULL;
	IDWriteTextFormat* pITextFormat = NULL;
	IDWriteFactory* pIDWriteFactory = NULL;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pIDWriteFactory));

	if (SUCCEEDED(hr))
	{
		hr = pIDWriteFactory->CreateTextFormat(
			L"Arial",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			10.0f * 96.0f / 72.0f,
			L"en-US",
			&pITextFormat
			);
	}

	if (SUCCEEDED(hr))
	{
		hr = d2dContext->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Red),
			&pIRedBrush
			);
	}

	// Actually draw the text at the origin.
	D2D1_RECT_F layoutRect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);

	if (SUCCEEDED(hr))
	{
		d2dContext->DrawText(
			L"Hello World",
			wcslen(L"Hello World"),
			pITextFormat,
			layoutRect,
			pIRedBrush
			);
	}

	this->d2dContext->EndDraw();
}
