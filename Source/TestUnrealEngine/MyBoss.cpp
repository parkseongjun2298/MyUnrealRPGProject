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
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);//��𼭵� ���̴�


	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/UI_BossHP.UI_BossHP_C'"));
	//�������Ʈ ��ũ�� _C�� �ؾ���

	if (UW.Succeeded())
	{
		HUDWidgetClass = UW.Class;
	}


	AIControllerClass = AMyAIBossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
		AnimInstance->OnAttackHit.AddUObject(this, &AMyBoss::AttackCheck);
		AnimInstance->OnReadyMove.AddUObject(this, &AMyBoss::ReadyMove);
		AnimInstance->OnCreateMeteor.AddUObject(this, &AMyBoss::CreateMeteor);
	}



	HPBar->InitWidget();

	//ü�±��̴�ui���⼭

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

	if (IsHit)
	{
		FTimerHandle TimerHandle;
		float Delay = 0.2f; // 2�� �Ŀ� ����
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyBoss::SetHitfalse, Delay);
	}


	
	 PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // �÷��̾� ��Ʈ�ѷ��� ����
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn(); // �÷��̾� ĳ���͸� ����
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
	if (IsAttacking || IsDie || IsHit)
		return;

	AnimInstance->PlayAttackMontage();

	

	
	AttackIndex = (AttackIndex + 1) % 4;

	


	IsAttacking = true;
	IsMontageChek = true;

}

void AMyBoss::Attack2()
{
	if ( IsDie || IsHit)
		return;

	AnimInstance->PlayAttackMontage2();




	AttackIndex2 = (AttackIndex2 + 1) % 2;




	IsAttacking = true;
	IsAttackMontageChek2 = true;

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







	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // �÷��̾� ��Ʈ�ѷ��� ����


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
				AMyPlayer* PlayerPawn = dynamic_cast<AMyPlayer*>(PlayerController->GetPawn()); // �÷��̾� ĳ���͸� ����
				if (PlayerPawn)
				{
					FDamageEvent DamageEvent;
					if (PlayerPawn->Get_ShiledCheck())
					{
						//�÷��̾ ����ҽ� �����ϰ� �۾��ϱ�

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

void AMyBoss::ReadyMove()
{
	auto Bullet = GetWorld()->SpawnActor<ABossRock>(GetActorLocation() + GetActorUpVector() * 500.f, FRotator::ZeroRotator);
	if (Bullet)
	{
		// ���� ���� ����
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
		// ���� ���� ����
		FVector DirectionVector = GetActorForwardVector();
		FRotator Rot = GetActorRotation();
		BlackHole->InitializeWithDirection(DirectionVector, Rot, GetController());
	}
	auto BlackHole2 = GetWorld()->SpawnActor<ABossBlackHole>(GetActorLocation() + GetActorUpVector(), FRotator::ZeroRotator);
	if (BlackHole2)
	{
		// ���� ���� ����
		FVector DirectionVector = GetActorForwardVector();
		FRotator Rot = GetActorRotation();
		BlackHole2->InitializeWithDirection(DirectionVector + GetActorRightVector(), Rot, GetController());


	}
	auto BlackHole3 = GetWorld()->SpawnActor<ABossBlackHole>(GetActorLocation() + GetActorUpVector(), FRotator::ZeroRotator);
	if (BlackHole3)
	{
		// ���� ���� ����
		FVector DirectionVector = GetActorForwardVector();
		FRotator Rot = GetActorRotation();
		BlackHole3->InitializeWithDirection(DirectionVector - GetActorRightVector(), Rot, GetController());

	}
}

void AMyBoss::Die()
{
	
	IsDie = true;
	// ���͸� ��Ȱ��ȭ
	SetActorEnableCollision(false);
	//SetActorHiddenInGame(true);

	// Ư�� �ð� �Ŀ� ���͸� ����
	FTimerHandle TimerHandle;
	float Delay = 3.0f; // 2�� �Ŀ� ����
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

	OnAttackEnd.Broadcast();//���� ���� 
}

void AMyBoss::OnAttackMontageEnded2(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsAttackMontageChek2 = false;

	OnAttackEnd2.Broadcast();//���� ���� 
}

float AMyBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	IsHit = true;
	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}

void AMyBoss::SetHitfalse()
{
	IsHit = false;
}

