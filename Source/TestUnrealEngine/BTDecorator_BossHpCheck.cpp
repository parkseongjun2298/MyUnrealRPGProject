// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossHpCheck.h"
#include"MyAIBossController.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"MyBoss.h"
#include "MyStatComponent.h"
UBTDecorator_BossHpCheck::UBTDecorator_BossHpCheck()
{
	NodeName = TEXT("BossHpCheck");
}

bool UBTDecorator_BossHpCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMyBoss* CurPawn = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurPawn == nullptr)
		return false;
	
	int32 Groogy = CurPawn->Stat->GetHp();
	 OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("BossHp")), Groogy);
	
	
	
	 return bResult && Groogy <= 50;
}
