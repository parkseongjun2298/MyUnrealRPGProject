// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UMyStatComponent* CharStat);
	void BindCharacterStatMP(class UMyStatComponent* CharStat);
protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdateCharacterManaStat();

private:
	TWeakObjectPtr<class UMyStatComponent> CurrentCharStat;
	UPROPERTY()
		class UProgressBar* HpBar;
	UPROPERTY()
		class UProgressBar* MpBar;


};
