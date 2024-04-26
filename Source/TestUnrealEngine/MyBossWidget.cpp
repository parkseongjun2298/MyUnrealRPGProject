// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossWidget.h"
#include"MyStatComponent.h"
#include"Components/ProgressBar.h"
void UMyBossWidget::BindHP(UMyStatComponent* StatComp)
{
	CurrentCharStat = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UMyBossWidget::UpdateCharacterStat);
}

void UMyBossWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));

}

void UMyBossWidget::UpdateCharacterStat()
{
	if (CurrentCharStat.IsValid())
		HpBar->SetPercent(CurrentCharStat->GetRatioHp());
}

