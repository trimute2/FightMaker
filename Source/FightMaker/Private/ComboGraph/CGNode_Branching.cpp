// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode_Branching.h"

void UCGNode_Branching::EvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) {
	for (UCGNode* child : ChildNodes) {
		child->Evaluate(ActionOutput, blackboard);
	}
}