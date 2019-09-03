#ifndef SANITY_H_
#define SANITY_H_

inline void Warning(const char* pName, unsigned int line, const char* pWarning);
inline void Error(const char* pName, unsigned int line, const char* pError);
inline int SoftPtrCheck(void* pVoid, const char* pName, unsigned int line);
inline void HardPtrCheck(void* pVoid, const char* pName, unsigned int line);

#endif /* SANITY_H_ */

