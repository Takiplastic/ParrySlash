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
	//����̃��b�V��
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

protected:
	//����̃R���W����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	class UBoxComponent* WeaponCollision;

	//����̍U����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float WeaponPower;

public:
	//���b�V���̃Q�b�^�[
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

	//�R���W�����̃Q�b�^�[
	FORCEINLINE class UBoxComponent* GetWeaponCollision() const { return WeaponCollision; }
	
	//�R���W�����̐؂�ւ�
	void SetIsEnabled(bool IsEnable);

	//�U���͂̃Z�b�^�[
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponPower(float power);

	//�U���͂̃Q�b�^�[
	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetWeaponPower();
	//�U�����菈��
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
