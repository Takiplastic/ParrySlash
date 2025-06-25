// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BattleState.h"

void ABattleGameMode::CheckGameState()
{
	UE_LOG(LogTemp, Log, TEXT("Called CheckGameState"));
	ABattleState* GS = GetGameState<ABattleState>();
	if (!GS) return;
	
	//敵の数が0ならゲームクリア
	if (GS->GetEnemyCount() <=0)
	{
		//ゲームクリア処理
		GS->SetBattlePhase(EBattlePhase::GameClear);
		//入力モード設定(UIOnly)
		ChangeInputModeToUI();
		return;
	}

	//プレイヤー数が0ならゲームオーバー
	if (GS->GetPlayerCount() <= 0)
	{
		//ゲームオーバー処理
		GS->SetBattlePhase(EBattlePhase::GameOver);
		//入力モード設定(UIOnly)
		ChangeInputModeToUI();
		return;
	}
}


void ABattleGameMode::ChangeInputModeToUI()
{

	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;

	// 一時的に入力をブロック
	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);

	// 0.1秒後に入力を戻す
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [PC]()
		{
			PC->SetIgnoreMoveInput(false);
			PC->SetIgnoreLookInput(false);
		}, 0.1f, false);
}