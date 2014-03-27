
#ifndef _OPTIONMAP_H_
#define _OPTIONMAP_H_


template <typename T>
struct _Type
{
	T* value;
	char* type;
	_Type(){value = NULL;type = NULL;}
	~_Type()
	{
		if ( value )
			delete value;
		value = NULL;
		if ( type )
			delete type;
		type = NULL;
	}
};

struct OPTION_MAP_ENTRY
{
	LPSTR pSection;
	LPSTR pValueName;
	long pVariable;
	LPSTR szType;
};


#define DECLARE_OPTION(type,name)								\
	_Type<type> name;

#define OPTION_VALUE_OFFSET(classname, option)					\
	(FIELD_OFFSET(classname,option) + FIELD_OFFSET(_Type<void>,value))

#define OPTION_TYPE_OFFSET(classname, option)						\
	(FIELD_OFFSET(classname,option) + FIELD_OFFSET(_Type<void>,type))

#define ACTUAL_ADDRESS( vir )									\
		(vir + (long)this)

#define OPTION_MAP_INC_SECTION(pSection,pValueName,pVariable,pType)\
	{pSection,pValueName,(long)pVariable,(char*)pType},

#define OPTION_MAP_NO_SECTION(pValueName,pVariable,pType)	\
	{"PUBLIC",pValueName,(long)pVariable,(char*)pType},

#define DECLARE_OPTION_MAP()								\
private:													\
	static	const OPTION_MAP_ENTRY _OptionEntries[];		\
	protected:												\
	static	const OPTION_MAP_ENTRY* PASCAL GetOptionMap() ;

#define BEGIN_OPTION_MAP(theClass)							\
	const OPTION_MAP_ENTRY* PASCAL theClass::GetOptionMap() \
	{return &theClass::_OptionEntries[0];}					\
	const OPTION_MAP_ENTRY theClass::_OptionEntries[]={

#define END_OPTION_MAP()									\
	{0,0,0,0}};			

#define DECLARE_DEFAULT_CONFIG_MAP()						\
	private:												\
		static	const OPTION_MAP_ENTRY _DefaultEntries[];	\
	protected:												\
		static	const OPTION_MAP_ENTRY* PASCAL GetDefaultMap() ;

#define BEGIN_DEFAULT_CONFIG(theClass)						\
	const OPTION_MAP_ENTRY* PASCAL theClass::GetDefaultMap()\
	{return &theClass::_DefaultEntries[0];}					\
	const OPTION_MAP_ENTRY theClass::_DefaultEntries[]={

#define END_DEFAULT_MAP()									\
	{0,0,0,0}};

