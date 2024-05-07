// Copyright Zero Games. All Rights Reserved.


#include "ZGlueManifestWriter.h"

#include "IZExportedEnum.h"
#include "IZExportedTypeRegistry.h"

void ZSharp::FZGlueManifestWriter::Write()
{
	IZExportedTypeRegistry::Get().ForeachExportedEnum([](IZExportedEnum& enm)
	{
		UE_LOG(LogTemp, Error, TEXT("Exported Enum Name: [%s] Module: [%s] SlotType: [%s] UnderlyingType: [%s]"),
			*enm.GetName(),
			*enm.GetModule(),
			*ToString(enm.GetSlotType()),
			*enm.GetUnderlyingType());
	});
}


