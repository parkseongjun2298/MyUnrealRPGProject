// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyBoss.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);//델리게이트쓰면 F 를 앞에붙여야함
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd2);//델리게이트쓰면 F 를 앞에붙여야함
UCLASS()
class TESTUNREALENGINE_API AMyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyBoss();

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
	void Attack2();
	void AttackCheck();
	void ReadyMove();
	void CreateMeteor();
	FOnAttackEnd OnAttackEnd;
	FOnAttackEnd2 OnAttackEnd2;
	void Die();
	void DestroyMonster();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnAttackMontageEnded2(UAnimMontage* Montage, bool bInterrupted);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SetHitfalse();

	void SetBulletColCheck() { isBulletColCheck = true; }
	bool GetIsHit() { return IsHit; }
	bool GetIsGrogy() { return IsGrogy; }

private:
	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsAttacking = false;

	UPROPERTY()
		bool AttackCoolTime = false;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsMontageChek = false;
	UPROPERTY(VisibleAnywhere, Category = Pawn)
		bool IsAttackMontageChek2 = false;

	UPROPERTY()
		class UBossAnimInstance* AnimInstance;

	UPROPERTY()
		int32 AttackIndex = 0;
	UPROPERTY()
		int32 AttackIndex2 = 0;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class UMyBossWidget> HUDWidgetClass;

	UPROPERTY()
		bool IsDie = false;
	UPROPERTY()
		bool IsHit = false;
	UPROPERTY()
		bool IsGrogy = false;
	UPROPERTY()
		bool isBulletColCheck = false;

	APlayerController* PlayerController;




private:

	UPROPERTY(VisibleAnywhere)
		class UMyBossWidget* HUDWidget;

	FVector TargetPos;

};
