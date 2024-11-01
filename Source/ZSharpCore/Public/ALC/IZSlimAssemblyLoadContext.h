// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZAssemblyLoadContext.h"

namespace ZSharp
{
	/**
 	 * Represents an active Slim ALC.
 	 *
 	 * [Library Interface] - you should just consume this and have no reason to implement this.
 	 */
	class IZSlimAssemblyLoadContext : public IZAssemblyLoadContext
	{
		
	public:
		/**
		 * Gets the name of this ALC.
		 * 
		 * @return The name of this ALC.
		 */
		virtual FString GetName() const = 0;
		
	};
}


