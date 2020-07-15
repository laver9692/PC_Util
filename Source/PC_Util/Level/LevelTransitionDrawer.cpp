// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionDrawer.h"

void ULevelTransitionDrawer::NativeConstruct() {
	levelManager = ALevelManager::GetInstance();
}