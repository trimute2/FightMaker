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

FFMAction UComboGraph::Evaluate(FFMAction& OutputAction, UBlackboardComponent * blackboard, int FirstEvalIndex)
{
	if (FirstEvalIndex >= 0) {
		BaseNodes[FirstEvalIndex]->Evaluate(OutputAction, blackboard);
	}

	RootNode->Evaluate(OutputAction, blackboard);
	/*for (UCGNode* node : RootNodes) {
		node->Evaluate(OutputAction, blackboard);
	}*/
	if (BaseNodes.Contains(OutputAction.EvaluatedNext)) {
		BaseNodes.Find(OutputAction.EvaluatedNext);
	}

	return OutputAction;
}

#if WITH_EDITOR
void UComboGraph::PostInitProperties() {
	Super::PostInitProperties();
	if (!HasAnyFlags(RF_ClassDefaultObject | RF_NeedLoad))
	{
		CreateGraph();
		RootNode = NewObject<UCGNode_Branching>(this);
	}

	//CacheAggregateValues();
}

void UComboGraph::AddReferencedObjects(UObject * InThis, FReferenceCollector & Collector)
{
	UComboGraph* This = CastChecked<UComboGraph>(InThis);

	Collector.AddReferencedObject(This->ComboGraphGraph, This);

	Super::AddReferencedObjects(InThis, Collector);
}

void UComboGraph::CreateGraph() {
	if (ComboGraphGraph == nullptr) {
		//USoundCueGraph* SoundCueGraph = CastChecked<USoundCueGraph>(FBlueprintEditorUtils::CreateNewGraph(InSoundCue, NAME_None, USoundCueGraph::StaticClass(), USoundCueGraphSchema::StaticClass()));

		ComboGraphGraph = UComboGraph::GetComboGraphModuleInterface()->CreateNewComboGraphGraph(this);

		//Graph = CastChecked<UEdGraph>(FBlueprintEditorUtils::CreateNewGraph(this, NAME_None, UComboGraphGraph::StaticClass(), UComboGraphSchema::StaticClass()));

		//return SoundCueGraph;
	}
}

void UComboGraph::SetUpNode(UCGNode* CGNode, bool bSelectNewNode)
{
	check(CGNode->GraphNode == NULL)

	UComboGraph::GetComboGraphModuleInterface()->SetupAssetNode(ComboGraphGraph, CGNode, bSelectNewNode);
}

void UComboGraph::RemoveNodeFromBase(UCGNode * CGNode)
{
	BaseNodes.Remove(CGNode);
}

UEdGraph* UComboGraph::GetGraph() {
	return ComboGraphGraph;
}

#endif
//move posload here
void UComboGraph::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITOR
	if (GIsEditor && !GetOutermost()->HasAnyPackageFlags(PKG_FilterEditorOnly)) {
		if (ensure(ComboGraphGraph)) {
			UComboGraph::GetComboGraphModuleInterface()->RemoveNullNodes(this);
		}
	}
#endif

}


#if WITH_EDITOR
/** Gets the Combo graph editor implementation. */
TSharedPtr<IComboGraphModuleInterface> UComboGraph::GetComboGraphModuleInterface()
{
	return ComboGraphModuleInterface;
}

void UComboGraph::SetComboGraphModuleInterface(TSharedPtr<IComboGraphModuleInterface> InComboGraphModuleInterface)
{
	check(!ComboGraphModuleInterface.IsValid());
	ComboGraphModuleInterface = InComboGraphModuleInterface;
}

void UComboGraph::CompileAssetNodesFromGraphNodes() {
	UComboGraph::GetComboGraphModuleInterface()->CompileAssetNodesFromGraphNodes(this);
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
	UComboGraph::GetComboGraphModuleInterface()->UpdateBlackBoardAsset(ComboGraphGraph);
	
}

int UComboGraph::FindNode(UCGNode * node)
{
	return BaseNodes.Find(node);
}


UCGNode_Branching * UComboGraph::GetRootNode()
{
	return RootNode;
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
		//FStripDataFlags StripFlags(Record.EnterField(FIELD_NAME_TEXT("SoundCueStripFlags")));
		FStripDataFlags StripFlags(Record.EnterField(FIELD_NAME_TEXT("ComboGraphStripFlags")));
#if WITH_EDITORONLY_DATA
		if (!StripFlags.IsEditorDataStripped() && ComboGraphGraph != NULL)
		{
			Record << NAMED_FIELD(ComboGraphGraph);
		}
#endif
	}
#if WITH_EDITOR
	else
	{
		Record << NAMED_FIELD(ComboGraphGraph);
	}
#endif
}

void UComboGraph::Refresh()
{
	RootNode->RefreshAsset();
	RootNode->DeterminePriority();
}