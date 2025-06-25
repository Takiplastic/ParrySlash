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

	//AI�p�̐ݒ�
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyController::StaticClass();
}

void ACOMCharacter::BeginPlay()
{
	Super::BeginPlay();
	//�^�[�Q�b�g��ݒ�
	TargetPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

}

void ACOMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//����ł�Ȃ牽�����Ȃ�
	//�^�[�Q�b�g�����Ȃ��܂��͍U�����Ȃ琧����s��Ȃ�
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
	//HPbar��\��������UI��̍��W���v�Z�����s������X�L�b�v
	if (PC->ProjectWorldLocationToScreen(GetActorLocation() + FVector(0, 0, 100), ScreenPos))
	{
		HPWidget->SetPositionInViewport(ScreenPos, true);
		//ProgressBar�p�̊������v�Z
		float Percent = FMath::Clamp(CharaHP / InitialHP, 0.0f, 1.0f);
		//Blueprint���̍X�V�֐����Ăԁ�Percent��n��
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

	//State���̃v���C���[�l�������炷
	ABattleState* GS = GetWorld() ? GetWorld()->GetGameState<ABattleState>() : nullptr;
	if (GS)
	{
		GS->SetEnemyCount(-1);
	}
}