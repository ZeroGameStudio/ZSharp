// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	private UnrealDelegateDefinition ScanDelegateModel(IUnrealDelegateModel delegateModel)
	{
		UnrealDelegateDefinition result = new()
		{
			Name = $"{delegateModel.Name}__DelegateSignature",
			DelegateType = delegateModel.BaseType!.Value.FullName switch
			{
				UNREAL_DELEGATE_BASE_TYPE_FULL_NAME => EDelegateType.Unicast,
				UNREAL_MULTICAST_INLINE_DELEGATE_BASE_TYPE_FULL_NAME => EDelegateType.Multicast,
				UNREAL_MULTICAST_SPARSE_DELEGATE_BASE_TYPE_FULL_NAME => EDelegateType.Sparse,
				_ => throw new NotSupportedException()
			},
			OuterClassName = delegateModel.Outer?.Name,
		};

		ProcessSpecifiers(result, delegateModel);
		
		ScanUProperties(result, delegateModel);

		if (delegateModel.Outer is not null) // @FIXME: I don't know wtf this is alright...
		{
			// Check for non-return out param.
			// IMPORTANT: Keep sync with function.
			if (result.Properties.Any(p => (p.PropertyFlags & (EPropertyFlags.OutParm | EPropertyFlags.ReturnParm)) == EPropertyFlags.OutParm))
			{
				result.FunctionFlags |= EFunctionFlags.HasOutParms;
			}
		}

		return result;
	}
	
}


