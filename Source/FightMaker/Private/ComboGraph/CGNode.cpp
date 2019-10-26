// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode.h"

DEFINE_LOG_CATEGORY(CGNodeSystem)

UCGNode::UCGNode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCGNode::InitializeFromAsset(UComboGraph & Asset)
{
	CGAsset = &Asset;
}

UBlackboardData * UCGNode::GetBlackboardAsset() const
{
	return CGAsset ? CGAsset->BlackboardAsset : NULL;
}

void UCGNode::Evaluate(FFMAction & ActionOutput, UBlackboardComponent * blackboard)
{
}
