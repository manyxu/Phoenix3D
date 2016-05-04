// PX2RenderWindow.hpp

#ifndef PX2RENDERWINDOW_HPP
#define PX2RENDERWINDOW_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Canvas.hpp"
#include "PX2Object.hpp"
#include "PX2Canvas.hpp"
#include "PX2FString.hpp"

namespace PX2
{

	class PX2_FOUNDATION_ITEM RenderWindow : public Object
	{
		PX2_DECLARE_RTTI;

	public:
		RenderWindow();
		virtual ~RenderWindow();

		void SetWindowHandle(void *handle);
		void *GetWindowHandle();

		void SetMain(bool main);
		bool IsMain() const;

		void SetTitle(const std::string &title);
		const std::string &GetTitle() const;

		void SetFloat(bool fl);
		bool IsFloat() const;

		void SetPosition(const APoint &pos);
		const APoint &GetPosition() const;

		void SetScreenSize(const Sizef &size);
		const Sizef& GetScreenSize() const;

		void MaxSize(bool maxSize);
		bool IsMaxSize() const;

		void Close();
		bool IsClosed() const;

		void SetDraging(bool draging);
		bool IsDraging() const;

		void ShowCursor(bool showCursor);
		bool IsShowCursor() const;

		void CaptureMouse(bool capture);
		bool IsCaptureMouse() const;

		RenderWindow *GetParentRenderWindow();
	
	public_internal:
		void SetParentRenderWindow(RenderWindow *rw);

	protected:
		RenderWindow *mParentRenderWindow;

		void *mWindowHandle;
		bool mIsMain;
		bool mIsFloat;
		std::string mTitle;
		APoint mPosition;
		Sizef mScreenSize;
		CanvasPtr mCanvas;

		bool mIsMaxSize;
		bool mIsClosed;

		bool mIsDraging;

		bool mIsCaptureMouse;
		APoint mPreCaptureMousePos;

		// Canvas;
	public:
		bool AddCanvas(const std::string &name, Canvas *canvas);
		bool IsHasCanvas(const std::string &name) const;
		bool RemoveCanvas(const std::string &name);
		bool RemoveCanvas(Canvas *step);
		void RemoveAllCanvas();
		Canvas *GetCanvas(const std::string &name);
		void SortCanvass();
		Canvas *GetCanvasScene();
		Canvas *GetCanvasUI();

	protected:
		CanvasPtr mCanvasScene;
		CanvasPtr mCanvasUI;
		std::map<FString, CanvasPtr> mCanvasMap;
		std::vector<Canvas *> mCanvasVec;
		
		// Tick
	public:
		void SetRenderer(Renderer *renderer);
		Renderer *GetRenderer();

		void Update(double appSeconds, double elapsedSeconds);
		void ComputeVisibleSetAndEnv();
		void Draw();

	protected:
		Renderer *mRenderer;

	public:
		virtual void DoExecute(Event *event);

		virtual void OnLeftDown(const APoint &worldPos);
		virtual void OnLeftUp(const APoint &worldPos);
		virtual void OnLeftDClick(const APoint &worldPos);
		virtual void OnMiddleDown(const APoint &worldPos);
		virtual void OnMiddleUp(const APoint &worldPos);
		virtual void OnMouseWheel(float delta, const APoint &worldPos);
		virtual void OnRightDown(const APoint &worldPos);
		virtual void OnRightUp(const APoint &worldPos);
		virtual void OnMotion(const APoint &worldPos);

	public_internal:
		void _CollectCanvas();
		std::vector<Canvas*> mCanvasCollect;

	protected:
		bool mIsLeftDown;

	public:
		enum CursorType
		{
			CT_CURSOR_NONE = 0,
			CT_CURSOR_ARROW,
			CT_CURSOR_RIGHT_ARROW,
			CT_CURSOR_BULLSEYE,
			CT_CURSOR_CHAR,
			CT_CURSOR_CROSS,
			CT_CURSOR_HAND,
			CT_CURSOR_IBEAM,
			CT_CURSOR_LEFT_BUTTON,
			CT_CURSOR_MAGNIFIER,
			CT_CURSOR_MIDDLE_BUTTON,
			CT_CURSOR_NO_ENTRY,
			CT_CURSOR_PAINT_BRUSH,
			CT_CURSOR_PENCIL,
			CT_CURSOR_POINT_LEFT,
			CT_CURSOR_POINT_RIGHT,
			CT_CURSOR_QUESTION_ARROW,
			CT_CURSOR_RIGHT_BUTTON,
			CT_CURSOR_SIZENESW,
			CT_CURSOR_SIZENS,
			CT_CURSOR_SIZENWSE,
			CT_CURSOR_SIZEWE,
			CT_CURSOR_SIZING,
			CT_CURSOR_SPRAYCAN,
			CT_CURSOR_WAIT,
			CT_CURSOR_WATCH,
			CT_CURSOR_BLANK,
			CT_CURSOR_MAX_TYPE
		};
		static void SetCursorType(CursorType ct);
		static CursorType GetCursorType();

	protected:
		static CursorType msCursorType;
	};

#include "PX2RenderWindow.inl"
	typedef PointerRef<RenderWindow> RenderWindowPtr;

}

#endif