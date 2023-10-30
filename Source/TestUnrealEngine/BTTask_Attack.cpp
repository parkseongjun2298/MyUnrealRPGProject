// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include"MyAIController.h"
#include"MyCharacter.h"
#include"BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto MyChar=Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyChar == nullptr)
		return EBTNodeResult::Failed;
	MyChar->Attack();
	bIsAttacking = true;

	//람다사용 알기

	MyChar->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});


	return Result;


}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
