// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFrameWork/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;


UCLASS()
class PARRYSLASH_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Third person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TPCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Block Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	/** Enable Target Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EnableTargetAction;

	/** Change Target Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeTargetAction;

public:
	APlayerCharacter();

protected:
	virtual void Tick(float DeltaTime) override;

	//�ړ�����
	void Move(const FInputActionValue& Value);
	//�J��������
	void Look(const FInputActionValue& Value);

	//APawn Interface
	virtual void NotifyControllerChanged() override ;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override ;

	//�U���J�n����(�G�L�����������p)
	void StartAttack() override;

	//���S����(State�X�V�p)
	virtual void Dead() override;

	//���͈͓��̓G�L�������擾/�X�V
	void UpdateNearEnemy();
	
	//�G�L�����X�e�[�^�XUI�̍X�V����
	void UpdateEnemyHPDisplay(TArray<class ACOMCharacter*> inRange);
	
	//�^�[�Q�b�g�t���O�؂�ւ�
	void SwitchIsTargeting();

	//�^�[�Q�b�g���b�N�I������
	void LockOn(class ABaseCharacter* targetChara);

	//�^�[�Q�b�g�L�����w��
	void DecideTarget();

	//�^�[�Q�b�g�L�����t���O
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	bool bIsTargeting;
	
	//�^�[�Q�b�g���̃L�����N�^�[
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class ABaseCharacter* TargetChara;

	//HP�o�[��\��/�^�[�Q�b�g�\�ȃL�����z��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TArray<class ACOMCharacter*> InRange;

public:
	//�J�����̃Q�b�^�[
	class UCameraComponent* GetThirdPersonCameraComponent() const { return TPCameraComponent; }
	
};
