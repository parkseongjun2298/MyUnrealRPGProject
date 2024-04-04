// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMap.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMiniMap : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void UpdateMiniMap(class UTexture2D* MiniMapTexture);

protected:
	virtual void NativeConstruct() override;
	


private:
	UPROPERTY()
		class UImage* MiniMapImage;

};
