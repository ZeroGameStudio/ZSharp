// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	/**
	 * Push a new Red Stack frame while this guard is in scope
	 * and automatically pop out when this guard goes out of scope.
	 *
	 * Usage:
	 * {
	 *     FZRedFrameScope scope;
	 *     IZSharpClr::Get().GetMasterAlc()->ZCall(handle, buffer);
	 * }
	 */
	struct ZSHARPCORE_API FZRedFrameScope
	{
		FZRedFrameScope();
		~FZRedFrameScope();
	private:
		void* CapturedAlc;
	};
}


