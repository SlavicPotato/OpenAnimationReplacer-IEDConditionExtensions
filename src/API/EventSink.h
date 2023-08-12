#pragma once

namespace Events
{
	template <class T>
	class EventSink
	{
		using event_type = T;

	public:
		virtual void Receive(const event_type& a_evn) = 0;
	};
}