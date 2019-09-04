#ifndef BUS_H_
#define BUS_H_

#include<stdint.h>
#include<stddef.h>

/* Generic object to hold any access functions */
struct _BUS_Obj_t
{
	/* Function to write a 8-bit word to bus */
	int (*pWrite8)(void*, uint32_t, uint8_t);
	/* Function to write a 8-bit word to bus */
	int (*pRead8)(void*, uint32_t, uint8_t*);
	/* Object required by read/write functions */
	void* pObj;
};
typedef struct _BUS_Obj_t BUS_Obj_t;
typedef BUS_Obj_t* pBUS_Obj_t;

#endif /* BUS_H_ */

