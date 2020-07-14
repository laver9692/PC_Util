// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Private/AsyncActionLoadPrimaryAsset.h"
#include "Engine/StreamableManager.h"
#include "LevelManager.generated.h"

DECLARE_DELEGATE_OneParam(LevelEvent, FName);
/// <summary>
/// Levelの遷移用
/// 参考
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
		/// レベルの読み込みが完了したときのコールバック(UFUNCTIONを付けなければいけないらしい)
		/// </summary>
		void OnLevelLoaded();

	/// <summary>
	/// レベルの変更、読込を行い、終わり次第、指定のレベルに遷移する。
	/// </summary>
	/// <param name="levelName">遷移するレベル名</param>
	/// <param name="isDrawOnLoaded">読込が完了し次第新しいレベルを表示するか</param>
	void ChangeLevel(const FName levelName, const bool isShowOnLoaded = false);
	void ChangeLevel(UObject* target, const FName levelName, const bool isShowOnLoaded = false);

	/// <summary>
	/// レベル変更時
	/// </summary>
	void LevelChanged(const FName newLevelName);

	/// <summary>
	/// レベルの表示
	/// </summary>
	/// <param name="newLevelName"></param>
	void ShowLevel();
	void ShowLevel(const FName newLevelName);
	void ShowLevel(const ULevelStreaming* newLevel);

	/// <summary>
	/// レベル読込完了時のイベント
	/// </summary>
	LevelEvent onLevelChanged;

	/// <summary>
	/// レベル表示時のイベント
	/// </summary>
	LevelEvent onLevelShowed;

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// レベルのロード状況の取得
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
	/// レベルの読み込み時に使うやつ
	/// </summary>
	FLatentActionInfo m_latentActionInfo;
	FPrimaryAssetId m_levelId;

	/// <summary>
	/// 今のレベル名
	/// </summary>
	FName m_currentLevelName;

	/// <summary>
	/// 読込中、もしくは読み込んだレベル名
	/// </summary>
	FName m_loadingLevelName;

	/// <summary>
	/// レベルを読み込み中か
	/// </summary>
	bool m_isLoadingLevel = false;

	/// <summary>
	/// レベルの読み込みが完了したらレベルを表示するか
	/// </summary>
	bool m_isShowOnLoaded = false;

	/// <summary>
	/// レベルの読み込みを同期処理で行うか
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
