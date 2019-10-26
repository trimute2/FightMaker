// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboGraph/ComboGraph.h"
#include "ComboGraph/CGNode.h"

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
		node->Evaluate(OutputAction, blackboard);
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