// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode_Action.h"
#include "ComboGraph/CGNode_Branching.h"
#include "ComboGraph/CGNode_Condition.h"
#include "Engine\Engine.h"

#define LOCTEXT_NAMESPACE "ActionNode"
UCGNode_Action::UCGNode_Action(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("ContextMenuName", "Action Node");
#endif
}

#if WITH_EDITOR
//thinking on it this actually needs to be used for every branch, not just for the action nodes
void UCGNode_Action::CompileComplexNode(TArray<UCGNode*> OutputNodes, TArray<UCGNode*> InputNodes)
{
	//check that this is in fact a complex compile
	bool isComplexCompile = false;
	if(InputNodes.Num() > 1){
		for (UCGNode* inputNode : InputNodes) {
			if (inputNode->IsA<UCGNode_Condition>()) {
				isComplexCompile = true;
				break;
			}
		}
	}
	if (!isComplexCompile)
	{
		for (UCGNode* inputNode : InputNodes) {
			inputNode->SetFlags(RF_Transactional);
			inputNode->Modify();
			inputNode->AddChildNodes(this);
			inputNode->PostEditChange();
		}
		SetFlags(RF_Transactional);
		Modify();
		SetChildNodes(OutputNodes);
		PostEditChange();
	}
	else {
		ChildNodes.Empty();

		UCGNode_Branching* HiddenNextNode = NewObject<UCGNode_Branching>(GetOuter());
		HiddenNextNode->SetChildNodes(OutputNodes);
		ChildNodes.Add(HiddenNextNode);

		for (UCGNode* inputNode : InputNodes) {
			inputNode->SetFlags(RF_Transactional);
			inputNode->Modify();
			if (inputNode->IsA<UCGNode_Action>()) {
				inputNode->AddChildNodes(this);
			}
			else {
				UCGNode_Action* DerivedNode = NewObject<UCGNode_Action>(GetOuter(), NAME_None, RF_Transactional, this);
				inputNode->AddChildNodes(DerivedNode);
			}
			inputNode->PostEditChange();
		}
	}
}
#endif

int UCGNode_Action::DeterminePriority()
{
	BranchPriority = Super::DeterminePriority();
	priority = Action.ActionInfo.Priority;
	return priority;
}

void UCGNode_Action::BuildCallbacks(FFMAction & ActionOutput)
{
	FFMAction copy;
	Super::BuildCallbacks(copy);
	Action = ActionOutput;
	Action.ActionInfo = ActionInfo;
}

void UCGNode_Action::EvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) {
 	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screen Message"));
	if (ActionOutput.FirstEvaluation) {
		for (UCGNode* child : ChildNodes) {
			child->Evaluate(ActionOutput, blackboard);
		}
	}
	else {
		ActionOutput.ConfirmAction.Clear();
		ActionOutput.UseableResponse = true;
		ActionOutput.ActionInfo = ActionInfo;
		ActionOutput.NextIndex = GetNodeIndex();
		//ActionOutput.EvaluatedNext = this;
	}
	//ActionOutput = Action;
}
#undef LOCTEXT_NAMESPACE