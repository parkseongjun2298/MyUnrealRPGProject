// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_GunHitCheck.h"
#include"MyAIGunController.h"
#include"MyGunCharacter.h"


UBTDecorator_GunHitCheck::UBTDecorator_GunHitCheck()
{
	NodeName = TEXT("GunHitCheck");
}

bool UBTDecorator_GunHitCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto MyGunChar = Cast<AMyGunCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyGunChar == nullptr)
		return false;

	if (MyGunChar->GetIsHit() == false)
		return false;
	else
		return true;
}
