#include "ComboGraph/ComboGraphEditor.h"
#include "ComboGraph/ComboGraph.h"
#include "ComboGraphGraph.h"
#include "EditorStyleSet.h"
#include "GraphEditorActions.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "ComboGraphNode_Base.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/Commands/GenericCommands.h"
#include "ComboGraphEditorCommands.h"
#include "ScopedTransaction.h"

#define LOCTEXT_NAMESPACE "ComboGraphEditor"

const FName FComboGraphEditor::GraphCanvasTabId(TEXT("ComboGraphEditor_GraphCanvas"));
const FName FComboGraphEditor::PropertiesTabId(TEXT("ComboGraphEditor_DetailView"));

FComboGraphEditor::FComboGraphEditor() :ComboGraphBeingEdited(nullptr) 
{
}

FComboGraphEditor::~FComboGraphEditor() {
	GEditor->UnregisterForUndo(this);
}

void FComboGraphEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ComboGraphEditor", "Combo Graph Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(GraphCanvasTabId, FOnSpawnTab::CreateSP(this, &FComboGraphEditor::SpawnTab_GraphCanvas))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FComboGraphEditor::SpawnTab_Properties))
		.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FComboGraphEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(GraphCanvasTabId);
	InTabManager->UnregisterTabSpawner(PropertiesTabId);
}

void FComboGraphEditor::InitComboGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UComboGraph* InitComboGraph)
{
	ComboGraphBeingEdited = InitComboGraph;

	GEditor->RegisterForUndo(this);

	FGraphEditorCommands::Register();
	FComboGraphEditorCommands::Register();

	CreateInternalWidgets();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Alpha_ComboGraph_Layout_v1")->
		AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.9f)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.8f)
					->AddTab(GraphCanvasTabId, ETabState::OpenedTab)->SetHideTabWell(true)

				)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.2f)
					->AddTab(PropertiesTabId, ETabState::OpenedTab)->SetHideTabWell(true)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, TEXT("ComboGraphEditorApp"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, InitComboGraph, false);

}

FName FComboGraphEditor::GetToolkitFName() const
{
	return FName("ComboGraphEditor");
}

FText FComboGraphEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "ComboGraph Editor");
}

FString FComboGraphEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "ComboGraph ").ToString();
}

FLinearColor FComboGraphEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FComboGraphEditor::AddReferencedObjects(FReferenceCollector & Collector)
{
	Collector.AddReferencedObject(ComboGraphBeingEdited);
}

void FComboGraphEditor::CreateInternalWidgets() {
	ComboGraphGraphEditor = CreateGraphEditorWidget();
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs Args;
	Args.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	Args.bHideSelectionTip = true;
	//Args.NotifyHook = this;

	
	DetailsView = PropertyModule.CreateDetailView(Args);
	DetailsView->SetObject(ComboGraphBeingEdited);
}

TSharedRef<SGraphEditor> FComboGraphEditor::CreateGraphEditorWidget() {
	if (!GraphEditorCommands.IsValid()) {
		GraphEditorCommands = MakeShareable(new FUICommandList);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FComboGraphEditor::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FComboGraphEditor::CanDeleteNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Rename,
			FExecuteAction::CreateSP(this, &FComboGraphEditor::OnRenameNode),
			FCanExecuteAction::CreateSP(this, &FComboGraphEditor::CanRenameNode)
		);

		GraphEditorCommands->MapAction(FComboGraphEditorCommands::Get().MakeRoot,
			FExecuteAction::CreateSP(this, &FComboGraphEditor::OnMakeRootNode),
			FCanExecuteAction::CreateSP(this, &FComboGraphEditor::CanMakeRootNode)
		);
	}
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_ComboGraph", "COMBO GRAPH");

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FComboGraphEditor::OnSelectedNodesChanged);
	InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FComboGraphEditor::OnNodeTitleCommitted);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(ComboGraphBeingEdited->GetGraph())
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

TSharedRef<SDockTab> FComboGraphEditor::SpawnTab_GraphCanvas(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == GraphCanvasTabId);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ComboGraphCanvasTitle", "Viewport"));

	if (ComboGraphGraphEditor.IsValid())
	{
		SpawnedTab->SetContent(ComboGraphGraphEditor.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FComboGraphEditor::SpawnTab_Properties(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == PropertiesTabId);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("ComboGraphDetailsTitle", "Details"))
		[
			DetailsView.ToSharedRef()
		];
}

