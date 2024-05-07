// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZBuildEngine.h"

namespace ZSharp
{
	class FZBuildEngine : public IZBuildEngine
	{
		
	public:
		static FZBuildEngine& Get();

	public:
		virtual void GenerateSolution() const override;
		virtual void GenerateGlue() const override;
		virtual void BuildSolution() const override;
		
	};
}


