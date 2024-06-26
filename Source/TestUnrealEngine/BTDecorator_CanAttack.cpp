// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include"MyAIController.h"
#include"MyAIGunController.h"
#include"MyPlayer.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"MyAIBossController.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const 
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurPawn == nullptr)
		return false;

	auto Target=Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;
	

	return bResult && Target->GetDistanceTo(CurPawn) <= 300.f;
}
