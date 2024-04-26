// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_GunCanAttack.h"
#include"MyAIGunController.h"
#include"MyPlayer.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"MyAIBossController.h"

UBTDecorator_GunCanAttack::UBTDecorator_GunCanAttack()
{
	NodeName = TEXT("GunCanAttack");
}

bool UBTDecorator_GunCanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurPawn == nullptr)
		return false;

	auto Target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;


	return bResult && Target->GetDistanceTo(CurPawn) <= 700.f;
	//bResult 가 true이며 타켓과의 거리가 700미만이어야함
}
