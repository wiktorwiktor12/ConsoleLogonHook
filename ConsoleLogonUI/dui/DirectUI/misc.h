#pragma once

namespace DirectUI {
// exported for int
template <typename T> class UILIB_API SafeArrayAccessor {
public:
  SafeArrayAccessor();
  ~SafeArrayAccessor();
  SafeArrayAccessor(const SafeArrayAccessor &) = delete;
  SafeArrayAccessor &operator=(const SafeArrayAccessor &) = delete;

  operator T *();
  long Access(SAFEARRAY *, UChar);
  int Count();

private:
  void *unk1;
  void *unk2;
  void *unk3;
};

class UILIB_API CritSecLock {
public:
  CritSecLock(CRITICAL_SECTION *);
  ~CritSecLock();
  CritSecLock &operator=(CritSecLock const &);
  void Unlock();
private:
	CRITICAL_SECTION* m_critcalSection;
};

template <class T, int> class DynamicArray;

class UILIB_API Expression {
public:
  Expression() = delete;
  Expression(const Expression &) = delete;
  ~Expression() = delete;

  Expression &operator=(Expression const &);
  void Destroy(void);
};

/*class UID
{
public:
};*/

class UILIB_API Surface {
public:
  enum EType {

  };

  Surface(Surface const &);
  Surface(void);
  virtual ~Surface(void);
  Surface &operator=(Surface const &);

  static enum EType __stdcall GetSurfaceType(unsigned int);
  static unsigned int __stdcall GetSurfaceType(enum EType);

  virtual void T1() = 0;
};

class UILIB_API DCSurface {
public:
  DCSurface(DCSurface const &);
  DCSurface(HDC);
  virtual ~DCSurface(void);
  DCSurface &operator=(DCSurface const &);

  HDC GetHDC(void);
  virtual Surface::EType GetType(void) const;
};

enum DUSER_MSG_FLAG : UINT32 {
  GMF_DIRECT = 0x00000000,      // OnMessage
  GMF_ROUTED = 0x00000001,      // PreviewMessage
  GMF_BUBBLED = 0x00000002,     // PostMessage
  GMF_EVENT = 0x00000003,       // Message -> Event
  GMF_DESTINATION = 0x00000003, // Message reach dest (same as event)
};

enum DUSER_INPUT_DEVICE : UINT32 {
  GINPUT_MOUSE = 0,
  GINPUT_KEYBOARD = 1,
  GINPUT_JOYSTICK = 2,
};

enum DUSER_INPUT_CODE : UINT32 {
  GMOUSE_MOVE = 0,
  GMOUSE_DOWN = 1,
  GMOUSE_UP = 2,
  GMOUSE_DRAG = 3,
  GMOUSE_HOVER = 4,
  GMOUSE_WHEEL = 5,
  GMOUSE_MAX = 5,

  GBUTTON_NONE = 0,
  GBUTTON_LEFT = 1,
  GBUTTON_RIGHT = 2,
  GBUTTON_MIDDLE = 3,
  GBUTTON_MAX = 3,
};

// bitflags
enum DUSER_INPUT_MODIFIERS : UINT32 {
  GMODIFIER_NONE = 0x00000000,

  GMODIFIER_LCONTROL = 0x00000001,
  GMODIFIER_RCONTROL = 0x00000002,
  GMODIFIER_LSHIFT = 0x00000004,
  GMODIFIER_RSHIFT = 0x00000008,
  GMODIFIER_LALT = 0x00000010,
  GMODIFIER_RALT = 0x00000020,
  GMODIFIER_LBUTTON = 0x00000040,
  GMODIFIER_RBUTTON = 0x00000080,
  GMODIFIER_MBUTTON = 0x00000100,

  GMODIFIER_CONTROL = (GMODIFIER_LCONTROL | GMODIFIER_RCONTROL),
  GMODIFIER_SHIFT = (GMODIFIER_LSHIFT | GMODIFIER_RSHIFT),
  GMODIFIER_ALT = (GMODIFIER_LALT | GMODIFIER_RALT),
};

struct Event {
  Element *target;
  UID type;
  bool handled;
  DUSER_MSG_FLAG flag;
};

struct InputEvent {
  Element *target;
  bool handled;
  DUSER_MSG_FLAG flag;
  DUSER_INPUT_DEVICE device;
  DUSER_INPUT_CODE code;
  DUSER_INPUT_MODIFIERS modifiers;
};

struct RectangleChange {};

struct ElementRuntimeId {};

class UILIB_API ResourceModuleHandles {
public:
  ResourceModuleHandles(void);
  ~ResourceModuleHandles(void);
  ResourceModuleHandles &operator=(ResourceModuleHandles const &);
  long GetHandle(unsigned short const *, HINSTANCE *);

private:
  static long volatile g_cRefCount;
};

template <class T> class UiaArray {
public:
};

class DeferCycle {};

} // namespace DirectUI
