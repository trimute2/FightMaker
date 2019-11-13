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
	if (priority>=ActionOutput.ActionInfo.Priority &&ConditionCheck(*blackboard)) {
		FFMAction copy;
		copy = ActionOutput;
		copy.ConfirmAction.AddDynamic(this, &UCGNode_Condition::ConfirmCondition);
		for (UCGNode* node : ChildNodes) {
			node->EvaluateNode(copy, blackboard);
		}
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