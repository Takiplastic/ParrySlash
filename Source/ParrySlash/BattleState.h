// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBattlePhase :uint8
{
	Waiting
	,Playing
	,GameOver
	,GameClear
};

// デリゲート定義
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, EBattlePhase, NewState);

UCLASS()
class PARRYSLASH_API ABattleState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	//バトルシーンの進行状況
	UPROPERTY(BlueprintReadOnly, Category ="Game State")
	EBattlePhase CurrentPhase = EBattlePhase::Waiting;

	//生存しているプレイヤー数
	UPROPERTY(BlueprintReadOnly, Category = "Game Info")
	int32 PlayerCount = 0;

	//生存している敵キャラ数
	UPROPERTY(BlueprintReadOnly, Category = "Game Info")
	int32 EnemyCount = 0;

public:
	//バトルシーンの状況設定
	void SetBattlePhase(EBattlePhase NewPhase);
	//プレイヤー数を追加
	void SetPlayerCount(int addvalue);
	//敵キャラ数を追加
	void SetEnemyCount(int addvalue);
	//プレイヤー数、敵キャラ数初期化
	void CountInitialActors();
	//カウントのゲッター
	FORCEINLINE int32 GetPlayerCount() { return PlayerCount; }
	FORCEINLINE int32 GetEnemyCount() { return EnemyCount; }

	UPROPERTY(BlueprintAssignable, Category = "Game Info")
	FOnGameStateChanged OnGameStateChanged;

};
