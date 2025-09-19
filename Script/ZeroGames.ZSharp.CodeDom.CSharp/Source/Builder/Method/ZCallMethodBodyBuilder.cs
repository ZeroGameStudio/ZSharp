// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class ZCallMethodBodyBuilder(string name, TypeReference? returnType, bool refGetter, bool needsUnsafeBlock, params ParameterDeclaration[]? parameters)
{

	public MethodBody Build()
	{
		string shortName = Name.Split(':').Last();
		string implPostfix = IsVirtual ? "_Implementation" : string.Empty;
		string handleFieldName = $"_zcallHandleFor{shortName}{implPostfix}";
		int32 numSlots = (IsStatic ? 0 : 1) + (Parameters?.Count ?? 0) + (ReturnType is null ? 0 : 1);

		Block getHandle =
$@"Thrower.ThrowIfNotInGameThread();

const string ZCALL_NAME = ""{Name}"";
IMasterAssemblyLoadContext __alc__ = MasterAlcCache.Instance;
{handleFieldName} ??= __alc__.GetZCallHandle(ZCALL_NAME);
ZCallHandle __handle__ = {handleFieldName}.Value;";

		StringBuilder bodySb = new();
		bodySb.Append(MakeOutBlackConjugates());
		bool needsBuffer = numSlots > 0;
		if (needsBuffer)
		{
			string setupBuffer = 
$@"const int32 NUM_SLOTS = {numSlots};
ZCallBufferSlot* __slots__ = stackalloc ZCallBufferSlot[NUM_SLOTS]
{{
{MakeSlots().Indent()}
}};
ZCallBuffer __buffer__ = new(__slots__, NUM_SLOTS);

";

			bodySb.Append(setupBuffer);
		}

		string bufferParameter = needsBuffer ? "&__buffer__" : "null";
		bodySb.Append(
$@"if (__alc__.ZCall(__handle__, {bufferParameter}) != EZCallErrorCode.Succeed)
{{
	throw new InvalidOperationException();
}}");

		if (needsBuffer)
		{
			bodySb.Append(MakeCopyOutsAndReturn());
		}

		Block body = NeedsUnsafeBlock ?
$@"unsafe
{{
{bodySb.ToString().Indent()}
}}" : bodySb.ToString();

		Block defaultValues = _defaultValueBuilder.Build();

		return new(getHandle, defaultValues, body);
	}

	public string Name { get; } = name;
	public TypeReference? ReturnType { get; } = returnType;
	public bool IsRefGetter { get; } = refGetter;
	public bool NeedsUnsafeBlock { get; } = needsUnsafeBlock;
	public IReadOnlyList<ParameterDeclaration>? Parameters { get; } = parameters;

	public bool IsStatic { get; set; }
	public bool IsVirtual { get; set; }

	public Block BeforeReturnBlock { get; set; } = new();

	protected string MakeOutBlackConjugates()
	{
		string result = string.Empty;
		foreach (var parameter in Parameters?.Where(p => p is { Kind: EParameterKind.Ref or EParameterKind.Out, Type.HasBlackConjugate: true }) ?? [])
		{
			string nullTest = parameter.Type.TypeName.EndsWith("?") ? "??" : string.Empty;
			result += $"{parameter.Name} {nullTest}= new();{Environment.NewLine}";
		}

		if (UseExplicitReturn)
		{
			result += $"{ReturnType!.Value.TypeName.TrimEnd('?')} __return__ = new();{Environment.NewLine}";
		}

		if (result.Length > 0)
		{
			result += Environment.NewLine;
		}

		return result;
	}

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
				string arg = parameter is { Type.HasBlackConjugate: true } or not { Kind: EParameterKind.Out } ? $"{cast}{parameter.Name}" : string.Empty;
				slots.Add($"ZCallBufferSlot.From{GetSlotType(parameter.Type)}({arg}),");
			}
		}

		if (ReturnType is not null)
		{
			string arg = UseExplicitReturn ? "__return__" : string.Empty;
			slots.Add($"ZCallBufferSlot.From{GetSlotType(ReturnType.Value)}({arg}),");
		}

		return string.Join(Environment.NewLine, slots);
	}

	protected string MakeCopyOutsAndReturn()
	{
		string result = string.Empty;
		List<string> copyOuts = new();
		for (int32 i = 0; i < Parameters?.Count; ++i)
		{
			var parameter = Parameters[i];
			int32 index = i + (IsStatic ? 0 : 1);
			if (parameter is { Kind: EParameterKind.In } or { Type.HasBlackConjugate: true })
			{
				continue;
			}

			copyOuts.Add($"{parameter.Name} = {MakeReturnValue(parameter.Type, index.ToString())};");
		}
		
		result += string.Join(Environment.NewLine, copyOuts);
		if (result.Length > 0)
		{
			result = Environment.NewLine + Environment.NewLine + result;
		}

		if (BeforeReturnBlock.Content != string.Empty)
		{
			result += $"{Environment.NewLine}{Environment.NewLine}{BeforeReturnBlock}";
		}
		
		if (ReturnType is not null)
		{
			result += $"{Environment.NewLine}{Environment.NewLine}";
			if (UseExplicitReturn)
			{
				result += "return __return__;";
			}
			else
			{
				result += $"return {MakeReturnValue(ReturnType!.Value, "NUM_SLOTS - 1")};";
			}
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

	private bool UseExplicitReturn => ReturnType?.HasBlackConjugate is true && !IsRefGetter;

	private ParameterDefaultValueBodyBuilder _defaultValueBuilder = new(parameters);

}


