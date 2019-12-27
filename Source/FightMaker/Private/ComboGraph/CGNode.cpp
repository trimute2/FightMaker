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

void UCGNode::AddChildNodes(TArray<UCGNode*>& children)
{
	ChildNodes.Append(children);
}

void UCGNode::AddChildNodes(UCGNode * child)
{
	ChildNodes.Add(child);
}

//overriden by nodes that can have a complex compile
void UCGNode::CompileComplexNode(TArray<UCGNode*> OutputNodes, TArray<UCGNode*> InputNodes)
{
}

void UCGNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	MarkPackageDirty();
}

void UCGNode::PostLoad()
{
	Super::PostLoad();
	// Make sure Combo nodes are transactional (so they work with undo system)
	SetFlags(RF_Transactional);
}

void UCGNode::AddReferencedObjects(UObject * InThis, FReferenceCollector & Collector)
{
	UCGNode* This = CastChecked<UCGNode>(InThis);

	Collector.AddReferencedObject(This->GraphNode, This);

	Super::AddReferencedObjects(InThis, Collector);
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

void UCGNode::BuildCallbacks(FFMAction & ActionOutput)
{
	for (UCGNode* Child : ChildNodes) {
		Child->BuildCallbacks(ActionOutput);
	}
}

int UCGNode::GetNodeIndex()
{
	return CGAsset->FindNode(this);
}

bool UCGNode::ShouldEvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) {
	return (/*bBranchHasAction &&*/ ActionOutput.ActionInfo.Priority <= priority);
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

void UCGNode::RefreshAsset()
{
	InitializeFromAsset(*CGAsset);
	for (UCGNode* child : ChildNodes) {
		child->RefreshAsset();
	}
}