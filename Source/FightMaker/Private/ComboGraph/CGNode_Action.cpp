// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/CGNode_Action.h"

#define LOCTEXT_NAMESPACE "ActionNode"
UCGNode_Action::UCGNode_Action(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("ContextMenuName", "Action Node");
#endif
}


void UCGNode_Action::Evaluate(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) {
	ActionOutput = Action;
}
#undef LOCTEXT_NAMESPACE