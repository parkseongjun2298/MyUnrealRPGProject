// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BT_GunSearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBT_GunSearchTarget : public UBTService
{
	GENERATED_BODY()
		UBT_GunSearchTarget();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
