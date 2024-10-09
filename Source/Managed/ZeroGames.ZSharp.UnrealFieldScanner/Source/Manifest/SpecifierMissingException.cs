// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

public class SpecifierMissingException : Exception
{
	
	public SpecifierMissingException(Type sourceSpecifierType, Type[] missingSpecifierTypes, MethodInfo declaringMethod, UnrealFieldDefinition fieldDef)
		: base(BuildMessage(sourceSpecifierType, missingSpecifierTypes, declaringMethod, fieldDef))
	{
		SourceSpecifierType = sourceSpecifierType;
		MissingSpecifierTypes = (Type[])missingSpecifierTypes.Clone();
		DeclaringMethod = declaringMethod;
		FieldDefinition = fieldDef;
	}

	public SpecifierMissingException(Type sourceSpecifierType, Type[] missingSpecifierTypes, MethodInfo declaringMethod, UnrealFieldDefinition fieldDef, Exception inner)
		: base(BuildMessage(sourceSpecifierType, missingSpecifierTypes, declaringMethod, fieldDef), inner)
	{
		SourceSpecifierType = sourceSpecifierType;
		MissingSpecifierTypes = (Type[])missingSpecifierTypes.Clone();
		DeclaringMethod = declaringMethod;
		FieldDefinition = fieldDef;
	}

	public Type SourceSpecifierType { get; }
	public Type[] MissingSpecifierTypes { get; }
	public MethodInfo DeclaringMethod { get; }
	public UnrealFieldDefinition FieldDefinition { get; }

	private static string BuildMessage(Type sourceSpecifierType, Type[] missingSpecifierTypes, MethodInfo declaringMethod, UnrealFieldDefinition fieldDef)
	{
		string sourceSpecifierTypeName = sourceSpecifierType.Name;
		string missingSpecifierTypeNames = string.Join(", ", missingSpecifierTypes.Select(t => t.Name));
		string processorParameters = string.Join(", ", declaringMethod.GetParameters().Select(p => p.ParameterType.Name));
		string processorSignature = $"{declaringMethod.ReturnType.Name} {declaringMethod.DeclaringType?.FullName}.{declaringMethod.Name}({processorParameters})";
		return $"Specifier [{sourceSpecifierTypeName}] on field [{fieldDef.GetDisplayName()}] requires missing [{missingSpecifierTypeNames}] declared by processor [{processorSignature}].";
	}
	
}