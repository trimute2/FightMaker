// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/ComboGraph.h"
#include "ComboGraph/CGNode.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(CGraphsystem)

#if WITH_EDITOR
TSharedPtr<IComboGraphModuleInterface> UComboGraph::ComboGraphModuleInterface = nullptr;
#endif

UComboGraph::UComboGraph(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FFMAction UComboGraph::Evaluate(UBlackboardComponent * blackboard)
{
	FFMAction OutputAction;
	for (UCGNode* node : RootNodes) {
		node->EvaluateNode(OutputAction, blackboard);
	}
	return OutputAction;
	//return FFMAction();
}

#if WITH_EDITOR
void UComboGraph::PostInitProperties() {
	Super::PostInitProperties();
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		CreateGraph();
	}

	//CacheAggregateValues();
}

void UComboGraph::CreateGraph() {
	if (Graph == nullptr) {
		//USoundCueGraph* SoundCueGraph = CastChecked<USoundCueGraph>(FBlueprintEditorUtils::CreateNewGraph(InSoundCue, NAME_None, USoundCueGraph::StaticClass(), USoundCueGraphSchema::StaticClass()));

		Graph = UComboGraph::GetComboGraphModuleInterface()->CreateNewComboGraphGraph(this);

		//Graph = CastChecked<UEdGraph>(FBlueprintEditorUtils::CreateNewGraph(this, NAME_None, UComboGraphGraph::StaticClass(), UComboGraphSchema::StaticClass()));

		//return SoundCueGraph;
	}
}

void UComboGraph::SetUpNode(UCGNode* CGNode, bool bSelectNewNode)
{
	check(CGNode->GraphNode == NULL)

	UComboGraph::GetComboGraphModuleInterface()->SetupAssetNode(Graph, CGNode, bSelectNewNode);
}

void UComboGraph::RemoveNodeFromBase(UCGNode * CGNode)
{
	BaseNodes.Remove(CGNode);
}

UEdGraph* UComboGraph::GetGraph() {
	return Graph;
}

void UComboGraph::GetInputNames(TArray<FString>& out) {
	//InputNameMap.GenerateKeyArray(out);
	out = Inputnames;
}

void UComboGraph::AddInputEvent(FString name) {
	//InputNameMap.Add(name, 1 << LowestUnset);
	//LowestUnset++;
	if (Inputnames.Num() < 32) {
		Inputnames.Add(name);
	}
}

void UComboGraph::SetComboGraphModuleInterface(TSharedPtr<IComboGraphModuleInterface> InComboGraphModuleInterface)
{
	check(!ComboGraphModuleInterface.IsValid());
	ComboGraphModuleInterface = InComboGraphModuleInterface;
}

/** Gets the sound cue graph editor implementation. */
TSharedPtr<IComboGraphModuleInterface> UComboGraph::GetComboGraphModuleInterface()
{
	return ComboGraphModuleInterface;
}

void UComboGraph::ChangeInputName(FString newName, int index) {
	if (index >= 0 && index < Inputnames.Num()&&!Inputnames.Contains(newName)) {
		Inputnames[index] = newName;
	}
}

void UComboGraph::MakeNodeRoot(UCGNode* newRoot)
{
	newRoot->bIsRoot = true;
	RootNodes.AddUnique(newRoot);
}

void UComboGraph::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property &&
		PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UComboGraph, BlackboardAsset))
	{
		UE_LOG(CGNodeSystem, Log, TEXT("BlackBoard Changed"));
	}
	else {
		UE_LOG(CGNodeSystem, Log, TEXT("updateing nodes"));
	}
	UComboGraph::GetComboGraphModuleInterface()->UpdateBlackBoardAsset(Graph);
	
}
#endif

void UComboGraph::Serialize(FStructuredArchive::FRecord Record)
{
	FArchive& UnderlyingArchive = Record.GetUnderlyingArchive();

	// Always force the duration to be updated when we are saving or cooking
	if (UnderlyingArchive.IsSaving() || UnderlyingArchive.IsCooking())
	{
		//generate some of the values
	}

	Super::Serialize(Record);

	if (UnderlyingArchive.UE4Ver() >= VER_UE4_COOKED_ASSETS_IN_EDITOR_SUPPORT) {
		FStripDataFlags StripFlags(Record.EnterField(FIELD_NAME_TEXT("ComboGraphStripFlags")));
#if WITH_EDITORONLY_DATA
		if (!StripFlags.IsEditorDataStripped())
		{
			Record << NAMED_FIELD(Graph);
		}
#endif
	}
#if WITH_EDITOR
	else
	{
		Record << NAMED_FIELD(Graph);
	}
#endif
}