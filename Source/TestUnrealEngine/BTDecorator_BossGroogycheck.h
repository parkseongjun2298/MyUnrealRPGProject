// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_BossGroogycheck.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTDecorator_BossGroogycheck : public UBTDecorator
{
	GENERATED_BODY()
		UBTDecorator_BossGroogycheck();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
