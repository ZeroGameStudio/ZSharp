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

		return result;
	}
	
}


