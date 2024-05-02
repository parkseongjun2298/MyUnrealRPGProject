// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_BossAttack2.h"
#include"MyAIBossController.h"
#include"MyBoss.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "GameFramework/PlayerController.h" // APlayerController 헤더 파일을 인클루드
#include "Kismet/GameplayStatics.h"

UBT_BossAttack2::UBT_BossAttack2()
{
	bNotifyTick = true;

}

EBTNodeResult::Type UBT_BossAttack2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	auto MyChar = Cast<AMyBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyChar == nullptr)
		return EBTNodeResult::Failed;



	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러를 얻음
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn(); // 플레이어 캐릭터를 얻음
		if (PlayerPawn)
		{
			TargetPos = PlayerPawn->GetActorLocation();
		}
	}

	FVector DirectVec = (TargetPos - MyChar->GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = FRotationMatrix::MakeFromX(DirectVec).Rotator();

	//MyChar->SetActorRotation(NewRotation);
	MyChar->SetActorLocation(TargetPos + FVector{500.f,0.f,500.f});
	MyChar->Attack2();
	bIsAttacking = true;

	//람다사용 알기

	MyChar->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});


	return Result;


}

void UBT_BossAttack2::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
