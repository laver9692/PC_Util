// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Input/Reply.h"
#include "ListDrawer.generated.h"
/**
 *
 */
UCLASS()
class PC_UTIL_API UListDrawer : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void CreateTest();
protected:
	UWidget* tmpWidget = nullptr;
};
