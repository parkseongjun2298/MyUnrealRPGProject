// Fill out your copyright notice in the Description page of Project Settings.


#include "HitEffect.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
// Sets default values
AHitEffect::AHitEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	
	OurParticleSystem->bAutoActivate = true;
	OurParticleSystem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	OurParticleSystem->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit1.P_ky_hit1'"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

}

void AHitEffect::DestroyOBJ()
{
	Destroy();
}

// Called when the game starts or when spawned
void AHitEffect::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	float Delay = 0.5f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AHitEffect::DestroyOBJ, Delay);
}

// Called every frame
void AHitEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

