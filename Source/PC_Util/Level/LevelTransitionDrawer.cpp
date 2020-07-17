// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionDrawer.h"
#include "Math/Color.h"

ULevelTransitionDrawer::ULevelTransitionDrawer(const class FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {

}
void ULevelTransitionDrawer::NativeConstruct() {
	Super::NativeConstruct();
	SetWidgets();
}

void ULevelTransitionDrawer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (m_isFading) {
		if (m_fadeImage == nullptr)return;
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

void ULevelTransitionDrawer::SetColor(float r, float g, float b, float a) {
	auto color = m_fadeImage->ColorAndOpacity;
	if (0 <= r)color.R = r;
	if (0 <= g)color.G = g;
	if (0 <= b)color.B = b;
	if (0 <= a)color.A = a;
	m_fadeImage->SetColorAndOpacity(color);
}

void ULevelTransitionDrawer::FadeIn(float fadeTime)
{
	m_fadeTime = -fadeTime;
	m_isFading = true;
	//Imageの色の初期化と、有効化
}

void ULevelTransitionDrawer::FadeOut(float fadeTime)
{
	SetColor(-1, -1, -1, 0);
	AddToViewport(INT32_MAX);
	//なぜかThisがNullになることがあるらしい
	m_fadeTime = fadeTime;
	m_isFading = true;
	//Imageの色の初期化と、有効化
	//ProgressBarのProgressの初期化とm_isLoadingの有効化
	m_progressBar->SetPercent(0);
	m_isLoading = true;
}

void ULevelTransitionDrawer::SetWidgets_Implementation()
{
}

void ULevelTransitionDrawer::SetProgress(float progress)
{
	if (m_progressBar == nullptr)return;
	m_progressBar->SetPercent(progress);
}

void ULevelTransitionDrawer::OnFadedIn()
{
	//ImageのActiveを切る
	m_isFading = false;
	onFadedIn.Broadcast();
	RemoveFromViewport();
}

void ULevelTransitionDrawer::OnFadedOut()
{
	m_isFading = false;
	onFadedOut.Broadcast();
}
