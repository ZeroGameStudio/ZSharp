// Copyright Zero Games. All Rights Reserved.

using ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

namespace ZeroGames.ZSharp.UnrealEngine.CoreUObject;

[UnrealFieldPath(__UNREAL_FIELD_PATH)]
[ConjugateKey("Unreal.DynamicMulticastInlineDelegate")]
public sealed class DynamicUnrealMulticastInlineDelegate : UnrealScriptStructBase
	, IConjugate<DynamicUnrealMulticastInlineDelegate>
	, IStaticUnrealFieldPath
	, IStaticSignature
{
	public static DynamicUnrealMulticastInlineDelegate BuildConjugate(IntPtr unmanaged) => new(unmanaged);
	
	public static string StaticUnrealFieldPath => throw new NotSupportedException();
	public static UDelegateFunction StaticSignature => throw new NotSupportedException();
	public override string UnrealFieldPath => throw new NotSupportedException();
	
	private DynamicUnrealMulticastInlineDelegate(IntPtr unmanaged) : base(unmanaged){}
	private const string __UNREAL_FIELD_PATH = "";
}


