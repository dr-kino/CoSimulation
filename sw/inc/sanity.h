#ifndef SANITY_H_
#define SANITY_H_

void Warning(const char* pName, unsigned int line, const char* pWarning);
void Error(const char* pName, unsigned int line, const char* pError);
int SoftPtrCheck(void* pVoid, const char* pName, unsigned int line);
void HardPtrCheck(void* pVoid, const char* pName, unsigned int line);

#endif /* SANITY_H_ */

