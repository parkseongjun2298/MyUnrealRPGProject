// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIBossController.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API AMyAIBossController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIBossController();

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
