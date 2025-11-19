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
    if (bRecoilCoroutineActive) co_return;
	bRecoilCoroutineActive = true;
	while (true){
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		RecoilProgress += DeltaTime;
		float Alpha = RecoilProgress / RecoilDuration;
		if (Alpha >= 1.0f){
			RecoilProgress = 0.0f;
			if (!bRecoiling){
				break;
			}
		}
		OwnerController->AddYawInput(RecoilForce.X * Alpha);
		OwnerController->AddPitchInput(RecoilForce.Y * Alpha);
		co_await UE5Coro::Latent::NextTick();
	}
	bRecoilCoroutineActive = false;
}

void UCPP_WeaponRecoilComponent::StartRecoil()
{
	bRecoiling = true;
}

void UCPP_WeaponRecoilComponent::EndRecoil()
{
	bRecoiling = true;
}