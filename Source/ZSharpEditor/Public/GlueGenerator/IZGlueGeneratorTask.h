// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPEDITOR_API IZGlueGeneratorTask
	{
	public:
		virtual ~IZGlueGeneratorTask(){}
	public:
		virtual void Cancel() = 0;
	};
}


