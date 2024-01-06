#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine
{	
	template<typename T>
	void Safe_Delete(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& pPointer)
	{
		if (nullptr != pPointer)
		{
			delete [] pPointer;
			pPointer = nullptr;
		}
	}

	template<typename T>
	struct SharedEnabler : public T 
	{
		SharedEnabler() : T() {}
	};

}

#endif // Engine_Function_h__
