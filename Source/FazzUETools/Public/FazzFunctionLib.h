#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"  // 添加这个 include
#include "FazzFunctionLib.generated.h"  // 添加 generated.h

UCLASS()  // 添加 UCLASS 宏
class FAZZUETOOLS_API UFazzFunctionLib : public UBlueprintFunctionLibrary  // 继承 UBlueprintFunctionLibrary，并前缀 U
{
    GENERATED_BODY()  // 添加 GENERATED_BODY 宏

public:
    // 移除非必要的构造函数/析构函数，因为库类不需要它们
    // FazzFunctionLib();  // 可移除
    // ~FazzFunctionLib();  // 可移除

    UFUNCTION(BlueprintCallable, Category = "Fazz Utilities")  // 添加 Category 以便蓝图分类
    static FVector2D AngleToScreenPosition(UObject* WorldContextObject, FRotator InAngle);
};