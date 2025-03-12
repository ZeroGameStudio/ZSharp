// Copyright Zero Games. All Rights Reserved.

using System.Collections.Immutable;
using System.Xml.Linq;

namespace ZeroGames.ZSharp.Build.Solution;

public class SolutionFileWriter
{

	public SolutionFileWriter(SolutionModel solution, Action<object> log)
	{
		_solution = solution;
		_log = log;
	}

	public async Task WriteAsync(CancellationToken cancellationToken)
	{
		XDocument doc = XmlHelper.MakeDocument();
		XElement root = new("Solution");
		doc.Add(root);
		
		XElement configurations = new("Configurations", _configurations.Select(configuration =>
		{
			XElement buildTypeElement = new("BuildType");
			buildTypeElement.SetAttributeValue("Name", configuration);
			return buildTypeElement;
		}));
		root.Add(configurations);

		void LootFolder(SolutionFolder folder)
		{
			XElement projectParentElement = root;
			if (folder.Parent is not null)
			{
				XElement folderElement = new("Folder");
				folderElement.SetAttributeValue("Name", folder.Path);
				
				root.Add(folderElement);
				projectParentElement = folderElement;
			}

			foreach (var project in folder.Projects)
			{
				XElement projectElement = new("Project");
				projectElement.SetAttributeValue("Path", project.Path);
				
				projectParentElement.Add(projectElement);
			}

			foreach (var child in folder.Children)
			{
				LootFolder(child);
			}
		}
		
		LootFolder(_solution.RootFolder);

		_log($"Generating solution file: {_solution.Path}");

		await using FileStream fs = File.Create(_solution.Path);
		await doc.SaveAsync(fs, SaveOptions.None, cancellationToken);
	}

	private static readonly ImmutableArray<string> _configurations = [ "DebugGame", "Development", "Shipping" ];

	private readonly SolutionModel _solution;
	private readonly Action<object> _log;

}


