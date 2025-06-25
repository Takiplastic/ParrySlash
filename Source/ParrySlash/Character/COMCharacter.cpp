// Fill out your copyright notice in the Description page of Project Settings.


#include "COMCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "COMHPWidget.h"
#include "BattleState.h"


ACOMCharacter::ACOMCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//AI用の設定
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController::StaticClass();
}

void ACOMCharacter::BeginPlay()
{
	Super::BeginPlay();
	//ターゲットを設定
	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

}

void ACOMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//死んでるなら何もしない
	//ターゲットがいないまたは攻撃中なら制御を行わない
	if (bIsDead || !TargetPlayer || bIsAttacking) return;

	float Distance = FVector::Dist(TargetPlayer->GetActorLocation(), GetActorLocation());

	if (Distance < AttackRange)
	{
		TryAttack();
	}
	else
	{
		MoveToPlayer();
	}
}


void ACOMCharacter::ShowHPWidget()
{
	if (!HPWidget && HPWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		HPWidget = CreateWidget<UCOMHPWidget>(PC, HPWidgetClass);
		if (HPWidget)
		{
			HPWidget->AddToViewport();
		}
		bIsWidgetVisible = true;
	}
}

void ACOMCharacter::HideHPWidget()
{
	if (HPWidget)
	{
		HPWidget->RemoveFromParent();
		HPWidget = nullptr;
	}

	bIsWidgetVisible = false;
}

void ACOMCharacter::UpdateHPWidgetPosition()
{
	if (!HPWidget || !bIsWidgetVisible) return;
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ScreenPos;
	//HPbarを表示させるUI上の座標を計算→失敗したらスキップ
	if (PC->ProjectWorldLocationToScreen(GetActorLocation() + FVector(0, 0, 100), ScreenPos))
	{
		HPWidget->SetPositionInViewport(ScreenPos, true);
		//ProgressBar用の割合を計算
		float Percent = FMath::Clamp(CharaHP / InitialHP, 0.0f, 1.0f);
		//Blueprint側の更新関数を呼ぶ→Percentを渡す
		HPWidget->UpdateHPBar(Percent);
	}

}

void ACOMCharacter::MoveToPlayer()
{
	UE_LOG(LogTemp, Log, TEXT("Call: MoveToPlayer"));
	AAIController* AIController = Cast<AAIController>(GetController());

	if (AIController && TargetPlayer)
	{
		AIController->MoveToActor(TargetPlayer, 200.0f);
	}
}

void ACOMCharacter::TryAttack()
{
	UE_LOG(LogTemp, Log, TEXT("Call: TryAttack"));
	if (GetWorld()->TimeSeconds - LastAttackTime < AttackCooldown) return;
	
	GetController()->StopMovement();
	StartAttack();
	LastAttackTime = GetWorld()->TimeSeconds;
}

void ACOMCharacter::Dead()
{
	Super::Dead();

	//State側のプレイヤー人数を減らす
	ABattleState* GS = GetWorld() ? GetWorld()->GetGameState<ABattleState>() : nullptr;
	if (GS)
	{
		GS->SetEnemyCount(-1);
	}
}