// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BattleState.h"

void ABattleGameMode::CheckGameState()
{
	UE_LOG(LogTemp, Log, TEXT("Called CheckGameState"));
	ABattleState* GS = GetGameState<ABattleState>();
	if (!GS) return;
	
	//�G�̐���0�Ȃ�Q�[���N���A
	if (GS->GetEnemyCount() <=0)
	{
		//�Q�[���N���A����
		GS->SetBattlePhase(EBattlePhase::GameClear);
		//���̓��[�h�ݒ�(UIOnly)
		ChangeInputModeToUI();
		return;
	}

	//�v���C���[����0�Ȃ�Q�[���I�[�o�[
	if (GS->GetPlayerCount() <= 0)
	{
		//�Q�[���I�[�o�[����
		GS->SetBattlePhase(EBattlePhase::GameOver);
		//���̓��[�h�ݒ�(UIOnly)
		ChangeInputModeToUI();
		return;
	}
}


void ABattleGameMode::ChangeInputModeToUI()
{

	
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;

	// �ꎞ�I�ɓ��͂��u���b�N
	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);

	// 0.1�b��ɓ��͂�߂�
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [PC]()
		{
			PC->SetIgnoreMoveInput(false);
			PC->SetIgnoreLookInput(false);
		}, 0.1f, false);
}