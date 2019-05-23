// ©Justin Camden 2019, all rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class HorusTarget : TargetRules
{
	public HorusTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Horus" } );
	}
}
