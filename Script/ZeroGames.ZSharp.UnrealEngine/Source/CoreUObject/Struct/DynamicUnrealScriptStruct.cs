// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[UnrealFieldPath(__UNREAL_FIELD_PATH)]
[ConjugateKey("Unreal.DynamicScriptStruct")]
public sealed class DynamicUnrealScriptStruct : UnrealScriptStructBase
	, IConjugate<DynamicUnrealScriptStruct>
	, IStaticUnrealFieldPath
	, IStaticStruct
{
	public static DynamicUnrealScriptStruct BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static string StaticUnrealFieldPath => throw new NotSupportedException();
	public static UScriptStruct StaticStruct => throw new NotSupportedException();
	
	public override string UnrealFieldPath => throw new NotSupportedException();
	
	private DynamicUnrealScriptStruct(IntPtr unmanaged) : base(unmanaged){}
	
	private const string __UNREAL_FIELD_PATH = "";
}


