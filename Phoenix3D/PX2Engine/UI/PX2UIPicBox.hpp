// PX2UIPicBox.hpp

#ifndef PX2UIPICBOX_HPP
#define PX2UIPICBOX_HPP

#include "PX2UIPre.hpp"
#include "PX2Float2.hpp"
#include "PX2Size.hpp"
#include "PX2Rect.hpp"
#include "PX2Texture2D.hpp"
#include "PX2Material.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2TriMesh.hpp"
#include "PX2Camera.hpp"
#include "PX2TexPackData.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIPicBox : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UIPicBox);
		PX2_DECLARE_STREAM(UIPicBox);

	public:
		UIPicBox(const std::string &filename = "Data/engine/default.png",
			int isDynamicBuffer=0);
		UIPicBox(const std::string &packName, const std::string &eleName,
			int isDynamicBuffer=0);
		virtual ~UIPicBox();

		enum PicBoxType
		{
			PBT_NORMAL,
			PBT_NINE,
			PBT_NORAML_UVREVERSE,
			PBT_MAX_TYPE
		};
		void SetPicBoxType(PicBoxType type);
		PicBoxType GetPicBoxType();

		// anchor
		void SetPivot(float anchX, float anchZ);
		void SetPivot(Float2 anchor);
		const Float2 &GetPivot() const;
		Float2 &GetPivot();

		// size
		void SetSize(float width, float height);
		void SetSize(const Sizef &size);
		const Sizef &GetSize() const;
		void SetWidth(float width);
		float GetWidth() const;
		void SetHeight(float height);
		float GetHeight() const;
		void MakeSizeWithTex();

		// cornorsize
		void SetTexCornerSize(float width, float height);
		void SetTexCornerSize(const Sizef &size);
		const Sizef &GetTexCornerSize() const;
		Sizef &GetTexCornerSize();
		void SetTexCornerWidth(float width);
		float GetTexCornerWidth() const;
		void SetTexCornerHeight(float height);
		float GetTexCornerHeight() const;

		virtual void SetAlpha(float alpha); //< 0-1

		enum PicBoxTexMode
		{
			PBTM_TEX,
			PBTM_TEXPACK_ELE,
			PBTM_MAX_MODE
		};
		PicBoxTexMode GetPicBoxTexMode() const;

		// base texture
		void SetTexture(const std::string &filename);
		void SetTexture(const std::string &texPackName, const std::string &eleName);
		void SetTexture(Texture *tex);
		Texture *GetTexture();
		const std::string &GetTextureFilename() const;
		const std::string &GetTexturePackName() const;
		const std::string &GetElementName() const;

		void SetUVRepeat(const Float2 &uvRepeat);

		virtual void OnForceBind();

	protected:
		void _Init();
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		void UpdateBuffers(float elapsedTime);
		void UpdateVertexBuffer(float elapsedTime, const std::vector<Float2> &uvs);
		void UpdateIndexBuffer();
		void ReCreateVBuffer();

		bool mIsDynamic;

		PicBoxType mPicBoxType;
		Float2 mPivotPoint;
		Sizef mSize;
		Sizef mCornerSize;

		TexPackElement mPackEle;
		bool mIsBufferNeedUpdate;

		PicBoxTexMode mPBTexMode;
		std::string mTexturePathname;
		std::string mTexturePackName;
		std::string mElementName;
	};

	PX2_REGISTER_STREAM(UIPicBox);
	typedef PointerRef<UIPicBox> UIPicBoxPtr;
#include "PX2UIPicBox.inl"

}

#endif