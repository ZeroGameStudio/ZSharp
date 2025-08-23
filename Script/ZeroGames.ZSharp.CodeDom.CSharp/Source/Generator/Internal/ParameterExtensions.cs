// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public static class ParameterExtensions
{
	extension(ParameterDeclaration @this)
	{
		public string KindText => @this.Kind switch
		{
			EParameterKind.Ref => "ref ",
			EParameterKind.Out => "out ",
			_ => string.Empty
		};
	}
}


