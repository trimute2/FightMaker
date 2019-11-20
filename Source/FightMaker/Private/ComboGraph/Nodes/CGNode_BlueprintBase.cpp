// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/Nodes/CGNode_BlueprintBase.h"
#include "BlueprintNodeHelpers.h"

UCGNode_BlueprintBase::UCGNode_BlueprintBase(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {

	UClass* StopAtClass = UCGNode_BlueprintBase::StaticClass();
	//find a way to copy the 
	//ConditionCheckImplemented = BlueprintNodeHelpers::HasBlueprintFunction(TEXT("PerformConditionCheck"), *this, StopAtClass);
}

bool UCGNode_BlueprintBase::ConditionCheck(UBlackboardComponent& blackboard)
{
	return false;
}