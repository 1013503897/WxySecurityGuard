#include "SecurityGuardSever.h"

#include <openssl/evp.h>
#pragma comment(lib, "libcrypto.lib")

vector<SOCKET> clients;
char* g_file_buffer;
CRITICAL_SECTION g_cs;
int g_length; 
void check_result(bool is_true, const char* error)
{
	if (is_true)
	{
		printf("error: %s\n", error);
		system("pause"); exit(-1);
	}
}

FILE_INFO g_file_info = { 0 };
HANDLE g_file;
DWORD WINAPI WorkerThread(LPVOID param)
{
	HANDLE iocp = (HANDLE)param;				
	DWORD read = 0;								
	SOCKET comkey = 0;							
	MY_OVERLAPPED* overlapped;					
	MYSQL mysql; 
	mysql_init(&mysql);

	mysql_real_connect(&mysql, "127.0.0.1", "root",
		"123456", "m_security_guards", 3306, nullptr, 0);
	while (true)
	{
		BOOL result = GetQueuedCompletionStatus(
			iocp,
			&read,
			(ULONG_PTR*)&comkey,
			(LPOVERLAPPED*)&overlapped,
			-1);
		if (result == FALSE && read == 0)
		{
			for (size_t i = 0; i < clients.size(); ++i)
			{
				if (comkey == clients[i])
				{
					clients.erase(clients.begin() + i);
					break;
				}
			}
		}
		if (g_file_info.section_count != 0)
		{
			FILE_SECTION file_section = { 0 };
			memcpy(&file_section, overlapped->wsabuf.buf, sizeof(file_section));
			memcpy(g_file_buffer + SECTION_SIZE * file_section.index, file_section.data, file_section.size);
			printf("第%d次传输", file_section.index);
			EnterCriticalSection(&g_cs);
			if (file_section.index == g_file_info.section_count-1)
			{
				DWORD dwBytesWrite = 0;
				DWORD dwBytesToWrite = g_file_info.file_size;
				char* tmpBuf = g_file_buffer;
				do {
					WriteFile(g_file, tmpBuf, dwBytesToWrite, &dwBytesWrite, NULL);
					if (dwBytesWrite == 0)
						break;
					dwBytesToWrite -= dwBytesWrite;
					tmpBuf += dwBytesWrite;
				} while (dwBytesToWrite > 0);
				printf("传输完成");
				CloseHandle(g_file);
				g_file_info.section_count = 0; 
				g_file = 0;
				delete g_file_buffer; 
			}
			LeaveCriticalSection(&g_cs);
		}
		else if(!memcmp(overlapped->wsabuf.buf, "FileInfo", sizeof("FileInfo")))
		{
			memcpy(&g_file_info, overlapped->wsabuf.buf, sizeof(g_file_info));
			printf("file: %s\n", g_file_info.name);
			CStringA tmpPath = "E:\\wxyServer\\SampleFile\\";
			tmpPath += g_file_info.name;
			g_file = CreateFileA(tmpPath, GENERIC_WRITE, FILE_SHARE_WRITE,
				NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			g_file_buffer = new char[g_file_info.file_size];
			insert(&mysql, g_file_info);
		}
		else if (!strcmp(overlapped->wsabuf.buf, "selectall"))
		{
			vector<SAMPLE_INFO> SampleVec;
			if (query_all(&mysql, SampleVec))
			{
				int count = SampleVec.size();
				send(comkey, (char*)&count, sizeof(int), 0);
				for (int i = 0; i < SampleVec.size(); i++)
				{
					send(comkey, SampleVec[i].md5, 33, 0);
					send(comkey, SampleVec[i].des, 100, 0);
				}
			}
			else
			{
				printf("query_all error\r\n");
			}
		}
		else if (!memcmp(overlapped->wsabuf.buf, "DeleteFile", sizeof("DeleteFile")-1))
		{
			delete_info(&mysql, overlapped->wsabuf.buf + sizeof("DeleteFile")-1);
		}
		else if (!memcmp(overlapped->wsabuf.buf, "DownLoad", sizeof("DownLoad") - 1))
		{
			download(&mysql, overlapped->wsabuf.buf + sizeof("DownLoad")-1, comkey);
		}
		else
		{
			evp_de_cipher((unsigned char*)overlapped->wsabuf.buf, (unsigned char*)overlapped->wsabuf.buf, 40);
			send(comkey, query(&mysql, overlapped->wsabuf.buf), 100, 0);
		}
		DWORD read = 0, Flags = 0; 
		WSARecv(
			comkey,
			&overlapped->wsabuf,
			1,
			&read,
			&Flags,
			(OVERLAPPED*)overlapped,
			NULL);
	}
	return 0;
}

int main()
{
	printf("Hellow World\r\n"); 
	InitializeCriticalSection(&g_cs);
	mysql_library_init(0, 0, NULL);
	HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SYSTEM_INFO system_info = { 0 };
	GetSystemInfo(&system_info);
	for (DWORD i = 0; i < system_info.dwNumberOfProcessors; ++i)
		CreateThread(NULL, NULL, WorkerThread, (LPVOID)iocp, NULL, NULL);
	WSADATA WsaData = { 0 };
	int result = WSAStartup(0x0202, &WsaData);
	check_result(result != 0 || 0x0202 != WsaData.wVersion, "WSAStartup();");

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	check_result(server == INVALID_SOCKET, "socket();");

	sockaddr_in server_addr = { AF_INET };		
	server_addr.sin_port = htons(0x1515);		
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	result = bind(server, (sockaddr*)&server_addr, sizeof(server_addr));
	check_result(result == SOCKET_ERROR, "bind();");

	result = listen(server, SOMAXCONN);
	check_result(result == SOCKET_ERROR, "listen();");

	while (true)
	{
		int size = sizeof(sockaddr_in);
		sockaddr_in client_addr = { 0 };
		SOCKET client = accept(server, (sockaddr*)&client_addr, &size);
		check_result(result == INVALID_SOCKET, "accept();");
		clients.push_back(client);
		CreateIoCompletionPort((HANDLE)client, iocp, client, 0);
		MY_OVERLAPPED* overlapped = new MY_OVERLAPPED{ 0 };
		overlapped->wsabuf.len = sizeof(_FILE_SECTION);
		overlapped->wsabuf.buf = new CHAR[overlapped->wsabuf.len]{ 0 };

		DWORD read = 0, Flags = 0;
		WSARecv(
			client,						
			&overlapped->wsabuf,		
			1,							
			NULL,						
			&Flags,						
			(OVERLAPPED*)overlapped,	
			NULL);						
	}
	DeleteCriticalSection(&g_cs);
	closesocket(server);
	WSACleanup();
	mysql_library_end();
	system("pause");
	return 0;
}

void insert(MYSQL* mysql, FILE_INFO FileInfo)
{
	CStringA tmp;
	CStringA tmpPath = "E:\\\\wxyServer\\\\SampleFile\\\\";
	tmpPath += FileInfo.name;
	tmp.Format("INSERT INTO md5_table(md5_value,file_fullpath,file_description) VALUES ('%s','%s','%s')",
		FileInfo.file_md5, tmpPath, FileInfo.file_description);
	mysql_query(mysql, tmp);
	if (mysql_errno(mysql))
	{
		return;
	}
}

char* query(MYSQL* mysql, LPCSTR md5)
{
	char* ret = new char[100]{ 0 };
	CStringA query_sql;
	query_sql.Format("SELECT * FROM md5_table WHERE md5_value = '%s'", md5);
	mysql_query(mysql, query_sql);
	if (mysql_errno(mysql))
	{
		int a = mysql_errno(mysql); 
		CStringA err;
		err.Format("%s", mysql_error(mysql));
		return ret;
	}
	MYSQL_RES* pRes = mysql_use_result(mysql);
	if (mysql_errno(mysql))
	{
		return ret;
	}
	MYSQL_ROW row = nullptr;

	if (row = mysql_fetch_row(pRes))
	{
		strcpy(ret, row[2]);
		mysql_free_result(pRes);
		return ret;
	}
	mysql_free_result(pRes);
	return ret;
}

bool query_all(MYSQL* mysql,vector<SAMPLE_INFO>& SampleVec)
{
	char* ret = new char[100]{ 0 };
	CStringA query_sql;
	query_sql.Format("SELECT * FROM md5_table");
	mysql_query(mysql, query_sql);
	if (mysql_errno(mysql))
	{
		int a = mysql_errno(mysql);
		CStringA err;
		err.Format("%s", mysql_error(mysql));
		printf("%s", err);
		return NULL;
	}
	MYSQL_RES* pRes = mysql_use_result(mysql);
	if (mysql_errno(mysql))
	{
		return ret;
	}
	MYSQL_ROW row = nullptr;
	while (row = mysql_fetch_row(pRes))
	{
		SAMPLE_INFO SampleInfp = { 0 };
		strcpy(SampleInfp.md5, row[0]);
		strcpy(SampleInfp.des, row[2]);
		SampleVec.push_back(SampleInfp);
	}
	mysql_free_result(pRes);
	return ret;
}

bool download(MYSQL* mysql, LPCSTR md5, SOCKET client)
{
	CStringA query_sql;
	query_sql.Format("SELECT file_fullpath FROM md5_table WHERE md5_value = '%s'", md5);
	mysql_query(mysql, query_sql); 
	if (mysql_errno(mysql))
	{
		int a = mysql_errno(mysql);
		CStringA err;
		err.Format("%s", mysql_error(mysql));
		return false;
	}
	MYSQL_RES* pRes = mysql_use_result(mysql);
	if (mysql_errno(mysql))
	{
		return false;
	}
	MYSQL_ROW row = nullptr;

	char file_fullpath[MAX_PATH] = { 0 };
	if (row = mysql_fetch_row(pRes))
	{
		strcpy(file_fullpath, row[0]);
		mysql_free_result(pRes);
	}
	HANDLE file = CreateFileA(file_fullpath, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD size = GetFileSize(file, NULL);

	int section_count = size % SECTION_SIZE == 0
		? size / SECTION_SIZE : size / SECTION_SIZE + 1;
	char filename[MAX_PATH] = { 0 }; 
	char* i = file_fullpath + strlen(file_fullpath);
	for (; i != file_fullpath; i--)
	{
		if (*i == '\\')
			break;
	}
	strcpy(filename, ++i);
	FILE_INFO file_info;
	strcpy(file_info.name, filename);
	file_info.section_count = section_count;
	send(client, (char*)&file_info, sizeof(file_info), 0);

	for (int i = 0; i < section_count; ++i)
	{
		FILE_SECTION file_section = { i };

		SetFilePointer(file, SECTION_SIZE * i, 0, FILE_BEGIN);

		ReadFile(file, file_section.data, SECTION_SIZE, &file_section.size, NULL);

		send(client, (char*)&file_section, sizeof(file_section), 0);
	}
}
void delete_info(MYSQL* mysql, LPCSTR md5)
{
	CStringA query_sql;
	query_sql.Format("DELETE FROM md5_table WHERE md5_value = '%s'", md5); 
	mysql_query(mysql, query_sql);
}

int evp_en_cipher(unsigned char* source_string, unsigned char* des_string, int length)
{
	// 创建一个通用加解密的对象，设置填充方式
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	// once_length 是一次加密的长度, out_length 加密后密文的长度 
	int once_length = 0, out_length = 0;

	// 设置加密时使用的算法+分组模式和 key，函数的倒数第二个参数可以
	// 用于指定初始化向量，最后一个参数是 1 表示加密，否则解密
	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 1);

	// 使用 EVP_CipherUpdate + EVP_CipherFinal 完成整个加密
	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	// 清理对象并返回长度
	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}

int evp_de_cipher(unsigned char* source_string, unsigned char* des_string, int length)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_set_padding(ctx, 1);

	int once_length = 0, out_length = 0;

	// 解密的代码和加密除 EVP_CipherInit_ex 的最后一个参数外基本相同
	unsigned char key[16] = "15pb";
	EVP_CipherInit_ex(ctx, EVP_des_ede3_ecb(), NULL, key, nullptr, 0);

	EVP_CipherUpdate(ctx, des_string, &once_length, source_string, length);
	out_length += once_length;
	EVP_CipherFinal(ctx, des_string + once_length, &once_length);
	out_length += once_length;

	// 为加密后的数据添加空字符
	des_string[out_length] = 0;

	EVP_CIPHER_CTX_free(ctx);
	return out_length;
}
