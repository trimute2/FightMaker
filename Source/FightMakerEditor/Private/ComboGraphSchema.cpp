// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraphSchema.h"
#include "EdGraph/EdGraph.h"
#include "UObject/UObjectIterator.h"
#include "ComboGraph/CGNode.h"
#include "ComboGraph/CGNode_Action.h"
#include "ComboGraph/CGNode_Condition.h"
#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "FightMakerEditorModule.h"
#include "AIGraphTypes.h"
//#include "AssetRegistryModule.h"
#include "ComboGraphGraph.h"
#include "ComboGraph/ComboGraph.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "ComboGraphSchema"

UEdGraphNode* FComboGraphSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	check(ComboNodeClass);

	UComboGraph* ComboGraph = Cast<UComboGraphGraph>(ParentGraph)->GetComboGraph();
	const FScopedTransaction Transaction(LOCTEXT("SoundCueEditorNewSoundNode", "Sound Cue Editor: New Sound Node"));
	ParentGraph->Modify();
	ComboGraph->Modify();

	UCGNode* NewNode = ComboGraph->ConstructComboNode<UCGNode>(ComboNodeClass, bSelectNewNode);

	NewNode->GraphNode->NodePosX = Location.X;
	NewNode->GraphNode->NodePosY = Location.Y;

	NewNode->GraphNode->AutowireNewNode(FromPin);
	ComboGraph->PostEditChange();
	ComboGraph->MarkPackageDirty();

	return NewNode->GraphNode;

	/*UEdGraphNode* ResultNode = NULL;

	if (NodeTemplate != NULL)
	{
		const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}
		NodeTemplate->Rename(NULL, ParentGraph);

		ParentGraph->AddNode(NodeTemplate, true);
		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		//TODO set any nodes insides flags
		//NodeTemplate->GenericGraphNode->SetFlags(RF_Transactional);
		NodeTemplate->SetFlags(RF_Transactional);

		ResultNode = NodeTemplate;
	}*/
	/*
	check(ComboNodeClass);

	// ComboNodeClass.thiscla
	//const FScopedTransaction Transaction
	//FGraphNodeCreator<ComboNodeClass::StaticClass()> NN(*ParentGraph);
	FGraphNodeCreator<UComboGraphNode_Base> NC(*ParentGraph);
	UEdGraphNode* n = NC.CreateNode();

	n->NodePosX = Location.X;
	n->NodePosY = Location.Y;
	NC.Finalize();


	return n;
	*/

	//ResultNode.Outer

	//return ResultNode;
}

void FComboGraphSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

