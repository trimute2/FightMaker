// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FightMakerEditor : ModuleRules
{
	public FightMakerEditor(ReadOnlyTargetRules Target) : base(Target)
	{

		PrivateIncludePaths.Add("FightMakerEditor/Private");

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				//"AIModule"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"AssetTools",
				//"AIModule"
			}
			);
			
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"FightMaker",
				"ContentBrowser",
				"EditorStyle",
				"InputCore",
				"Projects",
				"PropertyEditor",
				"UnrealEd",
				"GraphEditor",
				"AIModule",
				"AIGraph"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				"AssetTools"
			}
			);
	}
}
