// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_GunCanAttack.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTDecorator_GunCanAttack : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_GunCanAttack();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
