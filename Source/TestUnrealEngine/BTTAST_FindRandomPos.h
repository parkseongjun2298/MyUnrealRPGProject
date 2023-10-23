// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTAST_FindRandomPos.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTTAST_FindRandomPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTAST_FindRandomPos();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
