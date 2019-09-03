#include "pipeBus.h"
#include "sanity.h"
#include "log.h"

void showUsage(const char* name)
{

}

int main (int argc, const char*argv[])
{
	pPIPE_Obj_t pPIPE;
	
	/* Verify number of entries */
	if(argc < 2)
	{
		showUsage(argv[0]);
		return -1;
	}


	/* Success */
	return 0;

failure:
	/* failure */
	PIPE_Free(pPipe);
	return -1;
}
