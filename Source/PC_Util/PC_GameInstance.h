// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Level/LevelManager.h"
#include "Level/LevelTransitionDrawer.h"
#include "PC_GameInstance.generated.h"

/**
 *
 */
UCLASS()
class PC_UTIL_API UPC_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPC_GameInstance();

	static UPC_GameInstance* GetInstance();

	virtual	void OnStart()override;
	virtual	void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)override;
	virtual void Init()override;
	virtual void StartGameInstance()override;

	class ULevelTransitionDrawer* GetLevelTransitionDrawer() { return m_levelTransitionDrawer; }
protected:
	static UPC_GameInstance* m_instance;
	void InitLevelDrawer();

	class ULevelManager* m_levelManager = nullptr;
	class ULevelTransitionDrawer* m_levelTransitionDrawer = nullptr;
	TSubclassOf<class ULevelTransitionDrawer>WidgetClass = nullptr;
};
