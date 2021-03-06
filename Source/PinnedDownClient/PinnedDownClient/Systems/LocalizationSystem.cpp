#include "pch.h"

#include "Event.h"

#include "Events\LocalizationDataLoadedEvent.h"

#include "Components\LocalizationComponent.h"
#include "Components\TextComponent.h"

#include "Resources\PinnedDownResourceManager.h"
#include "Resources\TextResourceHandle.h"

#include "Systems\LocalizationSystem.h"

#include "Util\StringUtils.h"


using namespace PinnedDownClient::Components;
using namespace PinnedDownClient::Events;
using namespace PinnedDownClient::Resources;
using namespace PinnedDownClient::Systems;
using namespace PinnedDownClient::Util;


#define LOCALIZATION_FILE_NAME "English.lua"
#define LOCALIZATION_FILE_WNAME L"English.lua"


LocalizationSystem::LocalizationSystem()
{
}

void LocalizationSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, EntityInitializedEvent::EntityInitializedEventType);
	this->game->eventManager->AddListener(this, LocalizedTextChangedEvent::LocalizedTextChangedEventType);
	this->game->eventManager->AddListener(this, ResourceLoadedEvent::ResourceLoadedEventType);

	// Init Lua.
	this->lua = std::make_shared<LuaInstance>();
	this->lua->SetLogger(this->game->logger);

	// Load localization data.
	auto resourceManager = static_cast<PinnedDownResourceManager*>(this->game->resourceManager.get());
	resourceManager->LoadTextFromFile(LOCALIZATION_FILE_WNAME);
}

void LocalizationSystem::LoadLocalizationData()
{
	// Load data.
	auto localizationDataHandle = this->game->resourceManager->GetResource<TextResourceHandle>(LOCALIZATION_FILE_NAME);
	auto localizationDataString = WStringToString(localizationDataHandle->text);
	auto localizationData = localizationDataString.c_str();
	this->lua->ExecuteScript(localizationData);

	// Notify listeners.
	auto localizationDataLoadedEvent = std::make_shared<LocalizationDataLoadedEvent>();
	this->game->eventManager->QueueEvent(localizationDataLoadedEvent);
}

void LocalizationSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(EntityInitializedEvent);
	CALL_EVENT_HANDLER(LocalizedTextChangedEvent);
	CALL_EVENT_HANDLER(ResourceLoadedEvent);
}

EVENT_HANDLER_DEFINITION(LocalizationSystem, EntityInitializedEvent)
{
	this->LocalizeText(data.entity);
}

EVENT_HANDLER_DEFINITION(LocalizationSystem, LocalizedTextChangedEvent)
{
	this->LocalizeText(data.entity);
}

EVENT_HANDLER_DEFINITION(LocalizationSystem, ResourceLoadedEvent)
{
	if (!data.resourceName.compare(LOCALIZATION_FILE_NAME))
	{
		this->LoadLocalizationData();
	}
}

void LocalizationSystem::LocalizeText(Entity entity)
{
	auto localizationComponent = this->game->entityManager->GetComponent<LocalizationComponent>(entity, LocalizationComponent::LocalizationComponentType);

	if (localizationComponent != nullptr && !localizationComponent->localizationKey.empty())
	{
		// Store localized value.
		auto key = WStringToString(localizationComponent->localizationKey);
		auto value = this->lua->GetString(key.c_str());
		localizationComponent->localizationValue = value.empty() ? localizationComponent->localizationKey : StringToWString(value);

		auto textComponent = this->game->entityManager->GetComponent<TextComponent>(entity, TextComponent::TextComponentType);
		textComponent->text = localizationComponent->localizationValue;
	}
}