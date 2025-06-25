// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

/**
 * 
 */

UCLASS()
class PARRYSLASH_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	//�Q�[���̐i�s�󋵂��m�F
	void CheckGameState();
	//�Q�[���I�[�o�[����
	void GameOver();
	//�Q�[���N���A����
	void GameClear();
	//���͂�UI�I�����[�ɕύX
	void ChangeInputModeToUI();
};
