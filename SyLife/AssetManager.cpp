#include "Model.h"
#include "AssetManager.h"

unique_ptr<AssetManager>	g_assetManagerPtr;

void AssetManager::Init()
{

	Array<string> filepaths;

	recursive_directory_iterator end;
	for (recursive_directory_iterator it(boost::filesystem::path("assets/models")); it != end; ++it)
		if (!is_directory(*it)) filepaths.emplace_back((*it).path().string());

	for (const auto& filepath : filepaths)
	{
		Logger << Unicode::Widen(filepath);
		ptree pt;

		read_json(filepath, pt);

		auto m = MakeModel(pt.get<string>("type"));

		m->SetName(pt.get<string>("name"));
	}

	Logger << U"Path";
	for (const auto& m : filepaths)
		Logger << Unicode::Widen(m);

	Logger << U"Model";
	for (const auto& m : m_models)
		Logger << Unicode::Widen(m->GetName());

	for (const auto& m : m_models)
	{
		ptree pt;

		read_json(m->GetFilepath(), pt);

		try
		{
			m->load(pt);
		}
		catch (boost::property_tree::ptree_bad_path& e)
		{
			LOG_ERROR(U"JSONアセットの読み込みに問題が発生しました");
			LOG_ERROR(U" What:" + Unicode::Widen(string(e.what())));
			LOG_ERROR(U" Model:" + Unicode::Widen(pt.get<string>("type")));
			LOG_ERROR(U" Filepath:" + Unicode::Widen(m->GetFilepath()));

			System::Exit();
		}
		catch (Error& e)
		{
			LOG_ERROR(U" What:" + e.what());
			LOG_ERROR(U" Model:" + Unicode::Widen(pt.get<string>("type")));
			LOG_ERROR(U" Filepath:" + Unicode::Widen(m->GetFilepath()));

			System::Exit();
		}
	}
}

void AssetManager::AddModelsFromDirectory(const string& directory)
{
	directory_iterator end;

	for (directory_iterator it(boost::filesystem::path(directory.c_str())); it != end; ++it)
		if (!is_directory(*it)) AddModelFromFile((*it).path().string());
}

void AssetManager::AddModelFromFile(const string& filepath)
{
	ptree pt;

	read_json(filepath, pt);

	auto m = MakeModel(pt.get<string>("type"));

	m->SetName(pt.get<string>("name"));
}

shared_ptr<Model> AssetManager::GetModel(const string& name) const
{
	for (auto it = m_models.begin(); it != m_models.end(); ++it)
		if ((*it)->GetName() == name && dynamic_pointer_cast<Model>(*it) != nullptr) return dynamic_pointer_cast<Model>(*it);

	return nullptr;
}
