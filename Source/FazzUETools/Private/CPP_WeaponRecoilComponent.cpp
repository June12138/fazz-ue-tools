// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponRecoilComponent.h"
#include "UE5Coro/Public/UE5Coro.h"

// Sets default values for this component's properties
UCPP_WeaponRecoilComponent::UCPP_WeaponRecoilComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);
	// ...
}


// Called when the game starts
void UCPP_WeaponRecoilComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn){
		UE_LOG(LogTemp, Error, TEXT("Owner of UCPP_WeaponRecoilComponent is not APawn"));
		return;
	}
	OwnerPawn->ReceiveControllerChangedDelegate.AddDynamic(this, &UCPP_WeaponRecoilComponent::RefreshOwnerController);
	RefreshOwnerController(OwnerPawn, nullptr, OwnerPawn->GetController());
}
void UCPP_WeaponRecoilComponent::RefreshOwnerController(APawn* Pawn, AController* OldController, AController* NewController)
{
	if (!OwnerPawn){
		UE_LOG(LogTemp, Error, TEXT("Owner of UCPP_WeaponRecoilComponent is not APawn"));
		return; 
	}
	OwnerController = Cast<APlayerController>(NewController);
}

FVoidCoroutine UCPP_WeaponRecoilComponent::RecoilCoroutine()
{
    if (bRecoilCoroutineActive) {
		//UE_LOG(LogTemp, Warning, TEXT("RecoilCoroutine is already active"));
		co_return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Starting RecoilCoroutine"));
	bRecoilCoroutineActive = true;
	RecoilProgress = 0.0f;
	FVector2D TotalRecoilThisShot = FVector2D::ZeroVector; // 本轮射击的总后坐力，Debug用的
	while (true){
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		// 根据后座曲线计算当帧的alpha
		float Progress = RecoilProgress / RecoilDuration;
		float Alpha = RecoilCurve->GetFloatValue(Progress);
		FVector2D RecoilThisFrame = RecoilForce * DeltaTime * Alpha;
		// 积攒后坐力
		RecoilAccumulator += (RecoilThisFrame);
		// 输入
		OwnerController->AddYawInput(RecoilThisFrame.X);
		OwnerController->AddPitchInput(RecoilThisFrame.Y);
		TotalRecoilThisShot += RecoilThisFrame;
		//UE_LOG(LogTemp, Warning, TEXT("RecoilCoroutine: %f"), Alpha);
		RecoilProgress = FMath::Clamp(RecoilProgress + DeltaTime, 0.0f, RecoilDuration);
		if (Progress >= 1.0f){
			//UE_LOG(LogTemp, Warning, TEXT("TotalRecoilThisShot: %f"), TotalRecoilThisShot.Size()); // 打印TotalRecoilThisShot
			RecoilProgress = 0.0f;
			TotalRecoilThisShot = FVector2D::ZeroVector;
			if (!bRecoiling){
				// 本轮后座结束时，如果后座停止则跳出循环，结束协程
				break;
			}
		}
		co_await UE5Coro::Latent::NextTick();
	}
	// 后座截止
	bRecoilCoroutineActive = false;

	bIsRecovering = false;
	RecoverCoroutine(); // 开始恢复协程
}

void UCPP_WeaponRecoilComponent::StartRecoil()
{
	if (!IsValid(OwnerController)){
		UE_LOG(LogTemp, Error, TEXT("OwnerController is not set"));
		return;
	}
	if (RecoilCurve){
		bRecoiling = true;
		RecoilCoroutine();
	}else{
		UE_LOG(LogTemp, Error, TEXT("RecoilCurve is not set"));
	}
}

void UCPP_WeaponRecoilComponent::EndRecoil()
{
	bRecoiling = false;
}

FVoidCoroutine UCPP_WeaponRecoilComponent::RecoverCoroutine()
{
    if (!OwnerController){
        UE_LOG(LogTemp, Error, TEXT("OwnerController is not set"));
        co_return;
    }
	if (bIsRecovering){
		//UE_LOG(LogTemp, Warning, TEXT("RecoverCoroutine is already active"));
		co_return;
	}
	bIsRecovering = true;
    while (bIsRecovering && !bRecoilCoroutineActive)
    {
		SetRemainingRecovery();
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        // 计算本帧回中量：回中速度 × 帧时间
        float RecoveryThisFrame = RecoverSpeed * DeltaTime;
        // 计算当前剩余回中量的长度
        float RemainingLength = RemainingRecovery.Size();
        // 如果剩余量很小或本帧回中量超过剩余量，则完成回中
        if (RemainingLength <= RecoveryThisFrame)
        {
            // 施加剩余的全部回中量
            OwnerController->AddYawInput(-RemainingRecovery.X);
            OwnerController->AddPitchInput(-RemainingRecovery.Y);
			//UE_LOG(LogTemp, Warning, TEXT("RecoverCoroutine Complete"));
			// 重置累积量
			ResetAccumlatiors();
            break;
        }
        // 计算本帧回中向量：按比例缩放到回中速度
        FVector2D RecoveryThisFrameVector = RemainingRecovery.GetSafeNormal() * RecoveryThisFrame;
        // 施加本帧回中量
        OwnerController->AddYawInput(-RecoveryThisFrameVector.X);
        OwnerController->AddPitchInput(-RecoveryThisFrameVector.Y);
        // 更新剩余回中量
		RecoilAccumulator -= RecoveryThisFrameVector;
        co_await UE5Coro::Latent::NextTick();
    }
	bIsRecovering = false;
}

void UCPP_WeaponRecoilComponent::LookInput(FVector2D InputValue)
{
	if (bRecoilCoroutineActive){
		ControlInputAccumulator += InputValue;
	}
}

void UCPP_WeaponRecoilComponent::SetRemainingRecovery()
{
	for (int32 i = 0; i < 2; ++i){
        // 计算理论净补偿量（后坐力 + 玩家输入）
		float Recoil = RecoilAccumulator[i];
		float Control = ControlInputAccumulator[i];
        float Net = RecoilAccumulator[i] + ControlInputAccumulator[i];
        // 玩家操作没有抵消后座力
        bool bUnderCompensate = (FMath::Sign(Net) == FMath::Sign(Recoil));
		// 玩家操作和后座在同一方向
		bool bControlRecoilAlign = (FMath::Sign(Control) == FMath::Sign(Recoil));
		if (bUnderCompensate && !bControlRecoilAlign){
			RemainingRecovery[i] = Net;
		}else{
			RemainingRecovery[i] = 0.0f;
		}
    }
	// Log RecoilAccumulation和ControlInputAccumulation
	//UE_LOG(LogTemp, Warning, TEXT("RecoilAccumulator: %s"), *RecoilAccumulator.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("ControlInputAccumulator: %s"), *ControlInputAccumulator.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("RemainingRecovery: %s"), *RemainingRecovery.ToString());
}

void UCPP_WeaponRecoilComponent::ResetAccumlatiors()
{
	RecoilAccumulator = FVector2D::ZeroVector;
	ControlInputAccumulator = FVector2D::ZeroVector;
}
