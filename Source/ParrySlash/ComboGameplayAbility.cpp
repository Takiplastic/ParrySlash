// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGameplayAbility.h"
#include "AbilitySystemComponent.h"

void UComboGameplayAbility::AddGameplayTags(const FGameplayTagContainer GameplayTags)
{
	UAbilitySystemComponent* Comp = GetAbilitySystemComponentFromActorInfo();

	Comp->AddLooseGameplayTags(GameplayTags);
}

void UComboGameplayAbility::RemoveGameplayTags(const FGameplayTagContainer GameplayTags)
{
	UAbilitySystemComponent* Comp = GetAbilitySystemComponentFromActorInfo();

	Comp->RemoveLooseGameplayTags(GameplayTags);
}