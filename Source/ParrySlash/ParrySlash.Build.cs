// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ParrySlash : ModuleRules
{
	public ParrySlash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "AIModule","UMG","GameplayAbilities","GameplayTags", "GameplayTasks"});

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        
        // パスにプロジェクト名を追加
        string ProjectName = Target.ProjectFile.GetFileNameWithoutExtension();
        PublicIncludePaths.Add(ProjectName);
    }
}
