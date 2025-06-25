// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Logging/LogMacros.h"
#include "BaseCharacter.generated.h"


UCLASS(config = game)
class PARRYSLASH_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when had Controller
	virtual void PossessedBy(AController* NewController) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	//初期HP
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float InitialHP;
	
	//変化するHP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float CharaHP;
	
	//初期MP
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float InitialMP;
	
	//変化するMP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float CharaMP;
	
	//攻撃倍率増加幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float ATKRateIncreace;

	//初期攻撃倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float InitialATKRate=1.0f;

	//実際の攻撃倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float ATKRate;

	//防御倍率増加幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float DEFRateIncreace;

	//初期防御倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float InitialDEFRate = 1.0f;

	//実際の防御倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	float DEFRate;

	//攻撃中かどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	bool bIsAttacking;

	//防御中かどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	bool bIsBlocking;
	
	//パリィ有効フラグ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara")
	bool bIsParrying = false;

	//パリィ設定フラグ(パリィフラグ変更を一度だけ行う用)
	bool bChangedParry = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chara", meta = (AllowProtectedAccess = "true"))
	//死んでいるかどうか
	bool bIsDead;

	//Weaponコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chara")
	UChildActorComponent* WeaponComponent;

	//Weaponコンポーネント内の子アクタ
	class ABaseWeapon* WeaponActor;

	//メッシュのアニメーションインスタンス
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Animation")
	UAnimInstance* AnimInstance;
	
	//死亡アニメーションモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathMontage;
	
	//ノックバックアニメーションモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* NockBackMontage;

	//パリィアニメーションモンタージュ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* ParryMontage;

	//防御時ダメージサウンド
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* BlockSound;

	//パリィサウンド
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ParrySound;

public:
	//攻撃開始処理
	UFUNCTION(BlueprintCallable)
	virtual void StartAttack();
	
	//攻撃有効化処理
	UFUNCTION(BlueprintCallable)
	void EnableAttack();

	//攻撃無効化処理
	UFUNCTION(BlueprintCallable)
	void DisEnableAttack();

	//攻撃処理
	UFUNCTION(BlueprintCallable)
	void HitAttack(AActor* HitActor);
	
	//攻撃終了処理
	UFUNCTION(BlueprintCallable)
	void EndAttack();
	
	//モンタージュアニメーション再生
	void PlayAnimation(UAnimMontage* montage);
	
	//ダメージ処理
	UFUNCTION(BlueprintCallable)
	void GetDamage(float attackRate, float weaponPower);
	
	//死亡処理
	UFUNCTION(BlueprintCallable)
	virtual void Dead();
	
	//防御開始処理
	UFUNCTION(BlueprintCallable)
	void StartBlock();
	
	//防御終了処理
	UFUNCTION(BlueprintCallable)
	void EndBlock();

	//パリィ成功時の処理
	UFUNCTION(BlueprintCallable)
	void Parried();

	//パリィ有効化/無効化切り替え
	UFUNCTION(BlueprintCallable)
	void SwitchIsParrying(bool isParrying);

	//アビリティシステム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystem; }

	//アビリティリスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> AbilityList;

};
