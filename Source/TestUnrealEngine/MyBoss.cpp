// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBoss.h"

#include "Components/CapsuleComponent.h"
#include "BossAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyStatComponent.h"
#include"Components/WidgetComponent.h"
#include "MyBossWidget.h"
#include"MyPlayer.h"
#include"CinematicTriggerVolume.h"
#include"MyAIBossController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include"BossRock.h"
#include"BossBlackHole.h"
#include"HitEffect.h"
#include"BossShower.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMyBoss::AMyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Meshes/Gideon.Gideon'"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(200.f, 0.f, 500.f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);//어디서든 보이는


	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/UI_BossHP.UI_BossHP_C'"));
	//블루프린트 링크는 _C를 해야함

	if (UW.Succeeded())
	{
		HUDWidgetClass = UW.Class;
	}


	AIControllerClass = AMyAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMaterialFinder(TEXT("Material'/Game/ParagonGideon/FX/Materials/VolumeFog/NewMaterial.NewMaterial'"));
	if (DecalMaterialFinder.Succeeded())
	{
		DecalMaterial = DecalMaterialFinder.Object;
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load decal material."));
	}

}

// Called when the game starts or when spawned
void AMyBoss::BeginPlay()
{
	Super::BeginPlay();


	HUDWidget = Cast<UMyBossWidget> (CreateWidget(GetWorld(), HUDWidgetClass));
	HUDWidget->AddToViewport();
	HUDWidget->BindHP(Stat);
	
}

void AMyBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyBoss::OnAttackMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyBoss::OnAttackMontageEnded2);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyBoss::OnAttackMontageEnded3);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyBoss::AttackCheck);
		AnimInstance->OnReadyMove.AddUObject(this, &AMyBoss::ReadyMove);
		AnimInstance->OnCreateMeteor.AddUObject(this, &AMyBoss::CreateMeteor);
		AnimInstance->OnCreateShower.AddUObject(this, &AMyBoss::CreateShower);
	}



	HPBar->InitWidget();

	//체력깍이는ui여기서

	auto HpWidget = Cast<UMyBossWidget>(HPBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHP(Stat);
	}
}

// Called every frame
void AMyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Stat->GetHp() <= 0.f)
	{
		Die();
	}
	if (Stat->GetHp() <= Stat->GetMaxHp()/2 && !isGrogyCheck)
	{
		IsGrogy = true;
		isGrogyCheck = true;
	}

	if (IsGrogy)
	{
		FTimerHandle TimerHandle;
		float Delay = 6.f; // 6초 후에 제거
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyBoss::SetHitfalse, Delay);
	}

	//UE_LOG(LogTemp, Log, TEXT("IsGrogy 값: %s"), IsGrogy ? TEXT("true") : TEXT("false"));

	
	 PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러를 얻음
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn(); // 플레이어 캐릭터를 얻음
		if (PlayerPawn)
		{
			TargetPos = PlayerPawn->GetActorLocation();
		}
	}


	

	FVector DirectVec = (TargetPos - this->GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = FRotationMatrix::MakeFromX(DirectVec).Rotator();

	this->SetActorRotation(NewRotation);

}

// Called to bind functionality to input
void AMyBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyBoss::Attack()
{
	if (IsAttacking || IsDie || IsHit || IsGrogy)
		return;

	AnimInstance->PlayAttackMontage();

	

	
	AttackIndex = (AttackIndex + 1) % 4;

	


	IsAttacking = true;
	IsMontageChek = true;

}

void AMyBoss::Attack2()
{
	if ( IsDie  || IsGrogy)
		return;

	AnimInstance->PlayAttackMontage2();




	AttackIndex2 = (AttackIndex2 + 1) % 2;




	IsAttacking = true;
	IsAttackMontageChek2 = true;

}

void AMyBoss::Attack3()
{
	if (IsDie )
		return;

	AnimInstance->PlayAttackMontage3();

	AttackIndex3 = 1;
	IsAttacking = true;
	IsAttackMontageChek3 = true;





}

void AMyBoss::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
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
						//플레이어가 방어할시 딜감하게 작업하기

						HitResult.Actor->TakeDamage(Stat->GetAttack() / 2, DamageEvent, GetController(), this);
						//hit effect

						auto HitEffect = GetWorld()->SpawnActor<AHitEffect>(HitResult.Actor->GetActorLocation(), FRotator::ZeroRotator);
						PlayerPawn->Stat->OnUseSkill(-10.f);
					}
					else
					{

						HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
						//hit effect

						auto HitEffect = GetWorld()->SpawnActor<AHitEffect>(HitResult.Actor->GetActorLocation(), FRotator::ZeroRotator);

					}



				}

			}

		}

	}
}

