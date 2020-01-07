// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboGraph/Nodes/CGNode_BlackBoardBase.h"
#include "CGNode_BlackBoard.generated.h"

class FCGNodeBlackBoardDetails;
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
	UPROPERTY()
	uint8 OperationType;

#if WITH_EDITORONLY_DATA

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
	TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
	TEnumAsByte<EArithmeticKeyOperation::Type> ArithmeticOperation;

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
	TEnumAsByte<ETextKeyOperation::Type> TextOperation;

#endif //With editor only data

#if WITH_EDITOR

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif // with editor

	friend FCGNodeBlackBoardDetails;
};
