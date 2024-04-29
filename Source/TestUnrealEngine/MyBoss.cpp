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
		AnimInstance->OnAttackHit.AddUObject(this, &AMyBoss::AttackCheck);
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

	if (AttackIndex % 4 == 1)
	{
		auto Bullet = GetWorld()->SpawnActor<ABossRock>(GetActorLocation() + GetActorUpVector() * 200.f, FRotator::ZeroRotator);
		if (Bullet)
		{
			// ���� ���� ����
			FVector DirectionVector = GetActorForwardVector();
			FRotator Rot = GetActorRotation();
			Bullet->InitializeWithDirection(DirectionVector, Rot, GetController());

		}

		auto Bullet2 = GetWorld()->SpawnActor<ABossRock>(GetActorLocation() + GetActorRightVector() * -200.f, FRotator::ZeroRotator);
		if (Bullet2)
		{
			// ���� ���� ����
			FVector DirectionVector = GetActorForwardVector();
			FRotator Rot = GetActorRotation();
			Bullet2->InitializeWithDirection(DirectionVector, Rot, GetController());

		}
		auto Bullet3 = GetWorld()->SpawnActor<ABossRock>(GetActorLocation() + GetActorRightVector() * 200.f, FRotator::ZeroRotator);
		if (Bullet3)
		{
			// ���� ���� ����
			FVector DirectionVector = GetActorForwardVector();
			FRotator Rot = GetActorRotation();
			Bullet3->InitializeWithDirection(DirectionVector, Rot, GetController());

		}

		//AttackIndex 2 3 0 �϶� �ϳ��� ������ �������ֱ� �ؾ���!

	}


	IsAttacking = true;
	IsMontageChek = true;

}

void AMyBoss::AttackCheck()
{
	UE_LOG(LogTemp, Log, TEXT("ColCheck :"));
}

void AMyBoss::Die()
{
	IsDie = true;
	// ���͸� ��Ȱ��ȭ
	SetActorEnableCollision(false);
	//SetActorHiddenInGame(true);

	// Ư�� �ð� �Ŀ� ���͸� ����
	FTimerHandle TimerHandle;
	float Delay = 1.0f; // 2�� �Ŀ� ����
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

