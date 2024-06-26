// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwordEffect.generated.h"

UCLASS()
class TESTUNREALENGINE_API ASwordEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordEffect();

	void DestroyOBJ();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Weapon;
	class UParticleSystemComponent* OurParticleSystem;


	UPROPERTY()
		bool IsDie = false;


public:
	FVector CurrentLocation;
	FVector ForwardVector;
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere)
		class UMyStatComponent* Stat;
};
