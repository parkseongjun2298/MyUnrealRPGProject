// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicTriggerVolume.h"

#include "Kismet/GameplayStatics.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

ACinematicTriggerVolume::ACinematicTriggerVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    OnActorBeginOverlap.AddDynamic(this, &ACinematicTriggerVolume::OnPlayerEnterTrigger);
}

void ACinematicTriggerVolume::BeginPlay()
{
    Super::BeginPlay();
}

void ACinematicTriggerVolume::OnPlayerEnterTrigger(AActor* OverlappedActor, AActor* OtherActor)
{


    if (bTriggerEnabled)
    {
        // �÷��̾���� �浹 ���� �� ������ �������� �����մϴ�.


        FString LevelSequencePath = TEXT("'/Game/Cinematics/Sequences/Sequence/SequenceMaster.SequenceMaster'");
        CinematicSequence = LoadObject<ULevelSequence>(nullptr, *LevelSequencePath);
        if (CinematicSequence)
        {
            //// ULevelSequencePlayer �ν��Ͻ��� �����մϴ�.
            SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), CinematicSequence, FMovieSceneSequencePlaybackSettings(), MyLevelSequenceActor);

           
            //// �������� ����մϴ�.
            SequencePlayer->Play();

            bTriggerEnabled = false;

        }
    }
}