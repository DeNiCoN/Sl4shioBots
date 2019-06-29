/*
Copyright(c) 2019 DeNiCoN

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "poolAlloc.h"
#include <ctype.h>

int poolInit(PoolAllocator* alloc, char* buffer, size_t objSize, unsigned int objCount) 
{
	if (objSize < MIN_POOL_OBJECT_SIZE_BYTES) 
	{
		return 0;
	}
	alloc->buffer = buffer;
	alloc->nextFreeBlock = (void*)buffer;
	alloc->objSize = objSize;
	alloc->objCount = objCount;
	char* current = buffer;
	char* next;
	for (unsigned int i = 0; i < objCount; i++) 
	{
		next = current + objSize;
		*((uintptr_t*)current) = (uintptr_t) next;
		current = next;
	}
	*((uintptr_t*)(current - objSize)) = (uintptr_t) NULL;
	return 1;
}

void* poolAlloc(PoolAllocator* alloc) 
{
	if (alloc->nextFreeBlock)
	{
		void* ptr = alloc->nextFreeBlock;
		alloc->nextFreeBlock = (void*)(*(uintptr_t*)ptr); // take pointer from ptr and set nextFreeBlock to it
		return ptr;
	}
	return NULL;
}

void poolFree(PoolAllocator* alloc, void* pointer) 
{
	*((uintptr_t*) pointer) = (uintptr_t) alloc->nextFreeBlock; // write nextFreeBlock in point that pointer points
	alloc->nextFreeBlock = pointer;
}

void poolReset(PoolAllocator* alloc) 
{
	poolInit(alloc, alloc->buffer, alloc->objSize, alloc->objCount);
}