// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpChanged);
DECLARE_MULTICAST_DELEGATE(FOnMpChanged);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTUNREALENGINE_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetLevel(int32 Level);
	void OnAttacked(float DamageAmount);
	void OnUseSkill(float ManaAmount);
	void SetHp(int32 NewHp);
	void SetMp(int32 NewMp);
	int32 GetLevel() { return Level; }
	int32 GetHp() { return Hp; }
	int32 GetMaxHp() { return MaxHp; }
	float GetRatioHp() { return Hp / (float)MaxHp; }

	
	int32 GetMp() { return Mp; }
	int32 GetMaxMp() { return MaxMp; }
	float GetRatioMp() { return Mp / (float)MaxMp; }

	int32 GetAttack() { return Attack; }
	int32 GetSpeed() { return Speed; }
	void SetSpeed(int32 NewSpeed);
private:
	UPROPERTY(EditAnywhere, Category=Stat, Meta=(AllowPrivateAccess=true))
	int32 Level;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Hp;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 MaxHp;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Mp;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 MaxMp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Speed;

public:
	FOnHpChanged OnHpChanged;
	FOnMpChanged OnMpChanged;
};
