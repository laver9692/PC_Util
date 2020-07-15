// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Private/AsyncActionLoadPrimaryAsset.h"
#include "Engine/StreamableManager.h"
#include "LevelManager.generated.h"

//ChangeLevel->FadeOut->OnFadedOut(Event)->LoadLevel->Load����UpdateProgress->OnLoadedLevel(Event)->OpenLevel->FadeIn->OnFadedIn(Event)->OnLevelChanged(Event)
UENUM(BlueprintType)
enum class ELevelType : uint8
{
	LevelA,
	LevelB,
	LevelC
};

DECLARE_MULTICAST_DELEGATE_OneParam(LevelEvent, FName);
/// <summary>
/// Level�̑J�ڗp
/// �Q�l
/// https://hexadrive.jp/hexablog/program/41346/
/// </summary>
UCLASS()
class PC_UTIL_API ALevelManager : public AActor
{
	GENERATED_BODY()

		static const FName ON_LOADED_FUNC_NAME;
	static const FName DEFAULT_FNAME;
	static const TMap<ELevelType, FName> LevelNamesMap;

private:
	ALevelManager();
	~ALevelManager();
protected:
	static ALevelManager* m_instance;
public:
	// Sets default values for this actor's properties

	UFUNCTION()
		/// <summary>
		/// ���x���̓ǂݍ��݂����������Ƃ��̃R�[���o�b�N(UFUNCTION��t���Ȃ���΂����Ȃ��炵��)
		/// </summary>
		void OnLevelLoaded();

	/// <summary>
	/// ���x���̕ύX�A�Ǎ����s���A�I��莟��A�w��̃��x���ɑJ�ڂ���B
	/// </summary>
	/// <param name="target">���x���J�ڂɎg��UObject</param>
	/// <param name="levelType">�J�ڂ��郌�x��(�b��Ή�)</param>
	void ChangeLevel(UObject* target, const ELevelType levelType);
	void ChangeLevel(UObject* target, const FName levelName);
	void ChangeLevel(const FName levelName);

	/// <summary>
	/// ���x���ύX��
	/// </summary>
	void LevelChanged(const FName newLevelName);

	/// <summary>
	/// ���x���̕\��
	/// </summary>
	/// <param name="target"></param>
	void ShowLevel(UObject* target = nullptr);

	/// <summary>
	/// ���x���̓ǂݍ���
	/// </summary>
	/// <param name="levelName">�ǂݍ��ރ��x����(�l���f�t�H���g�Ȃ���ɐݒ肳��Ă��閼�O�œǂݍ���)</param>
	void LoadLevel(const FName levelName = DEFAULT_FNAME);

	/// <summary>
	/// ���x���Ǎ��������̃C�x���g
	/// </summary>
	LevelEvent onLevelChanged = LevelEvent();

	/// <summary>
	/// ���x���\�����̃C�x���g
	/// </summary>
	LevelEvent onLevelShowed = LevelEvent();

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// ���x���̃��[�h�󋵂̎擾
		/// </summary>
		/// <returns>0~1</returns>
		float GetLoadingProgress() {
		if (m_streamableHandle)return m_streamableHandle->GetProgress();
		return 0;
	}
	static ALevelManager* GetInstance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnFadedIn();
	void OnFadedOut();

	void CreateDrawer();

	void Init();

	/// <summary>
	/// ���x���̓ǂݍ��ݎ��Ɏg�����
	/// </summary>
	//FLatentActionInfo m_latentActionInfo;
	FPrimaryAssetId m_levelId;

	/// <summary>
	/// ���̃��x����
	/// </summary>
	FName m_currentLevelName;

	/// <summary>
	/// �Ǎ����A�������͓ǂݍ��񂾃��x����
	/// </summary>
	FName m_loadingLevelName;

	/// <summary>
	/// ���x����ǂݍ��ݒ���
	/// </summary>
	bool m_isLoadingLevel = false;

	/// <summary>
	/// ���x���̓ǂݍ��݂����������烌�x����\�����邩
	/// </summary>
	bool m_isShowOnLoaded = false;

	/// <summary>
	/// ���x���̓ǂݍ��݂𓯊������ōs����
	/// </summary>
	bool m_isLoadSync = false;

	bool m_isInit = false;
	UPROPERTY()
		class ULevelTransitionDrawer* m_drawer;

	//UAsyncActionLoadPrimaryAsset* m_loadingAsset;
	FStreamableHandle* m_streamableHandle;
	FStreamableDelegate m_onLevelLoadedDelegate;
	UObject* targetPtr = nullptr;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
