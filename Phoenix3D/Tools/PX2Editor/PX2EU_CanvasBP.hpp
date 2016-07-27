// PX2EU_CanvasBP.hpp

#ifndef PX2EU_CANVASBP_HPP
#define PX2EU_CANVASBP_HPP

#include "PX2EditorPre.hpp"
#include "PX2UICanvas.hpp"

namespace PX2
{

	//class PX2_EDITOR_ITEM EU_CanvasBP : public UICanvas
	//{
	//	PX2_DECLARE_RTTI;

	//public:
	//	EU_CanvasBP();
	//	virtual ~EU_CanvasBP();

	//public:
	//	virtual void OnEvent(Event *event);

	//	virtual void OnLeftDown(const APoint &worldPos);
	//	virtual void OnLeftUp(const APoint &worldPos);
	//	virtual void OnRightDown(const APoint &worldPos);
	//	virtual void OnRightUp(const APoint &worldPos);
	//	virtual void OnMotion(const APoint &worldPos);

	//protected:
	//	void CreateEditMenuOnBPPackage(const APoint &pos);
	//	void CreateEditMenuOnBPFile(const APoint &pos);
	//	void CreateEditMenuOnBPModule(const APoint &pos);
	//	void CreateEditMenuOnBPParam(const APoint &pos);

	//	void Pick(const APoint &screenPos, int type, int down_up_moving); // 1left 2right 3mid		
	//	Movable *JudgePickedObject(Movable *pickedMov,
	//		BPPackage* &lp, BPFile* &lg, BPModule* &lm, BPParam *&lmp);
	//	void ProcessSelectLeftMove(float horzPixel, float vertPixel);
	//	void UpdateLinkingPos(const APoint &screenPos);
	//};

	//PX2_REGISTER_STREAM(EU_CanvasBP);
	//typedef PointerRef<EU_CanvasBP> EU_CanvasBPPtr;

}

#endif