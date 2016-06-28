// PX2SizeNode.hpp

#ifndef PX2SIZENODE_HPP
#define PX2SIZENODE_HPP

#include "PX2Node.hpp"
#include "PX2Float2.hpp"
#include "PX2Size.hpp"
#include "PX2Rect.hpp"
#include "PX2Function.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM SizeNode : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_FUNCTION;
		PX2_DECLARE_STREAM(SizeNode);

	public:
		SizeNode();
		virtual ~SizeNode();

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		virtual void OnBeAttached();
		virtual void OnBeDetach();

		// Size
	public:
		void SetSize(float width, float height);
		void SetSize(const Sizef &size);
		const Sizef &GetSize() const;
		void SetWidth(float width);
		float GetWidth() const;
		void SetHeight(float height);
		float GetHeight() const;

		virtual void OnSizeChanged();

		Rectf GetLocalRect() const;
		Rectf GetWorldRect() const;
		APoint WorldPosToViewPortPos(const APoint &worldPos);
		bool IsInSizeRange(const SizeNode *node) const;
		bool IsIntersectSizeRange(const SizeNode *node) const;

		typedef void (SizeNode::*SizeChangeCallback)(SizeNode *tellObject);
		void SetSizeChangeCallback(SizeNode *object, SizeChangeCallback callback);

	protected:
		Sizef mSize;

		SizeChangeCallback mSizeChangeCallback;
		SizeNode *mSizeChangeTellToObject;

		// Pvoit
	public:
		void SetPivot(float x, float y);
		void SetPivot(const Float2 &pvoit);
		const Float2 &GetPvoit() const;

		virtual void OnPvoitChanged();

	protected:
		Float2 mPvoit;

		// Anchor Layout
	public:
		void EnableAnchorLayout(bool enable);
		bool IsAnchorLayoutEnable() const;

		void SetAnchorHor(float anchorX, float anchorY);
		void SetAnchorHor(const Float2 &anchor);
		const Float2 &GetAnchorHor() const;
		void SetAnchorParamHor(float param0, float param1);
		void SetAnchorParamHor(const Float2 &param);
		const Float2 &GetAnchorParamHor() const;

		void SetAnchorVer(float anchorX, float anchorY);
		void SetAnchorVer(const Float2 &anchor);
		const Float2 &GetAnchorVer() const;
		void SetAnchorParamVer(float param0, float param1);
		void SetAnchorParamVer(const Float2 &param);
		const Float2 &GetAnchorParamVer() const;

	public_internal:
		void _MarkRelatvieChange();
		AVector LeftBottomCornerOffset;

	protected:
		bool mIsAnchorLayoutEnable;
		Float2 mAnchorHor;
		Float2 mAnchorVer;
		Float2 mAnchorParamHor;
		Float2 mAnchorParamVer;

	public_internal:
		virtual void UpdateLayout(Movable *parent);
		virtual void UpdateLeftBottomCornerOffset(Movable *parent);

	protected:
		bool mIsLayoutChanged;

	};

	PX2_REGISTER_STREAM(SizeNode);
	typedef PointerRef<SizeNode> SizeNodePtr;
#include "PX2SizeNode.inl"

}

#endif