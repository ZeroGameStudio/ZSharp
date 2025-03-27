// Copyright Zero Games. All Rights Reserved.

using Microsoft.CodeAnalysis;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Emit.SourceGenerator.CSharp;

public static class EmitGeneratorHelper
{
	
	public static EParameterKind RefKindToParameterKind(RefKind refKind) => refKind switch
	{
		RefKind.None or RefKind.In => EParameterKind.In,
		RefKind.Out => EParameterKind.Out,
		RefKind.Ref or RefKind.RefReadOnlyParameter => EParameterKind.Ref,
		_ => throw new NotSupportedException()
	};

	public static EMemberVisibility AccessibilityToMemberVisibility(Accessibility accessibility) => accessibility switch
	{
		Accessibility.Public => EMemberVisibility.Public,
		Accessibility.Protected => EMemberVisibility.Protected,
		_ => EMemberVisibility.Private
	};

	public static string GetNormalizedTypeName(ITypeSymbol type)
	{
		string name = type.Name switch
		{
			nameof(Byte) => nameof(uint8),
			nameof(UInt16) => nameof(uint16),
			nameof(UInt32) => nameof(uint32),
			nameof(UInt64) => nameof(uint64),
			nameof(SByte) => nameof(int8),
			nameof(Int16) => nameof(int16),
			nameof(Int32) => nameof(int32),
			nameof(Int64) => nameof(int64),
			nameof(Single) => "float",
			nameof(Double) => "double",
			nameof(Boolean) => "bool",
			_ => string.Empty
		};

		if (name == string.Empty)
		{
			name = type.Name + (type.NullableAnnotation == NullableAnnotation.Annotated ? "?" : string.Empty);
			if (type is INamedTypeSymbol namedType && namedType.IsGenericType)
			{
				name += $"<{string.Join(", ", namedType.TypeArguments.Select(GetNormalizedTypeName))}>";
			}
		}

		return name;
	}

	public static string GetTypeNamespace(ITypeSymbol type) => type.ContainingNamespace.ToString();

	public static void LootNamespace(ITypeSymbol type, ICollection<string> result)
	{
		HashSet<string> lootedTypes = [];
		void Loot(ITypeSymbol cur)
		{
			if (!lootedTypes.Add(cur.MetadataName))
			{
				return;
			}

			result.Add(GetTypeNamespace(cur));
			if (type is INamedTypeSymbol { IsGenericType: true } namedType)
			{
				foreach (var typeArgument in namedType.TypeArguments)
				{
					Loot(typeArgument);
				}
			}
		}
		
		Loot(type);
	}

	public static TypeReference GetTypeReference(ITypeSymbol type) => new(GetNormalizedTypeName(type), type.TypeKind == TypeKind.Enum ? "int64" : null);
	
}


