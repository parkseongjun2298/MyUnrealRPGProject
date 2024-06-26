// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordEffect.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include"MyPlayer.h"
#include "GameFramework/PlayerController.h" // APlayerController 헤더 파일을 인클루드
#include "Kismet/GameplayStatics.h" // UGameplayStatics 헤더 파일을 인클루드
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "MyStatComponent.h"
// Sets default values
ASwordEffect::ASwordEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/InfinityBladeFireLands/Environments/Fire/Fire_Fortress/SM/Fire_Trough_2b.Fire_Trough_2b'"));
	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}
	RootComponent = Weapon;

	Weapon->SetupAttachment(RootComponent);

	Weapon->SetCollisionProfileName(TEXT("PlayerSkill"));

	

	

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러를 얻음
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn(); // 플레이어 캐릭터를 얻음
		if (PlayerPawn)
		{
			ForwardVector = PlayerPawn->GetActorForwardVector();
			// 이제 PlayerForwardVector를 사용하여 작업 수행
		}
	}


}

void ASwordEffect::DestroyOBJ()
{
	Destroy();
}

// Called when the game starts or when spawned
void ASwordEffect::BeginPlay()
{
	Super::BeginPlay();
	

	// 특정 시간 후에 몬스터를 제거
	FTimerHandle TimerHandle;
	float Delay = 2.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASwordEffect::DestroyOBJ, Delay);
}

// Called every frame
void ASwordEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();

	float Speed = 2000.0f; // 움직이는 속도 조절
	FVector NewLocation = CurrentLocation + (ForwardVector * Speed * DeltaTime);

	SetActorLocation(NewLocation);


	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 50.f;
	float AttackRadius = 50.f;

	



	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		Rotation, DrawColor, false, 2.f);


	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("SwordEffect Hit Actor : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;

		HitResult.Actor->TakeDamage(10.f, DamageEvent, PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0), this);

		DestroyOBJ();
	}


}

