// Fill out your copyright notice in the Description page of Project Settings.


#include "NextStageVolume.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
// Sets default values
ANextStageVolume::ANextStageVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransferVolume"));
	RootComponent = TransferVolume;

	TransferVolume->SetCollisionProfileName(TEXT("MyCollectible"));

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(RootComponent);
	OurParticleSystem->bAutoActivate = true;
	OurParticleSystem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_magicCircle1.P_ky_magicCircle1'"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}


}

// Called when the game starts or when spawned
void ANextStageVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANextStageVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANextStageVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn != nullptr) 
	{ UGameplayStatics::OpenLevel(this, TEXT("SecondMap")); }




}

