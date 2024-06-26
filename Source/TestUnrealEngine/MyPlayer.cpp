// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerAniminstance.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyStatComponent.h"
#include"Components/WidgetComponent.h"
#include"MyCharacterWidget.h"
#include"MyAIController.h"
#include"HUDWidget.h"
#include"MiniMap.h"
#include"MySkillWidget.h"

#include"HitEffect.h"
#include"FireTonado.h"
#include"SwordEffect.h"
// Sets default values
AMyPlayer::AMyPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 1000.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));


	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC'"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);//어디서든 보이는

	MPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("MPBAR"));
	MPBar->SetupAttachment(GetMesh());
	MPBar->SetRelativeLocation(FVector(0.f, 0.f, 360.f));
	MPBar->SetWidgetSpace(EWidgetSpace::Screen);//어디서든 보이는
	
	


	static ConstructorHelpers::FClassFinder<UHUDWidget> UW2(TEXT("WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	//블루프린트 링크는 _C를 해야함

	if (UW2.Succeeded())
	{
		HUDWidgetClass = UW2.Class;
	}

	static ConstructorHelpers::FClassFinder<UMiniMap> UW3(TEXT("WidgetBlueprint'/Game/UI/UI_MiniMap.UI_MiniMap_C'"));
	//블루프린트 링크는 _C를 해야함

	if (UW3.Succeeded())
	{
		HUDMiniMapClass = UW3.Class;
	}

	static ConstructorHelpers::FClassFinder<UMySkillWidget> UW4(TEXT("WidgetBlueprint'/Game/UI/UI_Skill.UI_Skill_C'"));
	//블루프린트 링크는 _C를 해야함

	if (UW4.Succeeded())
	{
		HUDSkillClass = UW4.Class;
	}
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	
	HUDWidget = Cast<UHUDWidget> (CreateWidget(GetWorld(), HUDWidgetClass));
	HUDWidget->AddToViewport();
	//FName WeaponSocket(TEXT("hand_l_socket"));

	//auto CurrentWeapon = GetWorld()->SpawnActor<AMyWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);

	//if (CurrentWeapon)
	//{
	//	//CurrentWeapon->AttachToComponent(GetMesh(),
	//	//	FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//	//	WeaponSocket);
	//}

	HUDWidget->BindCharacterStat(Stat);
	HUDWidget->BindCharacterStatMP(Stat);

	MiniMapWidget = Cast<UMiniMap>(CreateWidget(GetWorld(), HUDMiniMapClass));
	MiniMapWidget->AddToViewport();

	SkillWidget = Cast<UMySkillWidget>(CreateWidget(GetWorld(), HUDSkillClass));
	SkillWidget->AddToViewport();


}

void AMyPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{

		//애니몽타주에서 노티파이 이벤트연결시킬떄
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyPlayer::AttackCheck);
		AnimInstance->OnReadyFireTonado.AddUObject(this, &AMyPlayer::ReadyFireTonado);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnSkill_R_MontageEnded);
		AnimInstance->OnReadySkillE.AddUObject(this, &AMyPlayer::ReadySkill_E);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnSkill_E_MontageEnded);
		//AnimInstance->OnReadySkillQ.AddUObject(this, &AMyPlayer::ReadySkill_Q);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnSkill_Q_MontageEnded);
	}

	HPBar->InitWidget();

	//체력깍이는ui여기서

	auto HpWidget = Cast<UHUDWidget>(HPBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindCharacterStat(Stat);
	}

	MPBar->InitWidget();
	auto MpWidget = Cast<UHUDWidget>(MPBar->GetUserWidgetObject());
	if (MpWidget)
	{
		MpWidget->BindCharacterStatMP(Stat);
	}
	

}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (isHit)
	{
		FTimerHandle TimerHandle;
		float Delay = 0.2f; // 2초 후에 제거
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyPlayer::SetHitfalse, Delay);
	}

	if (isOnBuff)
	{
		BuffTime += DeltaTime;

		if (BuffTime >= 10.f)
		{
			isOnBuff = false;
			BuffTime = 0;
		}

	}


	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector NewLocation = GetActorLocation();

	// Move forward
	NewLocation += ForwardVector * Stat->GetSpeed() * InputComponent->GetAxisValue("UpDown") * DeltaTime;

	// Move right
	NewLocation += RightVector * Stat->GetSpeed() * InputComponent->GetAxisValue("LeftRight") * DeltaTime;

	// Set the new location
	SetActorLocation(NewLocation);



	//q스킬 쿨타임돌아가기

	if (bSkillQCooldown)
	{
		
		CurrentQCoolDownTime += DeltaTime;
		float ProgressPercent = FMath::Clamp(CurrentQCoolDownTime / QCoolDownTime, 0.0f, 1.0f);

		GetWorld()->GetTimerManager().SetTimer(QCoolTimerHandle, this, &AMyPlayer::SkillCoolOnQ, QCoolDownTime, false);

		SkillWidget->GetSkillProgressBar_Q()->SetPercent(1.f - ProgressPercent);

		if (CurrentQCoolDownTime >= QCoolDownTime)
		{
			// 쿨타임이 완료된 경우
			
			GetWorldTimerManager().ClearTimer(QCoolTimerHandle);
			QCoolTimerHandle.Invalidate();
			ProgressPercent = 0;
			CurrentQCoolDownTime = 0;
			bSkillQCooldown = false;
		}
	}
	//E
	if (bSkillECooldown)
	{

		CurrentECoolDownTime += DeltaTime;
		float ProgressPercent = FMath::Clamp(CurrentECoolDownTime / ECoolDownTime, 0.0f, 1.0f);

		GetWorld()->GetTimerManager().SetTimer(ECoolTimerHandle, this, &AMyPlayer::SkillCoolOnE, ECoolDownTime, false);

		SkillWidget->GetSkillProgressBar_E()->SetPercent(1.f - ProgressPercent);

		if (CurrentECoolDownTime >= ECoolDownTime)
		{
			// 쿨타임이 완료된 경우

			GetWorldTimerManager().ClearTimer(ECoolTimerHandle);
			ECoolTimerHandle.Invalidate();
			ProgressPercent = 0;
			CurrentECoolDownTime = 0;
			bSkillECooldown = false;
		}
	}
	//R
	if (bSkillRCooldown)
	{

		CurrentRCoolDownTime += DeltaTime;
		float ProgressPercent = FMath::Clamp(CurrentRCoolDownTime / RCoolDownTime, 0.0f, 1.0f);

		GetWorld()->GetTimerManager().SetTimer(RCoolTimerHandle, this, &AMyPlayer::SkillCoolOnR, RCoolDownTime, false);

		SkillWidget->GetSkillProgressBar_R()->SetPercent(1.f - ProgressPercent);

		if (CurrentRCoolDownTime >= RCoolDownTime)
		{
			// 쿨타임이 완료된 경우

			GetWorldTimerManager().ClearTimer(RCoolTimerHandle);
			RCoolTimerHandle.Invalidate();
			ProgressPercent = 0;
			CurrentRCoolDownTime = 0;
			bSkillRCooldown = false;
		}
	}

	
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayer::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyPlayer::Attack);
	PlayerInputComponent->BindAction(TEXT("Skill_R"), EInputEvent::IE_Pressed, this, &AMyPlayer::Skill_R);
	PlayerInputComponent->BindAction(TEXT("Skill_E"), EInputEvent::IE_Pressed, this, &AMyPlayer::Skill_E);
	PlayerInputComponent->BindAction(TEXT("EquipSword"), EInputEvent::IE_Pressed, this, &AMyPlayer::EquipSword);
	PlayerInputComponent->BindAction(TEXT("Shiled"), EInputEvent::IE_Pressed, this, &AMyPlayer::Shiled);
	PlayerInputComponent->BindAction(TEXT("Shiled"), EInputEvent::IE_Released, this, &AMyPlayer::ShiledDown);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AMyPlayer::Run);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AMyPlayer::RunFin);
	PlayerInputComponent->BindAction(TEXT("Buff"), EInputEvent::IE_Pressed, this, &AMyPlayer::Buff);


	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPlayer::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPlayer::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyPlayer::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPlayer::Pitch);
	PlayerInputComponent->BindAxis(TEXT("CameraArm"), this, &AMyPlayer::CameraArm);
}


