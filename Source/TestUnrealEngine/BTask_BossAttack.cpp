// Fill out your copyright notice in the Description page of Project Settings.


#include "BTask_BossAttack.h"
#include"MyAIBossController.h"
#include"MyBoss.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerController.h" // APlayerController ��� ������ ��Ŭ���
#include "Kismet/GameplayStatics.h"
UBTask_BossAttack::UBTask_BossAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTask_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto MyChar = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyChar == nullptr)
		return EBTNodeResult::Failed;

	if (MyChar->GetIsGrogy() == false)
	{

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // �÷��̾� ��Ʈ�ѷ��� ����
		if (PlayerController)
		{
			APawn* PlayerPawn = PlayerController->GetPawn(); // �÷��̾� ĳ���͸� ����
			if (PlayerPawn)
			{
				TargetPos = PlayerPawn->GetActorLocation();
			}
		}

		FVector DirectVec = (TargetPos - MyChar->GetActorLocation()).GetSafeNormal();
		FRotator NewRotation = FRotationMatrix::MakeFromX(DirectVec).Rotator();

		MyChar->SetActorRotation(NewRotation);
		MyChar->Attack();
		bIsAttacking = true;

		//���ٻ�� �˱�

		MyChar->OnAttackEnd.AddLambda([this]()
			{
				bIsAttacking = false;
			});

	}
		return Result;
	
}

void UBTask_BossAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
