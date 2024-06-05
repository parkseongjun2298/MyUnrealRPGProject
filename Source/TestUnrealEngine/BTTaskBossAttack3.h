// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskBossAttack3.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTTaskBossAttack3 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskBossAttack3();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool bIsAttacking = false;
	FVector TargetPos;

};
