// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "ComboGraphGraph.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTMAKEREDITOR_API UComboGraphGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

	void UpdateBlackBoardChange();
	
};
