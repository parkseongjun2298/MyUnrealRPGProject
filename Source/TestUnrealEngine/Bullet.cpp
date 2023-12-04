 // Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include"MyGunCharacter.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h" // APlayerController 헤더 파일을 인클루드
#include "Kismet/GameplayStatics.h" // UGameplayStatics 헤더 파일을 인클루드
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "MyStatComponent.h"
#include"MyPlayer.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	RootComponent = Weapon;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonRevenant/FX/Meshes/Shapes/SM_Burden_Projectile.SM_Burden_Projectile'"));
	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}

	
	Weapon->SetupAttachment(RootComponent);
	//Weapon->SetCollisionProfileName(TEXT("Attack"));
	


}

void ABullet::DestroyOBJ()
{
	Destroy();
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	FVector NewScale = FVector(0.5f, 0.5f, 0.5f);
	Weapon->SetWorldScale3D(NewScale);

	// 특정 시간 후에 몬스터를 제거
	FTimerHandle TimerHandle;
	float Delay = 2.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABullet::DestroyOBJ, Delay);
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();

	Weapon->SetWorldRotation(FRot);

	float Speed = 800.0f; // 움직이는 속도 조절
	FVector NewLocation = CurrentLocation + (ForwardVector * Speed * DeltaTime);

	SetActorLocation(NewLocation);


	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 30.f;
	float AttackRadius = 30.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);




	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러를 얻음


	if (bResult && HitResult.Actor.IsValid())
	{

		if (PlayerController)
		{
			AMyPlayer* PlayerPawn = dynamic_cast<AMyPlayer*>(PlayerController->GetPawn()); // 플레이어 캐릭터를 얻음
			if (PlayerPawn)
			{
				FDamageEvent DamageEvent;
				if (PlayerPawn->Get_ShiledCheck())
				{
					//UE_LOG(LogTemp, Log, TEXT("bullet Hit Actor : %s"), *HitResult.Actor->GetName());
					//플레이어가 방어할시 딜감하게 작업하기


					HitResult.Actor->TakeDamage(10 / 2, DamageEvent, Control, this);

					DestroyOBJ();
				}



				else
				{
					//UE_LOG(LogTemp, Log, TEXT("bullet Hit Actor : %s"), *HitResult.Actor->GetName());

					HitResult.Actor->TakeDamage(10, DamageEvent, Control, this);

					DestroyOBJ();
				}



			}

		}


	}
}

void ABullet::InitializeWithDirection(const FVector& DirectionVector,const FRotator& Rot,AController* Controler)
{
	ForwardVector = DirectionVector;
	FRot = Rot; 
	Control = Controler;
	
}

