// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BT_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBT_SearchTarget : public UBTService
{
	GENERATED_BODY()
public:

		UBT_SearchTarget();
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
