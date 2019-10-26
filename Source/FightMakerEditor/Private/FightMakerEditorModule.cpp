// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "FightMakerEditorModule.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "AssetToolsModule.h"
#include "DetailCustomizations/FMBlackboardSelectorDetails.h"
#include "ComboGraphAssetTypeActions.h"

#define LOCTEXT_NAMESPACE "FFightMakerEditorModule"

class FFightMakerEditorModule : public IFightMakerEditorModule {
public:
	FFightMakerEditorModule() :
		FightMakerAssetCategoryBit(EAssetTypeCategories::Misc)
	{}

	virtual uint32 GetFightMakerAssetCategory() const override { return FightMakerAssetCategoryBit; }
private:

	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type FightMakerAssetCategoryBit;
public:
	virtual void StartupModule() override
	{
		//FCoreDelegates::OnPostEngineInit.AddRaw(this, &FFightMakerEditorModule::OnPostEngineInit);
		OnPostEngineInit();
	}

	void OnPostEngineInit() {
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		FightMakerAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("FightMaker")), LOCTEXT("FightMakerAssetCategory", "FightMaker"));

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FComboGraphAssetTypeActions(FightMakerAssetCategoryBit)));

		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout("FMBlackboardKeySelector", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FFMBlackboardSelectorDetails::MakeInstance));
	}

	virtual void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
			{
				AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
			}
		}

		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.UnregisterCustomPropertyTypeLayout("FMBlackboardKeySelector");
		}
		CreatedAssetTypeActions.Empty();
	}

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		CreatedAssetTypeActions.Add(Action);
	}

	/*void RegisterSettings() {
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Editor", "Plugins", "FightMaker",
				LOCTEXT("TextAssetSettingsName", "Text Asset"),
				LOCTEXT("TextAssetSettingsDescription", "Configure the Text Asset plug-in."),
				GetMutableDefault<UTextAssetEditorSettings>()
			);
		}
	}*/
};
	
IMPLEMENT_MODULE(FFightMakerEditorModule, FightMakerEditor)

#undef LOCTEXT_NAMESPACE