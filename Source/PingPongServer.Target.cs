using UnrealBuildTool;
using System.Collections.Generic;
public class PingPongServerTarget : TargetRules
{
public PingPongServerTarget( TargetInfo Target) : base(Target)
{
Type = TargetType.Server;
ExtraModuleNames.AddRange( new string[] { "PingPong" } );
}
}
