#ifndef Engine_Macro_h__
#define Engine_Macro_h__

#define D3DCOLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))

#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif


#define NO_COPY(CLASSNAME)														\
		private:																\
		CLASSNAME(const CLASSNAME&);											\
		CLASSNAME& operator = (const CLASSNAME&);								

#define DECLARE_SINGLETON(CLASSNAME)											\
		NO_COPY(CLASSNAME)														\
		private:																\
		static shared_ptr<CLASSNAME> m_pInstance;								\
		public:																	\
		static CLASSNAME* GetInstance( void );									\
		static void DestroyInstance( void );									

#define IMPLEMENT_SINGLETON(CLASSNAME)											\
		shared_ptr<CLASSNAME> CLASSNAME::m_pInstance = nullptr;					\
		CLASSNAME* CLASSNAME::GetInstance( void ){								\
			if(nullptr == m_pInstance){											\
				struct SmatPointerEnabler : public CLASSNAME{					\
					SmatPointerEnabler() : CLASSNAME() {};						\
				};																\
				m_pInstance	= make_shared<SmatPointerEnabler>();				\
			}																	\
			return m_pInstance.get();											\
		}																		\
		void CLASSNAME::DestroyInstance(void) {									\
				if (nullptr != m_pInstance) {									\
					m_pInstance.reset();										\
				}																\
		}
		

#endif // Engine_Macro_h__
