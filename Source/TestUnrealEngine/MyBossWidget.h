// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyBossWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMyBossWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHP(class UMyStatComponent* StatComp);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	
private:
	UPROPERTY() //이걸함으로서 블루프린트의 프로그래서바랑 연동유도
	class UProgressBar* HpBar;

	TWeakObjectPtr<class UMyStatComponent> CurrentCharStat;
	
};
