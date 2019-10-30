// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraphNode_Base.h"
#include "ComboGraph/CGNode_Action.h"
#include "ComboGraphSchema.h"
#include "ComboGraphGraph.h"
#include "ComboGraphEditorCommands.h"


DEFINE_LOG_CATEGORY(CGGraphNodeSystem)

void UComboGraphNode_Base::CreateInputPins() {
	FEdGraphPinType ComboLinkPin;
	ComboLinkPin.PinCategory = UComboGraphSchema::PC_Combo;
	UEdGraphPin* InputPin = CreatePin(EGPD_Input, ComboLinkPin, TEXT("Previous"));
	InputPin->PinFriendlyName = FText::FromString(TEXT(" "));
}

void UComboGraphNode_Base::CreateOutputPins()
{
	FEdGraphPinType ComboLinkPin;
	ComboLinkPin.PinCategory = UComboGraphSchema::PC_Combo;
	UEdGraphPin* InputPin = CreatePin(EGPD_Output, ComboLinkPin, TEXT("Next"));
	InputPin->PinFriendlyName = FText::FromString(TEXT(" "));
}

void UComboGraphNode_Base::AllocateDefaultPins() {
	//CreatePin(EGPD_Input,)
	check(Pins.Num() == 0);

	CreateInputPins();
	CreateOutputPins();
}

void UComboGraphNode_Base::MakeRootNode()
{
	//UE_LOG(CGGraphNodeSystem, Log, TEXT("Make root called."));
	if (Node) {
		//UE_LOG(CGGraphNodeSystem, Log, TEXT("node found."));
		if (UComboGraph* CGAsset = Cast<UComboGraph>(GetOuter()->GetOuter())) {
			//UE_LOG(CGGraphNodeSystem, Log, TEXT("cg found."));
			CGAsset->MakeNodeRoot(Node);
			UEdGraphPin* outp = FindPin(TEXT("Previous"), EGPD_Input);
			if (outp) {
				outp->MarkPendingKill();
				Pins.Remove(outp);
				GetGraph()->NotifyGraphChanged();
				//DestroyPin(outp);
			}/*else{
				UE_LOG(CGGraphNodeSystem, Warning, TEXT("Cant find Pin."));
			}*/
		}
	}
}

void UComboGraphNode_Base::GetOutputPins(TArray<class UEdGraphPin*>& OutputPins)
{
	OutputPins.Empty();

	for (int32 PinIndex = 0; PinIndex < Pins.Num(); PinIndex++)
	{
		if (Pins[PinIndex]->Direction == EGPD_Output)
		{
			OutputPins.Add(Pins[PinIndex]);
		}
	}
}

//this function might be dumb
void UComboGraphNode_Base::GetBranchNodes(TArray<UComboGraphNode_Base*>& GraphNodes)
{
	GraphNodes.Add(this);
	if (Node && !Node->IsA<UCGNode_Action>()) {
		for (UEdGraphPin* pin : Pins) {
			if (pin->Direction == EGPD_Output) {
				for (UEdGraphPin* Linked : pin->LinkedTo) {
					if (UComboGraphNode_Base* LinkedNode = Cast<UComboGraphNode_Base>(Linked->GetOwningNode())) {
						LinkedNode->GetBranchNodes(GraphNodes);
					}
				}
			}
		}
	}
}

void UComboGraphNode_Base::PostPlacedNewNode()
{
	if (Node) {
		if (UComboGraph* CGAsset = Cast<UComboGraph>(GetOuter()->GetOuter())) {
			Node->InitializeFromAsset(*CGAsset);
			Node->GraphNode = this;
		}
		bCanRenameNode = Node->IsA(UCGNode_Action::StaticClass());
	}
}

bool UComboGraphNode_Base::CanCreateUnderSpecifiedSchema(const UEdGraphSchema * Schema) const
{
	return Schema->IsA(UComboGraphSchema::StaticClass());
}

FText UComboGraphNode_Base::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (Node) {
		return Node->ContextNodeTitle();
	}
	return FText::FromString(TEXT("TEST"));
}

FLinearColor UComboGraphNode_Base::GetNodeTitleColor() const
{
	if (Node->bIsRoot) {
		return FLinearColor(0.51f, 0.0f, 0.86f);
	}
	return Node->ContextNodeTitleColor();
}

/*void UComboGraphNode_Base::GetMenuEntries(FGraphContextMenuBuilder & ContextMenuBuilder) const
{
	const FText Name = FText::FromString(TEXT("TEST"));
	FFormatNamedArguments Arguments;
	Arguments.Add(TEXT("Name"), Name);
	const FText AddToolTip = FText::FromString(TEXT("adds test node"));
	const FText cat = FText::FromString(TEXT("Dev"));
	TSharedPtr<FComboGraphSchemaAction_NewNode> NewNodeAction(new FComboGraphSchemaAction_NewNode(cat, Name, AddToolTip, 0));
	NewNodeAction->NodeTemplate = NewObject<UComboGraphNode_Base>();
	ContextMenuBuilder.AddAction(NewNodeAction);
	//NewNodeAction->ComboNodeClass = GetClass();
}*/

void UComboGraphNode_Base::GetContextMenuActions(const FGraphNodeContextMenuBuilder & Context) const
{
	if (Context.Node) {
		if (Node&&Node->bCanBeRoot&&!Node->bIsRoot) {
			Context.MenuBuilder->AddMenuEntry(FComboGraphEditorCommands::Get().MakeRoot);
		}
	}
}

void UComboGraphNode_Base::OnRenameNode(const FString & NewName)
{
	//TODO: Make sure the names of nodes are unique
	if (UCGNode_Action* ActionNode = Cast<UCGNode_Action>(Node)) {
		ActionNode->ActionName = NewName;
	}
}

TSharedPtr<class INameValidatorInterface> UComboGraphNode_Base::MakeNameValidator() const
{
	UE_LOG(CGGraphNodeSystem, Warning, TEXT("MakeNameValidator called."));
	return NULL;
}