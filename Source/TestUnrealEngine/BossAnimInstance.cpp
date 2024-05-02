// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyBoss.h"

UBossAnimInstance::UBossAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Boss_Montage.Boss_Montage'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM2(TEXT("AnimMontage'/Game/Animations/Boss_Skill1.Boss_Skill1'"));
	if (AM2.Succeeded())
	{
		AttackMontage2 = AM2.Object;
	}

}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<AMyBoss>(Pawn);
		if (Character)
		{
			
			IsDie = Character->IsDie;
			IsHit = Character->IsHit;
			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;
			IsGrogy = Character->IsGrogy;
		}
	}
}

void UBossAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UBossAnimInstance::PlayAttackMontage2()
{
	Montage_Play(AttackMontage2, 1.f);
}

FName UBossAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UBossAnimInstance::AnimNotify_AttackHit()
{
	OnAttackHit.Broadcast();
}
void UBossAnimInstance::AnimNotify_ReadyMove()
{
	OnReadyMove.Broadcast();
}

void UBossAnimInstance::AnimNotify_CreateMeteor()
{
	OnCreateMeteor.Broadcast();
}
