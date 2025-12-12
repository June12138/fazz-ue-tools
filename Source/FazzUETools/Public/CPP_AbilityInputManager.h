// GASAbilityManagerComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "CPP_AbilityInputManager.generated.h"

class UInputAction;
class UAbilitySystemComponent;
class UGameplayAbility;
USTRUCT(BlueprintType)
struct FAbilityInputConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FGameplayTag AbilityTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FGameplayTag ReleaseTag;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FAZZUETOOLS_API UCPP_AbilityInputManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UCPP_AbilityInputManager();

    virtual void BeginPlay() override;
    // 能力设置
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Management")
    TMap<UInputAction*, FAbilityInputConfig> Configs;
    // 需要赋予Owner的能力
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Management")
    TArray<TSubclassOf<UGameplayAbility>> Abilities;
    void BindInputActions();
private:
    void SetAbilitySystemComponent();
    UAbilitySystemComponent* ASC;
    void OnInputPressed(UInputAction* InputAction);
    void OnInputReleased(UInputAction* InputAction);
    void GiveAbilities();
};