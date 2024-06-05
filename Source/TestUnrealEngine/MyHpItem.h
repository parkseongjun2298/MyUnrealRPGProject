// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyHpItem.generated.h"

UCLASS()
class TESTUNREALENGINE_API AMyHpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyHpItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Item;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* BoxTrigger;

	
	APlayerController* PlayerController;
private:
	UFUNCTION()
		void OnCharOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
