// Fill out your copyright notice in the Description page of Project Settings.
#include "ComboGraph/Nodes/CGNode_BlackBoard.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_NativeEnum.h"
#include "Engine/Engine.h"


#define LOCTEXT_NAMESPACE "BlackBoardNode"

UCGNode_BlackBoard::UCGNode_BlackBoard(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	ContextMenuName = LOCTEXT("ContextMenuName", "BlackBoard Condition");

	bCanBeRoot = true;
#endif
}

bool UCGNode_BlackBoard::ConditionCheck(UBlackboardComponent& blackboard)
{
	bool bResult = false;
	if (BlackboardKey.SelectedKeyType)
	{
		UBlackboardKeyType* KeyCDO = BlackboardKey.SelectedKeyType->GetDefaultObject<UBlackboardKeyType>();
		const uint8* KeyMemory = blackboard.GetKeyRawData(BlackboardKey.GetSelectedKeyID());

		// KeyMemory can be NULL if the blackboard has its data setup wrong, so we must conditionally handle that case.
		if (ensure(KeyCDO != NULL) && (KeyMemory != NULL))
		{
			const EBlackboardKeyOperation::Type Op = KeyCDO->GetTestOperation();
			switch (Op)
			{
			case EBlackboardKeyOperation::Basic:
				bResult = KeyCDO->WrappedTestBasicOperation(blackboard, KeyMemory, (EBasicKeyOperation::Type)OperationType);
				break;

			case EBlackboardKeyOperation::Arithmetic:
				bResult = KeyCDO->WrappedTestArithmeticOperation(blackboard, KeyMemory, (EArithmeticKeyOperation::Type)OperationType, IntValue, FloatValue);
				break;

			case EBlackboardKeyOperation::Text:
				bResult = KeyCDO->WrappedTestTextOperation(blackboard, KeyMemory, (ETextKeyOperation::Type)OperationType, StringValue);
				break;

			default:
				break;
			}
		}
	}

	return bResult;
	//return false;
}

void UCGNode_BlackBoard::ConfirmCondition(FFMActionInfo ActionInfo, UBlackboardComponent * BlackBoardInfo, AActor * Actor)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screen Message"));
	UBlackboardKeyType* KeyCDO = BlackboardKey.SelectedKeyType->GetDefaultObject<UBlackboardKeyType>();
	uint8* KeyMemory = BlackBoardInfo->GetKeyRawData(BlackboardKey.GetSelectedKeyID());
	if (ensure(KeyCDO != NULL) && (KeyMemory != NULL))
	{
		KeyCDO->WrappedClear(*BlackBoardInfo, KeyMemory);
	}
}

#if WITH_EDITOR

void UCGNode_BlackBoard::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property == NULL) {
		return;
	}

	const FName ChangedPropName = PropertyChangedEvent.Property->GetFName();
	if (ChangedPropName == GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, BlackboardKey.SelectedKeyName)) {
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass() ||
			BlackboardKey.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass()) {
			IntValue = 0;
		}
	}

#if WITH_EDITORONLY_DATA

	UBlackboardKeyType* KeyCDO = BlackboardKey.SelectedKeyType ? BlackboardKey.SelectedKeyType->GetDefaultObject<UBlackboardKeyType>() : NULL;
	if (ChangedPropName == GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, BasicOperation))
	{
		if (KeyCDO && KeyCDO->GetTestOperation() == EBlackboardKeyOperation::Basic)
		{
			OperationType = BasicOperation;
		}
	}
	else if (ChangedPropName == GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, ArithmeticOperation))
	{
		if (KeyCDO && KeyCDO->GetTestOperation() == EBlackboardKeyOperation::Arithmetic)
		{
			OperationType = ArithmeticOperation;
		}
	}
	else if (ChangedPropName == GET_MEMBER_NAME_CHECKED(UCGNode_BlackBoard, TextOperation))
	{
		if (KeyCDO && KeyCDO->GetTestOperation() == EBlackboardKeyOperation::Text)
		{
			OperationType = TextOperation;
		}
	}
#endif //WITH_EDITORONLY_DATA

}

#endif

#undef LOCTEXT_NAMESPACE