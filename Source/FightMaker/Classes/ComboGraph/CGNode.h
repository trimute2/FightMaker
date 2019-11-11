// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FMAction.h"
//#include "ComboGraph.h"
#include "CGNode.generated.h"

class UComboGraph;


FIGHTMAKER_API DECLARE_LOG_CATEGORY_EXTERN(CGNodeSystem, Log, All);

class UBlackboardData;
class UEdGraphNode;
/**
 * 
 */
UCLASS(Abstract)
class FIGHTMAKER_API UCGNode : public UObject
{
	GENERATED_UCLASS_BODY()

	virtual void InitializeFromAsset(UComboGraph& Asset);

	//maybe should be moved into black board base
	UBlackboardData* GetBlackboardAsset() const;

	//evaluate the current graph for actions
	virtual void Evaluate(FFMAction& ActionOutput, class UBlackboardComponent* blackboard);
	//virtual void

	//maybe move this to the graph representation later
	UPROPERTY()
	uint8 bIsRoot : 1;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FText ContextMenuName;

	virtual FText ContextNodeTitle() { return FText::FromString(TEXT("TEST")); };
	virtual FLinearColor ContextNodeTitleColor() { return FLinearColor(0.4f, 0.62f, 1.0f); };

	UPROPERTY()
	uint8 bCanBeRoot : 1;

	UPROPERTY()
	uint8 bIsConnectedToAction : 1;

	UPROPERTY()
	UEdGraphNode* GraphNode;
#endif
#if WITH_EDITOR
	virtual void SetChildNodes(TArray<UCGNode*>& children);
#endif
	virtual int DeterminePriority();
	virtual void Serialize(FArchive& Ar) override;

protected:

	//The highest priority action that is on this node path
	UPROPERTY()
	int priority;

	//The nodes that branch off of this node
	UPROPERTY()
	TArray<UCGNode*> ChildNodes;

private:

	UPROPERTY()
	UComboGraph* CGAsset;
};
