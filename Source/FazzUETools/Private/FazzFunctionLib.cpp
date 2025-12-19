// Fill out your copyright notice in the Description page of Project Settings.


#include "FazzFunctionLib.h"
#include "Kismet/KismetMathLibrary.h"
#include "Editor/UnrealEdEngine.h"

FVector2D UFazzFunctionLib::AngleToScreenPosition(APlayerController* PlayerController, FRotator InAngle, bool ZeroCenter){
    APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
    // 获得ViewportSize
    FVector2D ViewportSize;
    PlayerController->GetLocalPlayer()->ViewportClient->GetViewportSize(ViewportSize);
    // 获取摄像机位置和旋转
    FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
    FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
    FVector CameraDirection = UKismetMathLibrary::GetForwardVector(CameraRotation);
    // 根据InAngle，旋转CameraDirection
    FVector Direction = CameraDirection.RotateAngleAxis(InAngle.Yaw, UKismetMathLibrary::GetUpVector(CameraRotation));
    Direction = Direction.RotateAngleAxis(InAngle.Pitch, UKismetMathLibrary::GetRightVector(CameraRotation));
    Direction = Direction.RotateAngleAxis(InAngle.Roll, UKismetMathLibrary::GetForwardVector(CameraRotation));
    // 计算屏幕坐标
    FVector2D ScreenPosition;
    //UGameplayStatics::ProjectWorldLocationToScreen(CameraLocation + Direction * 1000, WorldContextObject);
    PlayerController->ProjectWorldLocationToScreen(CameraLocation + Direction * 1000, ScreenPosition);
    if (ZeroCenter) {
        ScreenPosition.X = ScreenPosition.X - ViewportSize.X / 2;
        ScreenPosition.Y = ScreenPosition.Y- (ViewportSize.Y / 2.0f);
    }
    return ScreenPosition;
}

void UFazzFunctionLib::ClientTravel(APlayerController* PlayerController, const FString& URL, ETravelType TravelType) {

    if (PlayerController) {
        PlayerController->ClientTravel(URL, TravelType);
    }
}