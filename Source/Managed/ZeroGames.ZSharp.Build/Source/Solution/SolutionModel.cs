// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.ZSharp.Build.Solution;

public class SolutionFolder
{

	public SolutionFolder GetOrAddChild(string name)
	{
		SolutionFolder? child = Children.SingleOrDefault(child => child.Name == name);
		if (child is null)
		{
			child = new()
			{
				Name = name,
				Parent = this,
			};
			Children.Add(child);
			Children.Sort((lhs, rhs) => StringComparer.Ordinal.Compare(lhs.Name, rhs.Name));
		}

		return child;
	}

	public void AddProject(ProjectModel project)
	{
		Projects.Add(project);
		Projects.Sort((lhs, rhs) => StringComparer.Ordinal.Compare(lhs.Name, rhs.Name));
	}
	
	public required string Name { get; set; }

	public string Path
	{
		get
		{
			List<string> nodes = [];
			SolutionFolder? cur = this;
			do
			{
				nodes.Add(cur.Name);
			} while ((cur = cur?.Parent) is { Parent: not null });
			nodes.Reverse();
			return string.Join('/', nodes);
		}
	}
	
	public required SolutionFolder? Parent { get; set; }
	public List<SolutionFolder> Children { get; set; } = [];
	public List<ProjectModel> Projects { get; set; } = [];
	
}

public class SolutionModel
{
	public required string Path { get; set; }
	public SolutionFolder RootFolder { get; } = new() { Name = string.Empty, Parent = null };
}