bool UComboGraphSchema::NodeHasAction(const UEdGraphPin* InputPin, const UEdGraphPin* OutputPin) const
{
	if (UComboGraphNode_Base* InputNode = Cast<UComboGraphNode_Base>(InputPin->GetOwningNode()))
	{
		if (InputNode->Node && InputNode->Node->IsA<UCGNode_Action>()&&OutputPin->LinkedTo.Num() > 0) {
			for (UEdGraphPin* linkedPin : OutputPin->LinkedTo) {
				if (UComboGraphNode_Base* linkedNode = Cast<UComboGraphNode_Base>(linkedPin->GetOwningNode()))
				{
					if (linkedNode->Node && linkedNode->Node->IsA<UCGNode_Action>()) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

bool UComboGraphSchema::ConditionLoopTest(const UEdGraphPin * InputPin, const UEdGraphPin * OutputPin) const
{
	UComboGraphNode_Base* OutputNode = Cast<UComboGraphNode_Base>(OutputPin->GetOwningNode());
	UComboGraphNode_Base* InputNode = Cast<UComboGraphNode_Base>(InputPin->GetOwningNode());
	if (OutputNode != NULL && InputNode != NULL)
	{
		if (OutputNode->Node && OutputNode->Node->IsA<UCGNode_Condition>() &&
			InputNode->Node && InputNode->Node->IsA<UCGNode_Condition>())
		{
			//im doing this with the graph nodes but i might want to do this with the runtime nodes instead
			TArray<UComboGraphNode_Base *> Nodes;
			//OutputNode->GetBranchNodes(Nodes);
			InputNode->GetBranchNodes(Nodes);

			//return Nodes.Contains(InputNode);
			return Nodes.Contains(OutputNode);
		}
	}
	return false;
}

void UComboGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const {
	//TSet<TSubclassOf<UCGNode> > Visited;

	const FText AddToolTip = LOCTEXT("NewComboGraphNodeTooltip", "Add node here");

	FFightMakerEditorModule& FightMakerEditorModule = FModuleManager::GetModuleChecked<FFightMakerEditorModule>("FightMakerEditor");
	FGraphNodeClassHelper* ClassCache = FightMakerEditorModule.GetClassCache().Get();
	TArray <FGraphNodeClassData> ClassData;

	ClassCache->GatherClasses(UCGNode::StaticClass(), ClassData);

	for (FGraphNodeClassData gncd : ClassData) {

		TSubclassOf<UCGNode> Node = gncd.GetClass();


		FText Name = Node.GetDefaultObject()->ContextMenuName;

		if (Name.IsEmpty())
		{
			FString Title = Node->GetName();
			Title.RemoveFromEnd("_C");
			Name = FText::FromString(Title);
		}

		TSharedPtr<FComboGraphSchemaAction_NewNode> NewNodeAction(new FComboGraphSchemaAction_NewNode(LOCTEXT("Combo Graph Action", "Base Combo Graph Node"), Name, AddToolTip, 0));
		NewNodeAction->ComboNodeClass = Node;
		//NewNodeAction->NodeTemplate = NewObject<UComboGraphNode_Base>(ContextMenuBuilder.OwnerOfTemporaries);
		//NewNodeAction->NodeTemplate->Node = NewObject<UCGNode>(NewNodeAction->NodeTemplate, Node);
		ContextMenuBuilder.AddAction(NewNodeAction);

		//Visited.Add(Node);
	}

	//bassed off of http://kantandev.com/articles/finding-all-classes-blueprints-with-a-given-base
	/*for (TObjectIterator<UClass> It; It; ++It) {
		if (!It->IsNative() || !It->IsChildOf<UCGNode>() || It->HasAnyClassFlags(CLASS_Abstract)
			|| It->HasMetaData(TEXT("HiddenNode"))) {
			continue;
		}
		TSubclassOf<UCGNode> Node = *It;
		if (FKismetEditorUtilities::IsClassABlueprintSkeleton(Node)) {
			continue;
		}
		FText Name = Node.GetDefaultObject()->ContextMenuName;

		if (Name.IsEmpty())
		{
			FString Title = Node->GetName();
			Title.RemoveFromEnd("_C");
			Name = FText::FromString(Title);
		}

		TSharedPtr<FComboGraphSchemaAction_NewNode> NewNodeAction(new FComboGraphSchemaAction_NewNode(LOCTEXT("Combo Graph Action", "Base Combo Graph Node"), Name, AddToolTip, 0));
		NewNodeAction->ComboNodeClass = Node;
		ContextMenuBuilder.AddAction(NewNodeAction);

		//Visited.Add(Node);
		/*if (It->IsChildOf<UCGNode>() &&!It->HasAnyClassFlags(CLASS_Abstract) && !Visited.Contains(*It)
			&& !(It->HasAnyClassFlags(CLASS_Native) && It->HasMetaData(TEXT("HiddenNode")))) {

			TSubclassOf<UCGNode> Node = *It;

			FText Name = Node.GetDefaultObject()->ContextMenuName;

			if (Name.IsEmpty())
			{
				FString Title = Node->GetName();
				Title.RemoveFromEnd("_C");
				Name = FText::FromString(Title);
			}

			TSharedPtr<FComboGraphSchemaAction_NewNode> NewNodeAction(new FComboGraphSchemaAction_NewNode(LOCTEXT("Combo Graph Action", "Base Combo Graph Node"), Name, AddToolTip, 0));
			NewNodeAction->ComboNodeClass = Node;
			//NewNodeAction->NodeTemplate = NewObject<UComboGraphNode_Base>(ContextMenuBuilder.OwnerOfTemporaries);
			//NewNodeAction->NodeTemplate->Node = NewObject<UCGNode>(NewNodeAction->NodeTemplate, Node);
			ContextMenuBuilder.AddAction(NewNodeAction);

			Visited.Add(Node);
		}*/
	//}

	/*FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();*/

	Super::GetGraphContextActions(ContextMenuBuilder);
}

void UComboGraphSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, FMenuBuilder* MenuBuilder, bool bIsDebugging) const {
	if (InGraphNode != NULL)
	{
		MenuBuilder->BeginSection("ComboGraphSchemaNodeGenericActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		
		MenuBuilder->AddMenuEntry(FGenericCommands::Get().Delete);
		if (InGraphNode->bCanRenameNode) {
			MenuBuilder->AddMenuEntry(FGenericCommands::Get().Rename);
		}

		MenuBuilder->EndSection();
	}

	Super::GetContextMenuActions(CurrentGraph, InGraphNode, InGraphPin, MenuBuilder, bIsDebugging);
}

const FPinConnectionResponse UComboGraphSchema::CanCreateConnection(const UEdGraphPin * A, const UEdGraphPin * B) const
{
	const UEdGraphPin* InputPin = NULL;
	const UEdGraphPin* OutputPin = NULL;
	if (!CategorizePinsByDirection(A, B, InputPin, OutputPin)) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins can not be of the same type"));
	}
	if (!NodeHasAction(InputPin,OutputPin)) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Nodes can only directly connect to one action"));
	}
	if (ConditionLoopTest(InputPin, OutputPin)) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Conditions can not loop unless they are seperated by an action"));
	}
	return FPinConnectionResponse();
}

bool UComboGraphSchema::TryCreateConnection(UEdGraphPin * A, UEdGraphPin * B) const
{
	bool bModified = UEdGraphSchema::TryCreateConnection(A, B);

	if (bModified)
	{
		CastChecked<UComboGraphGraph>(A->GetOwningNode()->GetGraph())->GetComboGraph()->CompileAssetNodesFromGraphNodes();
	}

	return bModified;
}

FLinearColor UComboGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor(0.95f, 0.02f, 0.02f, 1.0f);
}


const FName UComboGraphSchema::PC_Combo(TEXT("combo"));

#undef LOCTEXT_NAMESPACE