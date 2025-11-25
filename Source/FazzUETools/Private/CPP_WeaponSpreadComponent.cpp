// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponSpreadComponent.h"
#include "UE5Coro/Public/UE5Coro.h"

// Sets default values for this component's properties
UCPP_WeaponSpreadComponent::UCPP_WeaponSpreadComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);
	// ...
}


// Called when the game starts
void UCPP_WeaponSpreadComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentSpread = CurrentBaseSpread;
}


// Called every frame
void UCPP_WeaponSpreadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}
void UCPP_WeaponSpreadComponent::AddFireSpread()
{
	if (IsADS) return;
	CurrentSpread = FMath::Clamp(CurrentSpread + FireSpreadIncrement, FVector2D(0, 0), FireSpreadMax + CurrentBaseSpread);
	SpreadLerpCoroutine();
}
FVector2D UCPP_WeaponSpreadComponent::GetRandomSpread(){
	if (IsADS){
		return FVector2D(0, 0);
	}else{
		FVector2D RandomSpread = FVector2D(FMath::RandRange(-CurrentSpread.X/2, CurrentSpread.X/2), FMath::RandRange(-CurrentSpread.Y/2, CurrentSpread.Y/2));
		return RandomSpread;
	}
}

FVoidCoroutine UCPP_WeaponSpreadComponent::SpreadLerpCoroutine()
{
	if (bLerpCoroutineRunning) co_return;
	bLerpCoroutineRunning = true;
	// 获取世界DeltaTime
	float WorldDeltaTime = GetWorld()->GetDeltaSeconds();
	while (CurrentSpread != CurrentBaseSpread){
		CurrentSpread = FMath::Vector2DInterpConstantTo(CurrentSpread, CurrentBaseSpread, WorldDeltaTime, CurrentInterpolationRate);
		co_await UE5Coro::Latent::NextTick();
		//UE_LOG(LogTemp, Warning, TEXT("CurrentSpread: %s"), *CurrentSpread.ToString());
	}
	bLerpCoroutineRunning = false;
}

void UCPP_WeaponSpreadComponent::StartADS(){
	IsADS = true;
	CurrentBaseSpread = ADSBaseSpread;
	SpreadLerpCoroutine();
}

void UCPP_WeaponSpreadComponent::EndADS(){
	IsADS = false;
	CurrentBaseSpread = DefaultBaseSpread;
	SpreadLerpCoroutine();
}

void UCPP_WeaponSpreadComponent::SetBaseSpread_Implementation(FName Key){

}