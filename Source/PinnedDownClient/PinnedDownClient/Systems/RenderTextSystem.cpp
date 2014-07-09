
#include "pch.h"
#include "Core\Event.h"
#include "Core\EventManager.h"
#include "Systems\RenderTextSystem.h"
#include "../Helpers/DirectXHelper.h"
#include "Systems\Rendering\TextData.h"

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
	if (this->redBrush != nullptr)
	{
		this->redBrush.Reset();
	}

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

	// Debug text to draw.
	Rendering::TextData textData = Rendering::TextData();
	textData.Text = L"Hello World!\nHow are you?";
	textData.X = 250;
	textData.Y = 200;
	textData.Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

	this->d2dContext->BeginDraw();

	// Set text alignment.
	DX::ThrowIfFailed(
		this->textFormat->SetTextAlignment(textData.Alignment)
		);

	// TODO: Adjust translation via text metrics depending on alignment.
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(textData.X, textData.Y);
	this->d2dContext->SetTransform(screenTranslation);

	// Create final text layout for drawing.
	Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;

	DX::ThrowIfFailed(
		this->writeFactory->CreateTextLayout(
		textData.Text.c_str(),
		(uint32)textData.Text.length(),
		this->textFormat.Get(),
		500.0f, // Max width.
		500.0f, // Max height.
		&textLayout)
		);

	DWRITE_TEXT_METRICS metrics;
	DX::ThrowIfFailed(
		textLayout->GetMetrics(&metrics)
		);

	// Text metrics, such as line height, can be accessed here: metrics.height

	// Draw text.
	this->d2dContext->DrawTextLayout(
		D2D1::Point2F(0.f, 0.f),
		textLayout.Get(),
		this->redBrush.Get()
		);

	// TODO: Reset transform to previous state instead of just setting to origin.
	screenTranslation = D2D1::Matrix3x2F::Translation(0, 0);
	this->d2dContext->SetTransform(screenTranslation);

	this->d2dContext->EndDraw();
}
