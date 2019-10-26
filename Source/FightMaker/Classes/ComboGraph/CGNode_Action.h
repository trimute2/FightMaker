// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CGNode.h"
#include "FMAction.h"
#include "CGNode_Action.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTMAKER_API UCGNode_Action : public UCGNode
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere)
	FFMActionInfo ActionInfo;

	UPROPERTY()
	FFMAction Action;

	//The actions name
	UPROPERTY()
	FString ActionName;

#if WITH_EDITORONLY_DATA
	//The nodes title
	virtual FText ContextNodeTitle() override {
		if (ActionName.IsEmpty()) {
			return FText::FromString(TEXT("Action"));
		}
		return FText::FromString(ActionName);
	};

	virtual FLinearColor ContextNodeTitleColor() override { return FLinearColor(1.f, 0.0f, 0.0f, 1.0f); };
#endif

	//evaluate the current graph for actions
	virtual void Evaluate(FFMAction& ActionOutput, class UBlackboardComponent* blackboard) override;
};