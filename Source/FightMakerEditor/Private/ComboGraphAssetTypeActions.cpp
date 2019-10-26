
#include "ComboGraphAssetTypeActions.h"
#include "ComboGraph/ComboGraphEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ComboGraph/ComboGraph.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

FComboGraphAssetTypeActions::FComboGraphAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FComboGraphAssetTypeActions::GetName() const {
	return LOCTEXT("FComboGraphAssetTypeActionsName", "ComboGraph");
}

FColor FComboGraphAssetTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FComboGraphAssetTypeActions::GetSupportedClass() const
{
	return UComboGraph::StaticClass();
}

void FComboGraphAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UComboGraph* ComboGraph = Cast<UComboGraph>(*ObjIt))
		{
			TSharedRef<FComboGraphEditor> NewComboGraphEditor(new FComboGraphEditor());
			NewComboGraphEditor->InitComboGraphEditor(Mode, EditWithinLevelEditor, ComboGraph);
		}
	}
}

uint32 FComboGraphAssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
