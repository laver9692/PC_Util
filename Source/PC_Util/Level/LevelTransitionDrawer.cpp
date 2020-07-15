// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionDrawer.h"
#include "Math/Color.h"

ULevelTransitionDrawer::ULevelTransitionDrawer(const class FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {

}
void ULevelTransitionDrawer::NativeConstruct() {
	levelManager = ALevelManager::GetInstance();
}

void ULevelTransitionDrawer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (m_isFading) {
		auto color = m_fadeImage->ColorAndOpacity;
		float a = color.A + m_fadeTime * InDeltaTime;
		color.A = a;
		if (a < 0) {
			color.A = 0;
			m_fadeImage->SetColorAndOpacity(color);
			OnFadedIn();
		}
		else if (1 < a) {
			color.A = 1;
			m_fadeImage->SetColorAndOpacity(color);
			OnFadedOut();
		}
		else {
			color.A = a;
			m_fadeImage->SetColorAndOpacity(color);
		}
	}
}

void ULevelTransitionDrawer::FadeIn(float fadeTime)
{
	m_fadeTime = -fadeTime;
	m_isFading = true;
	//Image�̐F�̏������ƁA�L����
}

void ULevelTransitionDrawer::FadeOut(float fadeTime)
{
	m_fadeTime = -fadeTime;
	m_isFading = true;
	//Image�̐F�̏������ƁA�L����
	//ProgressBar��Progress�̏�������m_isLoading�̗L����
}

void ULevelTransitionDrawer::SetProgress(float progress)
{
	m_progressBar->SetPercent(progress);
}

void ULevelTransitionDrawer::OnFadedIn()
{
	//Image��Active��؂�
	m_isFading = false;
	onFadedIn.Broadcast();
}

void ULevelTransitionDrawer::OnFadedOut()
{
	m_isFading = false;
	onFadedOut.Broadcast();
}
