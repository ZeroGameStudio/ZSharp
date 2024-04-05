// Copyright Zero Games. All Rights Reserved.

#pragma once

#if ZSHARP_WITH_CORECLR

	#include "CoreCLR.h"

	namespace ZSharp
	{
		using FCLR = FCoreCLR;
	}

#elif ZSHARP_WITH_MONO

	#include "Mono.h"

	namespace ZSharp
	{
		using FCLR = FMono;
	}

#else

#error "No valid clr implementation!"

#endif
