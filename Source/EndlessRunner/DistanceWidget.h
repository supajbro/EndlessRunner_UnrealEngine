// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DistanceWidget.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UDistanceWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Distance")
    void UpdateDistanceDisplay(float Distance);

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DistanceText;
};

