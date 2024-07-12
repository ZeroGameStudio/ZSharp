// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class IZConjugateRegistry
	{
	public:
		virtual ~IZConjugateRegistry(){}
	public:
		virtual void Release() = 0;
		virtual void* BuildConjugate(void* userdata) = 0;
		virtual void ReleaseConjugate(void* unmanaged) = 0;
		virtual void PushRedFrame() = 0;
		virtual void PopRedFrame() = 0;
	};
}