void AMyPlayer::Attack()
{
	
	if (/*isAttackMode&&*/isEquipWeapon)
	{

		if (IsAttacking || isHit || IsSkill_R_MontageCheck)
			return;

		AnimInstance->PlayAttackMontage();

		AnimInstance->JumpToSection(AttackIndex);
		AttackIndex = (AttackIndex + 1) % 4;

		IsAttacking = true;
		IsMontageCheck = true;
		FHitResult HitResult;
		//bool TeleportCheck = false;
		//SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 50.f), true, &HitResult, ETeleportType::None);
	}


	if (isOnBuff)
	{
		//발사하는 거 소환여기서

		auto Fire = GetWorld()->SpawnActor<ASwordEffect>(GetActorLocation()+GetActorForwardVector(), FRotator(90.f, 0.f, 90.f) + GetActorRotation());


	}

}

void AMyPlayer::AttackCheck()
{
	/*if (isHit)
		isHit = false;*/

	if (AttackIndex >0)
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


		if (bResult && HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());


			FDamageEvent DamageEvent;


			HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);


			//hit effect

			auto HitEffect = GetWorld()->SpawnActor<AHitEffect>(HitResult.Actor->GetActorLocation(), FRotator::ZeroRotator);

		}

	}

	else //AttackIndex = 4
	{


		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		float AttackRange = 200.f;
		float AttackRadius = 200.f;

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


		if (bResult && HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());


			FDamageEvent DamageEvent;


			HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);


			//hit effect

			auto HitEffect = GetWorld()->SpawnActor<AHitEffect>(HitResult.Actor->GetActorLocation(), FRotator::ZeroRotator);

		}

	}



}

