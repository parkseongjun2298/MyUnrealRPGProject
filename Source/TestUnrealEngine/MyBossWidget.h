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
	UPROPERTY() //�̰������μ� �������Ʈ�� ���α׷����ٶ� ��������
	class UProgressBar* HpBar;

	TWeakObjectPtr<class UMyStatComponent> CurrentCharStat;
	
};
