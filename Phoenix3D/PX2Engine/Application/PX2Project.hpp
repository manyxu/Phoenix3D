// PX2Project.hpp

#ifndef PX2PROJECT_HPP
#define PX2PROJECT_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2Node.hpp"
#include "PX2Scene.hpp"
#include "PX2Size.hpp"
#include "PX2Float4.hpp"
#include "PX2Canvas.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UI.hpp"
#include "PX2BPPackage.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Project : public Object, public Singleton<Project>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;

	public:
		Project();
		virtual ~Project();

		// 使用该接口删除Project
		static void Destory();

		// ScreenOrientation
	public:
		enum ScreenOrientation
		{
			SO_LANDSCAPE,
			SO_PORTRAIT,
			SO_MAX_TYPE
		};
		void SetScreenOrientation(ScreenOrientation so);
		ScreenOrientation GetScreenOrientation() const;

	protected:
		ScreenOrientation _FromSOStr(const std::string &str);
		std::string _ToSOStr(ScreenOrientation so);

		ScreenOrientation mScreenOrientation;

		// Load Save
	public:
		bool Save(const std::string &filename); //< only used in editor
		bool SaveConfig(const std::string &filename); //< only used in editor	
		bool Load(const std::string &filename);

		// Config
	public:
		void SetSize(float width, float height);
		void SetSize(const Sizef &size);
		const Sizef & GetSize() const;
		float GetWidth() const;
		float GetHeight() const;

		void SetBackgroundColor(const Float4 &color);
		const Float4 &GetBackgroundColor() const;

		void SetProjBackgroundColor(const Float4 &color);
		const Float4 &GetProjBackgroundColor() const;

	protected:
		Sizef mSize;
		Float4 mBackgroundColor;
		Float4 mProjBackgroundColor;

		// Scene
	public:
		void SetScene(Scene *scene);
		Scene *GetScene();
		const std::string &GetSceneFilename() const;

	public_internal:
		void SetSceneFilename(const std::string &scenefilename);

	protected:
		std::string mSceneFilename;
		ScenePtr mScene;

		// UI
	public:
		void SetUI(UI *ui);
		UI *GetUI();
		const std::string &GetUIFilename() const;

	protected:
		std::string mUIFilename;
		UIPtr mUI;

		// BluePrint
	public:
		void SetBPPackage(BPPackage *package);
		BPPackage *GetBPPackage();
		const std::string &GetBPFilename() const;

	protected:
		std::string mBPFilename;
		BPPackagePtr mBPPackage;

		// edit setting
	public:
		void SetEdit_UICameraPercent(float percent);
		float GetEdit_UICameraPercent() const;

	protected:
		float mEdit_UICameraPercent;
	};

#define PX2_PROJ Project::GetSingleton()
#include "PX2Project.inl"

}

#endif