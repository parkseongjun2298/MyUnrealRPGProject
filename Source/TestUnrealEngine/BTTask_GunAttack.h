// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GunAttack.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTTask_GunAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_GunAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool bIsAttacking = false;
};
