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
			IsFalling = Character->GetMovementComponent()->IsFalling();
			IsDie = Character->IsDie;
			IsHit = Character->IsHit;
			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;
		}
	}
}

void UBossAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

FName UBossAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UBossAnimInstance::AnimNotify_AttackHit()
{
	OnAttackHit.Broadcast();
}
