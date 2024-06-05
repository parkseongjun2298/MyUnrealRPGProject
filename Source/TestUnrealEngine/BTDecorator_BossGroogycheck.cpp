// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_BossGroogycheck.h"
#include"MyAIBossController.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"MyBoss.h"
#include "MyStatComponent.h"
UBTDecorator_BossGroogycheck::UBTDecorator_BossGroogycheck()
{
	NodeName = TEXT("BossGroogyCheck");
}

bool UBTDecorator_BossGroogycheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMyBoss* CurPawn = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurPawn == nullptr)
		return false;

	bool Groogy = CurPawn->GetIsGrogy();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName(TEXT("BossGrogy")), Groogy);



	return bResult && Groogy;
}
