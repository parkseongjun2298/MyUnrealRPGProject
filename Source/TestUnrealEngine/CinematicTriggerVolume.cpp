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
        // 플레이어와의 충돌 감지 시 마스터 시퀀스를 실행합니다.


        FString LevelSequencePath = TEXT("'/Game/Cinematics/Sequences/Sequence/SequenceMaster.SequenceMaster'");
        CinematicSequence = LoadObject<ULevelSequence>(nullptr, *LevelSequencePath);
        if (CinematicSequence)
        {
            //// ULevelSequencePlayer 인스턴스를 생성합니다.
            SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), CinematicSequence, FMovieSceneSequencePlaybackSettings(), MyLevelSequenceActor);

           
            //// 시퀀스를 재생합니다.
            SequencePlayer->Play();

            bTriggerEnabled = false;

        }
    }
}