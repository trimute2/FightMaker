// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/Nodes/CGNode_BlackBoardBase.h"

void UCGNode_BlackBoardBase::InitializeFromAsset(UComboGraph & Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		UE_LOG(CGNodeSystem, Log, TEXT("Initializeing blackboardkey."));
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(CGNodeSystem, Warning, TEXT("Can't initialize due to missing blackboard data."));
		BlackboardKey.InvalidateResolvedKey();
	}

}

bool UCGNode_BlackBoardBase::ConditionCheck(UBlackboardComponent& blackboard)
{
	return false;
}
