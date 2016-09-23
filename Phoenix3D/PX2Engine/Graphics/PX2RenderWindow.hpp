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

	class PX2_ENGINE_ITEM RenderWindow : public Object
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

		void Show(bool show);
		bool IsShow() const;

		void SetDraging(bool draging);
		bool IsDraging() const;

		void ShowCursor(bool showCursor);
		bool IsShowCursor() const;

		void CaptureMouse(bool capture);
		bool IsCaptureMouse() const;

		RenderWindow *GetParentRenderWindow();
	
	public_internal:
		void SetParentRenderWindow(RenderWindow *rw);
		static int GetNextWindowID();

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
		bool mIsShow;

		bool mIsDraging;

		bool mIsCaptureMouse;
		APoint mPreCaptureMousePos;

		static int sNextWindowID;

		// Canvas;
	public:
		void SetMainCanvas(Canvas *mainCanvas);
		Canvas *GetMainCanvas();

	public_internal:
		bool AddCanvas(Canvas *canvas);
		bool IsHasCanvas(Canvas *canvas) const;
		bool RemoveCanvas(Canvas *canvas);

	protected:
		CanvasPtr mMainCanvas;
		std::vector<Canvas *> mCanvases;
		bool mIsCanvasesNeedReSort;
		
		// Tick
	public:
		void SetRenderer(Renderer *renderer);
		Renderer *GetRenderer();

		void Update(double appSeconds, double elapsedSeconds);
		void ComputeVisibleSet();
		void Draw();

	protected:
		Renderer *mRenderer;

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

	public:
		virtual void OnEvent(Event *ent);
	};

#include "PX2RenderWindow.inl"
	typedef PointerRef<RenderWindow> RenderWindowPtr;

}

#endif