// Copyright Zero Games. All Rights Reserved.

using Mono.Cecil;

namespace ZeroGames.ZSharp.UnrealFieldScanner;

internal class UnrealEnumFieldModel : UnrealFieldModel, IUnrealEnumFieldModel
{

	public UnrealEnumFieldModel(ModelRegistry modelRegistry, FieldDefinition fieldDef) : base(fieldDef.Name, modelRegistry, fieldDef)
	{
		Value = Convert.ToInt64(fieldDef.Constant);
	}

	public int64 Value { get; }
	
}


