// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode_Condition.h"

UCGNode_Condition::UCGNode_Condition(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UCGNode_Condition::EvaluateNode(FFMAction & ActionOutput, UBlackboardComponent * blackboard)
{
	if (priority>=ActionOutput.ActionInfo.Priority &&ConditionCheck(*blackboard)) {
		for (UCGNode* node : ChildNodes) {
			node->EvaluateNode(ActionOutput, blackboard);
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