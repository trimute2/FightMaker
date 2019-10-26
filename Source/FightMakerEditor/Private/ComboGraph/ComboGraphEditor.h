#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "GraphEditor.h"

class UComboGraph;
class UEdGraph;

class FComboGraphEditor : public FAssetEditorToolkit, public FGCObject
{
public:
	FComboGraphEditor();
	~FComboGraphEditor();

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	// End of IToolkit interface

	// FAssetEditorToolkit
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// End of FAssetEditorToolkit

	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FSerializableObject interface

	virtual TSet<UObject*> GetSelectedNodes() const;

public:
	void InitComboGraphEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UComboGraph* InitComboGraph);
protected:
	UComboGraph* ComboGraphBeingEdited;

	//delegates for graph editor commands
	void DeleteSelectedNodes();
	bool CanDeleteNodes();

	void OnRenameNode();
	bool CanRenameNode() const;

	void OnMakeRootNode();
	bool CanMakeRootNode();
	/** New Graph Editor */
	TSharedPtr<SGraphEditor> ComboGraphGraphEditor;

	TSharedPtr<class IDetailsView> DetailsView;


private:
	void CreateInternalWidgets();

	TSharedRef<SGraphEditor> CreateGraphEditorWidget();

	TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);

	TSharedRef<SDockTab> SpawnTab_Properties(const FSpawnTabArgs& Args);

	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);

	/**
	 * Called when a node's title is committed for a rename
	 *
	 * @param	NewText				New title text
	 * @param	CommitInfo			How text was committed
	 * @param	NodeBeingChanged	The node being changed
	 */
	void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);

	/** Command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;

	static const FName GraphCanvasTabId;

	static const FName PropertiesTabId;
};