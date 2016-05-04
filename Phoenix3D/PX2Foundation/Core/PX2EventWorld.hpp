// PX2EventWorld.hpp

#ifndef PX2EVENTWORLD_HPP
#define PX2EVENTWORLD_HPP

#include "PX2CorePre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2SmartPointerRef.hpp"

namespace PX2
{

	/// 消息世界
	class PX2_FOUNDATION_ITEM EventWorld : public Singleton<EventWorld>
	{
	public:
		EventWorld ();
		~EventWorld ();

		void Shutdown (bool shutdown);
		bool IsShutdown () const;

		void Update (float detalTime);

		// 消息句柄
		void ComeIn (EventHandler *handler);
		void GoOut (EventHandler *handler);

		void BroadcastingLocalEvent (Event* event);
		void BroadcastingNetEvent (Event* event);

	public_internal:
		bool IsInComingIn(const EventHandler *handler) const;

	private:
		bool _IsInComingOut(const EventHandler *handler) const;

		typedef std::vector<Event*> EventList;
		typedef std::vector<EventHandler*> EventHandlerList;

		void _UpdateEvent(float detalTime);
		void _BroadcastingEvent (Event* event);
		void SwapEventList ();

		bool mIsShoutdown;
		EventList* mEventList;
		EventHandlerList mHandlers;

		bool mIsUpdatingEvents;
		EventHandlerList mHandlersComingIn;
		EventHandlerList mHandlersGoingOut;
		EventList *mEventListBroadcasting;
	};

	typedef PointerRef<EventWorld> EventWorldPtr;

#define PX2_EW EventWorld::GetSingleton()

}

#endif