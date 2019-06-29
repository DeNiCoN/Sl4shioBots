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
#pragma once
#include <ctype.h>
#define MIN_POOL_OBJECT_SIZE_BYTES sizeof(int)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char* buffer;
	unsigned int objCount;
	size_t objSize;
	void* nextFreeBlock;
} PoolAllocator;
int poolInit(PoolAllocator* alloc, char* buffer, size_t objSize, unsigned int objCount); //places pointer to next free block in every block
void* poolAlloc(PoolAllocator* alloc); //remove pointer to next free block, return free block
void poolFree(PoolAllocator* alloc, void* pointer); // place pointer into block, set next free block to this pointer
void poolReset(PoolAllocator* alloc); //works like init

#ifdef __cplusplus
}
#endif