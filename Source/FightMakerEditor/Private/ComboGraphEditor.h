#pragma once

#include "CoreMinimal.h"
#include "UObject/GCObject.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"

class UComboGraph;

class FComboGraphEditor : public FAssetEditorToolkit, public FGCObject {
public:
	FComboGraphEditor();

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	// End of IToolkit interface

	// FAssetEditorToolkit

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	//virtual FText GetToolkitName() const override;
	//virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	//virtual FString GetDocumentationLink() const override;
	//virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit) override;
	//virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit) override;
	// End of FAssetEditorToolkit

	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	// End of FSerializableObject interface

protected:

	UComboGraph* ComboGraph;
};