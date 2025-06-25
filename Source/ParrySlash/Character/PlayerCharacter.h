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

	//移動処理
	void Move(const FInputActionValue& Value);
	//カメラ処理
	void Look(const FInputActionValue& Value);

	//APawn Interface
	virtual void NotifyControllerChanged() override ;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override ;

	//攻撃開始処理(敵キャラを向く用)
	void StartAttack() override;

	//死亡処理(State更新用)
	virtual void Dead() override;

	//一定範囲内の敵キャラを取得/更新
	void UpdateNearEnemy();
	
	//敵キャラステータスUIの更新処理
	void UpdateEnemyHPDisplay(TArray<class ACOMCharacter*> inRange);
	
	//ターゲットフラグ切り替え
	void SwitchIsTargeting();

	//ターゲットロックオン処理
	void LockOn(class ABaseCharacter* targetChara);

	//ターゲットキャラ指定
	void DecideTarget();

	//ターゲット有効化フラグ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	bool bIsTargeting;
	
	//ターゲット中のキャラクター
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class ABaseCharacter* TargetChara;

	//HPバーを表示/ターゲット可能なキャラ配列
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TArray<class ACOMCharacter*> InRange;

public:
	//カメラのゲッター
	class UCameraComponent* GetThirdPersonCameraComponent() const { return TPCameraComponent; }
	
};
