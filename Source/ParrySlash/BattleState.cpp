// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "Character/PlayerCharacter.h"
#include "Character/COMCharacter.h"
#include "BattleGameMode.h"

void ABattleState::BeginPlay()
{
	Super::BeginPlay();
	CountInitialActors();
}

void ABattleState::SetBattlePhase(EBattlePhase NewPhase)
{
	if (CurrentPhase != NewPhase)
	{
		CurrentPhase = NewPhase;
		//UI変更用のイベントディスパッチャー送信
		OnGameStateChanged.Broadcast(NewPhase);
		UE_LOG(LogTemp, Log, TEXT("バトルフェーズ変更: %d"), static_cast<uint8>(CurrentPhase));

	}
}

void ABattleState::SetPlayerCount(int addvalue)
{
	PlayerCount = FMath::Max(0, PlayerCount + addvalue);
	//ゲームモードでゲーム状況の判定
	ABattleGameMode* GM = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->CheckGameState();
	}

	UE_LOG(LogTemp, Log, TEXT("プレイヤー残り: %d"), PlayerCount);
}

void ABattleState::SetEnemyCount(int addvalue)
{
	EnemyCount = FMath::Max(0, EnemyCount + addvalue);
	ABattleGameMode* GM = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->CheckGameState();
	}
	UE_LOG(LogTemp, Log, TEXT("敵残り: %d"), EnemyCount);
}

void ABattleState::CountInitialActors()
{
	//プレイヤーキャラ数
	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), PlayerActors);
	PlayerCount = PlayerActors.Num();

	//敵キャラ数
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACOMCharacter::StaticClass(), EnemyActors);
	EnemyCount = EnemyActors.Num();

	UE_LOG(LogTemp, Log, TEXT("初期プレイヤー数: %d, 敵数: %d"), PlayerCount, EnemyCount);
}