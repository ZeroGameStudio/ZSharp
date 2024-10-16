// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal abstract class TypeGeneratorBase<TDefinition> where TDefinition : TypeDefinitionBase
{

	public string Generate(TDefinition definition)
	{
		StringBuilder sb = new();

		if (definition.Attributes.Count > 0)
		{
			sb.AppendLine(string.Join("\n", definition.Attributes.Select(_attributeListGenerator.Generate)));
		}

		string visibility = GetVisibilityText(definition);
		string modifiers = GetModifiersText(definition);
		string kind = GetTypeKindText(definition);
		string name = definition.Name;
		string baseList = definition.BaseTypes.Count > 0 ? $": {string.Join(", ", definition.BaseTypes)}" : string.Empty;
		string[] decls = [ visibility, modifiers, kind, name, baseList ];
		
		string typeDecl = $"{string.Join(" ", decls.Where(decl => !string.IsNullOrWhiteSpace(decl)))}";
		sb.Append(typeDecl);
		
		string typeBody = GenerateBody(definition);
		if (!string.IsNullOrWhiteSpace(typeBody))
		{
			sb.Append(
$@"
{{
{typeBody.Indent(1)}
}}");
		}
		else
		{
			sb.Append(';');
		}

		return sb.ToString();
	}

	protected abstract string GenerateBody(TDefinition definition);
	
	private string GetVisibilityText(TypeDefinitionBase definition) => definition.Visibility switch
	{
		EMemberVisibility.Public => "public",
		EMemberVisibility.Internal => "internal",
		EMemberVisibility.Protected => "protected",
		EMemberVisibility.Private => "private",
		_ => throw new NotSupportedException()
	};

	private string GetModifiersText(TypeDefinitionBase definition)
	{
		List<string> modifiers = new();
		
		if ((definition.Modifiers & ETypeModifiers.Abstract) != ETypeModifiers.None)
		{
			modifiers.Add("abstract");
		}
		
		if ((definition.Modifiers & ETypeModifiers.Sealed) != ETypeModifiers.None)
		{
			modifiers.Add("sealed");
		}
		
		if ((definition.Modifiers & ETypeModifiers.Static) != ETypeModifiers.None)
		{
			modifiers.Add("static");
		}
		
		if ((definition.Modifiers & ETypeModifiers.Unsafe) != ETypeModifiers.None)
		{
			modifiers.Add("unsafe");
		}
		
		if ((definition.Modifiers & ETypeModifiers.Partial) != ETypeModifiers.None)
		{
			modifiers.Add("partial");
		}

		return string.Join(" ", modifiers);
	}

	private string GetTypeKindText(TypeDefinitionBase definition) => definition.Kind switch
	{
		ETypeKind.Class => "class",
		ETypeKind.Interface => "interface",
		ETypeKind.Enum => "enum",
		_ => throw new NotSupportedException()
	};

	private readonly AttributeListGenerator _attributeListGenerator = new();

}


