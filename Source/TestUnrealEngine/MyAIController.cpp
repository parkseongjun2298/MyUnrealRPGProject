// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include"NavigationSystem.h"
#include"Blueprint/AiBlueprintHelperLibrary.h"
AMyAIController::AMyAIController()
{
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&AMyAIController::RandomMove,2.f,true,2.f);
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AMyAIController::RandomMove()
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
