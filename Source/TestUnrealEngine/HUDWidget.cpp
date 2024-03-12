// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include"MyStatComponent.h"
#include"Components/ProgressBar.h"

void UHUDWidget::BindCharacterStat(UMyStatComponent* CharStat)
{
	CurrentCharStat = CharStat;
	CharStat->OnHpChanged.AddUObject(this, &UHUDWidget::UpdateCharacterStat);

}

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));

	MpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_MPBar")));
}

void UHUDWidget::UpdateCharacterStat()
{
	if (CurrentCharStat.IsValid())
		HpBar->SetPercent(CurrentCharStat->GetRatioHp());
}
