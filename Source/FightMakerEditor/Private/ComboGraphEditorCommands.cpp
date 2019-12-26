
#include "ComboGraphEditorCommands.h"

#define LOCTEXT_NAMESPACE "ComboGraphEditorCommands"

void FComboGraphEditorCommands::RegisterCommands()
{
	UI_COMMAND(MakeRoot, "Make Root", "Makes this node the root of a series of attacks", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(RemoveFromRoot, "Remove From Root", "Remove this node from the root", EUserInterfaceActionType::Button, FInputChord())
}
#undef LOCTEXT_NAMESPACE