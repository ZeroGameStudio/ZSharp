// Copyright Zero Games. All Rights Reserved.

#pragma once

namespace ZSharp
{
	class ZSHARPBUILD_API IZBuildEngine
	{
	public:
		static IZBuildEngine& Get();
	public:
		virtual ~IZBuildEngine(){}
	public:
		virtual void GenerateSolution() const = 0;
		virtual void GenerateGlue() const = 0;
		virtual void BuildSolution() const = 0;
	};
}


