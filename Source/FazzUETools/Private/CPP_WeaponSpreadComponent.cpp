// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_WeaponSpreadComponent.h"
#include "UE5Coro/Public/UE5Coro.h"

// Sets default values for this component's properties
UCPP_WeaponSpreadComponent::UCPP_WeaponSpreadComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_WeaponSpreadComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCPP_WeaponSpreadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}
void UCPP_WeaponSpreadComponent::AddFireSpread()
{
	CurrentFireSpread = FMath::Clamp(CurrentFireSpread + FireSpreadIncrement, FVector2D(0, 0), FireSpreadMax);
	FireSpreadDecrease();
	UpdateCurrentSpread();
}
FVector2D UCPP_WeaponSpreadComponent::GetRandomSpread(){
	if (IsADS){
		return FVector2D(0, 0);
	}else{
		FVector2D RandomSpread = FVector2D(FMath::RandRange(-CurrentSpread.X/2, CurrentSpread.X/2), FMath::RandRange(-CurrentSpread.Y/2, CurrentSpread.Y/2));
		return RandomSpread;
	}
}

FVoidCoroutine UCPP_WeaponSpreadComponent::FireSpreadDecrease()
{
	if (FireSpreadDecreaseRunning) co_return;
	FireSpreadDecreaseRunning = true;
	// 获取世界DeltaTime
	float WorldDeltaTime = GetWorld()->GetDeltaSeconds();
	while (CurrentFireSpread != FVector2D(0, 0)){
		CurrentFireSpread = FMath::Vector2DInterpConstantTo(CurrentFireSpread, FVector2D(0, 0), WorldDeltaTime, FireSpreadDecrementRate);
		UpdateCurrentSpread();
		co_await UE5Coro::Latent::NextTick();
	}
	FireSpreadDecreaseRunning = false;
}

void UCPP_WeaponSpreadComponent::UpdateCurrentSpread()
{
	CurrentSpread = CurrentBaseSpread + CurrentFireSpread;
}

void UCPP_WeaponSpreadComponent::StartADS(){
	IsADS = true;
}

void UCPP_WeaponSpreadComponent::EndADS(){
	IsADS = false;
}