// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponRecoilComponent.h"
#include "UE5Coro/Public/UE5Coro.h"

// Sets default values for this component's properties
UCPP_WeaponRecoilComponent::UCPP_WeaponRecoilComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_WeaponRecoilComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn){
		UE_LOG(LogTemp, Error, TEXT("Owner of UCPP_WeaponRecoilComponent is not ACPP_Pawn"));
	}
	OwnerController = Cast<APlayerController>(OwnerPawn->GetController());
	if (!OwnerController){
		UE_LOG(LogTemp, Error, TEXT("Owner of UCPP_WeaponRecoilComponent is not ACPP_PlayerController"));
	}
	// ...
	
}


// Called every frame
void UCPP_WeaponRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVoidCoroutine UCPP_WeaponRecoilComponent::RecoilCoroutine()
{
    if (bRecoilCoroutineActive) {
		UE_LOG(LogTemp, Warning, TEXT("RecoilCoroutine is already active"));
		co_return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Starting RecoilCoroutine"));
	bRecoilCoroutineActive = true;
	while (true){
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		RecoilProgress += DeltaTime;
		float Progress = RecoilProgress / RecoilDuration;
		if (Progress >= 1.0f){
			RecoilProgress = 0.0f;
			UE_LOG(LogTemp, Warning, TEXT("Next Bullet"));
			if (!bRecoiling){
				break;
			}
		}
		float Alpha = RecoilCurve->GetFloatValue(Progress);
		OwnerController->AddYawInput(RecoilForce.X * Alpha);
		OwnerController->AddPitchInput(RecoilForce.Y * Alpha);
		UE_LOG(LogTemp, Warning, TEXT("RecoilCoroutine: %f"), Alpha);
		co_await UE5Coro::Latent::NextTick();
	}
	bRecoilCoroutineActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Ending RecoilCoroutine"));
}

void UCPP_WeaponRecoilComponent::StartRecoil()
{
	if (RecoilCurve){
		RecoilCoroutine();
		bRecoiling = true;
	}else{
		UE_LOG(LogTemp, Error, TEXT("RecoilCurve is not set"));
	}
}

void UCPP_WeaponRecoilComponent::EndRecoil()
{
	bRecoiling = false;
}