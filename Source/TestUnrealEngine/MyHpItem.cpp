// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHpItem.h"
#include "GameFramework/PlayerController.h"
#include"MyPlayer.h"
#include "Components/BoxComponent.h"
#include "MyStatComponent.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics 헤더 파일을 인클루드
// Sets default values
AMyHpItem::AMyHpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM"));

	RootComponent = BoxTrigger;
	Item->SetupAttachment(RootComponent);

	BoxTrigger->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGideon/FX/Meshes/Shapes/HP_Item.HP_Item'"));
	if (SW.Succeeded())
	{
		Item->SetStaticMesh(SW.Object);
	}

	BoxTrigger->SetCollisionProfileName(TEXT("HpItem"));
	Item->SetCollisionProfileName(TEXT("NoCollision"));


}

// Called when the game starts or when spawned
void AMyHpItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyHpItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMyHpItem::OnCharOverLap);
}

// Called every frame
void AMyHpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyHpItem::OnCharOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<AMyPlayer>(OtherActor);

	if (nullptr != Player)
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러를 얻음
		if (PlayerController)
		{
			AMyPlayer* PlayerPawn = dynamic_cast<AMyPlayer*>(PlayerController->GetPawn()); // 플레이어 캐릭터를 얻음
			if (PlayerPawn)
			{
				PlayerPawn->Stat->OnAttacked(-1);
			}

		}

		Item->SetHiddenInGame(true, true);
		SetActorEnableCollision(false);
		Destroy();



	}
}


