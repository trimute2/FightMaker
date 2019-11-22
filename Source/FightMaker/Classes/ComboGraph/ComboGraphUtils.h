// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ComboGraphUtils.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTMAKER_API UComboGraphUtils : public UObject
{
	GENERATED_BODY()
	
public:
	static FString GetShortTypeNameCG(const UObject* Ob);
};
