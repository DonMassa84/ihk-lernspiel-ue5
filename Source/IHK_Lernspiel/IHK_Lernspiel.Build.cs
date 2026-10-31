// Copyright 2024-2026 Daniel Massa. All Rights Reserved.

using UnrealBuildTool;

public class IHK_Lernspiel : ModuleRules
{
    public IHK_Lernspiel(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "DataTable",
            "Json",
            "Http",
            "Slate",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore"
        });

        // WebGL specific settings
#if !UE_5_4_OR_LATER
        if (Target.Platform == UnrealTargetPlatform.HTML5)
        {
            PublicDefinitions.Add("WITH_WEBGL=1");
        }
#endif

        // Enable C++17 for modern features
        CppStandard = CppStandardVersion.Cpp17;
    }
}