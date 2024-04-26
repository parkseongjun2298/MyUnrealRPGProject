// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIBossController.h"
#include"NavigationSystem.h"
#include"Blueprint/AiBlueprintHelperLibrary.h"
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardData.h"
#include"BehaviorTree/BlackboardComponent.h"

AMyAIBossController::AMyAIBossController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_Boss.BT_Boss'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/AI/BB_Player.BB_Player'"));
	if (BB.Succeeded())
	{
		BlackboardData = BB.Object;
	}
}

void AMyAIBossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AMyAIController::RandomMove,2.f,true,2.f);
	if (UseBlackboard(BlackboardData, Blackboard))
		if (RunBehaviorTree(BehaviorTree))
		{
			//todo
		}
}

void AMyAIBossController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMyAIBossController::RandomMove()
{
	auto CurPawn = GetPawn();

	UNavigationSystemV1* Navsystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (Navsystem == nullptr)
		return;
	FNavLocation RandomLocation;
	if (Navsystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, RandomLocation))

	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
	}
}
