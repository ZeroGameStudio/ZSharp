// Copyright Zero Games. All Rights Reserved.

#include "ZArchive_Interop.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Conjugate/ZRegularConjugateRegistries.h"
#include "Conjugate/ZConjugateRegistry_UObject.h"
#include "Conjugate/ZStrangeConjugateRegistries.h"
#include "Conjugate/ZObjectWrapperConjugateRegisties.h"
#include "StructUtils/InstancedStruct.h"

void ZSharp::FZArchive_Interop::SaveUInt8(FArchive& ar, uint8 value)
{
	ar << value;
}

uint8 ZSharp::FZArchive_Interop::LoadUInt8(FArchive& ar)
{
	uint8 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveUInt16(FArchive& ar, uint16 value)
{
	ar << value;
}

uint16 ZSharp::FZArchive_Interop::LoadUInt16(FArchive& ar)
{
	uint16 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveUInt32(FArchive& ar, uint32 value)
{
	ar << value;
}

uint32 ZSharp::FZArchive_Interop::LoadUInt32(FArchive& ar)
{
	uint32 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveUInt64(FArchive& ar, uint64 value)
{
	ar << value;
}

uint64 ZSharp::FZArchive_Interop::LoadUInt64(FArchive& ar)
{
	uint64 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveInt8(FArchive& ar, int8 value)
{
	ar << value;
}

int8 ZSharp::FZArchive_Interop::LoadInt8(FArchive& ar)
{
	int8 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveInt16(FArchive& ar, int16 value)
{
	ar << value;
}

int16 ZSharp::FZArchive_Interop::LoadInt16(FArchive& ar)
{
	int16 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveInt32(FArchive& ar, int32 value)
{
	ar << value;
}

int32 ZSharp::FZArchive_Interop::LoadInt32(FArchive& ar)
{
	int32 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveInt64(FArchive& ar, int64 value)
{
	ar << value;
}

int64 ZSharp::FZArchive_Interop::LoadInt64(FArchive& ar)
{
	int64 value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveFloat(FArchive& ar, float value)
{
	ar << value;
}

float ZSharp::FZArchive_Interop::LoadFloat(FArchive& ar)
{
	float value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveDouble(FArchive& ar, double value)
{
	ar << value;
}

double ZSharp::FZArchive_Interop::LoadDouble(FArchive& ar)
{
	double value;
	ar << value;
	return value;
}

void ZSharp::FZArchive_Interop::SaveString(FArchive& ar, FZConjugateHandle value)
{
	ar << *IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_String>().ConjugateUnsafe(value);
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadString(FArchive& ar)
{
	auto value = new FString;
	ar << *value;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_String>().Conjugate(value, true);
}

void ZSharp::FZArchive_Interop::SaveAnsiString(FArchive& ar, FZConjugateHandle value)
{
	ar << *IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>().ConjugateUnsafe(value);
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadAnsiString(FArchive& ar)
{
	auto value = new FAnsiString;
	ar << *value;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_AnsiString>().Conjugate(value, true);
}

void ZSharp::FZArchive_Interop::SaveUtf8String(FArchive& ar, FZConjugateHandle value)
{
	ar << *IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Utf8String>().ConjugateUnsafe(value);
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadUtf8String(FArchive& ar)
{
	auto value = new FUtf8String;
	ar << *value;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Utf8String>().Conjugate(value, true);
}

void ZSharp::FZArchive_Interop::SaveName(FArchive& ar, FZConjugateHandle value)
{
	ar << *IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Name>().ConjugateUnsafe(value);
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadName(FArchive& ar)
{
	auto value = new FName;
	ar << *value;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Name>().Conjugate(value, true);
}

void ZSharp::FZArchive_Interop::SaveText(FArchive& ar, FZConjugateHandle value)
{
	ar << *IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Text>().ConjugateUnsafe(value);
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadText(FArchive& ar)
{
	auto value = new FText;
	ar << *value;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_Text>().Conjugate(value, true);
}

void ZSharp::FZArchive_Interop::SaveObject(FArchive& ar, FZConjugateHandle value)
{
	UObject* obj = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe(value);
	ar << obj;
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadObject(FArchive& ar)
{
	UObject* value;
	ar << value;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().Conjugate(value);
}

void ZSharp::FZArchive_Interop::SaveScriptStruct(FArchive& ar, FZConjugateHandle value)
{
	FZSelfDescriptiveScriptStruct* sdvalue = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(value);
	FInstancedStruct instancedStruct;
	instancedStruct.InitializeAs(sdvalue->GetDescriptor(), static_cast<uint8*>(sdvalue->GetUnderlyingInstance()));
	instancedStruct.Serialize(ar);
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadScriptStruct(FArchive& ar)
{
	FInstancedStruct instancedStruct;
	instancedStruct.Serialize(ar);
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(instancedStruct.GetScriptStruct(), [instancedStruct](FZSelfDescriptiveScriptStruct& sdss){ instancedStruct.GetScriptStruct()->CopyScriptStruct(sdss.GetUnderlyingInstance(), instancedStruct.GetMemory()); });
}

uint8 ZSharp::FZArchive_Interop::NetSaveScriptStruct(FArchive& ar, FZConjugateHandle value, FZConjugateHandle map, uint8& success)
{
	FZSelfDescriptiveScriptStruct* sdvalue = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(value);
	FInstancedStruct instancedStruct;
	instancedStruct.InitializeAs(sdvalue->GetDescriptor(), static_cast<uint8*>(sdvalue->GetUnderlyingInstance()));
	bool suc;
	bool res = instancedStruct.NetSerialize(ar, IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<UPackageMap>(map), suc);
	success = suc;
	return res;
}

uint8 ZSharp::FZArchive_Interop::NetLoadScriptStruct(FArchive& ar, FZConjugateHandle& value, FZConjugateHandle map, uint8& success)
{
	FInstancedStruct instancedStruct;
	bool suc;
	bool res = instancedStruct.NetSerialize(ar, IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UObject>().ConjugateUnsafe<UPackageMap>(map), suc);
	value = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(instancedStruct.GetScriptStruct(), [instancedStruct](FZSelfDescriptiveScriptStruct& sdss){ instancedStruct.GetScriptStruct()->CopyScriptStruct(sdss.GetUnderlyingInstance(), instancedStruct.GetMemory()); });
	return res;
}

void ZSharp::FZArchive_Interop::SaveSoftObjectPath(FArchive& ar, FZConjugateHandle value)
{
	FZSelfDescriptiveScriptStruct* sdvalue = IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().ConjugateUnsafe(value);
	ar << *sdvalue->GetTypedUnderlyingInstance<FSoftObjectPath>();
}

ZSharp::FZConjugateHandle ZSharp::FZArchive_Interop::LoadSoftObjectPath(FArchive& ar)
{
	FSoftObjectPath value;
	ar << value;
	return IZSharpClr::Get().GetMasterAlc()->GetConjugateRegistry<FZConjugateRegistry_UScriptStruct>().Conjugate(TBaseStructure<FSoftObjectPath>::Get(), [value](FZSelfDescriptiveScriptStruct& sdss){ *sdss.GetTypedUnderlyingInstance<FSoftObjectPath>() = value; });
}

uint8 ZSharp::FZArchive_Interop::IsSaving(FArchive& ar)
{
	return ar.IsSaving();
}

uint8 ZSharp::FZArchive_Interop::IsLoading(FArchive& ar)
{
	return ar.IsLoading();
}

int64 ZSharp::FZArchive_Interop::Tell(FArchive& ar)
{
	return ar.Tell();
}

void ZSharp::FZArchive_Interop::Seek(FArchive& ar, int64 pos)
{
	ar.Seek(pos);
}

int64 ZSharp::FZArchive_Interop::GetTotalSize(FArchive& ar)
{
	return ar.TotalSize();
}


