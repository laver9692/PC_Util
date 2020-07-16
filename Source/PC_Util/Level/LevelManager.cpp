// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "LevelTransitionDrawer.h"
#include "UObject/ConstructorHelpers.h"

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

	//PrimaryAssetID��Type��ݒ肷��(Level��"Map")
	m_levelId.PrimaryAssetType = FPrimaryAssetType::FPrimaryAssetType("Map");

	m_onLevelLoadedDelegate.BindUFunction(this, ON_LOADED_FUNC_NAME);

	WidgetClass = ConstructorHelpers::FClassFinder<ULevelTransitionDrawer>(TEXT("/Game/LevelManager/LevelDrawer")).Class;

	AddToRoot();

	//�����Tick���Ă΂��悤�ɂȂ邩�Ǝv���΂����ł��Ȃ�����
	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 0;
	//PrimaryActorTick.TickGroup = ETickingGroup::TG_DuringPhysics;
	//PrimaryActorTick.Target = this;
	//bAllowTickBeforeBeginPlay = true;
	//SetActorTickEnabled(true);
	//RegisterAllActorTickFunctions(true, false);
}

ALevelManager::~ALevelManager()
{
	if (m_instance == this)m_instance = nullptr;
}

void ALevelManager::ChangeLevel(const FName levelName)
{
	m_isShowOnLoaded = true;
	//�ǂݍ��ރ��x�����̐ݒ�
	m_levelId.PrimaryAssetName = levelName;
	//LoadLevel(levelName);
	m_drawer->FadeOut(1);
}

void ALevelManager::ChangeLevel(UObject* target, const FName levelName)
{
	targetPtr = target;
	ChangeLevel(levelName);
}

void ALevelManager::ChangeLevel(UObject* target, const ELevelType levelType)
{
	//��ŏ�������
	targetPtr = target;
	Init();
	ChangeLevel(target, LevelNamesMap[levelType]);
}

void ALevelManager::OnLevelLoaded()
{
	m_isLoadingLevel = false;
	m_drawer->SetProgress(1);
	if (m_isShowOnLoaded) {
		ShowLevel();
		m_isShowOnLoaded = false;
	}
}
void ALevelManager::LevelChanged(const FName newLevelName)
{
	m_currentLevelName = newLevelName;
}

void ALevelManager::ShowLevel(UObject* target)
{
	if (target != nullptr) {
		targetPtr = target;
	}

	if (m_streamableHandle) {
		if (m_streamableHandle->IsLoadingInProgress())return;
		auto asset = m_streamableHandle->GetLoadedAsset();
		if (asset == nullptr)return;

		m_loadingLevelName = asset->GetFName();
	}
	if (targetPtr == nullptr)return;
	//this��OpenLevel����ƁA�Ȃ����J�ڂ��Ă���Ȃ������̂ŁA�b��Ή��Ƃ��āA�K���ȃI�u�W�F�N�g���g�p
	//LevelManager��Singleton�ł��邱�ƂƂ�����肪����\������
	UGameplayStatics::OpenLevel(targetPtr, m_loadingLevelName);
	LevelChanged(m_loadingLevelName);
}

void ALevelManager::LoadLevel(const FName levelName)
{
	//�ǂݍ���ł��郌�x���̕ۑ�(�ꉞ)
	m_loadingLevelName = levelName;
	//���Ǎ����ł���Ƃ����t���O�i�ꉞ�j
	m_isLoadingLevel = true;

	auto sptr = UAssetManager::Get().LoadPrimaryAsset(m_levelId, TArray<FName>(), m_onLevelLoadedDelegate);
	//���ۂɓǂݍ��݂��������Ȃ��i���ɓǂݍ��܂�Ă���j�ꍇ�A���̉������A�o�^���Ă���C�x���g����ɌĂ΂��̂ŁA���ӂ��K�v
	if (sptr) {
		m_streamableHandle = sptr.Get();
	}
	else
	{
		m_streamableHandle = nullptr;
	}
}

ALevelManager* ALevelManager::GetInstance()
{
	if (m_instance == nullptr) {
		m_instance = NewObject<ALevelManager>();
	}
	return m_instance;
}

// Called when the game starts or when spawned
//�Ă΂�Ȃ��B�V���O���g���֌W����H
void ALevelManager::BeginPlay()
{
	//Super::BeginPlay();
}

void ALevelManager::OnFadedIn()
{
	onLevelChanged.Broadcast(m_currentLevelName);
}

void ALevelManager::OnFadedOut()
{
	LoadLevel();
}

void ALevelManager::CreateDrawer()
{
	auto gameInstance = UGameplayStatics::GetGameInstance(targetPtr);
	auto widgetName = FName("LevelTransitionDrawer");
	check(gameInstance != nullptr);
	m_drawer = CreateWidget<ULevelTransitionDrawer>(gameInstance, WidgetClass, widgetName);
	check(m_drawer != nullptr);
	//Event�̓o�^
	m_drawer->onFadedIn.AddLambda([&] { OnFadedIn(); });
	m_drawer->onFadedOut.AddLambda([&] { OnFadedOut(); });

	m_drawer->AddToViewport(INT32_MAX);
}

void ALevelManager::Init()
{
	if (m_isInit)return;
	m_currentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
	if (m_drawer == nullptr) {
		CreateDrawer();
	}
	m_isInit = true;
}

// Called every frame
//�������Tick���Ă΂�Ȃ�
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (m_isLoadingLevel)return;
	if (m_streamableHandle == nullptr)return;
	m_drawer->SetProgress(m_streamableHandle->GetProgress());
}