void AMyBoss::ReadyMove()
{
	auto Bullet = GetWorld()->SpawnActor<ABossRock>(GetActorLocation() + GetActorUpVector() * 500.f, FRotator::ZeroRotator);
	if (Bullet)
	{
		// 방향 벡터 전달
		FVector DirectionVector = GetActorForwardVector();
		FRotator Rot = GetActorRotation();
		Bullet->InitializeWithDirection(DirectionVector, Rot, GetController());

	}

}

void AMyBoss::CreateMeteor()
{
	auto BlackHole = GetWorld()->SpawnActor<ABossBlackHole>(GetActorLocation() + GetActorUpVector() , FRotator::ZeroRotator);
	if (BlackHole)
	{
		// 방향 벡터 전달
		FVector DirectionVector = GetActorForwardVector();
		FRotator Rot = GetActorRotation();
		BlackHole->InitializeWithDirection(DirectionVector, Rot, GetController());
	}
	auto BlackHole2 = GetWorld()->SpawnActor<ABossBlackHole>(GetActorLocation() + GetActorUpVector(), FRotator::ZeroRotator);
	if (BlackHole2)
	{
		// 방향 벡터 전달
		FVector DirectionVector = GetActorForwardVector();
		FRotator Rot = GetActorRotation();
		BlackHole2->InitializeWithDirection(DirectionVector + GetActorRightVector(), Rot, GetController());


	}
	auto BlackHole3 = GetWorld()->SpawnActor<ABossBlackHole>(GetActorLocation() + GetActorUpVector(), FRotator::ZeroRotator);
	if (BlackHole3)
	{
		// 방향 벡터 전달
		FVector DirectionVector = GetActorForwardVector();
		FRotator Rot = GetActorRotation();
		BlackHole3->InitializeWithDirection(DirectionVector - GetActorRightVector(), Rot, GetController());

	}
}

void AMyBoss::CreateShower()
{

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러를 얻음
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn(); // 플레이어 캐릭터를 얻음
		
		auto BlackHole = GetWorld()->SpawnActor<ABossShower>(PlayerPawn->GetActorLocation() + GetActorUpVector() * 500, FRotator::ZeroRotator);
		if (BlackHole)
		{
			// 방향 벡터 전달
			FVector DirectionVector = GetActorUpVector();
			FRotator Rot = GetActorRotation();
			BlackHole->InitializeWithDirection(DirectionVector, Rot, GetController());


			FVector DecalSize = FVector(128, 128, 128); // Adjust the size as needed
			 UGameplayStatics::SpawnDecalAtLocation(this, DecalMaterial, DecalSize, PlayerPawn->GetActorLocation(), FRotator(0.f, -90.f, 0.f), 4.f);
			
		}




	}

	
}

void AMyBoss::Die()
{
	
	IsDie = true;
	// 몬스터를 비활성화
	SetActorEnableCollision(false);
	//SetActorHiddenInGame(true);

	// 특정 시간 후에 몬스터를 제거
	FTimerHandle TimerHandle;
	float Delay = 3.0f; // 2초 후에 제거
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyBoss::DestroyMonster, Delay);
}

void AMyBoss::DestroyMonster()
{
	HPBar->DestroyComponent();
	HUDWidget->RemoveFromParent();
	Destroy();
}

void AMyBoss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsMontageChek = false;

	OnAttackEnd.Broadcast();//공격 전파 
}

void AMyBoss::OnAttackMontageEnded2(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsAttackMontageChek2 = false;

	OnAttackEnd2.Broadcast();//공격 전파 
}

void AMyBoss::OnAttackMontageEnded3(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsAttackMontageChek3 = false;

	OnAttackEnd3.Broadcast();//공격 전파 
}

float AMyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	IsHit = true;
	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}

void AMyBoss::SetHitfalse()
{
	IsGrogy = false;
}

void AMyBoss::SpawnDecal(USceneComponent* AttachToComponent,FVector Location, FRotator Rotation)
{
	if (DecalMaterial)
	{
		FVector DecalSize = FVector(50.0f, 50.0f, 50.0f); // Adjust the size as needed
		float LifeSpan = 2.0f; // The decal will disappear after 10 seconds

		UGameplayStatics::SpawnDecalAttached(
			DecalMaterial,        // The decal material
			DecalSize,                  // The size of the decal
			AttachToComponent,          // The component to attach to
			NAME_None,                  // Attach socket name (optional)
			Location,                   // Location relative to the component
			Rotation,                   // Rotation relative to the component
			EAttachLocation::KeepRelativeOffset, // How to interpret the location and rotation
			LifeSpan
		);
	}
	
}

