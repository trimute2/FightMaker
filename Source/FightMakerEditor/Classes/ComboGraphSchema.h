// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "ComboGraphNode_Base.h"
#include "ComboGraphSchema.generated.h"

USTRUCT()
struct FIGHTMAKEREDITOR_API FComboGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	class UClass* ComboNodeClass;
	//TSubclassOf<UEdGraphNode>* ComboNodeClass;
	UComboGraphNode_Base* NodeTemplate;


	FComboGraphSchemaAction_NewNode()
		: FEdGraphSchemaAction()
		, ComboNodeClass(NULL)
	{}

	FComboGraphSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(MoveTemp(InNodeCategory), MoveTemp(InMenuDesc), MoveTemp(InToolTip), InGrouping)
		, ComboNodeClass(NULL)
	{}

	//~ Begin FEdGraphSchemaAction Interface
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//~ End FEdGraphSchemaAction Interface
};

/**
 * 
 */
UCLASS()
class FIGHTMAKEREDITOR_API UComboGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
	

public:

	static const FName PC_Combo;

	bool NodeHasAction(const UEdGraphPin* InputPin, const UEdGraphPin* OutputPin) const;
	bool ConditionLoopTest(const UEdGraphPin* InputPin, const UEdGraphPin* OutputPin) const;

	//~ Begin EdGraphSchema Interface
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, FMenuBuilder* MenuBuilder, bool bIsDebugging) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	//virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	//virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	//virtual bool ShouldHidePinDefaultValue(UEdGraphPin* Pin) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	//virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	//virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;
	//virtual void DroppedAssetsOnGraph(const TArray<struct FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const override;
	//virtual void DroppedAssetsOnNode(const TArray<FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraphNode* Node) const override;
	//virtual int32 GetNodeSelectionCount(const UEdGraph* Graph) const override;
	//virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	//~ End EdGraphSchema Interface
};
