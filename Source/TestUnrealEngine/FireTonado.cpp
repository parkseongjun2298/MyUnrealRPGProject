// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTonado.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
// Sets default values
AFireTonado::AFireTonado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'"));
	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}

	Weapon->SetupAttachment(RootComponent);
	

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(RootComponent);
	OurParticleSystem->bAutoActivate = true;
	OurParticleSystem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireStorm.P_ky_fireStorm'"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

}


void AFireTonado::DestroyOBJ()
{
	Destroy();
}

// Called when the game starts or when spawned
void AFireTonado::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Create FireTonado"));
	
	// 특정 시간 후에 몬스터를 제거
	FTimerHandle TimerHandle;
	float Delay = 4.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFireTonado::DestroyOBJ, Delay);
}

// Called every frame
void AFireTonado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

