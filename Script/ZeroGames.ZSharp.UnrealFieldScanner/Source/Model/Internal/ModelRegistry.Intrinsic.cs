// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.UnrealFieldScanner;

partial class ModelRegistry
{

	private void SetupIntrinsicTypes()
	{
		// IMPORTANT: Must setup from most base to derived.
		SetupIntrinsicType(typeof(object));
		SetupIntrinsicType(typeof(Array));
		SetupIntrinsicType(typeof(ValueType));
		SetupIntrinsicType(typeof(Enum));
		SetupIntrinsicType(typeof(Delegate));
		SetupIntrinsicType(typeof(void));
		
		SetupIntrinsicType(typeof(uint8));
		SetupIntrinsicType(typeof(uint16));
		SetupIntrinsicType(typeof(uint32));
		SetupIntrinsicType(typeof(uint64));
		SetupIntrinsicType(typeof(int8));
		SetupIntrinsicType(typeof(int16));
		SetupIntrinsicType(typeof(int32));
		SetupIntrinsicType(typeof(int64));
		SetupIntrinsicType(typeof(float));
		SetupIntrinsicType(typeof(double));
		SetupIntrinsicType(typeof(bool));
		
		SetupIntrinsicType(typeof(string));
	}

	private void SetupIntrinsicType(Type runtimeType)
	{
		string fullName = runtimeType.FullName!;
		_typeMap[fullName] = new IntrinsicTypeModel(this, runtimeType);
	}
	
}


