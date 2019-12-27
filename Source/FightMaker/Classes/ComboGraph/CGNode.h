// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FMAction.h"
//#include "ComboGraph.h"
#include "ComboGraph/ComboGraphUtils.h"
#include "CGNode.generated.h"

class UComboGraph;


FIGHTMAKER_API DECLARE_LOG_CATEGORY_EXTERN(CGNodeSystem, Log, All);

class UBlackboardData;
#if WITH_EDITOR
class UEdGraphNode;
#endif
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

	void Evaluate(FFMAction& ActionOutput, class UBlackboardComponent* blackboard);

	//evaluate the current graph for actions
	virtual void EvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard);
	//virtual void

	//maybe move this to the graph representation later
	//UPROPERTY()
	//uint8 bIsRoot : 1;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FText ContextMenuName;

	UPROPERTY()
	FString NodeName;

	virtual FString ContextNodeTitle() { return NodeName.Len() ? NodeName : UComboGraphUtils::GetShortTypeNameCG(this); };
	virtual FLinearColor ContextNodeTitleColor() { return FLinearColor(0.4f, 0.62f, 1.0f); };

	UPROPERTY()
	uint8 bCanBeRoot : 1;

	UPROPERTY()
	uint8 bIsRoot : 1;

	UPROPERTY()
	uint8 bIsConnectedToAction : 1;

	UPROPERTY()
	UEdGraphNode* GraphNode;
#endif
#if WITH_EDITOR
	virtual void SetChildNodes(TArray<UCGNode*>& children);
	virtual void AddChildNodes(TArray<UCGNode*>& children);
	virtual void AddChildNodes(UCGNode* child);
	virtual void CompileComplexNode(TArray<UCGNode*> OutputNodes, TArray<UCGNode*> InputNodes);

	//virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostLoad() override;
	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);
#endif
	virtual int DeterminePriority();
	virtual bool DetermineBranchHasAction();
	virtual void BuildCallbacks(FFMAction& ActionOutput);
	virtual void Serialize(FArchive& Ar) override;

	void RefreshAsset();

protected:

	//The highest priority action that is on this node path
	UPROPERTY()
	int priority;

	UPROPERTY()
	uint8 bBranchHasAction : 1;

	//The nodes that branch off of this node
	UPROPERTY()
	TArray<UCGNode*> ChildNodes;

	int GetNodeIndex();

	virtual bool ShouldEvaluateNode(FFMAction& ActionOutput, class UBlackboardComponent* blackboard);

private:

	UPROPERTY()
	UComboGraph* CGAsset;
};
