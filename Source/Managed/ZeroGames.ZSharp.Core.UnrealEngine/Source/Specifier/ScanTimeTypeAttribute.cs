// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Core.UnrealEngine.Specifier;

[AttributeUsage(AttributeTargets.Property)]
public class ScanTimeTypeAttribute : Attribute
{

	public ScanTimeTypeAttribute(string propertyName)
	{
		PropertyName = propertyName;
		ConstructorArgumentIndex = -1;
	}

	public ScanTimeTypeAttribute(int32 constructorArgumentIndex)
	{
		PropertyName = string.Empty;
		ConstructorArgumentIndex = constructorArgumentIndex;
	}
	
	public string PropertyName { get; }
	public int32 ConstructorArgumentIndex { get; }

}


