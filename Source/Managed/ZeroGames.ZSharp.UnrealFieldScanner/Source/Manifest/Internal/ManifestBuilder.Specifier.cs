// Copyright Zero Games. All Rights Reserved.

using System.Reflection;
using Microsoft.VisualBasic.CompilerServices;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ManifestBuilder
{

	[AttributeUsage(AttributeTargets.Method)]
	private class SpecifierProcessorAttribute : Attribute;

	private void ScanSpecifierProcessors()
	{
		Type actionGenericType = typeof(Action<,,>);
		Type targetType = typeof(ManifestBuilder);
		foreach (var method in targetType.GetMethods(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.Instance))
		{
			if (method.GetCustomAttribute<SpecifierProcessorAttribute>() is not null)
			{
				ParameterInfo[] parameters = method.GetParameters();
				if (parameters.Length != 3)
				{
					throw new InvalidOperationException();
				}

				if (!parameters[0].ParameterType.IsAssignableTo(typeof(UnrealFieldDefinition)))
				{
					throw new IncompleteInitialization();
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
				
				_specifierProcessorMap.TryGetValue(specifierParameterType, out var oldProcessor);
				Type newProcessorType = actionGenericType.MakeGenericType(method.GetParameters().Select(p => p.ParameterType).ToArray());
				Delegate newProcessor = Delegate.CreateDelegate(newProcessorType, this, method);
				Delegate finalProcessor = Delegate.Combine(oldProcessor, newProcessor);
				_specifierProcessorMap[specifierParameterType] = finalProcessor;
			}
		}
	}

	private void ProcessSpecifiers(UnrealFieldDefinition fieldDef, ISpecifierProvider specifierProvider)
	{
		object[] processorParameters = [ fieldDef, specifierProvider, null! ];
		foreach (var specifier in specifierProvider.Specifiers)
		{
			Type specifierType = specifier.GetType();
			if (_specifierProcessorMap.TryGetValue(specifierType, out var processor))
			{
				processorParameters[2] = specifier;
				processor.DynamicInvoke(processorParameters);
			}
		}
	}

	[SpecifierProcessor]
	private void ProcessSpecifier(UnrealFieldDefinition def, IUnrealFieldModel model, UMetaAttribute specifier)
	{
		foreach (var pair in specifier.Pairs)
		{
			string[] kv = pair.Split('=', 2, StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
			string key = kv.Length > 0 ? kv[0] : string.Empty;
			string value = kv.Length > 1 ? kv[1] : string.Empty;
			if (!string.IsNullOrWhiteSpace(key))
			{
				def.MetadataMap[key] = value;
			}
		}
	}
	
	private readonly Dictionary<Type, Delegate> _specifierProcessorMap = new();
	
}


