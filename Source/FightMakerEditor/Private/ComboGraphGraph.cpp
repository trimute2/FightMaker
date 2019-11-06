// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraphGraph.h"
#include "ComboGraph/ComboGraph.h"
#include "ComboGraphSchema.h"
#include "ComboGraph/CGNode.h"
#include "Kismet2/BlueprintEditorUtils.h"


class FComboGraphModuleInterface : public IComboGraphModuleInterface
{
public:
	FComboGraphModuleInterface()
	{

	}

	~FComboGraphModuleInterface() {

	}

	UEdGraph* CreateNewComboGraphGraph(UComboGraph* ComboGraph) {
		UComboGraphGraph* ComboGraphGraph = CastChecked<UComboGraphGraph>(FBlueprintEditorUtils::CreateNewGraph(ComboGraph, NAME_None, UComboGraphGraph::StaticClass(), UComboGraphSchema::StaticClass()));

		return ComboGraphGraph;
	}

	void SetupAssetNode(UEdGraph* CGGraph, UCGNode* InComboNode, bool bSelectNewNode) {
		FGraphNodeCreator<UComboGraphNode_Base> NodeCreator(*CGGraph);
		UComboGraphNode_Base *GraphNode = NodeCreator.CreateNode(bSelectNewNode);
		//GraphNode->SetSoundNode(InComboNode);
		GraphNode->Node = InComboNode;
		NodeCreator.Finalize();
	}

	void UpdateBlackBoardAsset(UEdGraph* Graph)
	{
		if (UComboGraphGraph* ComboGraphGraph = Cast<UComboGraphGraph>(Graph)) {
			ComboGraphGraph->UpdateBlackBoardChange();
		}
	}

	void CompileAssetNodesFromGraphNodes(UComboGraph* ComboGraph) {
		TArray<UComboGraphNode_Base*> ChildNodes;
		TArray<UEdGraphPin*> InputPins;

		
		//TODO: write compile function

		// step one fill out branches
		// look for nodes that belong in base nodes and entry nodes

	}
};

UComboGraphGraph::UComboGraphGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	if (!UComboGraph::GetComboGraphModuleInterface().IsValid()) {
		UComboGraph::SetComboGraphModuleInterface(TSharedPtr<FComboGraphModuleInterface>(new FComboGraphModuleInterface()));
	}
}

void UComboGraphGraph::UpdateBlackBoardChange()
{
	
	UComboGraph* CGAsset = Cast<UComboGraph>(GetOuter());
	if (CGAsset == nullptr)
	{
		return;
	}
	for (int32 Index = 0; Index < Nodes.Num(); ++Index)
	{
		UComboGraphNode_Base* MyNode = Cast<UComboGraphNode_Base>(Nodes[Index]);

		if (MyNode)
		{
			UCGNode* MyNodeInstance = Cast<UCGNode>(MyNode->Node);
			if (MyNodeInstance) {
				MyNodeInstance->InitializeFromAsset(*CGAsset);
			}
		}
	}
	/*
	for (int32 Index = 0; Index < Nodes.Num(); ++Index)
	{
		UBehaviorTreeGraphNode* MyNode = Cast<UBehaviorTreeGraphNode>(Nodes[Index]);

		if (MyNode)
		{
			UBTNode* MyNodeInstance = Cast<UBTNode>(MyNode->NodeInstance);
			if (MyNodeInstance)
			{
				UBehaviorTreeTypes::SetBTLoggingContext(MyNodeInstance);
				MyNodeInstance->InitializeFromAsset(*BTAsset);
			}

			for (int32 iDecorator = 0; iDecorator < MyNode->Decorators.Num(); iDecorator++)
			{
				UBTNode* DecoratorNodeInstance = MyNode->Decorators[iDecorator] ? Cast<UBTNode>(MyNode->Decorators[iDecorator]->NodeInstance) : NULL;
				if (DecoratorNodeInstance)
				{
					UBehaviorTreeTypes::SetBTLoggingContext(DecoratorNodeInstance);
					DecoratorNodeInstance->InitializeFromAsset(*BTAsset);
				}

				UBehaviorTreeGraphNode_CompositeDecorator* CompDecoratorNode = Cast<UBehaviorTreeGraphNode_CompositeDecorator>(MyNode->Decorators[iDecorator]);
				if (CompDecoratorNode)
				{
					CompDecoratorNode->OnBlackboardUpdate();
				}
			}

			for (int32 iService = 0; iService < MyNode->Services.Num(); iService++)
			{
				UBTNode* ServiceNodeInstance = MyNode->Services[iService] ? Cast<UBTNode>(MyNode->Services[iService]->NodeInstance) : NULL;
				if (ServiceNodeInstance)
				{
					UBehaviorTreeTypes::SetBTLoggingContext(ServiceNodeInstance);
					ServiceNodeInstance->InitializeFromAsset(*BTAsset);
				}
			}

			UBehaviorTreeTypes::SetBTLoggingContext(nullptr);
		}
	}
	*/
}