// Fill out your copyright notice in the Description page of Project Settings.


#include "FazzFunctionLib.h"
#include "Kismet/KismetMathLibrary.h"

FVector2D UFazzFunctionLib::AngleToScreenPosition(UObject* WorldContextObject, FRotator InAngle){
    // 获取PlayerCameraController
    UWorld* World = WorldContextObject->GetWorld();
    APlayerController* PlayerController = World->GetFirstPlayerController();
    APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager;
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
    return ScreenPosition;
}