UCGNode * FComboGraphEditor::CreateNewUCGNode(UClass * NewExpressionClass, const FVector2D & NodePos, bool bAutoSelect, bool bAutoAssignResource)
{
	//TODO finish this function
	UCGNode* newAssetNode = NULL;

	const FScopedTransaction Transaction(NSLOCTEXT("FightMakerEditor", "ComboGraphEditorNewNode", "Combo Graph Editor: New Node"));
	ComboGraphBeingEdited->Modify();

	//Call function to create UCGNode here

	if (newAssetNode)
	{
		//call function in the graph to add the node to the graph
	}

	/*
	const FScopedTransaction Transaction( NSLOCTEXT("UnrealEd", "MaterialEditorNewExpression", "Material Editor: New Expression") );
		Material->Modify();

		UObject* SelectedAsset = nullptr;
		if (bAutoAssignResource)
		{
			// Load selected assets
			FEditorDelegates::LoadSelectedAssetsIfNeeded.Broadcast();
			SelectedAsset = GEditor->GetSelectedObjects()->GetTop<UObject>();
		}

		NewExpression = UMaterialEditingLibrary::CreateMaterialExpressionEx(Material, MaterialFunction, NewExpressionClass, SelectedAsset, NodePos.X, NodePos.Y);

		if (NewExpression)
		{
			Material->MaterialGraph->AddExpression(NewExpression, bAutoSelect);

			// Select the new node.
			if ( bAutoSelect )
			{
				GraphEditor->SetNodeSelection(NewExpression->GraphNode, true);
			}
		}
		Material->MarkPackageDirty();

	RefreshExpressionPreviews();
	GraphEditor->NotifyGraphChanged();
	SetMaterialDirty();
	MaterialParametersOverviewWidget->UpdateEditorInstance(MaterialEditorInstance);
	return NewExpression;
	*/
	ComboGraphBeingEdited->MarkPackageDirty();

	return newAssetNode;
}

FGraphPanelSelectionSet FComboGraphEditor::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	if (ComboGraphGraphEditor.IsValid())
	{
		CurrentSelection = ComboGraphGraphEditor->GetSelectedNodes();
	}
	return CurrentSelection;
}


void FComboGraphEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	if (DetailsView.IsValid()) {
		TArray<UObject*> Selection;

		if (NewSelection.Num()) {
			for (TSet<class UObject*>::TConstIterator SetIt(NewSelection); SetIt; ++SetIt)
			{
				if (UComboGraphNode_Base* GraphNode = Cast <UComboGraphNode_Base>(*SetIt)) {
					Selection.Add(GraphNode->Node);
				}
			}
		}
		else {
			Selection.Add(ComboGraphBeingEdited);
		}
		DetailsView->SetObjects(Selection);
	}
}

void FComboGraphEditor::OnNodeTitleCommitted(const FText & NewText, ETextCommit::Type CommitInfo, UEdGraphNode * NodeBeingChanged)
{
	if (NodeBeingChanged)
	{
		const FScopedTransaction Transaction(LOCTEXT("RenameNode", "Rename Node"));
		NodeBeingChanged->Modify();
		NodeBeingChanged->OnRenameNode(NewText.ToString());
	}
}

void FComboGraphEditor::DeleteSelectedNodes()
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "ComboGraphEditorDeleteSelectedNode", "Delete Selected Node"));
	
	ComboGraphGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	ComboGraphGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);

		if (Node->CanUserDeleteNode())
		{
			if (UComboGraphNode_Base* ComboGraphNode = Cast<UComboGraphNode_Base>(Node))
			{
				UCGNode* DelNode = ComboGraphNode->Node;

				FBlueprintEditorUtils::RemoveNode(NULL, ComboGraphNode, true);

				//TODO: call the recompile graph method here once written

				ComboGraphBeingEdited->RemoveNodeFromBase(DelNode);

				ComboGraphBeingEdited->MarkPackageDirty();
			}
			else {
				FBlueprintEditorUtils::RemoveNode(NULL, Node, true);
			}
		}
	}
}

bool FComboGraphEditor::CanDeleteNodes()
{
	return GetSelectedNodes().Num() > 0;
}


//this shameless compy broght to you by datapreppipelineeditor
void FComboGraphEditor::OnRenameNode()
{
	if (ComboGraphGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode != NULL && SelectedNode->bCanRenameNode)
			{
				ComboGraphGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

//this shameless compy broght to you by datapreppipelineeditor
bool FComboGraphEditor::CanRenameNode() const
{
	
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	if (SelectedNodes.Num() == 1)
	{
		if (UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*SelectedNodes.CreateConstIterator()))
		{
			return SelectedNode->bCanRenameNode;
		}
	}

	return false;
}

void FComboGraphEditor::OnMakeRootNode()
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UComboGraphNode_Base* SelectedNode = Cast<UComboGraphNode_Base>(*NodeIt);
		if (SelectedNode != NULL)
		{
			SelectedNode->MakeRootNode();
			//TODO: Make the node a root node
			//ComboGraphGraphEditor->IsNodeTitleVisible(SelectedNode, true);
			break;
		}
	}
}


bool FComboGraphEditor::CanMakeRootNode()
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	if (SelectedNodes.Num() == 1)
	{
		if (UComboGraphNode_Base* SelectedNode = Cast<UComboGraphNode_Base>(*SelectedNodes.CreateConstIterator()))
		{
			return SelectedNode->Node && SelectedNode->Node->bCanBeRoot && !SelectedNode->Node->bIsRoot;
		}
	}
	return false;
}

void FComboGraphEditor::PostUndo(bool bSuccess)
{
	if (ComboGraphGraphEditor.IsValid())
	{
		ComboGraphGraphEditor->ClearSelectionSet();
		ComboGraphGraphEditor->NotifyGraphChanged();
	}
}

void FComboGraphEditor::NotifyPostChange(const FPropertyChangedEvent & PropertyChangedEvent, UProperty * PropertyThatChanged)
{
	if (ComboGraphGraphEditor.IsValid() && PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
	{
		ComboGraphGraphEditor->NotifyGraphChanged();
	}
}
#undef LOCTEXT_NAMESPACE