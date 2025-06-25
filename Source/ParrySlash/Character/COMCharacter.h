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

	//�U��������͈�
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 200.0f;
	
	//�U���̃N�[���^�C��
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackCooldown = 2.0f;
	
	//�U���Ԋu�w��p�̃^�C�}�[
	FTimerHandle AttackResetTimer;

	//�_���v���C���[
	APawn* TargetPlayer;

	//�Ō�ɍU����������
	float LastAttackTime;

	//�X�e�[�^�X�\���p��Widget�N���X(����HP�̂�)
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HPWidgetClass;
	
	//�X�e�[�^�X�\���p��Widget
	class UCOMHPWidget* HPWidget = nullptr;

	//�X�e�[�^�X��UI��ɕ\�����邩?
	bool bIsWidgetVisible = false;

public:
	//HPWidget��\��
	void ShowHPWidget();

	//HPWidget���\��
	void HideHPWidget();

	//HPWIdget�̈ʒu���X�V
	void UpdateHPWidgetPosition();

	//�v���C���[�Ɍ������Ĉړ�
	void MoveToPlayer();

	//�U�������s���邩���f
	void TryAttack();

	virtual void Dead() override;


};
