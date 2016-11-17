// dllmain.h : 模块类的声明。

class COverlayIconModule : public ATL::CAtlDllModuleT< COverlayIconModule >
{
public :
	DECLARE_LIBID(LIBID_OverlayIconLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OVERLAYICON, "{6A29588C-B0AD-4632-A4B4-1A02ABEAD03C}")
};

extern class COverlayIconModule _AtlModule;
