 // Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include"MyGunCharacter.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h" // APlayerController ��� ������ ��Ŭ���
#include "Kismet/GameplayStatics.h" // UGameplayStatics ��� ������ ��Ŭ���
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

	// Ư�� �ð� �Ŀ� ���͸� ����
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

	float Speed = 800.0f; // �����̴� �ӵ� ����
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




	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // �÷��̾� ��Ʈ�ѷ��� ����


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

