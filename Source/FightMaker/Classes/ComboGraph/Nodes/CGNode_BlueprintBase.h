// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraph/CGNode_Condition.h"
#include "CGNode_BlueprintBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class FIGHTMAKER_API UCGNode_BlueprintBase : public UCGNode_Condition
{
	GENERATED_UCLASS_BODY()
	

public:

	//virtual void RegisterCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor) override;

	//the action taken to rollback the condition
	//virtual void RollbackCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor) override;

	virtual bool ConditionCheck(class UBlackboardComponent& blackboard) override;

	virtual void RegisterCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor) override;

	virtual void RollbackCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor) override;

	virtual void ConfirmCondition(FFMActionInfo ActionInfo, class UBlackboardComponent* BlackBoardInfo, class AActor* Actor) override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	bool PerformConditionCheck(UBlackboardComponent* blackboard);

	UFUNCTION(BlueprintImplementableEvent)
	void RegisterNode(FFMActionInfo ActionInfo, UBlackboardComponent* blackboard, class AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent)
	void RollbackNode(FFMActionInfo ActionInfo, UBlackboardComponent* blackboard, class AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent)
	void ConfirmNode(FFMActionInfo ActionInfo, UBlackboardComponent* blackboard, class AActor* Actor);

	uint32 ConditionCheckImplemented : 4;

};
