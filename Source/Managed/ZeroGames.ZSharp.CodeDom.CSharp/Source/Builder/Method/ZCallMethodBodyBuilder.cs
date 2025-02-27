﻿// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ZCallMethodBodyBuilder(string name, TypeReference? returnType, bool needsUnsafeBlock, params ParameterDeclaration[]? parameters)
{

	public MethodBody Build()
	{
		StringBuilder sb = new();
		
		string shortName = Name.Split(':').Last();
		string implPostfix = IsVirtual ? "_Implementation" : string.Empty;
		string handleFieldName = $"_zcallHandleFor{shortName}{implPostfix}";
		int32 numSlots = (IsStatic ? 0 : 1) + (Parameters?.Count ?? 0) + (ReturnType is null ? 0 : 1);

		string getHandle = 
$@"Thrower.ThrowIfNotInGameThread();

const string ZCALL_NAME = ""{Name}"";
IMasterAssemblyLoadContext __alc__ = MasterAlcCache.Instance;
{handleFieldName} ??= __alc__.GetZCallHandle(ZCALL_NAME);
ZCallHandle __handle__ = {handleFieldName}.Value;";

		sb.Append(getHandle);

		bool needsBuffer = numSlots > 0;
		if (needsBuffer)
		{
			string setupBuffer = 
$@"const int32 NUM_SLOTS = {numSlots};
ZCallBufferSlot* __slots__ = stackalloc ZCallBufferSlot[NUM_SLOTS]
{{
{MakeSlots().Indent()}
}};
ZCallBuffer __buffer__ = new(__slots__, NUM_SLOTS);";

			sb.AppendLine();
			sb.AppendLine();
			sb.Append(setupBuffer);
		}

		sb.AppendLine();
		sb.AppendLine();

		string bufferParameter = needsBuffer ? "&__buffer__" : "null";
		sb.Append(
$@"if (__alc__.ZCall(__handle__, {bufferParameter}) != EZCallErrorCode.Succeed)
{{
	throw new InvalidOperationException();
}}");

		if (needsBuffer)
		{
			sb.Append(MakeCopyOutsAndReturn());
		}

		Block body = NeedsUnsafeBlock ?
$@"unsafe
{{
{sb.ToString().Indent()}
}}" : sb.ToString();

		Block defaultValues = _defaultValueBuilder.Build();
		
		return new(defaultValues, body);
	}

	public string Name { get; } = name;
	public TypeReference? ReturnType { get; } = returnType;
	public bool NeedsUnsafeBlock { get; } = needsUnsafeBlock;
	public IReadOnlyList<ParameterDeclaration>? Parameters { get; } = parameters;

	public bool IsStatic { get; set; }
	public bool IsVirtual { get; set; }

	public Block BeforeReturnBlock { get; set; } = new();

	protected string MakeSlots()
	{
		List<string> slots = new();
		if (!IsStatic)
		{
			slots.Add("ZCallBufferSlot.FromConjugate(this),");
		}

		if (Parameters is not null)
		{
			foreach (var parameter in Parameters)
			{
				string cast = !string.IsNullOrWhiteSpace(parameter.Type.UnderlyingType) ? $"({parameter.Type.UnderlyingType})" : string.Empty;
				string arg = parameter.Kind != EParameterKind.Out ? $"{cast}{parameter.Name}" : string.Empty;
				slots.Add($"ZCallBufferSlot.From{GetSlotType(parameter.Type)}({arg}),");
			}
		}

		if (ReturnType is not null)
		{
			slots.Add($"ZCallBufferSlot.From{GetSlotType(ReturnType.Value)}(),");
		}

		return string.Join(Environment.NewLine, slots);
	}

	protected string MakeCopyOutsAndReturn()
	{
		string result = string.Empty;
		if (Parameters?.Any(p => p.Kind != EParameterKind.In) ?? false)
		{
			result += $"{Environment.NewLine}{Environment.NewLine}";
		}
		
		List<string> copyOuts = new();
		for (int32 i = 0; i < Parameters?.Count; ++i)
		{
			var parameter = Parameters[i];
			if (parameter.Kind == EParameterKind.In)
			{
				continue;
			}

			int32 index = i + (IsStatic ? 0 : 1);
			copyOuts.Add($"{parameter.Name} = {MakeReturnValue(parameter.Type, index.ToString())};");
		}
		
		result += string.Join(Environment.NewLine, copyOuts);
		
		if (BeforeReturnBlock.Content != string.Empty)
		{
			result += $"{Environment.NewLine}{Environment.NewLine}{BeforeReturnBlock}";
		}
		
		if (ReturnType is not null)
		{
			result += $"{Environment.NewLine}{Environment.NewLine}";
			result += $"return {MakeReturnValue(ReturnType.Value, "NUM_SLOTS - 1")};";
		}
		
		return result;
	}

	protected string MakeReturnValue(TypeReference parameterType, string index)
	{
		bool isEnum = !string.IsNullOrWhiteSpace(parameterType.UnderlyingType);
		string cast = isEnum ? $"({parameterType.TypeName})" : string.Empty;
		string slotType = GetSlotType(parameterType);
		bool nullable = parameterType.TypeName.EndsWith("?");
		string notnullType = parameterType.TypeName.TrimEnd('?');
		string getTarget = slotType == "Conjugate" ? nullable && !parameterType.IsNullInNotNullOut ? $".GetTarget<{notnullType}>()" : $".GetTargetChecked<{notnullType}>()" : string.Empty;
		return $"{cast}__slots__[{index}].{slotType}{getTarget}";
	}

	protected string GetSlotType(TypeReference parameterType) => (string.IsNullOrWhiteSpace(parameterType.UnderlyingType) ? parameterType.TypeName : parameterType.UnderlyingType) switch
	{
		nameof(uint8) or nameof(Byte) or "byte" => "UInt8",
		nameof(uint16) or nameof(UInt16) or "ushort" => "UInt16",
		nameof(uint32) or nameof(UInt32) or "uint" => "UInt32",
		nameof(uint64) or nameof(UInt64) or "ulong" => "UInt64",
		nameof(int8) or nameof(SByte) or "sbyte" => "Int8",
		nameof(int16) or nameof(Int16) or "short" => "Int16",
		nameof(int32) or nameof(Int32) or "int" => "Int32",
		nameof(int64) or nameof(Int64) or "long" => "Int64",
		nameof(Single) or "float" => "Float",
		nameof(Double) or "double" => "Double",
		nameof(Boolean) or "bool" => "Bool",
		_ => "Conjugate",
	};

	private ParameterDefaultValueBodyBuilder _defaultValueBuilder = new(parameters);

}


