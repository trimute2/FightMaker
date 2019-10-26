// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Modules/ModuleManager.h"

class IFightMakerEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	//virtual void StartupModule() override;
	//virtual void ShutdownModule() override;

	virtual uint32 GetFightMakerAssetCategory() const = 0;
};
