// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTonado.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include"MyPlayer.h"
#include "GameFramework/PlayerController.h" // APlayerController ��� ������ ��Ŭ���
#include "Kismet/GameplayStatics.h" // UGameplayStatics ��� ������ ��Ŭ���
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "MyStatComponent.h"

// Sets default values
AFireTonado::AFireTonado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.


	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'"));
	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}

	Weapon->SetupAttachment(RootComponent);
	

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(Weapon);
	OurParticleSystem->bAutoActivate = true;
	OurParticleSystem->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_thunderStorm.P_ky_thunderStorm'"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	 PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // �÷��̾� ��Ʈ�ѷ��� ����
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn(); // �÷��̾� ĳ���͸� ����
		if (PlayerPawn)
		{
			ForwardVector = PlayerPawn->GetActorForwardVector();
			// ���� PlayerForwardVector�� ����Ͽ� �۾� ����
		}
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
	
	// Ư�� �ð� �Ŀ� ���͸� ����
	FTimerHandle TimerHandle;
	float Delay = 4.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFireTonado::DestroyOBJ, Delay);




	

}

// Called every frame
void AFireTonado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
	 CurrentLocation = GetActorLocation();
	
	float Speed = 200.0f; // �����̴� �ӵ� ����
	FVector NewLocation = CurrentLocation + (ForwardVector * Speed * DeltaTime);

	SetActorLocation(NewLocation);


	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 100.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);




	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("FireTonado Hit Actor : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;

		HitResult.Actor->TakeDamage(0.5f, DamageEvent, PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0) , this);


	}

}

