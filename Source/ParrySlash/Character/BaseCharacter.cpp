// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Weapon/BaseWeapon.h"
#include "Kismet/GamePlayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include <Cassert>
#include "AbilitySystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Weaponクラスコンポーネントを生成する
	WeaponComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponComponent"));
	WeaponComponent->SetupAttachment(GetMesh(), FName("GripPoint"));
	WeaponComponent->SetChildActorClass(ABaseWeapon::StaticClass());

	//アビリティシステムを生成
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();
	//WeaponActorの初期化
	assert(!WeaponComponent->GetChildActor());

	WeaponActor = Cast<ABaseWeapon>(WeaponComponent->GetChildActor());
	WeaponActor->SetOwner(this);
	UBoxComponent* WeaponCollision = WeaponActor->GetWeaponCollision();
	WeaponCollision->IgnoreActorWhenMoving(GetOwner(), true);

	//アビリティシステムにアビリティを追加
	if (AbilitySystem)
	{
		int32 inputID(0);
		if (HasAuthority() && AbilityList.Num() > 0) 
		{
			for (auto Ability : AbilityList)
			{
				if (Ability)
				{
					AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, inputID++));
				}
			}
		}

		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

// Called every frame

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystem->RefreshAbilityActorInfo();
}
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::StartAttack()
{
	UE_LOG(LogTemp, Log, TEXT("Call: StartAttack"));
	//現在防御中でない時のみ攻撃
	if (!bIsBlocking)
	{
		bIsAttacking = true;
		//Ability.Attackタグを取得して、攻撃を実行
		FGameplayTag AttackTag = FGameplayTag::RequestGameplayTag("Ability.Action.Attack");
		FGameplayTagContainer MyContainer;
		MyContainer.AddTag(AttackTag);
		AbilitySystem->TryActivateAbilitiesByTag(MyContainer, true);
		UE_LOG(LogTemp, Log, TEXT("Call: Ability.Attack"));
	}
}

void ABaseCharacter::EnableAttack()
{
	//現在攻撃中のときのみ有効化
	if (bIsAttacking)
	{
		if (WeaponActor)
		{
			WeaponActor->SetIsEnabled(true);
		}
	}
}

void ABaseCharacter::DisEnableAttack()
{
	WeaponActor->SetIsEnabled(false);
}

void ABaseCharacter::HitAttack(AActor* HitActor)
{
	//アクタが存在しないか破棄されているなら何もしない
	if (!HitActor || HitActor->IsActorBeingDestroyed()) 
	{
		return;
	}

	//当たったアクタがBaseCharacterクラスか
	ABaseCharacter* HitChara = Cast<ABaseCharacter>(HitActor);
	UE_LOG(LogTemp, Log, TEXT("HitChara = %s"), HitChara ? TEXT("true") : TEXT("false"));
	if (HitChara) 
	{
		//相手がパリィ有効なら攻撃は中止、のけぞる
		if (HitChara->bIsParrying)
		{
			//のけぞりモーション再生
			PlayAnimation(NockBackMontage);
			//パリィした側の処理呼び出し
			HitChara->Parried();
			//攻撃終了
			EndAttack();
		}
		else
		{
			//防御時
			HitChara->GetDamage(ATKRate, WeaponActor->GetWeaponPower());
			//攻撃が当たったら攻撃倍率を初期化
			ATKRate = 1.0f;
		}
	}
}


void ABaseCharacter::EndAttack()
{
	if (bIsAttacking) 
	{
		DisEnableAttack();
		bIsAttacking = false;
	}
}

void ABaseCharacter::PlayAnimation(UAnimMontage* montage)
{
	if (!montage) return;
	//アニメインスタンスが存在し、再生予定のアニメーションが再生されていないことを確認
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(montage)) 
	{
		AnimInstance->Montage_Play(montage);
	}

}

void ABaseCharacter::GetDamage(float AttackRate, float weaponPower)
{
	UE_LOG(LogTemp, Log, TEXT("Call Get Damage"));

	if (!this->bIsDead) 
	{
		if (bIsBlocking)
		{
			//パリィサウンド再生
			if (BlockSound)
				UGameplayStatics::PlaySoundAtLocation(this, BlockSound, GetActorLocation());
		}

		//ダメージは攻撃倍率(相手)*武器攻撃力(相手)/防御倍率(自分)
		CharaHP = CharaHP - AttackRate * weaponPower / DEFRate;
		UE_LOG(LogTemp, Log, TEXT("Get Damage: %f"), AttackRate * weaponPower / DEFRate);
		if (CharaHP <= 0) 
		{
			//HPが0以下なら死亡処理を実行
			Dead();
		}
	}
}

void ABaseCharacter::Dead()
{
	UE_LOG(LogTemp, Log, TEXT("Call Dead"));
	bIsDead = true;

	//移動無効化
	GetCharacterMovement()->DisableMovement();
	//コリジョン無効化
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponActor->SetIsEnabled(false);

	//コントローラー無効化
	if (GetController()) GetController()->UnPossess();

	//一定時間後に消す
	SetLifeSpan(5.0f);
}

void ABaseCharacter::StartBlock()
{
	//攻撃中かつ防御中でない時のみ
	if (!bIsAttacking && !bIsBlocking)
	{
		
		bIsBlocking = true;
		//防御倍率を増加させる
		DEFRate = DEFRateIncreace;
	}
}

void ABaseCharacter::EndBlock()
{
	if (bIsBlocking)
	{
		bIsBlocking = false;
		DEFRate = InitialDEFRate;
		bChangedParry = false;
	}
}


void ABaseCharacter::SwitchIsParrying(bool isParrying)
{
	bool oldflag = bIsParrying;

	//一度も切り替えられていないか
	if (!bChangedParry)
	{
		bIsParrying = isParrying;
		UE_LOG(LogTemp, Log, TEXT("Is Paryying: %s"), bIsParrying ? TEXT("true") : TEXT("false"));
		//切り替え前がパリィ有効で切り替えパリィ無効の状態ならこれ以降の通知でフラグを変化させない
		if (oldflag && !bIsParrying)
		{
			bChangedParry = true;
		}
	}

}

void ABaseCharacter::Parried()
{
	if (bIsBlocking)
	{
		//防御終了
		EndBlock();
		bIsParrying = false;
		//攻撃倍率設定
		ATKRate = ATKRateIncreace;
		//パリィサウンド再生
		if (ParrySound)
			UGameplayStatics::PlaySoundAtLocation(this, ParrySound, GetActorLocation());
		//パリィモンタージュ再生
		PlayAnimation(ParryMontage);
	}
}