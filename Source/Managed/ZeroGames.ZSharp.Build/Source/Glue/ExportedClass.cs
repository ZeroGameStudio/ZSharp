// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Glue;

public class ExportedClass : ExportedType
{
	public string BaseType { get; set; } = string.Empty;
	public List<string> Interfaces { get; set; } = new();
	public List<ExportedConstructor> Constructors { get; set; } = new();
	public List<ExportedMethod> Methods { get; set; } = new();
	public List<ExportedProperty> Properties { get; set; } = new();
	public List<ExportedEvent> Events { get; set; } = new();
}


