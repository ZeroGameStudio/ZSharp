// Copyright Zero Games. All Rights Reserved.

using System.Xml.Linq;

namespace ZeroGames.ZSharp.Build.Solution;

public class ProjectFileWriter
{

	public ProjectFileWriter(string unrealProjectDir, string zsharpPluginDir, ProjectModel project, Action<object> log)
	{
		_unrealProjectDir = unrealProjectDir;
		_zsharpPluginDir = zsharpPluginDir;
		_project = project;
		_log = log;
	}

	public async ValueTask WriteAsync(CancellationToken cancellationToken)
	{
		XDocument doc = XmlHelper.MakeDocument();
		XElement root = new("Project");
		root.SetAttributeValue("Sdk", "ZeroGames.ZSharp.Sdk/1.0.0-beta.7");
		doc.Add(root);
		
		OverrideDotNetProperties(root);
		OverrideZSharpProperties(root);
		AddProjectReferences(root);
		AddPackageReferences(root);
		AddExternalReferences(root);
		AddImplicitUsings(root);
		AddStaticUsings(root);
		AddTypeAliases(root);
		AddSources(root);
		AddMiscs(root);

		string path = _project.Path;
		_log($"Generating project file: {path}");

		string dir = Path.GetDirectoryName(path)!;
		if (!Directory.Exists(dir))
		{
			Directory.CreateDirectory(dir);
		}

		await using FileStream fs = File.Create(path);
		await doc.SaveAsync(fs, SaveOptions.None, cancellationToken);
	}
	
	private static string BooleanToString(bool value) => value.ToString().ToLower();

	private void OverrideDotNetProperties(XElement root)
	{
		root.Add(new XComment("Override .NET properties."));
		
		XElement propertyGroupElement = new("PropertyGroup");
		
		if (_project.OutputType is EProjectOutputType.Analyzer)
		{
			propertyGroupElement.Add(new XElement("IsRoslynComponent", "true"));
			propertyGroupElement.Add(new XElement("EnforceExtendedAnalyzerRules", "true"));
		}
		
		propertyGroupElement.Add(new XElement("TargetFramework", _project.TargetFramework));
		propertyGroupElement.Add(new XElement("LangVersion", _project.LanguageVersion));
		propertyGroupElement.Add(new XElement("RootNamespace", _project.RootNamespace));
		propertyGroupElement.Add(new XElement("AllowUnsafeBlocks", _project.AllowUnsafeBlocks));

		if (_project.Constants.Any())
		{
			propertyGroupElement.Add(new XElement("DefineConstants", $"$(DefineConstants);{string.Join(';', _project.Constants)}"));
		}
		
		if (_project.WarningsAsErrors.Count > 0)
		{
			propertyGroupElement.Add(new XElement("WarningsAsErrors", $"$(WarningsAsErrors);{string.Join(';', _project.WarningsAsErrors)}"));
		}

		if (!string.IsNullOrWhiteSpace(_project.Authors))
		{
			propertyGroupElement.Add(new XElement("Authors", _project.Authors));
		}

		if (!string.IsNullOrWhiteSpace(_project.Company))
		{
			propertyGroupElement.Add(new XElement("Company", _project.Company));
		}
		
		if (!string.IsNullOrWhiteSpace(_project.AssemblyVersion))
		{
			propertyGroupElement.Add(new XElement("AssemblyVersion", _project.AssemblyVersion));
		}
		
		if (!string.IsNullOrWhiteSpace(_project.FileVersion))
		{
			propertyGroupElement.Add(new XElement("FileVersion", _project.FileVersion));
		}
		
		if (!string.IsNullOrWhiteSpace(_project.NeutralLanguage))
		{
			propertyGroupElement.Add(new XElement("NeutralLanguage", _project.NeutralLanguage));
		}
		
		root.Add(propertyGroupElement);
	}
	
	private void OverrideZSharpProperties(XElement root)
	{
		root.Add(new XComment("Override Z# properties."));
		
		XElement propertyGroupElement = new("PropertyGroup");
		
		propertyGroupElement.Add(new XElement("ZSharpProjectName", _project.Name));
		propertyGroupElement.Add(new XElement("UnrealProjectDir", _unrealProjectDir));
		propertyGroupElement.Add(new XElement("ZSharpPluginDir", _zsharpPluginDir));
		propertyGroupElement.Add(new XElement("ZSharpProjectSourceDir", _project.SourceDir));
		propertyGroupElement.Add(new XElement("ZSharpOutputDir", _project.OutputDir));
		propertyGroupElement.Add(new XElement("MinimalImplicitUsings", BooleanToString(_project.UsesMinimalImplicitUsings)));
		
		root.Add(propertyGroupElement);
	}
	
	private void AddProjectReferences(XElement root)
	{
		if (_project.ProjectReferences.All(p => p.IsPrecompiled))
		{
			return;
		}
		
		root.Add(new XComment("Add project references."));
		
		XElement itemGroupElement = new("ItemGroup");

		foreach (var reference in _project.ProjectReferences.Where(p => !p.IsPrecompiled))
		{
			XElement referenceElement = new("ProjectReference");
			referenceElement.SetAttributeValue("Include", reference.Path);

			if (_project.OutputType is not EProjectOutputType.Analyzer)
			{
				referenceElement.SetAttributeValue("Private", BooleanToString(true));
			}
			
			if (reference.OutputType is EProjectOutputType.Analyzer)
			{
				referenceElement.SetAttributeValue("OutputItemType", "Analyzer");
				referenceElement.SetAttributeValue("ReferenceOutputAssembly", BooleanToString(false));
			}
			
			itemGroupElement.Add(referenceElement);
		}
		
		root.Add(itemGroupElement);
	}

