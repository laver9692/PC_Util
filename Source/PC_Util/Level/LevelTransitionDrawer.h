// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelManager.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "LevelTransitionDrawer.generated.h"

DECLARE_MULTICAST_DELEGATE(FadeEvent);

/**
 *
 */
UCLASS()
class PC_UTIL_API ULevelTransitionDrawer : public UUserWidget
{
	GENERATED_BODY()
public:
	//FadeOut->FadedOut(Event)->LoadLevel->UpdateProgress->LoadedLevel(Event)->OpenLevel->ChengedLevel->FadeIn->FadedIn(Event)
	ULevelTransitionDrawer(const class FObjectInitializer& ObjectInitializer);
	~ULevelTransitionDrawer();
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FadeEvent onFadedIn = FadeEvent();
	FadeEvent onFadedOut = FadeEvent();

	/// <summary>
	/// 画面の暗転解除 
	/// </summary>
	/// <param name="fadeTime">フェードにかける時間</param>
	void FadeIn(float fadeTime);

	/// <summary>
	/// 画面の暗転
	/// </summary>
	/// <param name="fadeTime">フェードにかける時間</param>
	void FadeOut(float fadeTime);
	UFUNCTION(BlueprintNativeEvent)
		void SetWidgets();
	virtual void SetWidgets_Implementation();

	/// <summary>
	/// 進捗の設定
	/// </summary>
	/// <param name="progress"></param>
	void SetProgress(float progress);

protected:
	float m_fadeTime = -1;

	void OnFadedIn();

	void OnFadedOut();

	void SetColor(float r = -1, float g = -1, float b = -1, float a = -1);

	bool m_isFading = false;
	bool m_isLoading = false;

	UPROPERTY(BlueprintReadWrite)
		UImage* m_fadeImage;
	UPROPERTY(BlueprintReadWrite)
		UProgressBar* m_progressBar;
};
