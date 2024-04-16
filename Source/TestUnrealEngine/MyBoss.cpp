// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBoss.h"

#include "Components/CapsuleComponent.h"
#include "MyGunCharacterAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyStatComponent.h"
#include"Components/WidgetComponent.h"
#include"MyCharacterWidget.h"

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
	HPBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);//��𼭵� ���̴�


	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HPBar.WBP_HPBar_C'"));
	//�������Ʈ ��ũ�� _C�� �ؾ���

	if (UW.Succeeded())
	{
		HPBar->SetWidgetClass(UW.Class);
		HPBar->SetDrawSize(FVector2D(200.f, 50.f));
	}


	//AIControllerClass = AMyAIGunController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AMyBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMyGunCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		
	}

	HPBar->InitWidget();

	//ü�±��̴�ui���⼭

	auto HpWidget = Cast<UMyCharacterWidget>(HPBar->GetUserWidgetObject());
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
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyGunCharacter::SetHitfalse, Delay);
	}

}

// Called to bind functionality to input
void AMyBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyBoss::Attack()
{
}

void AMyBoss::AttackCheck()
{
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
	Destroy();
}

void AMyBoss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsMontageChek = false;

	//OnAttackEnd.Broadcast();//���� ���� 
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