#define GET_DEFAULT_VALUE(_pSection,_pValueName,_pVariable,_pType)								\
{																									\
	OPTION_MAP_ENTRY* defEntry = (OPTION_MAP_ENTRY *)GetDefaultMap();							\
	while (defEntry&&defEntry->pValueName)															\
	{																							\
		if (((defEntry->pSection&&_pSection&&strcmp(defEntry->pSection,_pSection)==0)||(!defEntry->pSection&&!_pSection))\
			&&strcmp(defEntry->pValueName,_pValueName)==0)											\
		{																						\
			if (((char**)_pVariable)&&((char**)_pType))												\
			{																					\
				if ((char*)defEntry->szType)														\
				{																				\
					if (strcmp("INT",defEntry->szType) == 0)										\
					{																			\
						if(!(*((int**)_pVariable)))													\
						{																		\
							*((int**)_pVariable) = new int;											\
						}																		\
						**((int**)_pVariable) = *((int*)defEntry->pVariable);						\
						if (!*((char**)_pType))													\
						{																			\
							*(char**)_pType = new char[strlen(defEntry->szType)+1];				\
						}																			\
						strcpy(*((char**)_pType),defEntry->szType);								\
					}																				\
					else if (strcmp("FLOAT",defEntry->szType) == 0)								\
					{																				\
						if(!(*((float**)_pVariable)))											\
						{																			\
							*((float**)_pVariable) = new float;									\
						}																			\
						**((float**)_pVariable) = *((float*)defEntry->pVariable);				\
						if (!*((char**)_pType))														\
						{																		\
							*(char**)_pType = new char[strlen(defEntry->szType)+1];					\
						}																		\
						strcpy(*((char**)_pType),defEntry->szType);									\
					}																			\
					else if (strcmp("LONG",defEntry->szType) == 0)									\
					{																			\
						if(!(*((long**)_pVariable)))												\
						{																		\
							*((long**)_pVariable) = new long;										\
						}																		\
						**((long**)_pVariable) = *((long*)defEntry->pVariable);						\
						if (!*((char**)_pType))													\
						{																			\
							*(char**)_pType = new char[strlen(defEntry->szType)+1];				\
						}																			\
						strcpy(*((char**)_pType),defEntry->szType);								\
					}																				\
					else if(strcmp("QWORD", defEntry->szType) == 0)								\
					{																				\
						if(!(*((__int64**)_pVariable)))											\
						{																			\
							*((__int64**)_pVariable) = new __int64;								\
						}																			\
						**((__int64**)_pVariable) = *((__int64*)defEntry->pVariable);			\
						if (!*((char**)_pType))														\
						{																		\
							*(char**)_pType = new char[strlen(defEntry->szType)+1];					\
						}																		\
						strcpy(*((char**)_pType),defEntry->szType);									\
					}																			\
					else if (strcmp("CHAR",defEntry->szType) == 0)									\
					{																			\
						if(!(*((char**)_pVariable)))												\
						{																		\
							*((char**)_pVariable) = new char[strlen((char*)defEntry->pVariable)+1];	\
						}																		\
						strcpy(*((char**)_pVariable),(char*)defEntry->pVariable);					\
						if (!*((char**)_pType))													\
						{																			\
							*(char**)_pType = new char[strlen(defEntry->szType)+1];				\
						}																			\
						strcpy(*((char**)_pType),defEntry->szType);								\
					}																				\
				}																				\
				else																				\
				{																				\
					ASSERT(FALSE);																	\
				}																				\
			}																						\
			break;																				\
		}																							\
		defEntry++;																				\
	}																								\
}

#define READ_DEFAULT_CONFIG()								\
{															\
	const OPTION_MAP_ENTRY* mapEntry=GetOptionMap();		\
	while (mapEntry&&mapEntry->pValueName)					\
	{														\
		if(ACTUAL_ADDRESS(mapEntry->pVariable)&&ACTUAL_ADDRESS(mapEntry->szType))			\
		{													\
			GET_DEFAULT_VALUE(mapEntry->pSection,mapEntry->pValueName,(char**)ACTUAL_ADDRESS(mapEntry->pVariable),(char**)ACTUAL_ADDRESS(mapEntry->szType));\
			mapEntry++;										\
		}													\
		else												\
		{													\
			::MessageBox(NULL,"Read default Config fail","Option Map Error",MB_OK);	\
			exit(0);										\
		}													\
	}														\
}
#define SafeSetConfig(option,_value,_type)					\
{															\
	char __type[] = #_type;									\
	strupr(__type);											\
	if (!option.value)										\
	{														\
		option.value = new _type;							\
	}														\
	if (!option.type)										\
	{														\
		option.type = new char[strlen(__type)+1];			\
	}														\
	*option.value = (_type)_value;							\
	strcpy(option.type,__type);								\
}

#define CLEAN_UP_OPTION()									\
{															\
	/*清理参数占用空间*/									\
	const OPTION_MAP_ENTRY* mapEntry = GetOptionMap();		\
	while (mapEntry && mapEntry->pValueName)					\
	{														\
		if(ACTUAL_ADDRESS(mapEntry->pVariable)&&ACTUAL_ADDRESS(mapEntry->szType)&&*((void**)ACTUAL_ADDRESS(mapEntry->pVariable))&&*((void**)ACTUAL_ADDRESS(mapEntry->szType)))\
		{													\
			delete *((void**)ACTUAL_ADDRESS(mapEntry->pVariable));			\
			*((void**)ACTUAL_ADDRESS(mapEntry->pVariable)) = NULL;			\
			delete *((void**)ACTUAL_ADDRESS(mapEntry->szType));				\
			*((void**)ACTUAL_ADDRESS(mapEntry->szType)) = NULL;				\
		}													\
		mapEntry++;											\
	}														\
}

