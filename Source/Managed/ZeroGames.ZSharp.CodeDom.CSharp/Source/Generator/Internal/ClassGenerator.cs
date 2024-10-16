// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

internal class ClassGenerator : TypeGeneratorBase<ClassDefinition>
{
	
	protected override string GenerateBody(ClassDefinition definition)
	{
		if (definition.Members.Count == 0)
		{
			return string.Empty;
		}
		
		bool multiMember = definition.Members.Count > 1;
		StringBuilder sb = new();
		foreach (var member in definition.Members)
		{
			string memberBody = string.Empty;
			if (member is Block block)
			{
				_blockGenerator ??= new();
				memberBody = _blockGenerator.Generate(block);
			}
			else if (member is MethodDefinition method)
			{
				_methodGenerator ??= new();
				memberBody = _methodGenerator.Generate(method);
			}
			else if (member is ClassDefinition cls)
			{
				_classGenerator ??= new();
				memberBody = _classGenerator.Generate(cls);
			}

			if (!string.IsNullOrWhiteSpace(memberBody))
			{
				if (multiMember)
				{
					sb.AppendLine();
				}
				sb.Append(memberBody);
				if (multiMember)
				{
					sb.AppendLine();
				}
			}
		}
		
		return sb.ToString();
	}

	private BlockGenerator? _blockGenerator;
	private MethodGenerator? _methodGenerator;
	private ClassGenerator? _classGenerator;

}


