#include "JlmPackTraverseAllFiles.h"
#include "JPDCG.h"
#include "BinaryFile.h"
#include "JlmPack.h"
#include "JPDCG.h"
#include "memory.h"

#if defined(_WIN32) || defined(_WIN64)
#include "Windows.h"
#define _WINDOWS
#elif defined(__linux__)
#define _LINUX
#else
#error "Unsupported platform"
#endif
// 检验JlmPack_JLMHA，测试通过了
int main01() { // 11
	int errSign = 0;
	unsigned int i, InByteArray_size = 512;
	unsigned char RandomBytes;
	unsigned char* InByteArray = NULL;
	unsigned char* HashByteArray = NULL;

	unsigned int HashByteArray_size = 16;

	HashByteArray = (unsigned char*)malloc(HashByteArray_size * sizeof(unsigned char));
	InByteArray = (unsigned char*)malloc(InByteArray_size * sizeof(unsigned char));
	if (HashByteArray == NULL || InByteArray == NULL) goto ERR;
	// 初始化哈希值为0x00
	memset(HashByteArray, 0x00, HashByteArray_size);
	// 生成随机数
	srand((unsigned)time(NULL));
	printf("生成%d个随机待编码数据：\n", InByteArray_size);
	for (i = 0; i < InByteArray_size; ++i) {
		InByteArray[i] = rand() % 0xFF; // 0x01;// 
		printf("0x%02X,", InByteArray[i]);
	}
	printf("\n\n");
	// 计算InByteArray的哈希值
	RandomBytes = rand() % 0xFF;
	printf("RandomBytes = %02X\n", RandomBytes);
	errSign = JlmPack_JLMHA(RandomBytes, InByteArray, InByteArray_size, HashByteArray, HashByteArray_size);
	if (errSign == 0x00) {
		printf("InByteArray的哈希值：\n");
		for (i = 0; i < HashByteArray_size; ++i) {
			printf("%02X,", HashByteArray[i]);
		}
	}
	printf("\n\n");
ERR:
	if (HashByteArray) free(HashByteArray);
	if (InByteArray) free(InByteArray);
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}

// 部分函数的检测
int main1() { // 1
	// 设置为""的目的是跟随系统，支持windows和linux

	char DATES[200], NameAndExtension[200], Name[200], Extension[200];

	// 区分windows和linux
#ifdef _WINDOWS

	const char* url = "D:\\Lena.bmp";

	getFileNameAndExtension(url, NameAndExtension);
	printf("%s\n", NameAndExtension);
	getFileName(NameAndExtension, Name);
	printf("%s %lld\n", Name, strlen(Name));
	getFileExtension(NameAndExtension, Extension);
	printf("%s %lld\n", Extension, strlen(Extension));


	changeTime(getFileOrFolderTime(url), DATES);
	printf("%s\n", DATES);

	system("pause");
	return 0;

#elif defined(_LINUX)

	const char* url = "/home/wangjielin/Desktop/Lena.bmp";

	getFileNameAndExtension(url, NameAndExtension);
	printf("%s\n", NameAndExtension);
	getFileName(NameAndExtension, Name);
	printf("%s %d\n", Name, strlen(Name));
	getFileExtension(NameAndExtension, Extension);
	printf("%s %d\n", Extension, strlen(Extension));

	changeTime(getFileOrFolderTime(url), DATES);
	printf("%s\n", DATES);

	return 0;
#endif
}

