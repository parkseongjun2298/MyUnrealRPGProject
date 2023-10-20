// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIController();

	virtual void OnPossess(APawn* InPawn) override; //ºùÀÇÇß´Ù
	virtual void OnUnPossess() override; //ºùÀÇÇ®¸±‹š
private:
	void RandomMove();

private:
	FTimerHandle TimerHandle;
};
