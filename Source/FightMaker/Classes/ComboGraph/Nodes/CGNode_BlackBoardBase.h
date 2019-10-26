// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraph/CGNode_Condition.h"
#include "ComboGraph/ComboGraphTypes.h"
//#include "BehaviorTree/BehaviorTreeTypes.h"
#include "CGNode_BlackBoardBase.generated.h"

/**
 * 
 */
//TODO Make custom IDetailCustomization
UCLASS(Abstract)
class FIGHTMAKER_API UCGNode_BlackBoardBase : public UCGNode_Condition
{
	GENERATED_BODY()

	virtual void InitializeFromAsset(UComboGraph& Asset) override;

	virtual bool ConditionCheck(class UBlackboardComponent& blackboard) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FFMBlackboardKeySelector BlackboardKey;
};
