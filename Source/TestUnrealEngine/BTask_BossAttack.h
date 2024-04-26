// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTask_BossAttack.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTask_BossAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTask_BossAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool bIsAttacking = false;
	FVector TargetPos;
};
