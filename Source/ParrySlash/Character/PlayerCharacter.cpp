// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "COMCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "BattleState.h"

APlayerCharacter::APlayerCharacter()
{
	//カメラBoom生成
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//カメラとキャラの距離
	CameraBoom->TargetArmLength = 300.0f;
	//コントローラーに基づいてアームを回転させる
	CameraBoom->bUsePawnControlRotation = true;

	//カメラ生成
	TPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//カメラを1Boomにアタッチ
	TPCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TPCameraComponent->bUsePawnControlRotation = false;
	//キャラが移動方向に自動で回転
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateNearEnemy();
	UpdateEnemyHPDisplay(InRange);
	if (bIsTargeting) LockOn(TargetChara);
}
void APlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

		//Input Mapping Contextを追加
		if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//バインド設定
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving 
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		//Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAttack);
		//Blocking
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &APlayerCharacter::StartBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Canceled, this, &APlayerCharacter::EndBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndBlock);
		//Targetting
		EnhancedInputComponent->BindAction(EnableTargetAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchIsTargeting);
		EnhancedInputComponent->BindAction(ChangeTargetAction, ETriggerEvent::Started, this, &APlayerCharacter::DecideTarget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component!This template is built to use the Enhanced Input system.If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//防御中、攻撃中なら移動しない
	if (bIsAttacking || bIsBlocking) return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller && (MovementVector != FVector2D::ZeroVector))
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::StartAttack()
{
	//ターゲットモードなら攻撃前にターゲットの方向に向く
	if (bIsTargeting && TargetChara)
	{
		UE_LOG(LogTemp, Log, TEXT("AttackLockOn"));
		//ターゲットの方向を向くための目標回転を計算
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetChara->GetActorLocation());

		//現在のコントローラー回転を取得
		FRotator CurrentRot = GetControlRotation();

		//補完で滑らかに回転
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, GetWorld()->GetDeltaSeconds(), 1.0f);

		//コントローラーの回転を更新(カメラの向きも連動)
		//Controller->SetControlRotation(NewRot);

		//ActorのYawをターゲットに向ける
		FRotator NewActorRot = GetActorRotation();
		NewActorRot.Yaw = NewRot.Yaw;
		SetActorRotation(NewActorRot);
	}

	Super::StartAttack();
}

void APlayerCharacter::Dead()
{
	Super::Dead();

	//State側のプレイヤー人数を減らす
	ABattleState* GS = GetWorld() ? GetWorld()->GetGameState<ABattleState>() : nullptr;
	if (GS)
	{
		GS->SetPlayerCount(-1);
	}
}

void APlayerCharacter::UpdateNearEnemy()
{
	TArray<AActor*> NearbyEnemies;
	//Sphereによる衝突判定
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 1000.0f, { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) }, ACOMCharacter::StaticClass(), {}, NearbyEnemies);
	//敵キャラ配列の初期化
	InRange.Empty();
	//Actor配列をACOMCharacterセットに変換(重複無し,検索高速)
	for (AActor* Actor : NearbyEnemies)
	{
		ACOMCharacter* Enemy = Cast<ACOMCharacter>(Actor);
		if (!Enemy) continue;

		InRange.Add(Enemy);
	}
}

void APlayerCharacter::UpdateEnemyHPDisplay(TArray<ACOMCharacter*> inRange)
{
	//近くにいる敵キャラのHPバーを表示+表示位置更新
	for (ACOMCharacter* Enemy : inRange)
	{
		Enemy->ShowHPWidget();
		Enemy->UpdateHPWidgetPosition();
	}

	//範囲外の敵のWidgetは非表示に
	for (TActorIterator<ACOMCharacter> It(GetWorld()); It; ++It)
	{
		ACOMCharacter* Enemy = *It;
		if (!InRange.Contains(Enemy))
		{
			Enemy->HideHPWidget();
		}
	}
}

void APlayerCharacter::SwitchIsTargeting()
{
	bIsTargeting = !bIsTargeting;
	if (bIsTargeting) DecideTarget();
	else TargetChara = nullptr;
}

void APlayerCharacter::LockOn(class ABaseCharacter* targetChara)
{
	if (!targetChara) return;

	FVector Direction = (targetChara->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator LookRotation = Direction.Rotation();

	//必要に応じてPitch(上下)を抑える (TPS視点などではYawのみで十分)
	LookRotation.Pitch = -10.0f;
	//LookRotation.Roll = 0.0f;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		//補完アリならRInterpToでスムーズに回転
		FRotator CurrentRotation = PC->GetControlRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

		PC->SetControlRotation(NewRotation);
	}
}

void APlayerCharacter::DecideTarget()
{
	//ターゲットが無効ならreturn
	if (!bIsTargeting) return;
	//近くに敵キャラがいないならターゲットモード終了
	if (InRange.Num() == 0)
	{
		SwitchIsTargeting();
		return;
	}
	if (TargetChara == nullptr)
	{
		//初めてターゲット
		TargetChara = InRange[0];
	}
	else
	{
		//すでにターゲット中
		int32 Index = InRange.IndexOfByKey(TargetChara);
		UE_LOG(LogTemp, Log, TEXT("インデックス: %d"), Index);
		if (Index == INDEX_NONE)
		{
			return;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("次キャラインデックス: %d"), (Index + 1) % InRange.Num());
			TargetChara = InRange[(Index + 1) % InRange.Num()];
		}
	}
}