#pragma once
#include <afx.h>
#include <WinSock2.h>
#include <stdio.h>
#include <ws2tcpip.h>
#include "include/mysql.h"
#include <vector>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "lib/libmysql.lib")

#define SECTION_SIZE (1024)
using namespace std;

typedef struct _MY_OVERLAPPED
{
	OVERLAPPED overlapped;
	WSABUF wsabuf;
} MY_OVERLAPPED, * PMY_OVERLAPPED;

typedef struct _FILE_INFO
{
	char sign[9] = { "FileInfo" };
	char name[MAX_PATH];		// 文件的名称
	int section_count;			// 发送文件需要多少次
	int file_size;
	char file_md5[33]{ 0 };
	char file_description[100]{ 0 }; 
} FILE_INFO, * PFILE_INFO;

typedef struct _FILE_SECTION
{
	int index;					// 当前是第几块数据
	DWORD size;					// 当前这块数据的大小
	char data[SECTION_SIZE];	// 一次传输的大小
} FILE_SECTION, * PFILE_SECTION;
typedef struct _SAMPLE_INFO
{
	char md5[33] = { 0 };
	char des[100] = { 0 };
} SAMPLE_INFO, * PSAMPLE_INFO;

void insert(MYSQL* mysql, FILE_INFO FileInfo);
char* query(MYSQL* mysql, LPCSTR md5);
bool query_all(MYSQL* mysql, vector<SAMPLE_INFO>& SampleVec);
bool download(MYSQL* mysql, LPCSTR md5, SOCKET client);
void delete_info(MYSQL* mysql, LPCSTR md5);
DWORD WINAPI WorkerThread(LPVOID param);
void check_result(bool is_true, const char* error); 
int evp_de_cipher(unsigned char* source_string, unsigned char* des_string, int length);
int evp_en_cipher(unsigned char* source_string, unsigned char* des_string, int length);