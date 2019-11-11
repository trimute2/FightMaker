// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode.h"
#include "ComboGraph/ComboGraph.h"
#include "EngineUtils.h"

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

void UCGNode::Evaluate(FFMAction& ActionOutput, UBlackboardComponent* blackboard) {
	if (ShouldEvaluateNode(ActionOutput,blackboard)) {
		EvaluateNode(ActionOutput, blackboard);
	}
}

void UCGNode::EvaluateNode(FFMAction & ActionOutput, UBlackboardComponent * blackboard)
{
}

#if WITH_EDITOR
void UCGNode::SetChildNodes(TArray<UCGNode*>& children) {
	ChildNodes = children;
}
#endif

int UCGNode::DeterminePriority()
{
	priority = -1;
	for (UCGNode * child : ChildNodes) {
		int childPriority = child->DeterminePriority();
		priority = FMath::Max<int>(priority, childPriority);
	}
	return priority;
}

bool UCGNode::DetermineBranchHasAction() {
	bBranchHasAction = false;
	for (UCGNode* child : ChildNodes) {
		if (child->DetermineBranchHasAction()) {
			bBranchHasAction = true;
			return true;
		}
	}
	return false;
}

bool UCGNode::ShouldEvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) {
	return (bBranchHasAction && ActionOutput.ActionInfo.Priority <= priority);
}

void UCGNode::Serialize(FArchive& Ar) {
	Super::Serialize(Ar);
	 
	if (Ar.UE4Ver() >= VER_UE4_COOKED_ASSETS_IN_EDITOR_SUPPORT) {
		FStripDataFlags StripFlags(Ar);
#if WITH_EDITORONLY_DATA
		if (!StripFlags.IsEditorDataStripped()) {
			Ar << GraphNode;
		}
#endif
	}
#if WITH_EDITOR
	else{
		Ar << GraphNode;
	}
#endif
}
