// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/Nodes/CGNode_BlueprintBase.h"
#include "BlueprintNodeHelpers.h"

UCGNode_BlueprintBase::UCGNode_BlueprintBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	UClass* StopAtClass = UCGNode_BlueprintBase::StaticClass();
	//find a way to copy the 
	ConditionCheckImplemented = BlueprintNodeHelpers::HasBlueprintFunction(TEXT("PerformConditionCheck"), *this, *StopAtClass) << 0;
	ConditionCheckImplemented |= BlueprintNodeHelpers::HasBlueprintFunction(TEXT("RegisterNode"), *this, *StopAtClass) << 1;
	ConditionCheckImplemented |= BlueprintNodeHelpers::HasBlueprintFunction(TEXT("RollbackNode"), *this, *StopAtClass) << 2;
	ConditionCheckImplemented |= BlueprintNodeHelpers::HasBlueprintFunction(TEXT("ConfirmNode"), *this, *StopAtClass) << 3;
}

bool UCGNode_BlueprintBase::ConditionCheck(UBlackboardComponent& blackboard)
{
	if (ConditionCheckImplemented & 1)
	{
		return PerformConditionCheck(&blackboard);
	}
	return false;
}

void UCGNode_BlueprintBase::RegisterCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor)
{
	if (ConditionCheckImplemented & 1 << 1) {
		RegisterNode(ActionInfo, BlackBoardInfo, Actor);
	}
}

void UCGNode_BlueprintBase::RollbackCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor)
{
	if (ConditionCheckImplemented & 1 << 2) {
		RollbackNode(ActionInfo, BlackBoardInfo, Actor);
	}
}

void UCGNode_BlueprintBase::ConfirmCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor)
{
	if (ConditionCheckImplemented & 1 << 3) {
		ConfirmNode(ActionInfo, BlackBoardInfo, Actor);
	}
}
