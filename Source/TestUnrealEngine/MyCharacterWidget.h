// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMyCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindHP(class UMyStatComponent* StatComp);
	void UpdateHp();

private:
	UPROPERTY(meta = (BindWidget)) //이걸함으로서 블루프린트의 프로그래서바랑 연동유도
	class UProgressBar* PB_HPBar;
	


	TWeakObjectPtr<class UMyStatComponent> CurStatComp;
};
