// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public class SpecifierConflictException : Exception
{

	public SpecifierConflictException(Type sourceSpecifierType, Type[] conflictSpecifierTypes, MethodInfo declaringMethod, UnrealFieldDefinition fieldDef)
		: base(BuildMessage(sourceSpecifierType, conflictSpecifierTypes, declaringMethod, fieldDef))
	{
		SourceSpecifierType = sourceSpecifierType;
		ConflictSpecifierTypes = (Type[])conflictSpecifierTypes.Clone();
		DeclaringMethod = declaringMethod;
	}

	public SpecifierConflictException(Type sourceSpecifierType, Type[] conflictSpecifierTypes, MethodInfo declaringMethod, UnrealFieldDefinition fieldDef, Exception inner)
		: base(BuildMessage(sourceSpecifierType, conflictSpecifierTypes, declaringMethod, fieldDef), inner)
	{
		SourceSpecifierType = sourceSpecifierType;
		ConflictSpecifierTypes = (Type[])conflictSpecifierTypes.Clone();
		DeclaringMethod = declaringMethod;
	}

	public Type SourceSpecifierType { get; }
	public Type[] ConflictSpecifierTypes { get; }
	public MethodInfo DeclaringMethod { get; }

	private static string BuildMessage(Type sourceSpecifierType, Type[] conflictSpecifierTypes, MethodInfo declaringMethod, UnrealFieldDefinition fieldDef)
	{
		string sourceSpecifierTypeName = sourceSpecifierType.Name;
		string conflictSpecifierTypeNames = string.Join(", ", conflictSpecifierTypes.Select(t => t.Name));
		string processorParameters = string.Join(", ", declaringMethod.GetParameters().Select(p => p.ParameterType.Name));
		string processorSignature = $"{declaringMethod.ReturnType.Name} {declaringMethod.DeclaringType?.FullName}.{declaringMethod.Name}({processorParameters})";
		return $"Specifier [{sourceSpecifierTypeName}] on field [{fieldDef.GetDisplayName()}] conflicts with [{conflictSpecifierTypeNames}] declared by processor [{processorSignature}].";
	}
	
}


