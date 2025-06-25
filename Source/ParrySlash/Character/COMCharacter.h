// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "COMCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PARRYSLASH_API ACOMCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	

public:
	ACOMCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//攻撃をする範囲
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 200.0f;
	
	//攻撃のクールタイム
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackCooldown = 2.0f;
	
	//攻撃間隔指定用のタイマー
	FTimerHandle AttackResetTimer;

	//狙うプレイヤー
	APawn* TargetPlayer;

	//最後に攻撃した時刻
	float LastAttackTime;

	//ステータス表示用のWidgetクラス(現状HPのみ)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HPWidgetClass;
	
	//ステータス表示用のWidget
	class UCOMHPWidget* HPWidget = nullptr;

	//ステータスをUI状に表示するか?
	bool bIsWidgetVisible = false;

public:
	//HPWidgetを表示
	void ShowHPWidget();

	//HPWidgetを非表示
	void HideHPWidget();

	//HPWIdgetの位置を更新
	void UpdateHPWidgetPosition();

	//プレイヤーに向かって移動
	void MoveToPlayer();

	//攻撃を実行するか判断
	void TryAttack();

	virtual void Dead() override;


};
