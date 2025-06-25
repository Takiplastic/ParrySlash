// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "COMHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class PARRYSLASH_API UCOMHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateHPBar(float Percent);
};
