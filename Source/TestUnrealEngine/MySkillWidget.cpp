// Fill out your copyright notice in the Description page of Project Settings.


#include "MySkillWidget.h"
#include"MyStatComponent.h"
#include"Components/ProgressBar.h"
void UMySkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillBar_Q = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_Q")));

	SkillBar_E = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_E")));

	SkillBar_R = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_R")));

}
