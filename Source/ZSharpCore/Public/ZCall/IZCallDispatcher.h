// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpErrorCodes.h"

namespace ZSharp
{
	struct FZCallBuffer;

	/**
	 * Represents a ZCall.
	 */
	class IZCallDispatcher
	{

	public:
		/**
		 * Called when register this to Master ALC.
		 * Should return the unique name of this ZCall.
		 * If this is dynamically produced, returned name must always equal to the name used to create this.
		 *
		 * @return The unique name of this ZCall.
		 */
		virtual const FString& GetName() const = 0;

		/**
		 * Called when owning Master ALC is performing the ZCall with the same name as this.
		 * Should forward the call to its implementation or implement it inplace.
		 * See EZCallErrorCode for more details.
		 *
		 * @param buffer Parameters of this ZCall, including inputs and outputs.
		 * @return The error code of the ZCall.
		 */
		virtual EZCallErrorCode Dispatch(FZCallBuffer* buffer) const = 0;
		
	public:
		virtual ~IZCallDispatcher(){}
		
	};
}


