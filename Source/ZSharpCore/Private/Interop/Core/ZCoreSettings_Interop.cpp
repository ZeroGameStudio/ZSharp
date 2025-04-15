// Copyright Zero Games. All Rights Reserved.

#include "ZCoreSettings_Interop.h"

namespace ZSharp::ZCoreSettings_Interop_Private
{
	static TAutoConsoleVariable<bool> GCVarTreatManagedFatalAsError
	{
		TEXT("z#.TreatManagedFatalAsError"),
		false,
		TEXT("If enabled, managed fatal error won't crash the process.")
	};
	
	static TAutoConsoleVariable<bool> GCVarSuppressAlcUnloadedException
    {
    	TEXT("z#.SuppressAlcUnloadedException"),
    	true,
    	TEXT("If enabled, AlcUnloadedException will be ignored if it isn't caught.")
    };
}

uint8 ZSharp::FZCoreSettings_Interop::ShouldTreatManagedFatalAsError()
{
	return ZCoreSettings_Interop_Private::GCVarTreatManagedFatalAsError.GetValueOnGameThread();
}

uint8 ZSharp::FZCoreSettings_Interop::ShouldSuppressAlcUnloadedException()
{
	return ZCoreSettings_Interop_Private::GCVarSuppressAlcUnloadedException.GetValueOnGameThread();
}


