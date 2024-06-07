#pragma once

namespace DirectUI
{
	class UILIB_API EventManager
	{
	public:
		EventManager & operator=(EventManager const &);

		static long __stdcall AdviseEventAdded(int, SAFEARRAY *);
		static long __stdcall AdviseEventRemoved(int, SAFEARRAY *);
		static void __stdcall Close();
		static void __stdcall DestroyListener(Element *);
		static long __stdcall EndDefer(Element *);
		static long __stdcall EventListener(Element *, Event *);
		static bool __stdcall FWantAnyEvent(Element *);
		static long __stdcall FireStructureChangedEvent(Element *, StructureChangeType);
		
		static long __stdcall Init();
		static long __stdcall OnToolTip(Element *, unsigned long);
		static long __stdcall PropertyChangingListener(Element *, const PropertyInfo *, bool *);
		static long __stdcall PropertyListener(Element *, const PropertyInfo *, int, Value *, Value *);
		static bool __stdcall WantEvent(Schema::Event);
		static bool __stdcall WantPropertyEvent(int);

	private:
		static long __stdcall GetBool(VARIANT *, Value *);
		static void __stdcall GetExpandCollapseState(VARIANT *);
		static long __stdcall GetInt(VARIANT *, Value *);
		static long __stdcall GetString(VARIANT *, Value *);
		static void __stdcall GetToggleState(VARIANT *);
		static long __stdcall HandleAccChange(Element *, IRawElementProviderSimple *, Value *, Value *);
		static long __stdcall HandleAccDesc(Element *, IRawElementProviderSimple *, Value *, Value *);
		static long __stdcall HandleAccPatternChange(Element *, IRawElementProviderSimple *, unsigned int, unsigned int, int, VARIANT *, VARIANT *, void(__stdcall *)(VARIANT *));
		static long __stdcall HandleAccRoleEvent(IRawElementProviderSimple *, Value *, Value *);
		static long __stdcall HandleAccStateChange(IRawElementProviderSimple *, unsigned int, unsigned int, int, VARIANT *, VARIANT *);
		static long __stdcall HandleBoolProp(Element *, bool(__stdcall *)(Element *), IRawElementProviderSimple *, int, Value *, Value *);
		static long __stdcall HandleChildrenEvent(Element *, Value *, Value *);
		static long __stdcall HandleRangeValue(Element *, IRawElementProviderSimple *, Value *, Value *);
		static long __stdcall HandleScrollPos(Element *, IRawElementProviderSimple *, Value *, Value *);
		static long __stdcall HandleSelectedChange(IRawElementProviderSimple *, Value *);
		static long __stdcall HandleStringProp(IRawElementProviderSimple *, int, Value *, Value *);
		static long __stdcall HandleVisibilityChange(Element *, unsigned int);

		static long __stdcall AddRectangleChange(Element *, bool, bool);
		static bool __stdcall WantEvent(Schema::Event, int);
		static long __stdcall RaiseVisibilityEvents();
		static long __stdcall RaiseStructureEvents();
		static long __stdcall RaiseStructureChangedEvent(Element *, StructureChangeType);
		static long __stdcall RaiseChildRemovedEvent(const ElementRuntimeId &, Element *);
		static long __stdcall RaiseGeometryEventWorker(RectangleChange *, bool, bool, bool, bool);
		static long __stdcall RaiseGeometryEvents();

		static UiaArray<int> * g_pArrayPropertyEvent;
		static bool g_fWantAnyEvent;
		static unsigned int * __ptr32 g_eventRegisteredMap;
		static CRITICAL_SECTION g_cs;
		static int const cChangeBulk;
	};
}