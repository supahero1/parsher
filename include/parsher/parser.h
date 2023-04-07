#ifndef _parsher_async_h_
#define _parsher_async_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


enum psh_node_type
{
	psh_node_temp
};


struct psh_node_temp
{
	int temp;
};


struct psh_node
{
	enum psh_node_type type;

	union
	{
		struct psh_node_temp;
	} data;
};


#ifdef __cplusplus
}
#endif

#endif
