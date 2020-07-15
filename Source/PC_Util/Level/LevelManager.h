// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Private/AsyncActionLoadPrimaryAsset.h"
#include "Engine/StreamableManager.h"
#include "LevelManager.generated.h"

//ChangeLevel->FadeOut->OnFadedOut(Event)->LoadLevel->Load中はUpdateProgress->OnLoadedLevel(Event)->OpenLevel->FadeIn->OnFadedIn(Event)->OnLevelChanged(Event)
UENUM(BlueprintType)
enum class ELevelType : uint8
{
	LevelA,
	LevelB,
	LevelC
};

DECLARE_MULTICAST_DELEGATE_OneParam(LevelEvent, FName);
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
		/// レベルの読み込みが完了したときのコールバック(UFUNCTIONを付けなければいけないらしい)
		/// </summary>
		void OnLevelLoaded();

	/// <summary>
	/// レベルの変更、読込を行い、終わり次第、指定のレベルに遷移する。
	/// </summary>
	/// <param name="target">レベル遷移に使うUObject</param>
	/// <param name="levelType">遷移するレベル(暫定対応)</param>
	void ChangeLevel(UObject* target, const ELevelType levelType);
	void ChangeLevel(UObject* target, const FName levelName);
	void ChangeLevel(const FName levelName);

	/// <summary>
	/// レベル変更時
	/// </summary>
	void LevelChanged(const FName newLevelName);

	/// <summary>
	/// レベルの表示
	/// </summary>
	/// <param name="target"></param>
	void ShowLevel(UObject* target = nullptr);

	/// <summary>
	/// レベルの読み込み
	/// </summary>
	/// <param name="levelName">読み込むレベル名(値がデフォルトなら既に設定されている名前で読み込む)</param>
	void LoadLevel(const FName levelName = DEFAULT_FNAME);

	/// <summary>
	/// レベル読込完了時のイベント
	/// </summary>
	LevelEvent onLevelChanged = LevelEvent();

	/// <summary>
	/// レベル表示時のイベント
	/// </summary>
	LevelEvent onLevelShowed = LevelEvent();

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

	void OnFadedIn();
	void OnFadedOut();

	void CreateDrawer();

	void Init();

	/// <summary>
	/// レベルの読み込み時に使うやつ
	/// </summary>
	//FLatentActionInfo m_latentActionInfo;
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
