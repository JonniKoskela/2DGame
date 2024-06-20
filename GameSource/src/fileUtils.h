#pragma once
#include <assert.h>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <math.h>
#include <algorithm>


#define b8 char
#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long) 1024 * x)
#define MB(x) ((unsigned long long) 1024 * KB(x))
#define GB(x) ((unsigned long long) 1024 * MB(x))







struct BumpAllocator {
	size_t capacity;
	size_t used;
	char* memory;
};

BumpAllocator makeBumpAllocator(size_t size)
{
	BumpAllocator bump = {};

	bump.memory = (char*)malloc(size);

	if (bump.memory)
	{
		bump.capacity = size;
		memset(bump.memory, 0, size);
	}
	else
	{
		assert(false && "failed to allocate memory");
	}
	return bump;
}


char* bumpAlloc(BumpAllocator* allocator, size_t size)
{
	char* result = nullptr;

	size_t alignedSize = (size + 7) & ~7;

	if (allocator->used + alignedSize <= allocator->capacity)
	{
		result = allocator->memory + allocator->used;
		allocator->used += alignedSize;
	}
	else
	{
		assert(false && "allocator full");
	}
	return result;
}


long long getTimeStamp(const char* file)
{
	struct stat fileStat = {};
	stat(file, &fileStat);
	return fileStat.st_mtime;
}


bool file_exists(char* filePath)
{
	assert(filePath && "no filepath");

	auto file = fopen(filePath, "rb");
	if (!file)
	{
		return false;
	}
	fclose(file);

	return true;
}


long getFileSize(const char* filePath)
{
	assert(filePath && "no filepath");

	long fileSize = 0;
	auto file = fopen(filePath, "rb");
	if (!file)
	{

		return 0;
	}
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	return fileSize;
}


const char* readFile(const char* filePath, int* fileSize,const char* buffer)
{
	assert(filePath && "no filePath");
	assert(fileSize && "no fileSize");

	*fileSize = 0;
	auto file = fopen(filePath, "rb");
	if (!file)
	{
		return nullptr;
	}

	fseek(file, 0, SEEK_END);
	*fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (*fileSize <= 0)
	{
		std::cerr << "failed to determine file size" << std::endl;
	}


	memset((void*)buffer, 0, *fileSize);
	fread((void*)buffer, sizeof(char), *fileSize, file);

	fclose(file);

	return buffer;
}


const char* readFile(char* filePath, int* fileSize, BumpAllocator* bumpAllocator)
{
	const char* file = nullptr;
	long fileSize2 = getFileSize(filePath);

	if (fileSize2)
	{
		char* buffer = bumpAlloc(bumpAllocator, fileSize2 + 1);

		file = readFile(filePath, fileSize, buffer);
	}

	return file;
}


void writeFile(char* filePath, char* buffer, int size)
{
	assert(filePath && "no filepath");

	auto file = fopen(filePath, "wb");
	if (!file)
	{
		return;
	}

	fwrite(buffer, sizeof(char), size, file);
	fclose(file);
}


bool copyFile(const char* fileName,const char* outputName,const char* buffer)
{
	int fileSize = 0;
	const char* data = readFile(fileName, &fileSize, buffer);

	auto outputFile = fopen(outputName, "wb");
	if (!outputFile)
	{
		assert("failed to open output file at bool copyFile()");
		return false;
	}

	int result = fwrite(data, sizeof(char), fileSize, outputFile);
	if (!result)
	{
		return false;
	}

	fclose(outputFile);

	return true;
}


bool copyFile(const char* fileName,const char* outputName, BumpAllocator* bumpAllocator)
{
	char* file = 0;
	long fileSize = getFileSize(fileName);

	if (fileSize)
	{
		char* buffer = bumpAlloc(bumpAllocator, fileSize);

		return copyFile(fileName, outputName, buffer);
	}
	return false;
}


//MATHFUNCTIONS____________________________________________________________________
int sign(int x)
{
	return (x >= 0) ? 1 : -1;
}

float sign(float x)
{
	return (x >= 0) ? 1 : -1;
}


