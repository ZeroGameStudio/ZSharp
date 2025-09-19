// Copyright Zero Games. All Rights Reserved.

using System.Text;

namespace ZeroGames.ZSharp.CodeDom.CSharp;

public class StrangeZCallBodyBuilder(string zcallReplace, TypeReference? returnType, bool needsUnsafeBlock, params ParameterDeclaration[]? parameters) : ZCallMethodBodyBuilder(string.Empty, returnType, false, needsUnsafeBlock, parameters)
{

	public new MethodBody Build()
	{
		StringBuilder sb = new();
		sb.Append(MakeOutBlackConjugates());
		int32 numSlots = (IsStatic ? 0 : 1) + (Parameters?.Count ?? 0) + (ReturnType is null ? 0 : 1);
		bool needsBuffer = numSlots > 0;
		if (needsBuffer)
		{
			string setupBuffer = 
$@"Thrower.ThrowIfNotInGameThread();

const int32 NUM_SLOTS = {numSlots};
ZCallBufferSlot* __slots__ = stackalloc ZCallBufferSlot[NUM_SLOTS]
{{
{MakeSlots().Indent()}
}};
ZCallBuffer __buffer__ = new(__slots__, NUM_SLOTS);

";

			sb.Append(setupBuffer);
		}

		string bufferParameter = needsBuffer ? "&__buffer__" : "null";
		sb.Append(
$@"if ({ZCallReplace}({bufferParameter}) != EZCallErrorCode.Succeed)
{{
	throw new InvalidOperationException();
}}");
	
		if (needsBuffer)
		{
			sb.Append(MakeCopyOutsAndReturn());
		}

		Block body = NeedsUnsafeBlock ?
			$@"unsafe
{{
{sb.ToString().Indent()}
}}" : sb.ToString();

		return new(body);
	}

	public string ZCallReplace { get; } = zcallReplace;

}


