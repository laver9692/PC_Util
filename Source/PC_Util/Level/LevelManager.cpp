// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "LevelTransitionDrawer.h"
#include "UObject/ConstructorHelpers.h"
#include "../PC_GameInstance.h"

ULevelManager* ULevelManager::m_instance = nullptr;
const FName ULevelManager::ON_LOADED_FUNC_NAME = FName("OnLevelLoaded");
const FName ULevelManager::DEFAULT_FNAME = FName("Default");
const TMap<ELevelType, FName> ULevelManager::LevelNamesMap = {
	{ ELevelType::LevelA, FName("/Game/StarterContent/Maps/StarterMap") },
	{ ELevelType::LevelB, FName("/Game/StarterContent/Maps/Minimal_Default") },
	{ ELevelType::LevelC, FName("/Game/StarterContent/Maps/Advanced_Lighting") }
};

// Sets default values
ULevelManager::ULevelManager()
{
	if (m_instance != nullptr) {
		BeginDestroy();
		return;
	}
	//WidgetClass = ConstructorHelpers::FClassFinder<ULevelTransitionDrawer>(TEXT("/Game/LevelManager/LevelDrawer")).Class;
	//�����Tick���Ă΂��悤�ɂȂ邩�Ǝv���΂����ł��Ȃ�����
	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.TickInterval = 0;
	//PrimaryActorTick.TickGroup = ETickingGroup::TG_DuringPhysics;
	//PrimaryActorTick.Target = this;
	//bAllowTickBeforeBeginPlay = true;
	//SetActorTickEnabled(true);
	//RegisterAllActorTickFunctions(true, false);
}

ULevelManager::~ULevelManager()
{
	if (m_instance == this)m_instance = nullptr;
	m_drawer = nullptr;
}

void ULevelManager::ChangeLevel(const FName levelName)
{
	m_isShowOnLoaded = true;
	//�ǂݍ��ރ��x�����̐ݒ�
	m_levelId.PrimaryAssetName = levelName;
	//LoadLevel(levelName);
	m_drawer->FadeOut(1);
}

void ULevelManager::ChangeLevel(UObject* target, const FName levelName)
{
	targetPtr = target;
	ChangeLevel(levelName);
}

void ULevelManager::ChangeLevel(UObject* target, const ELevelType levelType)
{
	//��ŏ�������
	targetPtr = target;
	Init();
	ChangeLevel(target, LevelNamesMap[levelType]);
}

void ULevelManager::OnLevelLoaded()
{
	m_isLoadingLevel = false;
	m_drawer->SetProgress(1);
	if (m_isShowOnLoaded) {
		ShowLevel();
		m_isShowOnLoaded = false;
	}
}
void ULevelManager::LevelChanged(const FName newLevelName)
{
	m_currentLevelName = newLevelName;
}

void ULevelManager::ShowLevel(UObject* target)
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

void ULevelManager::LoadLevel(const FName levelName)
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

ULevelManager* ULevelManager::GetInstance()
{
	if (m_instance == nullptr) {
		m_instance = NewObject<ULevelManager>();
		m_instance->Init();
	}
	return m_instance;
}

// Called when the game starts or when spawned
//�Ă΂�Ȃ��B�V���O���g���֌W����H
//void ULevelManager::BeginPlay()
//{
//	//Super::BeginPlay();
//}

void ULevelManager::OnFadedIn()
{
	onLevelChanged.Broadcast(m_currentLevelName);
}

void ULevelManager::OnFadedOut()
{
	LoadLevel();
}

void ULevelManager::CreateDrawer()
{
	//auto gameInstance = UGameplayStatics::GetGameInstance(UGameplayStatics::GetActorOfClass(GetWorld(), AActor::StaticClass()));
	//auto widgetName = FName("LevelTransitionDrawer");
	//check(gameInstance != nullptr);
	//m_drawer = CreateWidget<ULevelTransitionDrawer>(gameInstance, WidgetClass, widgetName);
	//check(m_drawer != nullptr);
	////Event�̓o�^
	//m_drawer->onFadedIn.AddLambda([&] { OnFadedIn(); });
	//m_drawer->onFadedOut.AddLambda([&] { OnFadedOut(); });
}

void ULevelManager::Init()
{
	if (m_isInit)return;
	m_currentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//UObject�ɕς������ʁA��������Tick���g���Ȃ��̂ŃR�����g�A�E�g
	//PrimaryActorTick.bCanEverTick = true;

	//PrimaryAssetID��Type��ݒ肷��(Level��"Map")
	m_levelId.PrimaryAssetType = FPrimaryAssetType::FPrimaryAssetType("Map");

	m_onLevelLoadedDelegate.BindUFunction(this, ON_LOADED_FUNC_NAME);

	AddToRoot();
	if (m_drawer == nullptr) {
		m_drawer = UPC_GameInstance::GetInstance()->GetLevelTransitionDrawer();
		if (m_drawer != nullptr) {
			//if (!m_drawer->onFadedIn.IsBound())onFadedIn = FadeEvent();
			//if (!m_drawer->onFadedOut.IsBound())onFadedOut = FadeEvent();
			m_drawer->onFadedIn.AddLambda([this] { OnFadedIn(); });
			m_drawer->onFadedOut.AddLambda([this] { OnFadedOut(); });
		}
	}
	m_isInit = true;
}
