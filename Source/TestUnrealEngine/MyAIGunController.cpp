// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIGunController.h"
#include"NavigationSystem.h"
#include"Blueprint/AiBlueprintHelperLibrary.h"
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardData.h"
#include"BehaviorTree/BlackboardComponent.h"
AMyAIGunController::AMyAIGunController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_GunPlayer.BT_GunPlayer'"));
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

void AMyAIGunController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AMyAIController::RandomMove,2.f,true,2.f);
	if (UseBlackboard(BlackboardData, Blackboard))
		if (RunBehaviorTree(BehaviorTree))
		{
			//todo
		}
}

void AMyAIGunController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMyAIGunController::RandomMove()
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
