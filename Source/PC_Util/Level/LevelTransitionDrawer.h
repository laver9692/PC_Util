// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelManager.h"
#include "LevelTransitionDrawer.generated.h"

/**
 *
 */
UCLASS()
class PC_UTIL_API ULevelTransitionDrawer : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
protected:
	ALevelManager* levelManager = nullptr;
};
