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
		TArray<UCGNode*> ChildNodes;
		TArray<UComboGraphNode_Base*> ComplexCompile;
		TArray<UCGNode*> RootNodes;
		TArray<UEdGraphPin*> OutputPins;

		//The nodes that are the ends of the graph;
		TArray< UComboGraphNode_Base*> leafNodes;
		
		//TArray<UCGNode_Branching> branchEntryNodes;
		//TODO: write compile function

		// step one fill out branches
		// look for nodes that belong in base nodes and entry nodes

		//ComboGraph->RootNode.

		//determine if nodes can be constructed normally
		for (int32 NodeIndex = 0; NodeIndex < ComboGraph->ComboGraphGraph->Nodes.Num(); ++NodeIndex) {
			UComboGraphNode_Base* graphNode = Cast<UComboGraphNode_Base>(ComboGraph->ComboGraphGraph->Nodes[NodeIndex]);
			if (graphNode)
			{
				//check if this is a leaf node
				graphNode->GetOutputPins(OutputPins);
				if (OutputPins[0]->LinkedTo.Num() == 0) {
					leafNodes.Add(graphNode);
				}
				/*if (graphNode->bComplexCompile) {
					ComplexCompile.Add(graphNode);
				}
				else */if (graphNode->Node) {
					//attatch nodes together
					ChildNodes.Empty();
					for (UEdGraphPin * OutputPin : OutputPins) {
						for (UEdGraphPin * linkedTo : OutputPin->LinkedTo) {
							UComboGraphNode_Base * GraphChildNode = Cast<UComboGraphNode_Base>(linkedTo->GetOwningNode());
							if (GraphChildNode /*&& !GraphChildNode->bComplexCompile */&& GraphChildNode->Node) {
								ChildNodes.Add(GraphChildNode->Node);
							}
						}
					}
					graphNode->Node->SetFlags(RF_Transactional);
					graphNode->Node->Modify();
					graphNode->Node->SetChildNodes(ChildNodes);
					graphNode->Node->PostEditChange();

					if (graphNode->Node->bIsRoot) {
						RootNodes.Add(graphNode->Node);
					}
				}
			}
		}
		//TODO: get rid of complex compile in all forms
		/*
		TArray<UCGNode*> InputNodes;
		TArray<UCGNode*> OutputNodes;
		for (UComboGraphNode_Base* graphNode : ComplexCompile) {
			InputNodes.Empty();
			OutputNodes.Empty();
			OutputPins = graphNode->GetAllPins();
			for (UEdGraphPin* pin : OutputPins) {
				for (UEdGraphPin * linkedPin : pin->LinkedTo)
				{
					UComboGraphNode_Base * linkedNode = Cast<UComboGraphNode_Base>(linkedPin->GetOwningNode());
					if (linkedNode && linkedNode->Node) {
						if (pin->Direction == EGPD_Input) {
							InputNodes.Add(linkedNode->Node);
						}
						else {
							OutputNodes.Add(linkedNode->Node);
						}
					}
				}
			}
			graphNode->Node->CompileComplexNode(OutputNodes, InputNodes);
		}*/
		UCGNode* RootNode = ComboGraph->GetRootNode();
		RootNode->SetFlags(RF_Transactional);
		RootNode->Modify();
		RootNode->SetChildNodes(RootNodes);
		RootNode->PostEditChange();
		//ComboGraph->RootNode->SetFlags(RF_Transactional);
		//ComboGraph->RootNode->Modify();
		//ComboGraph->RootNode->SetChildNodes(RootNodes);
		//ComboGraph->RootNode->PostEditChange();

	}

	virtual void RemoveNullNodes(UComboGraph* ComboGraph) {
		// Deal with SoundNode types being removed - iterate in reverse as nodes may be removed
		for (int32 idx = ComboGraph->ComboGraphGraph->Nodes.Num() - 1; idx >= 0; --idx)
		{
			UComboGraphNode_Base* Node = Cast<UComboGraphNode_Base>(ComboGraph->ComboGraphGraph->Nodes[idx]);

			if (Node && Node->Node == NULL)
			{
				FBlueprintEditorUtils::RemoveNode(NULL, Node, true);
			}
		}
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

UComboGraph * UComboGraphGraph::GetComboGraph()
{
	return CastChecked<UComboGraph>(GetOuter());
}