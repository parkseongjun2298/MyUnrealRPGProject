// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToSection(int32 SectionIndex);

	FName GetAttackMontageName(int32 SectionIndex);

	void PlaySkill_R_Montage();
private:
	UFUNCTION()
		void AnimNotify_AttackHit();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsHit;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Skill_R_Montage;

public:
	FOnAttackHit OnAttackHit;
};
