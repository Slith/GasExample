// Copyright Epic Games, Inc. All Rights Reserved.

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
			"Engine", 
			"EnhancedInput",
			"GameplayAbilities",
			"InputCore"
		});
	}
}
