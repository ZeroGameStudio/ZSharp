// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPEDITOR_API IZGlueGenerator
	{
	public:
		static IZGlueGenerator& Get();
	public:
		virtual ~IZGlueGenerator(){}
	public:
		virtual void Generate(const TFunction<void()>& onComplete = nullptr) = 0;
		virtual bool IsBusy() = 0;
		virtual IZGlueGenerator* GetRunningTask() = 0;
	};
}


