// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"


ALevelManager* ALevelManager::m_instance = nullptr;
const FName ALevelManager::ON_LOADED_FUNC_NAME = FName("OnLevelLoaded");
const FName ALevelManager::DEFAULT_FNAME = FName("Default");
const TMap<ELevelType, FName> ALevelManager::LevelNamesMap = {
	{ ELevelType::LevelA, FName("/Game/StarterContent/Maps/StarterMap") },
	{ ELevelType::LevelB, FName("/Game/StarterContent/Maps/Minimal_Default") },
	{ ELevelType::LevelC, FName("/Game/StarterContent/Maps/Advanced_Lighting") }
};
// Sets default values
ALevelManager::ALevelManager()
{
	if (m_instance != nullptr) {
		Destroy();
		return;
	}
	m_instance = this;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PrimaryAssetIDÇÃTypeÇê›íËÇ∑ÇÈ(LevelÇÕ"Map")
	m_levelId.PrimaryAssetType = FPrimaryAssetType::FPrimaryAssetType("Map");

	m_onLevelLoadedDelegate.BindUFunction(this, ON_LOADED_FUNC_NAME);
	AddToRoot();
}

void ALevelManager::ChangeLevel(const FName levelName, const bool isShowOnLoaded)
{
	m_levelId.PrimaryAssetName = levelName;
	m_isShowOnLoaded = isShowOnLoaded;
	m_loadingLevelName = levelName;
	m_isLoadingLevel = true;

	auto sptr = UAssetManager::Get().LoadPrimaryAsset(m_levelId, TArray<FName>(), m_onLevelLoadedDelegate);
	if (sptr) {
		m_streamableHandle = sptr.Get();
	}
	else
	{
		m_streamableHandle = nullptr;
	}
}

void ALevelManager::ChangeLevel(UObject* target, const FName levelName, const bool isShowOnLoaded)
{
	targetPtr = target;
	ChangeLevel(levelName, isShowOnLoaded);
}

void ALevelManager::ChangeLevel(UObject* target, const ELevelType levelType, const bool isShowOnLoaded)
{
	ChangeLevel(target, LevelNamesMap[levelType], isShowOnLoaded);
}

void ALevelManager::OnLevelLoaded()
{
	m_isLoadingLevel = false;
	if (m_isShowOnLoaded)ShowLevel();
}
void ALevelManager::LevelChanged(const FName newLevelName)
{
	m_currentLevelName = newLevelName;
}

void ALevelManager::ShowLevel()
{
	if (m_streamableHandle) {
		if (m_streamableHandle->IsLoadingInProgress())return;
		auto asset = m_streamableHandle->GetLoadedAsset();
		if (asset == nullptr)return;
		
		m_loadingLevelName = asset->GetFName();
	}
	if (targetPtr == nullptr)return;
	UGameplayStatics::OpenLevel(targetPtr, m_loadingLevelName);
	LevelChanged(m_loadingLevelName);
}

ALevelManager* ALevelManager::GetInstance()
{
	if (m_instance == nullptr) {
		m_instance = NewObject<ALevelManager>();
	}
	return m_instance;
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	m_currentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_isLoadingLevel)return;

}



