// ©Justin Camden 2019, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class HorusServerTarget : TargetRules
{
    public HorusServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;

        ExtraModuleNames.AddRange(new string[] { "Horus" });
    }
}
