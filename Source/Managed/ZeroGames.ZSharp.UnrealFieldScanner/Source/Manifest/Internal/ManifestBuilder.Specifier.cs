// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Reflection;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{
	
	private void ScanSpecifierProcessors()
	{
		Type actionGenericType = typeof(Action<,,>);

		Type[] processorTypes = [ typeof(SpecifierProcessor) ];
		foreach (var processorType in processorTypes)
		{
			object? processorInstance = processorType == typeof(ManifestBuilder) ? this : !processorType.IsAbstract ? Activator.CreateInstance(processorType) : null;
			foreach (var method in processorType.GetMethods(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance))
			{
				if (method.GetCustomAttribute<SpecifierProcessorAttribute>() is {} attribute)
				{
					ParameterInfo[] parameters = method.GetParameters();
					if (parameters.Length != 3)
					{
						throw new InvalidOperationException();
					}

					if (!parameters[0].ParameterType.IsAssignableTo(typeof(UnrealFieldDefinition)))
					{
						throw new InvalidOperationException();
					}

					if (!parameters[1].ParameterType.IsAssignableTo(typeof(ISpecifierProvider)))
					{
						throw new InvalidOperationException();
					}

					Type specifierParameterType = parameters[2].ParameterType;
					if (!specifierParameterType.IsAssignableTo(typeof(IUnrealReflectionSpecifier)))
					{
						throw new InvalidOperationException();
					}

					if (!_specifierProcessorMap.TryGetValue(specifierParameterType, out var recs))
					{
						recs = new();
						_specifierProcessorMap[specifierParameterType] = recs;
					}
					
					Type newProcessorType = actionGenericType.MakeGenericType(method.GetParameters().Select(p => p.ParameterType).ToArray());
					Delegate processor = Delegate.CreateDelegate(newProcessorType, processorInstance, method);
					recs.Add(new(processor, attribute));
				}
			}
		}
	}

	private void ProcessSpecifiers(UnrealFieldDefinition fieldDef, ISpecifierProvider specifierProvider)
	{
		Type[] allSpecifierTypes = specifierProvider.Specifiers.Distinct().Select(spec => spec.GetType()).ToArray();
		
		object[] processorParameters = [ fieldDef, specifierProvider, null! ];
		foreach (var specifier in specifierProvider.Specifiers)
		{
			Type specifierType = specifier.GetType();
			if (_specifierProcessorMap.TryGetValue(specifierType, out var recs))
			{
				processorParameters[2] = specifier;
				foreach (var rec in recs)
				{
					Type[] hierarchicalRequiredSpecifierTypes = specifier.HierarchicalRequirements.Concat(rec.Attribute.HierarchicalRequirements).ToArray();
					Type[] exactRequiredSpecifierTypes = specifier.ExactRequirements.Concat(rec.Attribute.ExactRequirements).ToArray();
					if (TryGetSpecifierMissings(allSpecifierTypes, specifierType, hierarchicalRequiredSpecifierTypes, exactRequiredSpecifierTypes, out var missings))
					{
						throw new SpecifierMissingException(specifierType, missings, rec.Processor.Method, fieldDef);
					}
					
					Type[] hierarchicalConflictSpecifierTypes = specifier.HierarchicalConflicts.Concat(rec.Attribute.HierarchicalConflicts).ToArray();
					Type[] exactConflictSpecifierTypes = specifier.ExactConflicts.Concat(rec.Attribute.ExactConflicts).ToArray();
					if (TryGetSpecifierConflicts(allSpecifierTypes, specifierType, hierarchicalConflictSpecifierTypes, exactConflictSpecifierTypes, out var conflicts))
					{
						throw new SpecifierConflictException(specifierType, conflicts, rec.Processor.Method, fieldDef);
					}
					
					rec.Processor.DynamicInvoke(processorParameters);
				}
			}
		}
	}

	private bool TryGetSpecifierMissings(Type[] allSpecifierTypes, Type currentSpecifierType, Type[] hierarchicalRequiredSpecifierTypes, Type[] exactRequiredSpecifierTypes, [NotNullWhen(true)] out Type[]? missings)
	{
		List<Type>? result = null;
		
		foreach (var requiredSpecifierType in hierarchicalRequiredSpecifierTypes)
		{
			bool miss = true;
			foreach (var specifierTypeToTest in allSpecifierTypes)
			{
				if (specifierTypeToTest == currentSpecifierType)
				{
					continue;
				}
				
				if (specifierTypeToTest.IsAssignableTo(requiredSpecifierType))
				{
					miss = false;
					break;
				}
			}
			
			if (miss)
			{
				result ??= new();
				result.Add(requiredSpecifierType);
			}
		}
		
		foreach (var requiredSpecifierType in exactRequiredSpecifierTypes)
		{
			bool miss = true;
			foreach (var specifierTypeToTest in allSpecifierTypes)
			{
				if (specifierTypeToTest == currentSpecifierType)
				{
					continue;
				}
				
				if (specifierTypeToTest == requiredSpecifierType)
				{
					miss = false;
					break;
				}
			}
			
			if (miss)
			{
				result ??= new();
				result.Add(requiredSpecifierType);
			}
		}

		missings = result?.ToArray();
		return missings is not null;
	}

	private bool TryGetSpecifierConflicts(Type[] allSpecifierTypes, Type currentSpecifierType, Type[] hierarchicalConflictSpecifierTypes, Type[] exactConflictSpecifierTypes, [NotNullWhen(true)] out Type[]? conflicts)
	{
		List<Type>? result = null;
		
		foreach (var specifierTypeToTest in allSpecifierTypes)
		{
			if (specifierTypeToTest == currentSpecifierType)
			{
				continue;
			}
			
			bool broken = false;
			foreach (var conflictSpecifierType in hierarchicalConflictSpecifierTypes)
			{
				if (specifierTypeToTest.IsAssignableTo(conflictSpecifierType))
				{
					result ??= new();
					result.Add(specifierTypeToTest);
					broken = true;
					break;
				}
			}

			if (broken)
			{
				continue;
			}
						
			foreach (var conflictSpecifierType in exactConflictSpecifierTypes)
			{
				if (specifierTypeToTest == conflictSpecifierType)
				{
					result ??= new();
					result.Add(specifierTypeToTest);
					break;
				}
			}
		}

		conflicts = result?.ToArray();
		return conflicts is not null;
	}

	private readonly record struct ProcessorRec(Delegate Processor, SpecifierProcessorAttribute Attribute);
	
	private readonly Dictionary<Type, List<ProcessorRec>> _specifierProcessorMap = new();
	
}


