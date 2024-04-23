// Copyright Zero Games. All Rights Reserved.


#include "UnrealEngine_Interop.h"

DEFINE_LOG_CATEGORY_STATIC(LogZSharpScript, Log, All);

void ZSharp::FZUnrealEngine_Interop::Log(uint8 level, const TCHAR* message)
{
	switch (level)
	{
	case 1:
		{
			UE_LOG(LogZSharpScript, Log, TEXT("%s"), message);
			break;
		}
	case 2:
		{
			UE_LOG(LogZSharpScript, Warning, TEXT("%s"), message);
			break;
		}
	case 3:
		{
			UE_LOG(LogZSharpScript, Error, TEXT("%s"), message);
			break;
		}
	default:
		{
			break;
		}
	}
}


