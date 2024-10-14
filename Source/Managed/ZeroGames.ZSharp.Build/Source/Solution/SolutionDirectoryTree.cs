// Copyright Zero Games. All Rights Reserved.

using System.Collections;

namespace ZeroGames.ZSharp.Build.Solution;

public class SolutionDirectoryTree : IEnumerable<SolutionDirectoryTree.Node>
{
        
	public class Node(string path, Node parent)
	{
		public string Path { get; } = path;
		public string Name { get; } = path.Split('/').LastOrDefault(string.Empty);
		public Node Parent { get; } = parent;
		public List<Node> Children { get; } = new();
	}
        
	public IEnumerator<Node> GetEnumerator()
	{
		List<Node> flattenNodes = new();
		GetChildren(_root, flattenNodes);
		return flattenNodes.GetEnumerator();
	}

	public void AddDirectory(string path)
	{
		path = path.TrimStart('/').TrimEnd('/');
		
		string[] names = path.Split('/');
		Node cur = _root;
		for (int32 i = 0; i < names.Length; ++i)
		{
			var name = names[i];
			Node? child = cur.Children.SingleOrDefault(node => node.Name == name);
			if (child is null)
			{
				child = new(string.Join('/', names.Take(i + 1)), cur);
				cur.Children.Add(child);
			}

			cur = child;
		}
	}

	IEnumerator IEnumerable.GetEnumerator()
	{
		return GetEnumerator();
	}

	private void GetChildren(Node parent, List<Node> result)
	{
		if (!string.IsNullOrWhiteSpace(parent.Name))
		{
			result.Add(parent);
		}

		foreach (var child in parent.Children)
		{
			GetChildren(child, result);
		}
	}
	
	private Node _root = new(string.Empty, null!);

}


