// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Kismet/GameplayStatics.h"
#include "Level/LevelManager.h"

// Sets default values
ATestActor::ATestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATestActor::OnLoaded()
{
	isLoaded = true;
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto pCon = UGameplayStatics::GetPlayerController(this, 0);
	if (pCon->IsInputKeyDown(EKeys::One)) {
		ULevelManager::GetInstance()->ChangeLevel(this, ELevelType::LevelA);
	}
	else if (pCon->IsInputKeyDown(EKeys::Two)) {
		ULevelManager::GetInstance()->ChangeLevel(this, ELevelType::LevelB);
	}
	else if (pCon->IsInputKeyDown(EKeys::Three)) {
		ULevelManager::GetInstance()->ChangeLevel(this, ELevelType::LevelC);
	}
}

