// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIGunController.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API AMyAIGunController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIGunController();

	virtual void OnPossess(APawn* InPawn) override; //�����ߴ�
	virtual void OnUnPossess() override; //����Ǯ����
private:
	void RandomMove();

private:
	FTimerHandle TimerHandle;


	UPROPERTY()
		class UBehaviorTree* BehaviorTree;
	UPROPERTY()
		class UBlackboardData* BlackboardData;
};
