// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskBossAttack3.h"
#include"MyAIBossController.h"
#include"MyBoss.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerController.h" // APlayerController ��� ������ ��Ŭ���
#include "Kismet/GameplayStatics.h"
UBTTaskBossAttack3::UBTTaskBossAttack3()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskBossAttack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto MyChar = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyChar == nullptr)
		return EBTNodeResult::Failed;

	

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

		
		MyChar->Attack3();
		bIsAttacking = true;

		//���ٻ�� �˱�

		MyChar->OnAttackEnd.AddLambda([this]()
			{
				bIsAttacking = false;
			});

	
	return Result;
}

void UBTTaskBossAttack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