	private void AddPackageReferences(XElement root)
	{
		if (_project.PackageReferences.Count == 0)
		{
			return;
		}
		
		root.Add(new XComment("Add package references."));
		
		XElement itemGroupElement = new("ItemGroup");

		foreach (var reference in _project.PackageReferences)
		{
			XElement referenceElement = new("PackageReference");
			referenceElement.SetAttributeValue("Include", reference.Name);
			referenceElement.SetAttributeValue("Version", reference.Version);
			if (reference.IsPrivate)
			{
				referenceElement.SetAttributeValue("PrivateAssets", "all");
			}
			
			itemGroupElement.Add(referenceElement);
		}
		
		root.Add(itemGroupElement);
	}

	private void AddExternalReferences(XElement root)
	{
		if (_project.ExternalReferences.Count == 0)
		{
			return;
		}
		
		root.Add(new XComment("Add external references."));
		
		XElement itemGroupElement = new("ItemGroup");

		foreach (var reference in _project.ExternalReferences)
		{
			XElement referenceElement = new("Reference");
			referenceElement.SetAttributeValue("Include", reference.Name);
			referenceElement.Add(new XElement("HintPath", reference.Path));
			
			itemGroupElement.Add(referenceElement);
		}
		
		root.Add(itemGroupElement);
	}

	private void AddImplicitUsings(XElement root)
	{
		if (!_project.Usings.Any())
		{
			return;
		}
		
		root.Add(new XComment("Add implicit usings."));
		
		XElement itemGroupElement = new("ItemGroup");

		foreach (var @using in _project.Usings)
		{
			XElement usingElement = new("Using");
			usingElement.SetAttributeValue("Include", @using);
			
			itemGroupElement.Add(usingElement);
		}
		
		root.Add(itemGroupElement);
	}

	private void AddStaticUsings(XElement root)
	{
		if (!_project.StaticUsings.Any())
		{
			return;
		}
		
		root.Add(new XComment("Add static usings."));
		
		XElement itemGroupElement = new("ItemGroup");
		
		foreach (var @using in _project.StaticUsings)
		{
			XElement usingElement = new("Using");
			usingElement.SetAttributeValue("Include", @using);
			usingElement.SetAttributeValue("Static", BooleanToString(true));
			
			itemGroupElement.Add(usingElement);
		}
		
		root.Add(itemGroupElement);
	}

	private void AddTypeAliases(XElement root)
	{
		if (!_project.Aliases.Any())
		{
			return;
		}
		
		root.Add(new XComment("Add type aliases."));
		
		XElement itemGroupElement = new("ItemGroup");
		
		foreach (var pair in _project.Aliases)
		{
			XElement usingElement = new("Using");
			usingElement.SetAttributeValue("Include", pair.Key);
			usingElement.SetAttributeValue("Alias", pair.Value);
			
			itemGroupElement.Add(usingElement);
		}
		
		root.Add(itemGroupElement);
	}

	private void AddSources(XElement root)
	{
		root.Add(new XComment("Add sources."));
		
		XElement itemGroupElement = new("ItemGroup");
		
		XElement sourceElement = new("Compile");
		sourceElement.SetAttributeValue("Include", "$(ZSharpProjectSourceDir)/**/*.cs");

		itemGroupElement.Add(sourceElement);
		
		XElement glueElement = new("Compile");
		glueElement.SetAttributeValue("Include", "$(UnrealProjectDir)/Intermediate/ZSharp/Glue/$(ZSharpProjectName)/**/*.cs");
		glueElement.SetAttributeValue("Condition", "Exists('$(UnrealProjectDir)/Intermediate/ZSharp/Glue/$(ZSharpProjectName)')");

		itemGroupElement.Add(glueElement);
		
		root.Add(itemGroupElement);
	}

	private void AddMiscs(XElement root)
	{
		if (_project.OutputType is EProjectOutputType.Analyzer)
		{
			ProjectModel[] nonAnalyzerReferences = _project.ProjectReferences.Where(project => project.OutputType is not EProjectOutputType.Analyzer).ToArray();
			if (nonAnalyzerReferences.Length == 0)
			{
				return;
			}
			
			root.Add(new XElement("PropertyGroup", new XElement("GetTargetPathDependsOn", "$(GetTargetPathDependsOn);GetDependencyTargetPaths")));

			XElement getDependencyTargetPathsElement = new("Target");
			getDependencyTargetPathsElement.SetAttributeValue("Name", "GetDependencyTargetPaths");
			
			XElement itemGroupElement = new("ItemGroup");
			foreach (var reference in nonAnalyzerReferences)
			{
				XElement monikerElement = new("TargetPathWithTargetPlatformMoniker");
				monikerElement.SetAttributeValue("Include", $"$(TargetDir)/{reference.Name}.dll");
				monikerElement.SetAttributeValue("IncludeRuntimeDependency", BooleanToString(false));
				
				itemGroupElement.Add(monikerElement);
			}
			
			getDependencyTargetPathsElement.Add(itemGroupElement);
			root.Add(getDependencyTargetPathsElement);
		}
	}

	private readonly string _unrealProjectDir;
	private readonly string _zsharpPluginDir;
	private readonly ProjectModel _project;
	private readonly Action<object> _log;

}


