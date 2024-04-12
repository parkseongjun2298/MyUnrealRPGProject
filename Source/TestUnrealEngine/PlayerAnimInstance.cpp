// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyPlayer.h"
UPlayerAnimInstance::UPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Player_Montage.Player_Montage'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> R_M(TEXT("AnimMontage'/Game/Animations/Skill_R_Montage.Skill_R_Montage'"));
	if (R_M.Succeeded())
	{
		Skill_R_Montage = R_M.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> E_M(TEXT("AnimMontage'/Game/Animations/Skill_E_Montage.Skill_E_Montage'"));
	if (E_M.Succeeded())
	{
		Skill_E_Montage = E_M.Object;
	}


	static ConstructorHelpers::FObjectFinder<UAnimMontage> Q_M(TEXT("AnimMontage'/Game/Animations/Skill_Q_Montage.Skill_Q_Montage'"));
	if (Q_M.Succeeded())
	{
		Skill_Q_Montage = Q_M.Object;
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		//플레이어속도
		Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<AMyPlayer>(Pawn);
		if (Character)
		{
			//IsAttackMode = Character->isAttackMode;
			IsEquip = Character->isEquipWeapon;
			IsFalling = Character->GetMovementComponent()->IsFalling();
			IsHit = Character->isHit;
			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;
			IsShiled = Character->isShiled;
		}
	}
}

void UPlayerAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UPlayerAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UPlayerAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Att%d"), SectionIndex));
}

void UPlayerAnimInstance::PlaySkill_R_Montage()
{
	Montage_Play(Skill_R_Montage, 1.f);
}

void UPlayerAnimInstance::PlaySkill_E_Montage()
{
	Montage_Play(Skill_E_Montage, 1.f);
}



void UPlayerAnimInstance::AnimNotify_AttackHit()
{
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
	OnAttackHit.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_ReadyFireTonado()
{
	OnReadyFireTonado.Broadcast();
}

void UPlayerAnimInstance::AnimNotify_ReadySkillE()
{
	OnReadySkillE.Broadcast();
}


void UPlayerAnimInstance::PlaySkill_Q_Montage()
{
	Montage_Play(Skill_Q_Montage, 1.f);
}
void UPlayerAnimInstance::AnimNotify_ReadySkillQ()
{
	OnReadySkillQ.Broadcast();
}