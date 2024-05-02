// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBlackHole.h"
#include"MyBoss.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h" // APlayerController ��� ������ ��Ŭ���
#include "Kismet/GameplayStatics.h" // UGameplayStatics ��� ������ ��Ŭ���
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "MyStatComponent.h"
#include"MyPlayer.h"
#include"HitEffect.h"
// Sets default values
ABossBlackHole::ABossBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackHole"));
	RootComponent = Weapon;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGideon/FX/Meshes/Shapes/SM_Portal_Pointer.SM_Portal_Pointer'"));
	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}


	Weapon->SetupAttachment(RootComponent);
	Weapon->SetCollisionProfileName(TEXT("MonsterSkill"));
}

void ABossBlackHole::DestroyOBJ()
{
	Destroy();
}

// Called when the game starts or when spawned
void ABossBlackHole::BeginPlay()
{
	Super::BeginPlay();

	FVector NewScale = FVector(2.f, 2.f, 2.f);
	Weapon->SetWorldScale3D(NewScale);

	// Ư�� �ð� �Ŀ� ���͸� ����
	FTimerHandle TimerHandle;
	float Delay = 5.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABossBlackHole::DestroyOBJ, Delay);
	
}

// Called every frame
void ABossBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentLocation = GetActorLocation();

	Weapon->SetWorldRotation(FRot);

	float Speed = 1000.0f; // �����̴� �ӵ� ����
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



	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // �÷��̾� ��Ʈ�ѷ��� ����

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
				AMyPlayer* PlayerPawn = dynamic_cast<AMyPlayer*>(PlayerController->GetPawn()); // �÷��̾� ĳ���͸� ����
				if (PlayerPawn)
				{
					FDamageEvent DamageEvent;
					if (PlayerPawn->Get_ShiledCheck())
					{
						//UE_LOG(LogTemp, Log, TEXT("bullet Hit Actor : %s"), *HitResult.Actor->GetName());
						//�÷��̾ ����ҽ� �����ϰ� �۾��ϱ�


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

}

void ABossBlackHole::InitializeWithDirection(const FVector& DirectionVector, const FRotator& Rot, AController* Controler)
{
	ForwardVector = DirectionVector;
	FRot = Rot;
	Control = Controler;
}

