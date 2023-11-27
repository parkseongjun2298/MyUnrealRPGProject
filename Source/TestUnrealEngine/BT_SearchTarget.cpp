// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_SearchTarget.h"
#include"MyAIController.h"
#include"MyAIGunController.h"
#include"MyPlayer.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"DrawDebugHelpers.h"
UBT_SearchTarget::UBT_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBT_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurPawn == nullptr)
		return;
	UWorld* World = CurPawn->GetWorld();
	FVector Center = CurPawn->GetActorLocation();
	float SearchRadius = 500.f;

	if (World == nullptr)
		return;

	

	TArray <FOverlapResult>overlapresults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurPawn);

	bool bResult = World->OverlapMultiByChannel(
		overlapresults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams

	);

	if (bResult)
	{
		for (auto& OverlapResult : overlapresults)
		{
			AMyPlayer* MyChar = Cast<AMyPlayer>(OverlapResult.GetActor());
			if (MyChar && MyChar->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), MyChar);

				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f);
				return;

			}
		}
		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f);
	}



}
