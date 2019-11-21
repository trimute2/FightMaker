// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIGraphTypes.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "ComboGraphAssetTypeActions.h"

#include "Modules/ModuleManager.h"

class IFightMakerEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	//virtual void StartupModule() override;
	//virtual void ShutdownModule() override;

	virtual uint32 GetFightMakerAssetCategory() const = 0;

	virtual void InitClassCache() = 0;

	TSharedPtr<struct FGraphNodeClassHelper> GetClassCache() { return ClassCache; }

protected:

	TSharedPtr<struct FGraphNodeClassHelper> ClassCache;

};

class FFightMakerEditorModule : public IFightMakerEditorModule {
public:
	FFightMakerEditorModule() :
		FightMakerAssetCategoryBit(EAssetTypeCategories::Misc)
	{}
	virtual uint32 GetFightMakerAssetCategory() const override { return FightMakerAssetCategoryBit; }
	virtual void InitClassCache() override;
private:

	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type FightMakerAssetCategoryBit;
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OnPostEngineInit();
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

};