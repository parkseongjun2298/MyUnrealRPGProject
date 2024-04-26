// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAST_FindRandomPos.h"
#include "MyAIController.h"
#include"MyAIGunController.h"
#include"NavigationSystem.h"
#include"Blueprint/AiBlueprintHelperLibrary.h"
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardData.h"
#include"BehaviorTree/BlackboardComponent.h"

UBTTAST_FindRandomPos::UBTTAST_FindRandomPos()
{
	NodeName = TEXT("FindRandomPos");//언리얼bt에생성할 함수명
}

EBTNodeResult::Type UBTTAST_FindRandomPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto CurPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurPawn == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* Navsystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (Navsystem == nullptr)
		return  EBTNodeResult::Failed;
	FNavLocation RandomLocation;
	if (Navsystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, RandomLocation))

	{
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandomPos")),RandomLocation.Location); //블랙보드에서 만든이름넣기
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
