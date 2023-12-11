// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DistanceCheck.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTDecorator_DistanceCheck : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_DistanceCheck();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
