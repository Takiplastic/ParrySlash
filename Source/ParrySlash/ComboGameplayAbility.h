// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ComboGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PARRYSLASH_API UComboGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
	//AbilitySystemComponentのComponentのGameplayTagCountContainerに新しいGameplayTagを追加する
	UFUNCTION(BlueprintCallable, Category = "GamePlayAbility")
	virtual void AddGameplayTags(const FGameplayTagContainer GameplayTags);

	//AbilitySystemComponentのGameplayTagCountContainerのGameplayTagを削除する
	UFUNCTION(BlueprintCallable, Category = "GamePlayAbility")
	virtual void RemoveGameplayTags(const FGameplayTagContainer GameplayTags);

};
