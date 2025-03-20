// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedAssembly
{
	public string Name { get; set; } = null!;
	public required List<ExportedEnum> Enums { get; set; }
	public required List<ExportedClass> Classes { get; set; }
	public required List<ExportedDelegate> Delegates { get; set; }
	
	public IEnumerable<ExportedType> ExportedTypes => Classes.AsEnumerable<ExportedType>().Concat(Enums).Concat(Delegates);
}


