// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyStatComponent.h"
#include"Components/WidgetComponent.h"
#include"MyCharacterWidget.h"
#include"MyAIController.h"
#include"MyPlayer.h"
#include "GameFramework/PlayerController.h" // APlayerController 헤더 파일을 인클루드
#include "Kismet/GameplayStatics.h" // UGameplayStatics 헤더 파일을 인클루드

#include"HitEffect.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.f,0.f,200.f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);//어디서든 보이는


	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HPBar.WBP_HPBar_C'"));
	//블루프린트 링크는 _C를 해야함

	if (UW.Succeeded())
	{
		HPBar->SetWidgetClass(UW.Class);
		HPBar->SetDrawSize(FVector2D(200.f, 50.f));
	}


	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//FName WeaponSocket(TEXT("hand_l_socket"));

	//auto CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);

	//if (CurrentWeapon)
	//{
	//	//CurrentWeapon->AttachToComponent(GetMesh(),
	//	//	FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//	//	WeaponSocket);
	//}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
	}

	HPBar->InitWidget();

	//체력깍이는ui여기서

	auto HpWidget = Cast<UMyCharacterWidget>(HPBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHP(Stat);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Stat->GetHp() <= 0.f)
	{
		Die();
	}

	if (IsHit)
	{
		FTimerHandle TimerHandle;
		float Delay = 0.5f; // 2초 후에 제거
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyCharacter::SetHitfalse, Delay);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}


void AMyCharacter::Attack()
{

	if (IsAttacking || IsDie || IsHit)
		return;

	AnimInstance->PlayAttackMontage();

	AnimInstance->JumpToSection(AttackIndex);
	AttackIndex = (AttackIndex + 1) % 3;

	IsAttacking = true;
	IsMontageChek = true;
	//AttackCoolTime = true;
}

void AMyCharacter::AttackCheck()
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



void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsMontageChek = false;

	OnAttackEnd.Broadcast();//공격 전파 
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	IsHit = true;
	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}

void AMyCharacter::SetHitfalse()
{
	IsHit = false;
}


void AMyCharacter::Die()
{

	IsDie = true;
	// 몬스터를 비활성화
	SetActorEnableCollision(false);
	//SetActorHiddenInGame(true);

	// 특정 시간 후에 몬스터를 제거
	FTimerHandle TimerHandle;
	float Delay = 1.0f; // 2초 후에 제거
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyCharacter::DestroyMonster, Delay);
}

void AMyCharacter::DestroyMonster()
{
	// 액터를 제거합니다.
	Destroy();
}
