// Copyright Zero Games. All Rights Reserved.

using System.Text;
using ZeroGames.ZSharp.CodeDom.CSharp;

namespace ZeroGames.ZSharp.Build.Glue;

public class ClassWriter
{
	
	public ClassWriter(ExportedAssemblyRegistry registry, ExportedClass exportedClass, string directory, string fileName)
	{
		_exportedClass = exportedClass;
		_abstractionFilePath = $"{directory}/{fileName}.Abstraction.g.cs";
		_implementationFilePath = $"{directory}/{fileName}.Implementation.g.cs";
	}

	public void Write()
	{
		Write(true);
		Write(false);
	}

	private void Write(bool abstraction)
	{
		EExportedClassKind classKind = _exportedClass.IsClass ? EExportedClassKind.Class : _exportedClass.IsInterface ? EExportedClassKind.Interface : _exportedClass.IsStruct ? EExportedClassKind.Struct : _exportedClass.IsPlain ? EExportedClassKind.Plain : throw new NotSupportedException();
		ExportedClassBuilder builder = new(abstraction, classKind, _exportedClass.Namespace, _exportedClass.Name, _exportedClass.UnrealFieldPath);
		var usings = NamespaceHelper.LootNamespace(_exportedClass).Where(ns => ns != _exportedClass.Namespace);
		foreach (var ns in usings)
		{
			builder.AddUsingNamespace(ns);
		}

		// Attributes
		if (abstraction && _exportedClass.IsHashableStruct)
		{
			builder.AddAttributeBefore("Hashable");
		}

		if (abstraction && _exportedClass.IsImplementableInterface)
		{
			builder.AddAttributeBefore("Implementable");
		}
		
		if (!abstraction && _exportedClass.ConjugateRegistryId > 0)
		{
			builder.AddAttributeAfter("ConjugateRegistryId", _exportedClass.ConjugateRegistryId.ToString());
		}
		
		// Base type
		if (GetBaseType() is { } baseType)
		{
			bool setBaseTypeInImpl = baseType == UNREAL_CONJUGATE_BASE_TYPE_NAME
			                         || baseType == UNREAL_OBJECT_BASE_TYPE_NAME
			                         || baseType == UNREAL_SCRIPT_STRUCT_BASE_TYPE_NAME;
			if (setBaseTypeInImpl == !abstraction)
			{
				builder.SetBaseType(baseType);
			}
		}
		
		// Interfaces
		if (abstraction)
		{
			foreach (var implementedInterface in _exportedClass.Interfaces)
			{
				builder.AddInterface(implementedInterface.ToString(false));
			}
		}
		
		// Methods
		foreach (var method in _exportedClass.Methods.OrderBy(method => method.IsPublic ? 1 : method.IsProtected ? 2 : 3))
		{
			EMemberVisibility visibility = method.IsPublic ? EMemberVisibility.Public : method.IsProtected ? EMemberVisibility.Protected : EMemberVisibility.Private;

			bool @static = method.IsStatic;
			List<ParameterDeclaration> parameters = new();
			if (_exportedClass.IsImplementableInterface)
			{
				@static = true;
				parameters.Add(new(EParameterKind.In, new("UnrealObject", null), "@this"));
			}

			bool hasOutParameter = method.Parameters.Any(p => p is { IsReturn: false, IsOut: true });
			bool hasDefaultValue = false;
			foreach (var parameter in method.Parameters)
			{
				if (parameter.IsReturn)
				{
					break;
				}
			
				EParameterKind kind = parameter.IsInOut ? EParameterKind.Ref : parameter.IsOut ? EParameterKind.Out : EParameterKind.In;

				ParameterDefaultValue defaultValue = default;
				if (!hasOutParameter)
				{
					defaultValue = abstraction ? new(new(parameter.DefaultValue.Signature.Text), new(parameter.DefaultValue.Body.Text)) : new(default, new(parameter.DefaultValue.Body.Text));
					if (abstraction)
					{
						if (!string.IsNullOrWhiteSpace(defaultValue.Signature.Content))
						{
							hasDefaultValue = true;
						}
						else if (hasDefaultValue)
						{
							defaultValue = new(new("default"), default);
						}
					}
				}
				
				AttributeDeclaration[]? attributes = abstraction && parameter is { IsNullInNotNullOut: true, IsInOut: true } ? [ new("NotNull") ] : null;
				parameters.Add(new(kind, new(parameter.Type.ToString(), parameter.UnderlyingType, parameter.IsNullInNotNullOut), parameter.Name, defaultValue, attributes));
			}

			TypeReference? returnType = method.ReturnParameter?.Type.ToString() is {} returnTypeName ? new(returnTypeName, method.ReturnParameter.UnderlyingType) : null;
			
			builder.AddMethod(visibility, false, @static, method.Name, method.ZCallName, returnType, parameters.ToArray());
			if (!abstraction)
			{
				builder.AddStaticFieldIfNotExists(new("ZCallHandle?", null), $"_zcallHandleFor{method.ZCallName.Split(':').Last()}");
			}

			if (method.IsVirtual && !_exportedClass.IsInterface /* @FIXME: Interface event implementation */)
			{
				// Ignoring abstract allows to call parent on both virtual and abstract functions,
				// providing a unified approach that reduces cognitive load, but would slightly decrease performance.
				bool ignoreAbstract = false;
				string implName = $"{method.Name}_Implementation";
				MethodDefinition? methodDefinition = null;
				// Remove default value info.
				ParameterDeclaration[] eventImplParameters = new ParameterDeclaration[parameters.Count];
				for (int32 i = 0; i < eventImplParameters.Length; ++i)
				{
					ParameterDeclaration parameter = parameters[i];
					eventImplParameters[i] = new(parameter.Kind, parameter.Type, parameter.Name, parameter.Attributes?.Declarations.ToArray() ?? []);
				}
				if (!method.IsAbstract || ignoreAbstract)
				{
					// Virtual function uses uf:/ protocol so we need to manually convert to uf!:/
					string implZCallName = method.ZCallName.Insert(2, "!");
					methodDefinition = builder.AddMethod(EMemberVisibility.Protected, true, false, implName, implZCallName, returnType, eventImplParameters);
					if (!abstraction)
					{
						builder.AddStaticFieldIfNotExists(new("ZCallHandle?", null), $"_zcallHandleFor{method.ZCallName.Split(':').Last()}_Implementation");
					}
				}
				else if (abstraction)
				{
					methodDefinition = builder.AddPureVirtualMethod(EMemberVisibility.Protected, implName, returnType, eventImplParameters);
				}

				if (abstraction)
				{
					// Method name may be aliased so we take original UFunction name from ZCall name.
					string eventNameLiteral = $"\"{method.ZCallName.Split(':').Last()}\"";
					methodDefinition?.AddAttributeListBefore(new AttributeDeclaration("EventImplementation", eventNameLiteral));
				}
			}
		}
		
		// Properties
		foreach (var property in _exportedClass.Properties.OrderBy(prop => prop.IsPublic ? 1 : prop.IsProtected ? 2 : 3))
		{
			EMemberVisibility visibility = property.IsPublic ? EMemberVisibility.Public : property.IsProtected ? EMemberVisibility.Protected : EMemberVisibility.Private;
			builder.AddProperty(visibility, new(property.Type.ToString(), property.UnderlyingType, property.IsNullInNotNullOut), property.Name, property.ZCallName, property.Index, !property.IsWritable, abstraction && property.IsNullInNotNullOut);
			if (!abstraction)
			{
				builder.AddStaticFieldIfNotExists(new("ZCallHandle?", null), $"_zcallHandleFor{property.ZCallName.Split(':').Last()}");
			}
		}
		
		CompilationUnit compilationUnit = builder.Build();
		CSharpGenerator generator = new();
		string content = generator.Generate(compilationUnit);
		
		using StreamWriter sw = new(File.Create(abstraction ? _abstractionFilePath : _implementationFilePath), Encoding.UTF8);
		sw.Write(content);
	}

	private string? GetBaseType()
	{
		if (string.IsNullOrWhiteSpace(_exportedClass.BaseType.Name))
		{
			if (_exportedClass.IsPlain)
			{
				return UNREAL_CONJUGATE_BASE_TYPE_NAME;
			}

			if (_exportedClass.IsClass)
			{
				return UNREAL_OBJECT_BASE_TYPE_NAME;
			}

			if (_exportedClass.IsStruct)
			{
				return UNREAL_SCRIPT_STRUCT_BASE_TYPE_NAME;
			}

			if (_exportedClass.IsInterface)
			{
				return null;
			}

			throw new InvalidOperationException($"Invalid exported class {_exportedClass.Name}");
		}

		return _exportedClass.BaseType.ToString(false);
	}
	
	private const string UNREAL_CONJUGATE_BASE_TYPE_NAME = "UnrealConjugateBase";
	private const string UNREAL_OBJECT_BASE_TYPE_NAME = "UnrealObjectBase";
	private const string UNREAL_SCRIPT_STRUCT_BASE_TYPE_NAME = "UnrealScriptStructBase";
	
	private ExportedClass _exportedClass;
	private string _abstractionFilePath;
	private string _implementationFilePath;

}


