// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "ComboGraph/CGNode.h"
#include "ComboGraphNode_Base.generated.h"

FIGHTMAKEREDITOR_API DECLARE_LOG_CATEGORY_EXTERN(CGGraphNodeSystem, Log, All);


/**
 * graphical representation of UGCNode
 */
UCLASS()
class FIGHTMAKEREDITOR_API UComboGraphNode_Base : public UEdGraphNode
{
	GENERATED_BODY()

public:
	//virtual void CreateInputPins() {};
	virtual void CreateInputPins();

	//virtual void CreateOutputPins() {};
	virtual void CreateOutputPins();

	void MakeRootNode();

	void GetOutputPins(TArray<class UEdGraphPin*>& OutputPins);

	//get the nodes on this branch
	void GetBranchNodes(TArray<UComboGraphNode_Base*>& GraphNodes);

	//////////////////////////////////
	// UEdGraphNode interface.

	virtual void AllocateDefaultPins() override;
	virtual void PostPlacedNewNode() override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	//virtual void GetMenuEntries(struct FGraphContextMenuBuilder & ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(const FGraphNodeContextMenuBuilder & Context) const override;
	virtual void OnRenameNode(const FString& NewName) override;
	virtual TSharedPtr<class INameValidatorInterface> MakeNameValidator() const override;

	UPROPERTY(Instanced)
	UCGNode* Node;
};
