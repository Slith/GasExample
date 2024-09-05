// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class GasExample : ModuleRules
{
	public GasExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"CoreUObject",
			"DeveloperSettings",
			"Engine",
			"EnhancedInput",
			"GameplayAbilities",
			"GameplayTags",
			"InputCore"
		});

		PrivateIncludePaths.AddRange(new string[]
		{
			"GasExample",
			"GasExample/GAS"
		});

	}
}
