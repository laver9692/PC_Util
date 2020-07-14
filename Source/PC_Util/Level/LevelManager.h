// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Private/AsyncActionLoadPrimaryAsset.h"
#include "Engine/StreamableManager.h"
#include "LevelManager.generated.h"

DECLARE_DELEGATE_OneParam(LevelEvent, FName);
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

private:
	ALevelManager();
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
	/// <param name="levelName">�J�ڂ��郌�x����</param>
	/// <param name="isDrawOnLoaded">�Ǎ�������������V�������x����\�����邩</param>
	void ChangeLevel(const FName levelName, const bool isShowOnLoaded = false);
	void ChangeLevel(UObject* target, const FName levelName, const bool isShowOnLoaded = false);

	/// <summary>
	/// ���x���ύX��
	/// </summary>
	void LevelChanged(const FName newLevelName);

	/// <summary>
	/// ���x���̕\��
	/// </summary>
	/// <param name="newLevelName"></param>
	void ShowLevel();
	void ShowLevel(const FName newLevelName);
	void ShowLevel(const ULevelStreaming* newLevel);

	/// <summary>
	/// ���x���Ǎ��������̃C�x���g
	/// </summary>
	LevelEvent onLevelChanged;

	/// <summary>
	/// ���x���\�����̃C�x���g
	/// </summary>
	LevelEvent onLevelShowed;

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

	/// <summary>
	/// ���x���̓ǂݍ��ݎ��Ɏg�����
	/// </summary>
	FLatentActionInfo m_latentActionInfo;
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

	//UAsyncActionLoadPrimaryAsset* m_loadingAsset;
	FStreamableHandle* m_streamableHandle;
	FStreamableDelegate m_onLevelLoadedDelegate;
	UObject* targetPtr = nullptr;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
