
#include "ComboGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "ComboGraphEditorCommands"

void FComboGraphEditorCommands::RegisterCommands()
{
	UI_COMMAND(MakeRoot, "Make Root", "Makes this node the root of a series of attacks", EUserInterfaceActionType::Button, FInputChord());
}
#undef LOCTEXT_NAMESPACE