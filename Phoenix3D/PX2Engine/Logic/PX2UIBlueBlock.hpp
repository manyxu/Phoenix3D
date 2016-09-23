// PX2UIBlueBlock.hpp

#ifndef PX2UIBLUEBLOCK_HPP
#define PX2UIBLUEBLOCK_HPP

#include "PX2LogicPre.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIFText.hpp"

namespace PX2
{

	class LogicModule;

	class PX2_ENGINE_ITEM UIBlueblock : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIBlueblock);

	public:
		UIBlueblock();
		virtual ~UIBlueblock();

		virtual void SetActivate(bool act);

		UIFText *GetText();

	protected:
		UIFTextPtr mUIFText;

	public:
		void SetLogicModule(LogicModule *module);
		LogicModule *GetLogicModule();

		enum BlockType
		{
			BFT_EVENT,
			BFT_FUNCTION,
			BFT_CONTROL,
			BFT_OPERATOR,
			BFT_OPERATOR_FUCTION,
			BFT_DATA,
			BFT_MAX_TYPE
		};

	protected:
		LogicModule *mLogicModule;

	public:
		enum BlockShapeType
		{
			BST_BARRECAP,
			BST_BARRE,
			BST_BARRE2,
			BST_BARRE3,
			BST_CAPSULE,
			BST_NAND,
		};
		void SetShape(BlockShapeType type);

		// show
	protected:
		UIFramePtr mBlockFrame;
		BlockShapeType mBlockShapeType;
		UIButtonPtr mBlockButton;
	};

	PX2_REGISTER_STREAM(UIBlueblock);
	typedef PointerRef<UIBlueblock> UIBlueblockPtr;

}

#endif