// 检验目录的全部功能
int main2() { // 2
	// 设置为""的目的是跟随系统，支持windows和linux

	CatalogInfor* Catalog = NULL, * Catalog1 = NULL, ** Catalog2 = NULL, * Catalog3 = NULL;
	const char** pathList = NULL;
	const char** pathList1 = NULL;
	unsigned long long bufflength = 0;
	unsigned char* buff = NULL;
	char DateTime[80];
	int i, errSign = 0, listSize = 2;
	int delIdList[2] = { 20,2 }, delListSize = 2;
	pathList = (char**)malloc(listSize * sizeof(char*));
	pathList1 = (char**)malloc(listSize * sizeof(char*));
	if (pathList == NULL) return 0;
	for (i = 0; i < listSize; ++i) {
		pathList[i] = (char*)malloc(URL_BYTE_LIMIT);
		pathList1[i] = (char*)malloc(URL_BYTE_LIMIT);
	}

	// 区分windows和linux
#ifdef _WINDOWS

	const char* url1 = "D:\\CatalogTest";
	const char* url2 = "D:\\Lena.bmp";
	const char* url3 = "D:\\360AI浏览器下载";
	const char* url4 = "D:\\中国计算机学会推荐国际学术会议和期刊目录-2022.pdf";

#elif defined(_LINUX)

	const char* url1 = "/home/wangjielin/Desktop/CatalogTest";
	const char* url2 = "/home/wangjielin/Desktop/Lena.bmp";
	const char* url3 = "/home/wangjielin/Desktop/JLMAUDIO";
	const char* url4 = "/home/wangjielin/Desktop/WJLHashAlgorithm";

#endif

	// 把链接复制到列表中
	strcpy(pathList[0], url1);
	strcpy(pathList[1], url2);

	strcpy(pathList1[0], url3);
	strcpy(pathList1[1], url4);

	// 测试目录创建，通过测试
	Catalog = CreateCatalog(pathList, listSize, &errSign);
	if (errSign == 0 && Catalog != NULL) {
		printf("Catalog->CatalogListSize = %d\n", Catalog->CatalogListSize);
		printf("Catalog->urlsListSize = %d\n", Catalog->urlsListSize);
		printf("Catalog->byteslength = %llu\n", Catalog->byteslength);
		printf("Catalog->MaxID = %d\n", Catalog->MaxID);
		for (i = 1; i < Catalog->CatalogListSize; ++i) {
			changeTime(Catalog->CatalogList[i].time, DateTime);
			printf("%d %d %d %llu %s %s\n", Catalog->CatalogList[i].ID, Catalog->CatalogList[i].type, Catalog->CatalogList[i].FID, Catalog->CatalogList[i].byteslength, Catalog->CatalogList[i].name, DateTime);
		}
		printf("\n\n");
		for (i = 0; i < Catalog->urlsListSize; ++i) {
			printf("%d->%s\n", Catalog->urlsList[i].ID, Catalog->urlsList[i].url);
		}
		printf("\n\n");

		// 设置规则
		setRule(Catalog, 1000);
		// 检验标识
		printf("SignCheck %d\n", checkSign(Catalog));

		// 重命名文件夹，通过测试
		const char* newName1 = "AUDIOS";
		reSetName(Catalog, 3, newName1, &errSign);
		printf("reSetName errSign = %d\n", errSign);
		const char* newName2 = "您好.wav";
		reSetName(Catalog, 6, newName2, &errSign);
		printf("reSetName errSign = %d\n", errSign);
		for (i = 1; i < Catalog->CatalogListSize; ++i) {
			changeTime(Catalog->CatalogList[i].time, DateTime);
			printf("%d %d %d %llu %s %s\n", Catalog->CatalogList[i].ID, Catalog->CatalogList[i].type, Catalog->CatalogList[i].FID, Catalog->CatalogList[i].byteslength, Catalog->CatalogList[i].name, DateTime);
		}
		printf("\n\n");


		// 测试向Catalog1中添加目录对象，测试通过
		printf("--------------------------------------------------------------------\n");
		Catalog1 = AddCatalog(Catalog, 1, pathList1, listSize, &errSign);
		printf("AddCatalog errSign = %d\n", errSign);
		printf("Catalog1->CatalogListSize = %d\n", Catalog1->CatalogListSize);
		printf("Catalog1->urlsListSize = %d\n", Catalog1->urlsListSize);
		printf("Catalog1->byteslength = %llu\n", Catalog1->byteslength);
		printf("Catalog1->MaxID = %d\n", Catalog1->MaxID);
		printf("Catalog1->rule = %d\n", Catalog1->rule);
		printf("Catalog1->sign = %s\n", Catalog1->sign);
		for (i = 1; i < Catalog1->CatalogListSize; ++i) {
			changeTime(Catalog1->CatalogList[i].time, DateTime);
			printf("%d %d %d %llu %s %s\n", Catalog1->CatalogList[i].ID, Catalog1->CatalogList[i].type, Catalog1->CatalogList[i].FID, Catalog1->CatalogList[i].byteslength, Catalog1->CatalogList[i].name, DateTime);
		}
		printf("\n\n");
		for (i = 0; i < Catalog1->urlsListSize; ++i) {
			printf("%d->%s\n", Catalog1->urlsList[i].ID, Catalog1->urlsList[i].url);
		}
		printf("\n\n");

		// 测试删除某些ID后，测试通过
		printf("--------------------------------------------------------------------\n");
		Catalog2 = DelCatalog(Catalog1, delIdList, delListSize, &errSign);
		printf("DelCatalog errSign = %d\n", errSign);
		printf("Catalog2[0]->CatalogListSize = %d\n", Catalog2[0]->CatalogListSize);
		printf("Catalog2[0]->urlsListSize = %d\n", Catalog2[0]->urlsListSize);
		printf("Catalog2[0]->byteslength = %llu\n", Catalog2[0]->byteslength);
		printf("Catalog2[0]->MaxID = %d\n", Catalog2[0]->MaxID);
		printf("Catalog2[0]->rule = %d\n", Catalog2[0]->rule);
		printf("Catalog2[0]->sign = %s\n", Catalog2[0]->sign);
		for (i = 1; i < Catalog2[0]->CatalogListSize; ++i) {
			changeTime(Catalog2[0]->CatalogList[i].time, DateTime);
			printf("%d %d %d %llu %s %s\n", Catalog2[0]->CatalogList[i].ID, Catalog2[0]->CatalogList[i].type, Catalog2[0]->CatalogList[i].FID, Catalog2[0]->CatalogList[i].byteslength, Catalog2[0]->CatalogList[i].name, DateTime);
		}
		printf("\n\n");
		for (i = 0; i < Catalog2[0]->urlsListSize; ++i) {
			printf("%d->%s\n", Catalog2[0]->urlsList[i].ID, Catalog2[0]->urlsList[i].url);
		}
		printf("\n\n");

		printf("%d\n", Catalog2[1]->CatalogListSize);
		for (i = 0; i < Catalog2[1]->CatalogListSize; ++i) {
			changeTime(Catalog2[1]->CatalogList[i].time, DateTime);
			printf("%d %d %d %llu %s %s\n", Catalog2[1]->CatalogList[i].ID, Catalog2[1]->CatalogList[i].type, Catalog2[1]->CatalogList[i].FID, Catalog2[1]->CatalogList[i].byteslength, Catalog2[1]->CatalogList[i].name, DateTime);
		}
		printf("\n\n");


		// 测试序列化和反序列化，测试通过
		printf("--------------------------------------------------------------------\n");
		buff = CatalogToBytesArray(Catalog2[0], &bufflength, &errSign);
		printf("%lld, errSign = %d\n", bufflength, errSign);
		Catalog3 = BytesArrayToCatalog(buff, bufflength, &errSign);
		printf("errSign = %d\n", errSign);
		printf("Catalog3->CatalogListSize = %d\n", Catalog3->CatalogListSize);
		printf("Catalog1->urlsListSize = %d\n", Catalog3->urlsListSize);
		printf("Catalog1->byteslength = %llu\n", Catalog3->byteslength);
		printf("Catalog1->MaxID = %d\n", Catalog3->MaxID);
		printf("Catalog1->rule = %d\n", Catalog3->rule);
		printf("Catalog1->sign = %s\n", Catalog3->sign);
		for (i = 1; i < Catalog3->CatalogListSize; ++i) {
			changeTime(Catalog3->CatalogList[i].time, DateTime);
			printf("%d %d %d %llu %s %s\n", Catalog3->CatalogList[i].ID, Catalog3->CatalogList[i].type, Catalog3->CatalogList[i].FID, Catalog3->CatalogList[i].byteslength, Catalog3->CatalogList[i].name, DateTime);
		}
		printf("\n\n");
	}

	// 释放资源
	if (pathList) {
		for (i = listSize - 1; i >= 0; --i) if (pathList[i]) free(pathList[i]);
		free(pathList);
	}
	if (pathList1) {
		for (i = listSize - 1; i >= 0; --i) if (pathList1[i]) free(pathList1[i]);
		free(pathList1);
	}
	if (buff) free(buff);
	if (Catalog) {
		if (Catalog->CatalogList) free(Catalog->CatalogList);
		if (Catalog->urlsList) free(Catalog->urlsList);
		free(Catalog);
	}
	if (Catalog1) {
		if (Catalog1->CatalogList) free(Catalog1->CatalogList);
		if (Catalog1->urlsList) free(Catalog1->urlsList);
		free(Catalog1);
	}
	if (Catalog2) {
		if (Catalog2[1]) {
			if (Catalog2[1]->CatalogList) free(Catalog2[1]->CatalogList);
			if (Catalog2[1]->urlsList) free(Catalog2[1]->urlsList);
			free(Catalog2[1]);
		}
		if (Catalog2[0]) {
			if (Catalog2[0]->CatalogList) free(Catalog2[0]->CatalogList);
			if (Catalog2[0]->urlsList) free(Catalog2[0]->urlsList);
			free(Catalog2[0]);
		}
		free(Catalog2);
	}
	if (Catalog3) {
		if (Catalog3->CatalogList) free(Catalog3->CatalogList);
		if (Catalog3->urlsList) free(Catalog3->urlsList);
		free(Catalog3);
	}
	// 结束
#ifdef _WINDOWS
	//system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}

// 检验JlmPack_JLMHA和JlmPack_JLMSE，测试通过了
int main3() { // 3
	int errSign = 0;
	unsigned int i;
	unsigned char* RandomBytes = NULL, * RandomBytes1 = NULL;
	unsigned char* InByteArray = NULL;
	unsigned char* HashByteArray = NULL;
	unsigned char* OutByteArray = NULL, * OutByteArray1 = NULL;
	unsigned int InByteArray_size = 512;
	unsigned int OutByteArray_size = 512 * 4;
	unsigned int OutByteArray_size1 = 512 * 4;
	unsigned char RandomByte;
	unsigned char* ucPassword = "123456";
	unsigned int uiPasswordLen = 6;
	unsigned char* ucPassword1 = "123456";
	unsigned int uiPassword1Len = 6;
	unsigned int RandomBytes_size = 64;
	unsigned int HashByteArray_size = 4;

	HashByteArray = (unsigned char*)malloc(HashByteArray_size * sizeof(unsigned char));
	RandomBytes = (unsigned char*)malloc(RandomBytes_size * sizeof(unsigned char));
	RandomBytes1 = (unsigned char*)malloc(RandomBytes_size * sizeof(unsigned char));
	InByteArray = (unsigned char*)malloc(InByteArray_size * sizeof(unsigned char));
	OutByteArray = (unsigned char*)malloc(OutByteArray_size * sizeof(unsigned char));
	OutByteArray1 = (unsigned char*)malloc(OutByteArray_size1 * sizeof(unsigned char));
	if (HashByteArray == NULL || RandomBytes == NULL || RandomBytes1 == NULL || InByteArray == NULL || OutByteArray == NULL || OutByteArray1 == NULL) goto ERR;
	// 初始化哈希值为0x00
	memset(HashByteArray, 0x00, HashByteArray_size);
	// 生成随机数
	srand((unsigned)time(NULL));
	printf("生成%d个随机数：\n", RandomBytes_size);
	for (i = 0; i < RandomBytes_size; ++i) {
		RandomBytes[i] = rand() % 0x80;
		RandomBytes1[i] = 0x00;
		printf("0x%02X,", RandomBytes[i]);
	}
	printf("\n\n");
	printf("生成%d个随机待编码数据：\n", InByteArray_size);
	for (i = 0; i < InByteArray_size; ++i) {
		InByteArray[i] = rand() % 0x80;
		printf("0x%02X,", InByteArray[i]);
	}
	printf("\n\n");

	errSign = JlmPack_JLMSE(RandomBytes, RandomBytes_size, ucPassword, uiPasswordLen, InByteArray, InByteArray_size, OutByteArray, &OutByteArray_size, ENCODE_MODEL);
	if (errSign == 0x00) {
		printf("编码后的字节为：%d个\n", OutByteArray_size);
		for (i = 0; i < OutByteArray_size; ++i) {
			printf("0x%02X,", OutByteArray[i]);
		}
	}
	else {
		printf("错误！编号为：%d", errSign);
	}
	printf("\n\n");

	errSign = JlmPack_JLMSE(RandomBytes1, RandomBytes_size, ucPassword1, uiPassword1Len, OutByteArray, OutByteArray_size, OutByteArray1, &OutByteArray_size1, DECODE_MODEL);
	if (errSign == 0x00) {
		printf("\n\n译码后的随机数为：\n");
		for (i = 0; i < RandomBytes_size; ++i) {
			printf("0x%02X,", RandomBytes1[i]);
		}
		printf("\n");
		printf("译码后的字节为：%d个\n", OutByteArray_size1);
		for (i = 0; i < OutByteArray_size1; ++i) {
			if (OutByteArray1[i] != InByteArray[i]) {
				printf("译码错误！");
				goto ERR;
			}
			printf("0x%02X,", OutByteArray1[i]);
		}
	}
	else {
		printf("错误！编号为：%d", errSign);
	}
	printf("\n\n编译码正确！!\n");
	// 计算InByteArray的哈希值
	/*RandomByte = rand() % 0xFF;
	errSign = JlmPack_JLMHA(RandomByte, InByteArray, InByteArray_size, HashByteArray, HashByteArray_size);
	if (errSign == 0x00) {
		printf("InByteArray的哈希值：\n");
		for (i = 0; i < HashByteArray_size; ++i) {
			printf("%02X,", HashByteArray[i]);
		}
	}
	printf("\n\n");*/
ERR:
	if (HashByteArray) free(HashByteArray);
	if (RandomBytes) free(RandomBytes);
	if (InByteArray) free(InByteArray);
	if (OutByteArray) free(OutByteArray);
	if (OutByteArray1) free(OutByteArray1);
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}

// 检验JlmPack_JLMSE对二进制文件的压缩解压
int main33() { // 3
	int errSign = 0;
	unsigned int i;
	unsigned char* RandomBytes = NULL, * RandomBytes1 = NULL;
	unsigned char* InByteArray = NULL;
	unsigned char* HashByteArray = NULL;
	unsigned char* OutByteArray = NULL, * OutByteArray1 = NULL;
	unsigned int InByteArray_size = 734003200, truerSize = 0;
	unsigned int OutByteArray_size = 734003200;
	unsigned int OutByteArray_size1 = 734003200, truewSize = 0;
	unsigned char RandomByte;
	unsigned char* ucPassword = "123456";
	unsigned int uiPasswordLen = 6;
	unsigned char* ucPassword1 = "123456";
	unsigned int uiPassword1Len = 6;
	unsigned int RandomBytes_size = 64;
	unsigned int HashByteArray_size = 4;
	FILE* readfile = NULL, * writefile = NULL;
	HashByteArray = (unsigned char*)malloc(HashByteArray_size * sizeof(unsigned char));
	RandomBytes = (unsigned char*)malloc(RandomBytes_size * sizeof(unsigned char));
	RandomBytes1 = (unsigned char*)malloc(RandomBytes_size * sizeof(unsigned char));
	InByteArray = (unsigned char*)malloc(InByteArray_size * sizeof(unsigned char));
	OutByteArray = (unsigned char*)malloc(OutByteArray_size * sizeof(unsigned char));
	OutByteArray1 = (unsigned char*)malloc(OutByteArray_size1 * sizeof(unsigned char));
	if (HashByteArray == NULL || RandomBytes == NULL || RandomBytes1 == NULL || InByteArray == NULL || OutByteArray == NULL || OutByteArray1 == NULL) goto ERR;

	// 生成随机数
	srand((unsigned)time(NULL));
	printf("生成%d个随机数：\n", RandomBytes_size);
	for (i = 0; i < RandomBytes_size; ++i) {
		RandomBytes[i] = rand() % 0xFF;
		RandomBytes1[i] = 0x00;
		printf("%02X,", RandomBytes[i]);
	}
	printf("\n\n");
	// 读取二进制
	readfile = OpenBinaryFile("D:\\注册表备份.reg", READ);
	writefile = OpenBinaryFile("D:\\注册表备份11.reg", WRITE);
	// 一次性把文件读取进来
	truerSize = Read(readfile, InByteArray, InByteArray_size);
	printf("实际文件长度为：%d\n", truerSize);

	errSign = JlmPack_JLMSE(RandomBytes, RandomBytes_size, ucPassword, uiPasswordLen, InByteArray, truerSize, OutByteArray, &OutByteArray_size, ENCODE_MODEL);
	if (errSign == 0x00) {
		printf("编码后的字节为：%d个\n", OutByteArray_size);
		for (i = 0; i < OutByteArray_size && i < 1024; ++i) {
			printf("%02X,", OutByteArray[i]);
		}
	}
	else {
		printf("错误！编号为：%d", errSign);
	}
	printf("\n\n");


	errSign = JlmPack_JLMSE(RandomBytes1, RandomBytes_size, ucPassword1, uiPassword1Len, OutByteArray, OutByteArray_size, OutByteArray1, &OutByteArray_size1, DECODE_MODEL);
	if (errSign == 0x00) {
		printf("\n\n译码后的随机数为：\n");
		for (i = 0; i < RandomBytes_size; ++i) {
			printf("%02X,", RandomBytes1[i]);
		}
		printf("\n");
		printf("译码后的字节为：%d个\n", OutByteArray_size1);
		for (i = 0; i < OutByteArray_size1; ++i) {
			if (OutByteArray1[i] != InByteArray[i]) {
				printf("译码错误！");
				goto ERR;
			}
			if (i > OutByteArray_size1 - 1024) printf("%02X,", OutByteArray1[i]);
		}
	}
	else {
		printf("错误！编号为：%d", errSign);
	}
	printf("\n\n编译码正确！!\n");
	truewSize = Write(writefile, OutByteArray1, OutByteArray_size1);
	printf("实际文件长度为：%d\n", truewSize);

ERR:
	if (HashByteArray) free(HashByteArray);
	if (RandomBytes) free(RandomBytes);
	if (InByteArray) free(InByteArray);
	if (OutByteArray) free(OutByteArray);
	if (OutByteArray1) free(OutByteArray1);
	if (readfile) Close(readfile);
	if (writefile) Close(writefile);
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}

// 测试创建目录，测试通过
int Create() { // 4
	// 设置为""的目的是跟随系统，支持windows和linux
	 // en_US.UTF-8

	VARIABLES* vars = NULL;
	int i, errSign = 0, listSize = 2;
	unsigned char* ucPassword = "123456";
	unsigned char* owner = "湖南遥昇通信技术有限公司";
	unsigned char* promulgator = "马力哥";
	unsigned char* contact = "18163682727";
	unsigned char* versionNumber = "V1.0.0";
	unsigned char* signature = "测试测试！";
	unsigned long datetime = 0;
	unsigned char authorization = 0;

	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	if (vars == NULL) goto ERR;

	// 设置里面的参数
	vars->RandomBytes_size = RANDOM_BYTE_LIMIT; // 建议为2-8个字节
	vars->Passwords_size = (int)(strlen(ucPassword) + 1);
	vars->pathListSize = 3;
	vars->rule = 0;
	vars->pathList = (unsigned char**)malloc(vars->pathListSize * sizeof(unsigned char*));
	if (vars->pathList == NULL) goto ERR;
	for (i = 0; i < vars->pathListSize; ++i) {
		vars->pathList[i] = (unsigned char*)malloc(URL_BYTE_LIMIT);
		if (vars->pathList[i] == NULL) goto ERR;
	}
	vars->jlmFile = (JLM_FILE*)malloc(sizeof(JLM_FILE));
	if (vars->jlmFile == NULL) goto ERR;
	// vars->jlmExtra = NULL;
	vars->jlmExtra = (JLM_EXTRA*)malloc(sizeof(JLM_EXTRA));
	if (vars->jlmExtra == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;
	// 设置扩展信息
	strncpy(vars->jlmExtra->owner, owner, strlen(owner));
	vars->jlmExtra->owner[strlen(owner)] = '\0';
	strncpy(vars->jlmExtra->promulgator, promulgator, strlen(promulgator));
	vars->jlmExtra->promulgator[strlen(promulgator)] = '\0';
	strncpy(vars->jlmExtra->contact, contact, strlen(contact));
	vars->jlmExtra->contact[strlen(contact)] = '\0';
	strncpy(vars->jlmExtra->versionNumber, versionNumber, strlen(versionNumber));
	vars->jlmExtra->versionNumber[strlen(versionNumber)] = '\0';
	strncpy(vars->jlmExtra->signature, signature, strlen(signature));
	vars->jlmExtra->signature[strlen(signature)] = '\0';
	vars->jlmExtra->datetime = datetime;
	vars->jlmExtra->authorization = authorization;
	// 开辟随机数的缓存空间
	vars->RandomBytes = (unsigned char*)malloc(vars->RandomBytes_size * sizeof(unsigned char));
	vars->Passwords = (unsigned char*)malloc(vars->Passwords_size * sizeof(unsigned char));
	if (vars->RandomBytes == NULL || vars->Passwords == NULL) goto ERR;
	// 开辟jlm路径的缓存空间
	vars->jlmFileUrl = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->jlmFileUrl == NULL) goto ERR;

	// 此处需要从服务器上获取某个账户的证书指纹或SDK的商业授权下的证书指纹，如果某个账户下有指纹则vars->license_size为指纹的实际长度，否则vars->license_size = 0
	vars->license = (unsigned char*)malloc(JLM_FILE_HASH_BYTE_LIMIT);
	vars->license_size = 0;
	if (vars->license == NULL) goto ERR;
	// 开辟临时运算缓存
	vars->WriteOrReadBuff_Size = vars->OutByteArray_Size = vars->InByteArray_Size = BINARY_FILE_MAX_LIMIT;
	vars->InByteArray = (unsigned char*)malloc(vars->InByteArray_Size * sizeof(unsigned char));
	vars->OutByteArray = (unsigned char*)malloc(vars->OutByteArray_Size * sizeof(unsigned char));
	vars->WriteOrReadBuff = (unsigned char*)malloc(vars->WriteOrReadBuff_Size * sizeof(unsigned char));
	if (vars->InByteArray == NULL || vars->OutByteArray == NULL || vars->WriteOrReadBuff == NULL) goto ERR;
	// 开辟日志文件
	vars->log = (JLM_LOG*)malloc(sizeof(JLM_LOG));
	if (vars->log == NULL) goto ERR;
	// 设定每个包的大小，如果不设置则默认按照JLMPACK_BYTES_LIMIT来设置
	vars->jlmFile->packBytesLength = 629145600;// 1073741824;
	// 区分windows和linux
#ifdef _WINDOWS

	printf("_WINDOWS\n");
	const char* url1 = "D:\\CatalogTest"; // D:\\注册表备份.reg D:\\CatalogTest
	const char* url2 = "D:\\中国计算机学会推荐国际学术会议和期刊目录-2022.pdf"; // 中国计算机学会推荐国际学术会议和期刊目录-2022.pdf
	const char* url3 = "D:\\目录测试";
	const char* outurl = "D:\\test.jlm";
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license";
#elif defined(_LINUX)

	printf("_LINUX\n");
	const char* url1 = "/home/wangjielin/Desktop/CatalogTest";
	const char* url2 = "/home/wangjielin/Desktop/Lena.bmp";
	const char* url3 = "/home/wangjielin/Desktop/JLMAUDIO";
	const char* outurl = "/home/wangjielin/Desktop/测试.jlm";
	const char* licensePath = "/home/wangjielin/Desktop/yesine_jlmpack_2024_test.license";
#endif
	strcpy(vars->licenseFilePath, licensePath);
	strcpy(vars->pathList[0], url1);
	strcpy(vars->pathList[1], url2);
	strcpy(vars->pathList[2], url3);
	strcpy(vars->Passwords, ucPassword);
	strcpy(vars->jlmFileUrl, outurl);

	srand((unsigned)time(NULL));

	// 调用创建接口
	errSign = JlmPack_Create(vars);
	printf("errSign = %d\n", errSign);
	if (errSign == 0) {
		printf("创建成功！\n");
		printf("vars->log->type = %d\n", vars->log->type);
		printf("vars->log->fileName = %s\n", vars->log->fileName);
		printf("vars->log->consistencyMark = ");
		for (i = 0; i < PACK_FILE_HASH_BYTE_LIMIT; ++i) {
			printf("%02X,", vars->log->consistencyMark[i]);
		}
		printf("\n");
		printf("vars->log->datetime = %lld\n", vars->log->datetime);
		printf("vars->totalBytes = %lld, vars->cumulativeBytes = %lld\n", vars->totalBytes, vars->cumulativeBytes);
	}

ERR:
	if (vars) {
		if (vars->InByteArray) free(vars->InByteArray);
		if (vars->OutByteArray) free(vars->OutByteArray);
		if (vars->WriteOrReadBuff) free(vars->WriteOrReadBuff);
		if (vars->log) free(vars->log);
		if (vars->jlmFile) free(vars->jlmFile);
		if (vars->jlmExtra) free(vars->jlmExtra);
		if (vars->RandomBytes)  free(vars->RandomBytes);
		if (vars->Passwords)  free(vars->Passwords);
		if (vars->jlmFileUrl)  free(vars->jlmFileUrl);
		if (vars->license) free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		if (vars->pathList) {
			for (i = vars->pathListSize - 1; i >= 0; --i) if (vars->pathList[i]) free(vars->pathList[i]);
			free(vars->pathList);
		}
		free(vars);
	}
}

// 测试解包，测试通过
int Unpack() { // 5
	// 设置为""的目的是跟随系统，支持windows和linux
	 //en_US.UTF-8

	VARIABLES* vars = NULL;
	int i, errSign = 0;
	unsigned char* ucPassword = "123456";

	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	if (vars == NULL) goto ERR;

	// 设置里面的参数
	vars->RandomBytes_size = RANDOM_BYTE_LIMIT;   // 编码和解码这个值必须相同
	vars->Passwords_size = strlen(ucPassword) + 1;
	vars->pathListSize = 1;
	vars->rule = 0;
	vars->license_size = 0;
	vars->IdList_Size = 2;

	vars->pathList = (unsigned char**)malloc(vars->pathListSize * sizeof(unsigned char*));
	if (vars->pathList == NULL) goto ERR;
	for (i = 0; i < vars->pathListSize; ++i) {
		vars->pathList[i] = (unsigned char*)malloc(URL_BYTE_LIMIT);
		if (vars->pathList[i] == NULL) goto ERR;
	}
	vars->jlmFile = (JLM_FILE*)malloc(sizeof(JLM_FILE));
	if (vars->jlmFile == NULL) goto ERR;
	// vars->jlmExtra = NULL;
	vars->jlmExtra = (JLM_EXTRA*)malloc(sizeof(JLM_EXTRA));
	if (vars->jlmExtra == NULL) goto ERR;
	// 开辟随机数的缓存空间
	vars->RandomBytes = (unsigned char*)malloc(vars->RandomBytes_size * sizeof(unsigned char));
	vars->Passwords = (unsigned char*)malloc(vars->Passwords_size * sizeof(unsigned char));
	if (vars->RandomBytes == NULL || vars->Passwords == NULL) goto ERR;
	// 开辟jlm路径的缓存空间
	vars->jlmFileUrl = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->jlmFileUrl == NULL) goto ERR;
	// 同上
	vars->license = (unsigned char*)malloc(JLM_FILE_HASH_BYTE_LIMIT);
	if (vars->license == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;
	// 开辟临时运算缓存，译码过程中开辟相同的缓存大小即可
	vars->WriteOrReadBuff_Size = vars->InByteArray_Size = BINARY_FILE_MAX_LIMIT;
	// 这个地方很重要，因为译码时，能释放比输入更大的空间
	vars->OutByteArray_Size = BINARY_FILE_MAX_LIMIT * 4;
	vars->InByteArray = (unsigned char*)malloc(vars->InByteArray_Size * sizeof(unsigned char));
	vars->OutByteArray = (unsigned char*)malloc(vars->OutByteArray_Size * sizeof(unsigned char));
	vars->WriteOrReadBuff = (unsigned char*)malloc(vars->WriteOrReadBuff_Size * sizeof(unsigned char));
	if (vars->InByteArray == NULL || vars->OutByteArray == NULL || vars->WriteOrReadBuff == NULL) goto ERR;
	// 开辟日志文件
	vars->log = (JLM_LOG*)malloc(sizeof(JLM_LOG));
	if (vars->log == NULL) goto ERR;
	// ID列表
	vars->IdList = (int*)malloc(vars->IdList_Size * sizeof(int));
	if (vars->IdList == NULL)  goto ERR;
	// 复制密码，去除“\0”
	strcpy(vars->Passwords, ucPassword);
	// 区分windows和linux
#ifdef _WINDOWS
	printf("_WINDOWS\n");
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "D:\\test.jlm";
	//const char* jlmFileUrl = "D:\\test_pack4.jlm";
	const char* outurl = "D:\\testDecodes\\"; // 解码的文件夹路径
	// 输入解码ID号
	vars->IdList[0] = 0; //2
	vars->IdList[1] = 18;

#elif defined(_LINUX)
	printf("_LINUX\n");
	const char* licensePath = "/home/wangjielin/Desktop/yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "/home/wangjielin/Desktop/测试.jlm";
	//const char* jlmFileUrl = "/home/wangjielin/Desktop/test_pack3.jlm";
	const char* outurl = "/home/wangjielin/Desktop/testDecode/"; // 解码的文件夹路径
	// 输入解码ID号，由于linux和windows的排序可能不一致，译码同一个东西
	vars->IdList[0] = 0;
	vars->IdList[1] = 18;

#endif
	strcpy(vars->licenseFilePath, licensePath);
	// jlm的文件路径
	strcpy(vars->jlmFileUrl, jlmFileUrl);
	// 把outurl赋值给vars->pathList[0]
	strcpy(vars->pathList[0], outurl);

	printf("vars->pathList[0] = %s\n", vars->pathList[0]);

	// 给vars->IdList中的数据
	errSign = JlmPack_Unpack(vars);
	printf("errSign = %d\n", errSign);

	if (errSign == 0) {
		printf("译码成功！\n");
		printf("vars->log->type = %d\n", vars->log->type);
		printf("vars->log->fileName = %s\n", vars->log->fileName);
		printf("vars->log->consistencyMark = ");
		for (i = 0; i < PACK_FILE_HASH_BYTE_LIMIT; ++i) {
			printf("%02X,", vars->log->consistencyMark[i]);
		}
		printf("\n");
		printf("vars->log->datetime = %lld\n", vars->log->datetime);
		printf("vars->totalBytes = %lld, vars->cumulativeBytes = %lld\n", vars->totalBytes, vars->cumulativeBytes);
	}

ERR:
	if (vars) {
		if (vars->InByteArray) free(vars->InByteArray);
		if (vars->OutByteArray) free(vars->OutByteArray);
		if (vars->WriteOrReadBuff) free(vars->WriteOrReadBuff);
		if (vars->log) free(vars->log);
		if (vars->jlmFile) free(vars->jlmFile);
		if (vars->jlmExtra) free(vars->jlmExtra);
		if (vars->RandomBytes)  free(vars->RandomBytes);
		if (vars->Passwords)  free(vars->Passwords);
		if (vars->jlmFileUrl)  free(vars->jlmFileUrl);
		if (vars->license) free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		if (vars->IdList) free(vars->IdList);
		if (vars->pathList) {
			for (i = vars->pathListSize - 1; i >= 0; --i) if (vars->pathList[i]) free(vars->pathList[i]);
			free(vars->pathList);
		}
		free(vars);
	}
}
// 联合测试创建和解包
int main5() { // 5

	Create();
#ifdef _WINDOWS
	Sleep(3000); // windows下暂停3秒
#elif defined(_LINUX)
	sleep(3);   // linux下暂停3秒
#endif
	printf("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	Unpack();

	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}

// 测试追加文件，经过测试追加后能正常解包
int main6() { // 6
	// 设置为""的目的是跟随系统，支持windows和linux
	 //en_US.UTF-8

	VARIABLES* vars = NULL;
	int i, errSign = 0;
	unsigned char* ucPassword = "123456";

	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	if (vars == NULL) goto ERR;

	// 设置里面的参数
	vars->RandomBytes_size = RANDOM_BYTE_LIMIT;   // 编码和解码这个值必须相同
	vars->Passwords_size = strlen(ucPassword) + 1;
	vars->pathListSize = 2;  // 待添加到jlm文件包中的目录
	vars->rule = 0;
	vars->license_size = 0;
	vars->IdList_Size = 1;  // 添加文件时只能设置为1，否则将报错

	vars->pathList = (unsigned char**)malloc(vars->pathListSize * sizeof(unsigned char*));
	if (vars->pathList == NULL) goto ERR;
	for (i = 0; i < vars->pathListSize; ++i) {
		vars->pathList[i] = (unsigned char*)malloc(URL_BYTE_LIMIT);
		if (vars->pathList[i] == NULL) goto ERR;
	}
	vars->jlmFile = (JLM_FILE*)malloc(sizeof(JLM_FILE));
	if (vars->jlmFile == NULL) goto ERR;
	// vars->jlmExtra = NULL;
	vars->jlmExtra = (JLM_EXTRA*)malloc(sizeof(JLM_EXTRA));
	if (vars->jlmExtra == NULL) goto ERR;
	// 开辟随机数的缓存空间
	vars->RandomBytes = (unsigned char*)malloc(vars->RandomBytes_size * sizeof(unsigned char));
	vars->Passwords = (unsigned char*)malloc(vars->Passwords_size * sizeof(unsigned char));
	if (vars->RandomBytes == NULL || vars->Passwords == NULL) goto ERR;
	// 开辟jlm路径的缓存空间
	vars->jlmFileUrl = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->jlmFileUrl == NULL) goto ERR;
	// 同上
	vars->license = (unsigned char*)malloc(JLM_FILE_HASH_BYTE_LIMIT);
	if (vars->license == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;
	// 开辟临时运算缓存，译码过程中开辟相同的缓存大小即可
	vars->WriteOrReadBuff_Size = vars->OutByteArray_Size = vars->InByteArray_Size = BINARY_FILE_MAX_LIMIT;
	vars->InByteArray = (unsigned char*)malloc(vars->InByteArray_Size * sizeof(unsigned char));
	vars->OutByteArray = (unsigned char*)malloc(vars->OutByteArray_Size * sizeof(unsigned char));
	vars->WriteOrReadBuff = (unsigned char*)malloc(vars->WriteOrReadBuff_Size * sizeof(unsigned char));
	if (vars->InByteArray == NULL || vars->OutByteArray == NULL || vars->WriteOrReadBuff == NULL) goto ERR;
	// 开辟日志文件
	vars->log = (JLM_LOG*)malloc(sizeof(JLM_LOG));
	if (vars->log == NULL) goto ERR;
	// ID列表
	vars->IdList = (int*)malloc(vars->IdList_Size * sizeof(int));
	if (vars->IdList == NULL)  goto ERR;
	// 复制密码，去除“\0”
	strcpy(vars->Passwords, ucPassword);
	// 区分windows和linux
#ifdef _WINDOWS
	printf("_WINDOWS\n");
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "D:\\test.jlm";
	//const char* jlmFileUrl = "D:\\test_pack5.jlm";
	const char* addUrl1 = "D:\\目录测试1"; // 待添加的文件路径
	const char* addUrl2 = "D:\\Lena.bmp"; // 待添加的文件路径
	// 输入解码ID号
	vars->IdList[0] = 0; //2

#elif defined(_LINUX)
	printf("_LINUX\n");
	const char* licensePath = "/home/wangjielin/Desktop/yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "/home/wangjielin/Desktop/test.jlm";
	//const char* jlmFileUrl = "/home/wangjielin/Desktop/test_pack9.jlm";
	const char* addUrl1 = "/home/wangjielin/Desktop/WJLCoding"; // 解码的文件夹路径
	const char* addUrl2 = "/home/wangjielin/Desktop/JLMAudio"; // 解码的文件夹路径
	// 输入解码ID号，由于linux和windows的排序可能不一致，译码同一个东西
	vars->IdList[0] = 3;

#endif
	strcpy(vars->licenseFilePath, licensePath);
	// jlm的文件路径
	strcpy(vars->jlmFileUrl, jlmFileUrl);
	// 把outurlX赋值给vars->pathList
	strcpy(vars->pathList[0], addUrl1);
	strcpy(vars->pathList[1], addUrl2);

	printf("vars->pathList[0] = %s\n", vars->pathList[0]);
	printf("vars->pathList[1] = %s\n", vars->pathList[1]);

	errSign = JlmPack_Insert(vars);
	printf("errSign = %d\n", errSign);

	if (errSign == 0) {
		printf("追加成功！\n");
		printf("vars->log->type = %d\n", vars->log->type);
		printf("vars->log->fileName = %s\n", vars->log->fileName);
		printf("vars->log->consistencyMark = ");
		for (i = 0; i < PACK_FILE_HASH_BYTE_LIMIT; ++i) {
			printf("%02X,", vars->log->consistencyMark[i]);
		}
		printf("\n");
		printf("vars->log->datetime = %lld\n", vars->log->datetime);
		printf("vars->totalBytes = %lld, vars->cumulativeBytes = %lld\n", vars->totalBytes, vars->cumulativeBytes);
	}

ERR:
	if (vars) {
		if (vars->InByteArray) free(vars->InByteArray);
		if (vars->OutByteArray) free(vars->OutByteArray);
		if (vars->WriteOrReadBuff) free(vars->WriteOrReadBuff);
		if (vars->log) free(vars->log);
		if (vars->jlmFile) free(vars->jlmFile);
		if (vars->jlmExtra) free(vars->jlmExtra);
		if (vars->RandomBytes)  free(vars->RandomBytes);
		if (vars->Passwords)  free(vars->Passwords);
		if (vars->jlmFileUrl)  free(vars->jlmFileUrl);
		if (vars->license) free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		if (vars->IdList) free(vars->IdList);
		if (vars->pathList) {
			for (i = vars->pathListSize - 1; i >= 0; --i) if (vars->pathList[i]) free(vars->pathList[i]);
			free(vars->pathList);
		}
		free(vars);
	}
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}
// 测试删除某些ID即一下的目录信息，测试通过
int main7() { // 7
	// 设置为""的目的是跟随系统，支持windows和linux
	 //en_US.UTF-8

	VARIABLES* vars = NULL;
	int i, errSign = 0;
	unsigned char* ucPassword = "123456";

	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	if (vars == NULL) goto ERR;

	// 设置里面的参数
	vars->RandomBytes_size = RANDOM_BYTE_LIMIT;   // 编码和解码这个值必须相同
	vars->Passwords_size = strlen(ucPassword) + 1;
	vars->pathListSize = 0;  // 删除目录，此参数为0
	vars->rule = 0;
	vars->license_size = 0;
	vars->IdList_Size = 3;  // 添加文件时只能设置为1，否则将报错

	vars->pathList = (unsigned char**)malloc(vars->pathListSize * sizeof(unsigned char*));
	if (vars->pathList == NULL) goto ERR;
	for (i = 0; i < vars->pathListSize; ++i) {
		vars->pathList[i] = (unsigned char*)malloc(URL_BYTE_LIMIT);
		if (vars->pathList[i] == NULL) goto ERR;
	}
	vars->jlmFile = (JLM_FILE*)malloc(sizeof(JLM_FILE));
	if (vars->jlmFile == NULL) goto ERR;
	// vars->jlmExtra = NULL;
	vars->jlmExtra = (JLM_EXTRA*)malloc(sizeof(JLM_EXTRA));
	if (vars->jlmExtra == NULL) goto ERR;
	// 开辟随机数的缓存空间
	vars->RandomBytes = (unsigned char*)malloc(vars->RandomBytes_size * sizeof(unsigned char));
	vars->Passwords = (unsigned char*)malloc(vars->Passwords_size * sizeof(unsigned char));
	if (vars->RandomBytes == NULL || vars->Passwords == NULL) goto ERR;
	// 开辟jlm路径的缓存空间
	vars->jlmFileUrl = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->jlmFileUrl == NULL) goto ERR;
	// 同上
	vars->license = (unsigned char*)malloc(JLM_FILE_HASH_BYTE_LIMIT);
	if (vars->license == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;
	// 开辟临时运算缓存，译码过程中开辟相同的缓存大小即可
	vars->WriteOrReadBuff_Size = vars->OutByteArray_Size = vars->InByteArray_Size = BINARY_FILE_MAX_LIMIT;
	vars->InByteArray = (unsigned char*)malloc(vars->InByteArray_Size * sizeof(unsigned char));
	vars->OutByteArray = (unsigned char*)malloc(vars->OutByteArray_Size * sizeof(unsigned char));
	vars->WriteOrReadBuff = (unsigned char*)malloc(vars->WriteOrReadBuff_Size * sizeof(unsigned char));
	if (vars->InByteArray == NULL || vars->OutByteArray == NULL || vars->WriteOrReadBuff == NULL) goto ERR;
	// 开辟日志文件
	vars->log = (JLM_LOG*)malloc(sizeof(JLM_LOG));
	if (vars->log == NULL) goto ERR;
	// ID列表
	vars->IdList = (int*)malloc(vars->IdList_Size * sizeof(int));
	if (vars->IdList == NULL)  goto ERR;
	// 复制密码，去除“\0”
	strcpy(vars->Passwords, ucPassword);
	// 区分windows和linux
#ifdef _WINDOWS
	printf("_WINDOWS\n");
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "D:\\test.jlm";
	//const char* jlmFileUrl = "D:\\test_pack2.jlm";
	// 输入待删除的ID号，为0时
	vars->IdList[0] = 3; //2
	vars->IdList[1] = 7;
	vars->IdList[2] = 23;

#elif defined(_LINUX)
	printf("_LINUX\n");
	const char* licensePath = "/home/wangjielin/Desktop/yesine_jlmpack_2024_test.license";
	//const char* jlmFileUrl = "/home/wangjielin/Desktop/test.jlm";
	const char* jlmFileUrl = "/home/wangjielin/Desktop/test_pack2.jlm";
	// 输入待删除的ID号
	vars->IdList[0] = 5;
	vars->IdList[1] = 18;
	vars->IdList[2] = 19;

#endif
	strcpy(vars->licenseFilePath, licensePath);
	// jlm的文件路径
	strcpy(vars->jlmFileUrl, jlmFileUrl);

	errSign = JlmPack_Delete(vars);
	printf("errSign = %d\n", errSign);

	if (errSign == 0) {
		printf("删除成功！\n");
		printf("vars->log->type = %d\n", vars->log->type);
		printf("vars->log->fileName = %s\n", vars->log->fileName);
		printf("vars->log->consistencyMark = ");
		for (i = 0; i < PACK_FILE_HASH_BYTE_LIMIT; ++i) {
			printf("%02X,", vars->log->consistencyMark[i]);
		}
		printf("\n");
		printf("vars->log->datetime = %lld\n", vars->log->datetime);
		printf("vars->totalBytes = %lld, vars->cumulativeBytes = %lld\n", vars->totalBytes, vars->cumulativeBytes);
	}

ERR:
	if (vars) {
		if (vars->InByteArray) free(vars->InByteArray);
		if (vars->OutByteArray) free(vars->OutByteArray);
		if (vars->WriteOrReadBuff) free(vars->WriteOrReadBuff);
		if (vars->log) free(vars->log);
		if (vars->jlmFile) free(vars->jlmFile);
		if (vars->jlmExtra) free(vars->jlmExtra);
		if (vars->RandomBytes)  free(vars->RandomBytes);
		if (vars->Passwords)  free(vars->Passwords);
		if (vars->jlmFileUrl)  free(vars->jlmFileUrl);
		if (vars->license) free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		if (vars->IdList) free(vars->IdList);
		if (vars->pathList) {
			for (i = vars->pathListSize - 1; i >= 0; --i) if (vars->pathList[i]) free(vars->pathList[i]);
			free(vars->pathList);
		}
		free(vars);
	}
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}

// 测试重命名功能，测试通过
int main8() { // 8
	// 设置为""的目的是跟随系统，支持windows和linux
	 //en_US.UTF-8

	VARIABLES* vars = NULL;
	int i, errSign = 0;
	unsigned char* ucPassword = "123456";

	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	if (vars == NULL) goto ERR;

	// 设置里面的参数
	vars->RandomBytes_size = RANDOM_BYTE_LIMIT;   // 编码和解码这个值必须相同
	vars->Passwords_size = strlen(ucPassword) + 1;
	vars->pathListSize = 1;  // 重命名需要通过vars->pathList[0]传递新的文件名
	vars->rule = 0;
	vars->license_size = 0;
	vars->IdList_Size = 1;  // 每次只能针对一个文件名进行重命名

	vars->pathList = (unsigned char**)malloc(vars->pathListSize * sizeof(unsigned char*));
	if (vars->pathList == NULL) goto ERR;
	for (i = 0; i < vars->pathListSize; ++i) {
		vars->pathList[i] = (unsigned char*)malloc(URL_BYTE_LIMIT);
		if (vars->pathList[i] == NULL) goto ERR;
	}
	vars->jlmFile = (JLM_FILE*)malloc(sizeof(JLM_FILE));
	if (vars->jlmFile == NULL) goto ERR;
	// vars->jlmExtra = NULL;
	vars->jlmExtra = (JLM_EXTRA*)malloc(sizeof(JLM_EXTRA));
	if (vars->jlmExtra == NULL) goto ERR;
	// 开辟随机数的缓存空间
	vars->RandomBytes = (unsigned char*)malloc(vars->RandomBytes_size * sizeof(unsigned char));
	vars->Passwords = (unsigned char*)malloc(vars->Passwords_size * sizeof(unsigned char));
	if (vars->RandomBytes == NULL || vars->Passwords == NULL) goto ERR;
	// 开辟jlm路径的缓存空间
	vars->jlmFileUrl = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->jlmFileUrl == NULL) goto ERR;
	// 同上
	vars->license = (unsigned char*)malloc(JLM_FILE_HASH_BYTE_LIMIT);
	if (vars->license == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;
	// 开辟临时运算缓存，译码过程中开辟相同的缓存大小即可
	vars->WriteOrReadBuff_Size = vars->OutByteArray_Size = vars->InByteArray_Size = BINARY_FILE_MAX_LIMIT;
	vars->InByteArray = (unsigned char*)malloc(vars->InByteArray_Size * sizeof(unsigned char));
	vars->OutByteArray = (unsigned char*)malloc(vars->OutByteArray_Size * sizeof(unsigned char));
	vars->WriteOrReadBuff = (unsigned char*)malloc(vars->WriteOrReadBuff_Size * sizeof(unsigned char));
	if (vars->InByteArray == NULL || vars->OutByteArray == NULL || vars->WriteOrReadBuff == NULL) goto ERR;
	// 开辟日志文件
	vars->log = (JLM_LOG*)malloc(sizeof(JLM_LOG));
	if (vars->log == NULL) goto ERR;
	// ID列表
	vars->IdList = (int*)malloc(vars->IdList_Size * sizeof(int));
	if (vars->IdList == NULL)  goto ERR;
	// 复制密码，去除“\0”
	strcpy(vars->Passwords, ucPassword);
	// 区分windows和linux
#ifdef _WINDOWS
	printf("_WINDOWS\n");
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "D:\\test.jlm";
	//const char* jlmFileUrl = "D:\\test_pack2.jlm";
	unsigned char* newName = "草帽女孩.bmp";
	// 输入待重命名的ID号，为0时
	vars->IdList[0] = 30; //2

#elif defined(_LINUX)
	printf("_LINUX\n");
	const char* licensePath = "/home/wangjielin/Desktop/yesine_jlmpack_2024_test.license";
	//const char* jlmFileUrl = "/home/wangjielin/Desktop/test.jlm";
	const char* jlmFileUrl = "/home/wangjielin/Desktop/test_pack2.jlm";
	unsigned char* newName = "草帽女孩.bmp";
	// 输入待重命名的ID号
	vars->IdList[0] = 18; //2

#endif
	strcpy(vars->licenseFilePath, licensePath);
	// jlm的文件路径
	strcpy(vars->jlmFileUrl, jlmFileUrl);
	strcpy(vars->pathList[0], newName);

	errSign = JlmPack_Rename(vars);
	printf("errSign = %d\n", errSign);

	if (errSign == 0) {
		printf("重命名成功！\n");
		printf("vars->log->type = %d\n", vars->log->type);
		printf("vars->log->fileName = %s\n", vars->log->fileName);
		printf("vars->log->consistencyMark = ");
		for (i = 0; i < PACK_FILE_HASH_BYTE_LIMIT; ++i) {
			printf("%02X,", vars->log->consistencyMark[i]);
		}
		printf("\n");
		printf("vars->log->datetime = %lld\n", vars->log->datetime);
		printf("vars->totalBytes = %lld, vars->cumulativeBytes = %lld\n", vars->totalBytes, vars->cumulativeBytes);
	}

ERR:
	if (vars) {
		if (vars->InByteArray) free(vars->InByteArray);
		if (vars->OutByteArray) free(vars->OutByteArray);
		if (vars->WriteOrReadBuff) free(vars->WriteOrReadBuff);
		if (vars->log) free(vars->log);
		if (vars->jlmFile) free(vars->jlmFile);
		if (vars->jlmExtra) free(vars->jlmExtra);
		if (vars->RandomBytes)  free(vars->RandomBytes);
		if (vars->Passwords)  free(vars->Passwords);
		if (vars->jlmFileUrl)  free(vars->jlmFileUrl);
		if (vars->license)free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		if (vars->IdList) free(vars->IdList);
		if (vars->pathList) {
			for (i = vars->pathListSize - 1; i >= 0; --i) if (vars->pathList[i]) free(vars->pathList[i]);
			free(vars->pathList);
		}
		free(vars);
	}
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}

// 预览目录测试
int main9() { // 9
	// 设置为""的目的是跟随系统，支持windows和linux
	 //en_US.UTF-8

	JLM_CATALOGINFOR* catalogs = NULL;
	VARIABLES* vars = NULL;
	int i, j, errSign = 0;
	unsigned char* ucPassword = "123456";

	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	if (vars == NULL) goto ERR;

	// 设置里面的参数
	vars->RandomBytes_size = RANDOM_BYTE_LIMIT;   // 编码和解码这个值必须相同
	vars->Passwords_size = strlen(ucPassword) + 1;
	vars->rule = 0;
	vars->license_size = 0;
	vars->IdList_Size = 1;  // 预览仅针对一个ID号

	vars->jlmFile = (JLM_FILE*)malloc(sizeof(JLM_FILE));
	if (vars->jlmFile == NULL) goto ERR;
	// vars->jlmExtra = NULL;
	vars->jlmExtra = (JLM_EXTRA*)malloc(sizeof(JLM_EXTRA));
	if (vars->jlmExtra == NULL) goto ERR;
	// 开辟随机数的缓存空间
	vars->RandomBytes = (unsigned char*)malloc(vars->RandomBytes_size * sizeof(unsigned char));
	vars->Passwords = (unsigned char*)malloc(vars->Passwords_size * sizeof(unsigned char));
	if (vars->RandomBytes == NULL || vars->Passwords == NULL) goto ERR;
	// 开辟jlm路径的缓存空间
	vars->jlmFileUrl = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->jlmFileUrl == NULL) goto ERR;
	// 同上
	vars->license = (unsigned char*)malloc(JLM_FILE_HASH_BYTE_LIMIT);
	if (vars->license == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;
	// 开辟临时运算缓存，译码过程中开辟相同的缓存大小即可
	vars->WriteOrReadBuff_Size = vars->OutByteArray_Size = vars->InByteArray_Size = BINARY_FILE_MAX_LIMIT;
	vars->InByteArray = (unsigned char*)malloc(vars->InByteArray_Size * sizeof(unsigned char));
	vars->OutByteArray = (unsigned char*)malloc(vars->OutByteArray_Size * sizeof(unsigned char));
	vars->WriteOrReadBuff = (unsigned char*)malloc(vars->WriteOrReadBuff_Size * sizeof(unsigned char));
	if (vars->InByteArray == NULL || vars->OutByteArray == NULL || vars->WriteOrReadBuff == NULL) goto ERR;
	// 开辟日志文件
	vars->log = (JLM_LOG*)malloc(sizeof(JLM_LOG));
	if (vars->log == NULL) goto ERR;
	// ID列表
	vars->IdList = (int*)malloc(vars->IdList_Size * sizeof(int));
	if (vars->IdList == NULL)  goto ERR;
	// 复制密码，去除“\0”
	strcpy(vars->Passwords, ucPassword);
	// 区分windows和linux
#ifdef _WINDOWS
	printf("_WINDOWS\n");
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "D:\\test.jlm";
	//const char* jlmFileUrl = "D:\\test_pack2.jlm";
	// 输入待预览的ID号
	vars->IdList[0] = 0; //2

#elif defined(_LINUX)
	printf("_LINUX\n");
	const char* licensePath = "/home/wangjielin/Desktop/yesine_jlmpack_2024_test.license";
	//const char* jlmFileUrl = "/home/wangjielin/Desktop/test.jlm";
	const char* jlmFileUrl = "/home/wangjielin/Desktop/test_pack2.jlm";
	// 输入待预览的ID号
	vars->IdList[0] = 18; //2

#endif
	strcpy(vars->licenseFilePath, licensePath);
	// jlm的文件路径
	strcpy(vars->jlmFileUrl, jlmFileUrl);

	// 读取目录
	catalogs = JlmPack_BrowseCatalog(vars, &errSign);
	printf("errSign = %d\n", errSign);

	if (errSign == 0) {
		printf("预览成功！\n");
		printf("vars->log->type = %d\n", vars->log->type);
		printf("vars->log->fileName = %s\n", vars->log->fileName);
		printf("vars->log->consistencyMark = ");
		for (i = 0; i < PACK_FILE_HASH_BYTE_LIMIT; ++i) {
			printf("%02X,", vars->log->consistencyMark[i]);
		}
		printf("\n");
		printf("vars->log->datetime = %lld\n", vars->log->datetime);
		printf("vars->totalBytes = %lld, vars->cumulativeBytes = %lld\n", vars->totalBytes, vars->cumulativeBytes);

		// 打印测试
		printf("catalogs->ID = %d\n", catalogs->ID);
		printf("catalogs->FID = %d\n", catalogs->FID);
		printf("catalogs->name = %s\n", catalogs->name);
		printf("catalogs->time = %lld\n", catalogs->time);
		printf("catalogs->byteslength = %llu\n", catalogs->byteslength);
		printf("catalogs->CatalogListSize = %d\n", catalogs->CatalogListSize);
		for (i = 0; i < catalogs->CatalogListSize; ++i) {
			printf("%d %d %d %s %llu ", catalogs->CatalogList[i].ID, catalogs->CatalogList[i].type, catalogs->CatalogList[i].FID, catalogs->CatalogList[i].name, catalogs->CatalogList[i].byteslength);
			printf("HASH:");
			for (j = 0; j < sizeof(catalogs->CatalogList[i].hv); ++j) {
				printf("%02x", catalogs->CatalogList[i].hv[j]);
			}
			printf("\n");
		}
		printf("\n");
	}

ERR:
	if (vars) {
		if (vars->InByteArray) free(vars->InByteArray);
		if (vars->OutByteArray) free(vars->OutByteArray);
		if (vars->WriteOrReadBuff) free(vars->WriteOrReadBuff);
		if (vars->log) free(vars->log);
		if (vars->jlmFile) free(vars->jlmFile);
		if (vars->jlmExtra) free(vars->jlmExtra);
		if (vars->RandomBytes)  free(vars->RandomBytes);
		if (vars->Passwords)  free(vars->Passwords);
		if (vars->jlmFileUrl)  free(vars->jlmFileUrl);
		if (vars->IdList) free(vars->IdList);
		if (vars->license)free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		free(vars);
	}
	// 注意catalogs必须释放
	if (catalogs) {
		if (catalogs->CatalogList) free(catalogs->CatalogList);
		free(catalogs);
	}
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}
// 数字证书验证测试
int main11() { // 10

	VARIABLES* vars = NULL;
	int i, errSign = 0;
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license"; //

	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	if (vars == NULL) goto ERR;
	vars->license_size = JLM_FILE_HASH_BYTE_LIMIT;
	vars->license = (unsigned char*)malloc(vars->license_size);
	if (vars->license == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;

	// yesine_jlmpack_2024_test.license路径
	strcpy(vars->licenseFilePath, licensePath);

	errSign = JlmPack_GetLicense(vars);

ERR:
	if (vars) {
		if (vars->license) free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		free(vars);
	}
	system("pause");
	return 0;
}
// 外部回调函数，实例，把解码得到的数据写入到文件中
void JLMPACK_Callback(VARIABLES* var, CALLBACK_VARIABLES* cbVars)
{
	//VARIABLES* thisVar = va_arg(args, VARIABLES*);
	FILE* fp = (FILE*)cbVars->p;

	// 利用回调函数将var->WriteOrReadBuff中的数据写入到文件中
	printf("解码出了%lld个字节。\n", var->effectiveBytes);

	Write(fp, var->WriteOrReadBuff, var->effectiveBytes);

	// 继续译码，当var->WriteOrReadBuff存在数据时，程序将处于阻塞状态
	var->status = 1;
}

// 译码到缓存测试，通过回调函数来处理
int main() { // 11
	VARIABLES* vars = NULL;
	CALLBACK_VARIABLES* cbVars = NULL;
	int i, errSign = 0;
	unsigned char* ucPassword = "123456";
	long long len;
	vars = (VARIABLES*)malloc(sizeof(VARIABLES));
	cbVars = (CALLBACK_VARIABLES*)malloc(sizeof(CALLBACK_VARIABLES));
	if (vars == NULL || cbVars == NULL) goto ERR;

	// 设置里面的参数
	vars->RandomBytes_size = RANDOM_BYTE_LIMIT;   // 编码和解码这个值必须相同
	vars->Passwords_size = strlen(ucPassword) + 1;
	vars->rule = 0;
	vars->license_size = 0;
	vars->IdList_Size = 1;  // 预览仅针对一个ID号

	vars->jlmFile = (JLM_FILE*)malloc(sizeof(JLM_FILE));
	if (vars->jlmFile == NULL) goto ERR;
	// vars->jlmExtra = NULL;
	vars->jlmExtra = (JLM_EXTRA*)malloc(sizeof(JLM_EXTRA));
	if (vars->jlmExtra == NULL) goto ERR;
	// 开辟随机数的缓存空间
	vars->RandomBytes = (unsigned char*)malloc(vars->RandomBytes_size * sizeof(unsigned char));
	vars->Passwords = (unsigned char*)malloc(vars->Passwords_size * sizeof(unsigned char));
	if (vars->RandomBytes == NULL || vars->Passwords == NULL) goto ERR;
	// 开辟jlm路径的缓存空间
	vars->jlmFileUrl = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->jlmFileUrl == NULL) goto ERR;
	// 同上
	vars->license = (unsigned char*)malloc(JLM_FILE_HASH_BYTE_LIMIT);
	if (vars->license == NULL) goto ERR;
	vars->licenseFilePath = (unsigned char*)malloc(URL_BYTE_LIMIT);
	if (vars->licenseFilePath == NULL) goto ERR;
	// 开辟临时运算缓存，译码过程中开辟相同的缓存大小即可
	vars->InByteArray_Size = BINARY_FILE_MAX_LIMIT;
	vars->WriteOrReadBuff_Size = vars->OutByteArray_Size = BINARY_FILE_MAX_LIMIT * 4;
	vars->InByteArray = (unsigned char*)malloc(vars->InByteArray_Size * sizeof(unsigned char));
	vars->OutByteArray = (unsigned char*)malloc(vars->OutByteArray_Size * sizeof(unsigned char));
	vars->WriteOrReadBuff = (unsigned char*)malloc(vars->WriteOrReadBuff_Size * sizeof(unsigned char));
	if (vars->InByteArray == NULL || vars->OutByteArray == NULL || vars->WriteOrReadBuff == NULL) goto ERR;
	// 开辟日志文件
	vars->log = (JLM_LOG*)malloc(sizeof(JLM_LOG));
	if (vars->log == NULL) goto ERR;
	// ID列表
	vars->IdList = (int*)malloc(vars->IdList_Size * sizeof(int));
	if (vars->IdList == NULL)  goto ERR;
	// 复制密码，去除“\0”
	strcpy(vars->Passwords, ucPassword);
	// 区分windows和linux
#ifdef _WINDOWS
	printf("_WINDOWS\n");
	const char* licensePath = "D:\\yesine_jlmpack_2024_test.license";
	const char* jlmFileUrl = "D:\\test.jlm";
	const char* OutFileUrl = "D:\\xxx.wav";
	//const char* jlmFileUrl = "D:\\test_pack2.jlm";
	// 输入译码的ID号，这里是指译码的文件
	vars->IdList[0] = 2;

#elif defined(_LINUX)
	printf("_LINUX\n");
	const char* licensePath = "/home/wangjielin/Desktop/yesine_jlmpack_2024_test.license";
	//const char* jlmFileUrl = "/home/wangjielin/Desktop/test.jlm";
	const char* jlmFileUrl = "/home/wangjielin/Desktop/test_pack2.jlm";

	const char* OutFileUrl = "/home/wangjielin/Desktop/xxx.wav";
	// 输入待预览的ID号
	vars->IdList[0] = 18; //2

#endif
	// 商业授权
	strcpy(vars->licenseFilePath, licensePath);
	// jlm的文件路径
	strcpy(vars->jlmFileUrl, jlmFileUrl);
	// 以写的模式
	cbVars->p = (void*)OpenBinaryFile(OutFileUrl, WRITE);

	// 启动译码
	errSign = JlmPack_ExtractToCache(vars, JLMPACK_Callback, cbVars);

	//释放fp
	Close(cbVars->p);

	printf("errSign = %d\n", errSign);

ERR:
	if (vars) {
		if (vars->InByteArray) free(vars->InByteArray);
		if (vars->OutByteArray) free(vars->OutByteArray);
		if (vars->WriteOrReadBuff) free(vars->WriteOrReadBuff);
		if (vars->log) free(vars->log);
		if (vars->jlmFile) free(vars->jlmFile);
		if (vars->jlmExtra) free(vars->jlmExtra);
		if (vars->RandomBytes)  free(vars->RandomBytes);
		if (vars->Passwords)  free(vars->Passwords);
		if (vars->jlmFileUrl)  free(vars->jlmFileUrl);
		if (vars->IdList) free(vars->IdList);
		if (vars->license)free(vars->license);
		if (vars->licenseFilePath) free(vars->licenseFilePath);
		free(vars);
	}
	if (cbVars) {
		if (cbVars->p) free(cbVars->p);
		free(cbVars);
	}
	// 结束
#ifdef _WINDOWS
	system("pause");
	return 0;
#elif defined(_LINUX)
	return 0;
#endif
}