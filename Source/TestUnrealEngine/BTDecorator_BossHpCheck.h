// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_BossHpCheck.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTDecorator_BossHpCheck : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_BossHpCheck();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
