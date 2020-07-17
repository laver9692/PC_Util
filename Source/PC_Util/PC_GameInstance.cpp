// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_GameInstance.h"
//#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Math/Color.h"
#include "Misc/DateTime.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"

UPC_GameInstance* UPC_GameInstance::m_instance = nullptr;
UPC_GameInstance* UPC_GameInstance::GetInstance() {
	return m_instance;
}

UPC_GameInstance::UPC_GameInstance() {
	WidgetClass = ConstructorHelpers::FClassFinder<ULevelTransitionDrawer>(TEXT("/Game/LevelManager/LevelDrawer")).Class;
}

//OnWorldChanged->Init->OnStart();
void Log(int32 key, const FString& text)
{
	auto t = (const char*)TCHAR_TO_ANSI(*text);
	//if (GEngine) GEngine->AddOnScreenDebugMessage(key, 10, FColor::Red, text);
	UE_LOG(LogTemp, Warning, TEXT("%s"), t);
}

FString GetText(const FString& text) {
	return text;
	//return FDateTime::Now().ToString() + (text);
}

void UPC_GameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Log(1, GetText("OnWorldChanged"));
}

void UPC_GameInstance::Init()
{
	m_instance = this;
	auto widgetName = FName("LevelTransitionDrawer");
	m_levelTransitionDrawer = CreateWidget<ULevelTransitionDrawer>(this, WidgetClass, widgetName);
	m_levelTransitionDrawer->SetWidgets();

	m_levelManager = ULevelManager::GetInstance();
	Log(2, GetText("Init"));
}

void UPC_GameInstance::StartGameInstance()
{
	Log(3, GetText("StartGameInstance"));
}

void UPC_GameInstance::OnStart()
{
	Log(4, GetText("OnStart"));
}
