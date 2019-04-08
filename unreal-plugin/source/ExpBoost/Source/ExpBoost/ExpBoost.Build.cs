using UnrealBuildTool;

public class ExpBoost : ModuleRules
{
    public ExpBoost(TargetInfo target)
    {
       PrivateIncludePaths.AddRange(new string[] { "LevelBox/Private" });
      
       PublicDependencyModuleNames.AddRange( new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd" } );
    }
}