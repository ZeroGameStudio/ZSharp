// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class CompilationUnit
{
	
	public void AddHeaderBlock(string block) => _headerBlocks.Add(new(block));
	public void AddUsing(UsingStatement usingStatement) => _usings.Add(usingStatement);
	public void AddType(TypeDefinitionBase typeDefinition) => _types.Add(typeDefinition);
	public void AddEndBlock(string block) => _endBlocks.Add(new(block));

	public IReadOnlyList<Block> HeaderBlocks => _headerBlocks;
	public IReadOnlyList<UsingStatement> Usings => _usings;
	public NamespaceStatement? Namespace { get; set; }
	public IReadOnlyList<TypeDefinitionBase> Types => _types;
	public IReadOnlyList<Block> EndBlocks => _endBlocks;
	
	private readonly List<Block> _headerBlocks = new();
	private readonly List<UsingStatement> _usings = new();
	private readonly List<TypeDefinitionBase> _types = new();
	private readonly List<Block> _endBlocks = new();

}


