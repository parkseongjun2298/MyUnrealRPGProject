// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireTonado.generated.h"

UCLASS()
class TESTUNREALENGINE_API AFireTonado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireTonado();
	
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
};
