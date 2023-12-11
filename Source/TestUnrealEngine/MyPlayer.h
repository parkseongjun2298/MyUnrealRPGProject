// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

DECLARE_MULTICAST_DELEGATE(FOnSkillREnd);
UCLASS()
class TESTUNREALENGINE_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void AttackCheck();
	void ReadyFireTonado();
	void ReadySkill_E();
	FOnAttackEnd OnAttackEnd;
	FOnSkillREnd OnSkill_R_End;

	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnSkill_R_MontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnSkill_E_MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SetHitfalse();

	void Skill_R();
	void Skill_E();
	void EquipSword();
	void Shiled();
	void ShiledDown();
	bool Get_ShiledCheck() { return isShiled; }
	void Run();
	void RunFin();
	void Buff();

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsMontageCheck = false;
	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsSkill_R_MontageCheck = false;
	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsSkill_E_MontageCheck = false;



	UPROPERTY()
		class UPlayerAnimInstance* AnimInstance;

	UPROPERTY()
		int32 AttackIndex = 0;
	
public:

	UPROPERTY()
		float UpDownValue = 0;

	UPROPERTY()
		float LeftRightValue = 0;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UMyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HPBar;


	UPROPERTY()
		bool isHit = false;
	UPROPERTY()
		bool isEquipWeapon = false;
	UPROPERTY()
		bool isAttackMode = false;
	UPROPERTY()
		int32 iEquipCount = 0;
	UPROPERTY()
		bool isShiled = false;

	UPROPERTY()
		bool isOnBuff = false;

	UPROPERTY()
		float BuffTime = 0.f;


	UPROPERTY(EditAnywhere)
		class AFireTonado* FireTonado;
	UPROPERTY()
		bool isReadyFireTonado = false;

};
