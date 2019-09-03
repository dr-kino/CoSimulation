#ifndef LOG_H_
#define LOG_H_

struct _LOG_Obj_t;
typedef struct _LOG_Obj_t LOG_Obj_t;
typedef LOG_Obj_t* pLOG_Obj_t;

inline void LOG_Warning(pLOG_Obj_t, const char* pName, unsigned int line, const char* pWarning);
inline void LOG_PtrCheck(pLOG_Obj_t, void* pVoid, const char* pName, unsigned int line);

#endif /* SANITY_H_ */

