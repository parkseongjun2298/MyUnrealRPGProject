// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterWidget.h"
#include"MyStatComponent.h"
#include"Components/ProgressBar.h"
void UMyCharacterWidget::BindHP(class UMyStatComponent* StatComp)
{
	CurStatComp = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UMyCharacterWidget::UpdateHp);
	
}

void UMyCharacterWidget::UpdateHp()
{
	if(CurStatComp.IsValid())
		PB_HPBar->SetPercent(CurStatComp->GetRatioHp());
}


