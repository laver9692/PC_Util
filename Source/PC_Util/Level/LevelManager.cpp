// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"


ALevelManager* ALevelManager::m_instance = nullptr;
const FName ALevelManager::ON_LOADED_FUNC_NAME = FName("OnLevelLoaded");
const FName ALevelManager::DEFAULT_FNAME = FName("Default");

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

	//コールバックする対象？
	m_latentActionInfo.CallbackTarget = this;
	//読込完了時に呼び出す関数名
	m_latentActionInfo.ExecutionFunction = ON_LOADED_FUNC_NAME;
	//UUIDの初期化
	m_latentActionInfo.UUID = 1;

	//PrimaryAssetIDのTypeを設定する(Levelは"Map")
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
	//m_loadingAsset = UAsyncActionLoadPrimaryAsset::AsyncLoadPrimaryAsset(GetWorld(), m_levelId, TArray<FName>());
	//auto levelStream = UGameplayStatics::GetStreamingLevel(GetWorld(), levelName);

	//if (levelStream && levelStream->IsLevelLoaded()) {
	//	ShowLevel(levelStream);
	//	return;
	//}
	//UUIDの初期化 読込中のUDIDと同じだとエラーが出る
	//m_latentActionInfo.UUID += 1;

	//UGameplayStatics::LoadStreamLevel(this, levelName, isShowOnLoaded, false, m_latentActionInfo);
}

void ALevelManager::ChangeLevel(UObject* target, const FName levelName, const bool isShowOnLoaded)
{
	targetPtr = target;
	ChangeLevel(levelName, isShowOnLoaded);
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
		if (!asset)return;
		m_loadingLevelName = asset->GetFName();
	}
	if (targetPtr == nullptr)return;
	UGameplayStatics::OpenLevel(targetPtr, m_loadingLevelName);
	LevelChanged(m_loadingLevelName);

}

void ALevelManager::ShowLevel(const FName newLevelName)
{
	if (!newLevelName.IsValid())return;
	FName newName = FName(newLevelName);
	auto levelId = new FPrimaryAssetId(m_levelId);
	if (newName == DEFAULT_FNAME) {
		newName = m_levelId.PrimaryAssetName;
	}
	else {
		levelId->PrimaryAssetName = newName;
	}

	//auto world = UAssetManager::Get().GetPrimaryAssetObject<UWorld>(m_levelId);
	TArray<UObject*>objects = TArray<UObject*>();
	if (!UAssetManager::Get().GetPrimaryAssetObjectList(m_levelId.PrimaryAssetType, objects)) return;
	for (auto obj : objects) {
		if (newName.ToString().Contains(obj->GetFName().ToString())) {
			UGameplayStatics::OpenLevel(this, newName);
			LevelChanged(newName);
		}
	}
	//if (!world)return;
	//ULevelStreaming* levelStream = UGameplayStatics::GetStreamingLevel(GetWorld(), newLevelName);
	//指定したレベル名でレベルストリームが取得できない場合は処理を停止
	//check(levelStream != nullptr);
	//ShowLevel(levelStream);
	//levelStream->SetShouldBeVisible
}

void ALevelManager::ShowLevel(const ULevelStreaming* newLevel)
{
	if (!newLevel)return;
	auto newLevelName = newLevel->GetFName();
	UGameplayStatics::OpenLevel(this, newLevelName);
	LevelChanged(newLevelName);
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