#define DUMP_OPTION(writer)																\
{																						\
	const OPTION_MAP_ENTRY* mapEntry = GetOptionMap();									\
	while (mapEntry && mapEntry->pValueName)											\
	{																					\
		writer<<mapEntry->pValueName<<"\t:";											\
		if(																				\
			ACTUAL_ADDRESS(mapEntry->pVariable)				&&							\
			ACTUAL_ADDRESS(mapEntry->szType)				&&							\
			*((void**)ACTUAL_ADDRESS(mapEntry->pVariable))	&&							\
			*((void**)ACTUAL_ADDRESS(mapEntry->szType)))								\
		{																				\
			if (_tcscmp("INT",*((char**)ACTUAL_ADDRESS(mapEntry->szType))) == 0)		\
			{																			\
				writer<<"VALUE:"<<**((int**)ACTUAL_ADDRESS(mapEntry->pVariable));		\
				writer<<" TYPE:"<< *((char**)ACTUAL_ADDRESS(mapEntry->szType));			\
			}																			\
			else if (_tcscmp("LONG",*((char**)ACTUAL_ADDRESS(mapEntry->szType))) == 0)	\
			{																			\
				writer<<"VALUE:"<<**((long**)ACTUAL_ADDRESS(mapEntry->pVariable));		\
				writer<<" TYPE:"<< *((char**)ACTUAL_ADDRESS(mapEntry->szType));			\
			}																			\
			else if (_tcscmp("FLOAT",*((char**)ACTUAL_ADDRESS(mapEntry->szType))) == 0)	\
			{																			\
				writer<<"VALUE:"<<**((float**)ACTUAL_ADDRESS(mapEntry->pVariable));		\
				writer<<" TYPE:"<< *((char**)ACTUAL_ADDRESS(mapEntry->szType));			\
			}																			\
			else if (_tcscmp("CHAR",*((char**)ACTUAL_ADDRESS(mapEntry->szType))) == 0)	\
			{																			\
				writer<<"VALUE:"<<*((char**)ACTUAL_ADDRESS(mapEntry->pVariable));		\
				writer<<" TYPE:"<< *((char**)ACTUAL_ADDRESS(mapEntry->szType));			\
			}																			\
		}																				\
		else																			\
		{																				\
			writer<<"VALUE:NULL TYPE:NULL";												\
		}																				\
		writer<<"\r\n";																	\
		mapEntry++;																		\
	}																					\
}

#define ASSERT_VALID_OPTION()															\
{																						\
	const OPTION_MAP_ENTRY* mapEntry = GetOptionMap();									\
	while (mapEntry && mapEntry->pValueName)											\
	{																					\
		ASSERT(																			\
			ACTUAL_ADDRESS(mapEntry->pVariable)				&&							\
			ACTUAL_ADDRESS(mapEntry->szType)				&&							\
			*((void**)ACTUAL_ADDRESS(mapEntry->pVariable))	&&							\
			*((void**)ACTUAL_ADDRESS(mapEntry->szType)));								\
		mapEntry++;																		\
	}																					\
}


#define CLEAN_UP_DEF_OPTION()															\
{																						\
 	/*清理默认参数占用空间*/															\
	const OPTION_MAP_ENTRY* defEntry = GetDefaultMap();									\
	while (defEntry && defEntry->pVariable)												\
	{																					\
		delete (void*)defEntry->pVariable;												\
		memset((void*)(&defEntry->pVariable),0, sizeof(void*));							\
		defEntry ++ ;																	\
	}																					\
}

#endif //OPTION_H

