#include "MiniMap.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"
#include "Components/Image.h"


void UMiniMap::UpdateMiniMap(UTexture2D* MiniMapTexture)
{
    //if (MiniMapImage && MiniMapTexture)
    //{
    //    // 미니맵 텍스처를 이미지 위젯의 브러시로 설정
    //    FSlateBrush Brush;
    //    Brush.SetResourceObject(MiniMapTexture);
    //    MiniMapImage->SetBrush(Brush);
    //}
}

void UMiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	MiniMapImage = Cast<UImage>(GetWidgetFromName(TEXT("MiniMap")));
	
}
