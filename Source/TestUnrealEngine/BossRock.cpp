// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRock.h"

#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h" // APlayerController 헤더 파일을 인클루드
#include "Kismet/GameplayStatics.h" // UGameplayStatics 헤더 파일을 인클루드
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "MyStatComponent.h"
#include"MyPlayer.h"
#include"HitEffect.h"

// Sets default values
ABossRock::ABossRock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	


	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rock"));
	RootComponent = Weapon;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGideon/FX/Meshes/Shapes/SM_Sphere_FlippedNormals.SM_Sphere_FlippedNormals'"));
	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}


	Weapon->SetupAttachment(RootComponent);
	Weapon->SetCollisionProfileName(TEXT("MonsterSkill"));
}

void ABossRock::DestroyOBJ()
{
	
	Destroy();
}

// Called when the game starts or when spawned
void ABossRock::BeginPlay()
{
	Super::BeginPlay();
	

	FVector NewScale = FVector(1.f, 1.f, 1.f);
	Weapon->SetWorldScale3D(NewScale);

	// 특정 시간 후에 몬스터를 제거
	FTimerHandle TimerHandle;
	float Delay = 5.0f;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABossRock::DestroyOBJ, Delay);

	
}

// Called every frame
void ABossRock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	



	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 50.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
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


	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러를 얻음

	if (HitResult.Actor.IsValid() && HitResult.Actor->GetName() != "BP_MyPlayer_C_0")
	{
		//UE_LOG(LogTemp, Log, TEXT("bullet Hit Actor : %s"), *HitResult.Actor->GetName());
		//DestroyOBJ();
		return;
	}

	else
	{
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
						//hit effect

						auto HitEffect = GetWorld()->SpawnActor<AHitEffect>(HitResult.Actor->GetActorLocation(), FRotator::ZeroRotator);

						DestroyOBJ();
					}



				}

			}


		}

	}


	CurrentLocation = GetActorLocation();

	Weapon->SetWorldRotation(FRot);
	AMyPlayer* PlayerPawn = dynamic_cast<AMyPlayer*>(PlayerController->GetPawn()); // 플레이어 캐릭터를 얻음
	float Speed = FVector::Distance(PlayerPawn->GetActorLocation(), CurrentLocation); // 움직이는 속도 조절
	FVector NewLocation = CurrentLocation + (ForwardVector * Speed * DeltaTime) + (FVector(0, 0, -Gravity / 10) * Time * Time * 0.5f);
	SetActorLocation(NewLocation);
	Time += DeltaTime;

}

void ABossRock::InitializeWithDirection(const FVector& DirectionVector, const FRotator& Rot, AController* Controler)
{
	ForwardVector = DirectionVector;
	FRot = Rot;
	Control = Controler;
}

