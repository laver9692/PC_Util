// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class PC_UTIL_API ATestActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestActor();
	UPROPERTY(EditAnywhere)
		FName LevelToLoad;
	bool isPressed = false;
	bool isLoading = false;
	bool isLoaded = false;
	FLatentActionInfo m_latentActionInfo;
	UFUNCTION()
		void OnLoaded();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
