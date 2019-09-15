#ifndef PIPEBUS_H
#define PIPEBUS_H
#include <stdint.h>
#include "bus.h"

struct _PIPE_Obj_t;
typedef struct _PIPE_Obj_t PIPE_Obj_t;
typedef PIPE_Obj_t* pPIPE_Obj_t;


pPIPE_Obj_t PIPE_Alloc(void);
void PIPE_Free(pPIPE_Obj_t);
int PIPE_Init(pPIPE_Obj_t, const char*, const char*);

int PIPE_Write(pPIPE_Obj_t, uint32_t, uint32_t);

void PIPE_Interactive(pPIPE_Obj_t);
int PIPE_WriteToBus(pPIPE_Obj_t, uint32_t, uint32_t);
int PIPE_ReadFromBus(pPIPE_Obj_t, uint32_t, uint32_t*);
int PIPE_EndSimulation(pPIPE_Obj_t);
void PIPE_SyncPipe(pPIPE_Obj_t);

int PIPE_Write8(void*, uint32_t, uint8_t);
int PIPE_Read8(void*, uint32_t, uint8_t*);

void* PIPE_GetRWObj(pPIPE_Obj_t);

#endif /* PIPEBUS_H */
