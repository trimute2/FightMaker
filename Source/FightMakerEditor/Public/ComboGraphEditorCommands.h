#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorStyleSet.h"

class FComboGraphEditorCommands : public TCommands< FComboGraphEditorCommands> {
public:
	FComboGraphEditorCommands()
		: TCommands<FComboGraphEditorCommands>("ComboGraphEditor", NSLOCTEXT("Contexts", "ComboGraphEditor", "Combo Graph Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	TSharedPtr<FUICommandInfo> MakeRoot;

	TSharedPtr<FUICommandInfo> RemoveFromRoot;

	virtual void RegisterCommands() override;
};