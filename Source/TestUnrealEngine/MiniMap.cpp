#include "MiniMap.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"
#include "Components/Image.h"


void UMiniMap::UpdateMiniMap(UTexture2D* MiniMapTexture)
{
    //if (MiniMapImage && MiniMapTexture)
    //{
    //    // �̴ϸ� �ؽ�ó�� �̹��� ������ �귯�÷� ����
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
