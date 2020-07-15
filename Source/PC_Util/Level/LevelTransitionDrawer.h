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
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	FadeEvent onFadedIn = FadeEvent();
	FadeEvent onFadedOut = FadeEvent();

	/// <summary>
	/// ��ʂ̈Ó]���� 
	/// </summary>
	/// <param name="fadeTime">�t�F�[�h�ɂ����鎞��</param>
	void FadeIn(float fadeTime);

	/// <summary>
	/// ��ʂ̈Ó]
	/// </summary>
	/// <param name="fadeTime">�t�F�[�h�ɂ����鎞��</param>
	void FadeOut(float fadeTime);
	UFUNCTION(BluePrintCallable)
		void SetWidgets(UImage* image, UProgressBar* bar) {
		m_fadeImage = image;
		m_progressBar = bar;
	}

	/// <summary>
	/// �i���̐ݒ�
	/// </summary>
	/// <param name="progress"></param>
	void SetProgress(float progress);

protected:
	UPROPERTY(BlueprintReadOnly)
		ALevelManager* levelManager = nullptr;

	UPROPERTY(BlueprintReadOnly)
		float m_fadeTime = -1;

	UFUNCTION(BlueprintCallable)
		void OnFadedIn();

	UFUNCTION(BlueprintCallable)
		void OnFadedOut();

	bool m_isFading = false;
	bool m_isLoading = false;

	UPROPERTY()
		UImage* m_fadeImage;
	UPROPERTY()
		UProgressBar* m_progressBar;
};
