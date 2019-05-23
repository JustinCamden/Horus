// ©Justin Camden 2019, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HorusEditorTarget : TargetRules
{
    public HorusEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "Horus", "HorusEditor" });
    }

}
