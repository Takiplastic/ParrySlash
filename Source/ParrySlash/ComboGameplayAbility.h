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
	
	//AbilitySystemComponent‚ÌComponent‚ÌGameplayTagCountContainer‚ÉV‚µ‚¢GameplayTag‚ğ’Ç‰Á‚·‚é
	UFUNCTION(BlueprintCallable, Category = "GamePlayAbility")
	virtual void AddGameplayTags(const FGameplayTagContainer GameplayTags);

	//AbilitySystemComponent‚ÌGameplayTagCountContainer‚ÌGameplayTag‚ğíœ‚·‚é
	UFUNCTION(BlueprintCallable, Category = "GamePlayAbility")
	virtual void RemoveGameplayTags(const FGameplayTagContainer GameplayTags);

};
