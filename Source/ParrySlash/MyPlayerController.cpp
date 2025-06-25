// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(this, WidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			//���̓��[�h�ݒ�(GameOnly)
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
			bShowMouseCursor = false;
		}
	}

}