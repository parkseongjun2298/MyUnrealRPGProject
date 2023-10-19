// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevel(Level);
}

void UMyStatComponent::SetLevel(int32 NewLevel)
{
	auto MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(NewLevel);
		if (StatData)
		{
			Level = StatData->Level;
			SetHp(StatData->MaxHp);//ui랑 관련댄거면 이렇게 함수로 관리하는게 ui랑연동더오류없이댐
			MaxHp = StatData->MaxHp;
			Attack = StatData->Attack;
		}
	}
}

void UMyStatComponent::OnAttacked(float DamageAmount)
{
	int NewHp = Hp-DamageAmount;

	SetHp(NewHp);
	//UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), Hp);
}

void UMyStatComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	if (Hp < 0)
		Hp = 0;
	OnHpChanged.Broadcast();

}
