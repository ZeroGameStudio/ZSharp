// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public static class DelegateHelper
{
	
	public static ParameterDeclaration ToSignatureParameterDecl(ParameterDeclaration source)
	{
		EParameterKind kind = source.Kind;
		TypeReference sourceType = source.Type;
		string targetTypeName = sourceType.TypeName;
		if (kind is EParameterKind.In or EParameterKind.Ref)
		{
			if (source.Type.IsNullInNotNullOut && targetTypeName.EndsWith("?"))
			{
				targetTypeName = targetTypeName.Substring(0, targetTypeName.Length - 1);
			}
		}
		else if (kind is EParameterKind.Out)
		{
			if (source.Type.IsNullInNotNullOut && !targetTypeName.EndsWith("?"))
			{
				targetTypeName += "?";
			}
		}
		
		AttributeDeclaration[]? targetAttributes = source.Attributes?.Declarations.ToArray();
		if (targetAttributes is not null)
		{
			for (int32 i = 0; i < targetAttributes.Length; ++i)
			{
				if (targetAttributes[i].Name == "NotNull")
				{
					targetAttributes[i] = new("AllowNull");
				}
			}
		}

		TypeReference targetType = new(targetTypeName, sourceType.UnderlyingType, sourceType.IsNullInNotNullOut);
		return new(kind, targetType, source.Name, source.DefaultValue, targetAttributes);
	}
	
}


