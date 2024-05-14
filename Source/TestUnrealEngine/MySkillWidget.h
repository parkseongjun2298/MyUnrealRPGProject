// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MySkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMySkillWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
public:
	UProgressBar* GetSkillProgressBar_Q() { return SkillBar_Q; };
	UProgressBar* GetSkillProgressBar_E() { return SkillBar_E; };
	UProgressBar* GetSkillProgressBar_R() { return SkillBar_R; };

private:
	TWeakObjectPtr<class UMyStatComponent> CurrentCharStat;
	UPROPERTY()
		class UProgressBar* SkillBar_Q;
	UPROPERTY()
		class UProgressBar* SkillBar_E;
	UPROPERTY()
		class UProgressBar* SkillBar_R;
};
