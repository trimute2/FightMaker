// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGNode.h"
#include "CGNode_Condition.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FIGHTMAKER_API UCGNode_Condition : public UCGNode
{
	GENERATED_UCLASS_BODY()

#if WITH_EDITORONLY_DATA
	//virtual FText ContextNodeTitle() override { return FText::FromString(TEXT("Condition")); };
#endif

	virtual void BuildCallbacks(FFMAction & ActionOutput) override;

	//evaluate the current graph for actions
	virtual void EvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) override;
	
	//the condition check that this node attempts
	virtual bool ConditionCheck(class UBlackboardComponent& blackboard) { return false; };

	//the action taken to register the condition
	UFUNCTION()
	virtual void RegisterCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor);

	//the action taken to rollback the condition
	UFUNCTION()
	virtual void RollbackCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor);

	//the action taken to consume the condition
	UFUNCTION()
	virtual void ConfirmCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor);

};
