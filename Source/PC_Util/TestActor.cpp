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
	if (isPressed)return;
	auto isKey = 0 < UGameplayStatics::GetPlayerController(this, 0)->GetInputAnalogKeyState(FKey::FKey("Z"));
	if (isKey && !isLoading) {
		//m_latentActionInfo.UUID = m_latentActionInfo.UUID + 1;
		//m_latentActionInfo.CallbackTarget = this;
		//m_latentActionInfo.ExecutionFunction = FName("OnLoaded");
		//UGameplayStatics::LoadStreamLevel(this, FName("/Game/StarterContent/Maps/Minimal_Default"), true, false, m_latentActionInfo);
		//ALevelManager::GetInstance()->ChangeLevel(FName("/Game/StarterContent/Maps/StarterMap"), true);
		//auto levelStream = UGameplayStatics::GetStreamingLevel(GetWorld(), FName("/Game/StarterContent/Maps/Minimal_Default"));
		//auto i = 0;
		//if (levelStream) {
		//	i = 1;
		//}
		//else
		//{
		//	i = 2;
		//}
		//ALevelManager::GetInstance()->ChangeLevel(FName("Minimal_Default"), true);
		ALevelManager::GetInstance()->ChangeLevel(this, FName("/Game/StarterContent/Maps/Minimal_Default"), true);
		//UGameplayStatics::OpenLevel(this, FName("/Game/StarterContent/Maps/Minimal_Default"));
		//isLoading = true;
		//isPressed = true;
	}
	//if (!isLoaded)return;
	//isKey = 0 < UGameplayStatics::GetPlayerController(this, 0)->GetInputAnalogKeyState(FKey::FKey("X"));
	//if (isKey) {
	//	UGameplayStatics::OpenLevel(this, FName("/Game/StarterContent/Maps/Minimal_Default"));
	//	isPressed = true;
	//}
}

