// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyGunCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);//델리게이트쓰면 F 를 앞에붙여야함
UCLASS()
class TESTUNREALENGINE_API AMyGunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyGunCharacter();

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
	FOnAttackEnd OnAttackEnd;

	void Die();
	void DestroyMonster();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SetHitfalse();

	void SetBulletColCheck() { isBulletColCheck = true; }
	
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsAttacking = false;

	UPROPERTY()
		bool AttackCoolTime = false;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsMontageChek = false;

	UPROPERTY()
		class UMyGunCharacterAnimInstance* AnimInstance;

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
		bool IsDie = false;
	UPROPERTY()
		bool IsHit = false;
	UPROPERTY()
		bool isBulletColCheck = false;

	APlayerController* PlayerController;

};
