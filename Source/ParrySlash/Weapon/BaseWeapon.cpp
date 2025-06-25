// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Character/BaseCharacter.h"


// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//オブジェクトのメッシュコンポーネントを生成する
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//BoxComponentの作成
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	WeaponCollision->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	//コリジョンは無効にして初期化
	SetIsEnabled(false);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	//衝突判定初期化
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//攻撃判定処理をイベントにバインド
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnWeaponOverlap);

}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::SetIsEnabled(bool IsEnabled)
{
	if (IsEnabled)
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	else
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::SetWeaponPower(float power)
{
	WeaponPower = power;
}

float ABaseWeapon::GetWeaponPower()
{
	return WeaponPower;
}

void ABaseWeapon::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Hit Actor"));
	if (OtherActor && !OtherActor->IsActorBeingDestroyed() && OtherActor != GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *OtherActor->GetName());
		//親にBaseCharaComponentが含まれるならそのなかのHitAttackメソッドを呼び出す
		if (ABaseCharacter* MyOwner = Cast<ABaseCharacter>(GetOwner()))
			MyOwner->HitAttack(OtherActor);
	}
}