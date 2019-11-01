// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode.h"
#include "ComboGraph/ComboGraph.h"

DEFINE_LOG_CATEGORY(CGNodeSystem)

UCGNode::UCGNode(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
#if WITH_EDITORONLY_DATA
	, GraphNode(NULL)
#endif // WITH_EDITORONLY_DATA
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
