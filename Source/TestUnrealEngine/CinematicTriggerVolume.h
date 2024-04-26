// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "CinematicTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API ACinematicTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
public:
    ACinematicTriggerVolume();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere)
        class ULevelSequence* CinematicSequence;

    UPROPERTY(EditAnywhere)
        class ULevelSequencePlayer* SequencePlayer;
    UPROPERTY(EditAnywhere)
    class ALevelSequenceActor* MyLevelSequenceActor;

    UFUNCTION()
        void OnPlayerEnterTrigger(class AActor* OverlappedActor, class AActor* OtherActor);

public:
    bool Get_FinishCine() { return bTriggerEnabled; }
private:
    bool bTriggerEnabled = true;
};
