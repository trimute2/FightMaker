// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraph/Nodes/CGNode_BlackBoardBase.h"
#include "CGNode_BlackBoard.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTMAKER_API UCGNode_BlackBoard : public UCGNode_BlackBoardBase
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual bool ConditionCheck(class UBlackboardComponent& blackboard) override;

	virtual void ConfirmCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor) override;

protected:

	/** value for arithmetic operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
	int32 IntValue;

	/** value for arithmetic operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
	float FloatValue;

	/** value for string operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
	FString StringValue;

	/** operation type */
	UPROPERTY(EditAnywhere)
	uint8 OperationType;
};
