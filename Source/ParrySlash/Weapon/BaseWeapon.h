// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class PARRYSLASH_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//武器のメッシュ
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

protected:
	//武器のコリジョン
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	class UBoxComponent* WeaponCollision;

	//武器の攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float WeaponPower;

public:
	//メッシュのゲッター
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

	//コリジョンのゲッター
	FORCEINLINE class UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
	
	//コリジョンの切り替え
	void SetIsEnabled(bool IsEnable);

	//攻撃力のセッター
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponPower(float power);

	//攻撃力のゲッター
	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetWeaponPower();
	//攻撃判定処理
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
