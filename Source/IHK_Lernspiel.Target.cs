// Copyright 2024-2026 Daniel Massa. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class IHK_LernspielTarget : TargetRules
{
    public IHK_LernspielTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("IHK_Lernspiel");
        
        // WebGL optimizations
#if !UE_5_4_OR_LATER
        if (Platform == UnrealTargetPlatform.HTML5)
        {
            bUseUnityBuild = true;
            bUsePCHFiles = true;
            bOmitPCDebugInfoInDevelopment = true;
        }
#endif
    }
}