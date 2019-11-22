// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode_Condition.h"

UCGNode_Condition::UCGNode_Condition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCGNode_Condition::BuildCallbacks(FFMAction & ActionOutput)
{
	FFMAction copy;
	copy = ActionOutput;
	copy.ConfirmAction.AddDynamic(this, &UCGNode_Condition::ConfirmCondition);
}

void UCGNode_Condition::EvaluateNode(FFMAction & ActionOutput, UBlackboardComponent * blackboard)
{
	ActionOutput.FirstEvaluation = false;
	if (ConditionCheck(*blackboard)) {
		//FFMAction copy;
		//copy = ActionOutput;
		//copy.ConfirmAction.AddDynamic(this, &UCGNode_Condition::ConfirmCondition);
		for (UCGNode* node : ChildNodes) {
			node->EvaluateNode(ActionOutput, blackboard);
		}
		ActionOutput.ConfirmAction.AddDynamic(this, &UCGNode_Condition::ConfirmCondition);
		ActionOutput.RegisterAction.AddDynamic(this, &UCGNode_Condition::RegisterCondition);
		ActionOutput.RollbackAction.AddDynamic(this, &UCGNode_Condition::RollbackCondition);
	}
}

void UCGNode_Condition::RegisterCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor)
{
}

void UCGNode_Condition::RollbackCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor)
{
}

void UCGNode_Condition::ConfirmCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor)
{
}