void AMyPlayer::ReadyFireTonado()
{
	isReadyFireTonado = true;

	if (isReadyFireTonado)
	{
		auto Fire = GetWorld()->SpawnActor<AFireTonado>(GetActorLocation(), FRotator::ZeroRotator);

	}
}

void AMyPlayer::ReadySkill_E()
{
	//look방향으로 이동
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 600.f));
}

void AMyPlayer::UpDown(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("UpDown %f"), Value);

	UpDownValue = Value;
	if (!IsMontageCheck && !IsSkill_R_MontageCheck && !IsSkill_E_MontageCheck && !isShiled)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), Value);
}

void AMyPlayer::LeftRight(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("LeftRight %f"), Value);
	LeftRightValue = Value;
	if (!IsMontageCheck && !IsSkill_R_MontageCheck && !IsSkill_E_MontageCheck && !isShiled)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), Value);
}

void AMyPlayer::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMyPlayer::Pitch(float Value)
{
	//AddControllerPitchInput(Value);
}

void AMyPlayer::CameraArm(float Value)
{
	
	SpringArm->TargetArmLength -= (Value*20.f);
}

void AMyPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsMontageCheck = false;

	OnAttackEnd.Broadcast();//공격 전파 
}

void AMyPlayer::OnSkill_R_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

	IsSkill_R_MontageCheck = false;
	OnSkill_R_End.Broadcast();//공격 전파 
	isReadyFireTonado = false;
}

void AMyPlayer::OnSkill_E_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsSkill_E_MontageCheck = false;
}

void AMyPlayer::OnSkill_Q_MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsSkill_Q_MontageCheck = false;
}

float AMyPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	
	isHit = true;
	return DamageAmount;

}

void AMyPlayer::SetHitfalse()
{
	isHit = false;
}

void AMyPlayer::Skill_R()
{
	


	if (/*isAttackMode&&*/ isEquipWeapon && !bSkillRCooldown && Stat->GetMp()>20)
	{
		AnimInstance->PlaySkill_R_Montage();

		IsSkill_R_MontageCheck = true;

		Stat->OnUseSkill(20.f);

		SkillWidget->GetSkillProgressBar_R()->SetPercent(1.f);

		bSkillRCooldown = true;
	}
}

void AMyPlayer::Skill_E()
{
	

	if (/*isAttackMode&&*/ isEquipWeapon && !bSkillECooldown && Stat->GetMp() > 10)
	{
		AnimInstance->PlaySkill_E_Montage();

		IsSkill_E_MontageCheck = true;

		Stat->OnUseSkill(10.f);

		SkillWidget->GetSkillProgressBar_E()->SetPercent(1.f);

		bSkillECooldown = true;
	}
}

void AMyPlayer::EquipSword()
{

	iEquipCount += 1;
	if (iEquipCount % 2 == 1)
	{
		isEquipWeapon = true;
		//isAttackMode = true;
	}
	if (iEquipCount % 2 == 0)
	{
		isEquipWeapon = false;
		//isAttackMode = false;
	}

}

void AMyPlayer::Shiled()
{
	isShiled = true;
	
	

}

void AMyPlayer::ShiledDown()
{
	isShiled = false;
}

void AMyPlayer::Run()
{
	Stat->SetSpeed(400);

}

void AMyPlayer::RunFin()
{
	Stat->SetSpeed(1);
}

void AMyPlayer::Buff()
{

	if (/*isAttackMode&&*/ isEquipWeapon && !bSkillQCooldown &&Stat->GetMp() >20)
	{

		bSkillQCooldown = true;
		AnimInstance->PlaySkill_Q_Montage();

		IsSkill_Q_MontageCheck = true;

		isOnBuff = true;

		Stat->OnUseSkill(20.f);

		SkillWidget->GetSkillProgressBar_Q()->SetPercent(1.f);

		


		
	}
	


}

void AMyPlayer::SkillCoolOnQ()
{
	bSkillQCooldown = false;

}

void AMyPlayer::SkillCoolOnE()
{
	bSkillECooldown = false;

}

void AMyPlayer::SkillCoolOnR()
{
	bSkillRCooldown = false;

}

