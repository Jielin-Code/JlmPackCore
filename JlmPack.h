#pragma once
/**************************************************************************
 * @作者 王杰林 2024-06-20
 * @功能 遍历全部目录的文件夹以及文件，并保存到目录对象中，支持linux和windows
 * @版权 湖南遥昇通信技术有限公司
 * @版本 1.0
 * @邮件 254908447@qq.com
 **************************************************************************/
#ifndef JLMPACK_H  
#define JLMPACK_H

 // 包含配置头文件，必不可少
#include "Config.h"

// JLM文件的头部信息，头部信息必须是放在第1个包的初始位置，下面的顺序不能修改的，修改可能会出错
typedef struct
{
	unsigned char jlmHeadSign[4];                              // JLM压缩包文件的头部标识为{ 'J', 'L', 'M', ' '}，音频为{ 'J', 'L', 'M', 'A'}，视频为{ 'J', 'L', 'M', 'V'}，无法修改，就算传递进来也会被重置
	unsigned char consistencyMark[PACK_FILE_HASH_BYTE_LIMIT];  // 用来统一主包和子包以及系统后台信息，具有相同的consistencyMark才算一个JLM文件
	unsigned int packID;							           // 包编号分别为1,2,3,...等等，注意一定是从1开始
	unsigned int catalogBytesLength;                           // 目录的原始字节长度，尚未编码的字节长度，也是jlm文件除了JLM_FILE以外的目录存放长度
	unsigned int packSize;                                     // 整个jlm根据packBytesLength切割成packSize个子包
	unsigned int packBytesLength;                              // 单个包的字节数量（默认最大单个包大小为PACK_BYTES_LIMIT字节数）
	unsigned int encryptionWindow;                             // 加密轮函数的字节窗口，自动设定为BINARY_FILE_MAX_LIMIT - 2097152，解码时该值修改后将无法解密
	unsigned int extraSize;                                    // 记录JLM_EXTRA的字节数，如果extraSize = 0说明没有JLM_EXTRA，如果extraSize = sizeof(JLM_EXTRA)说明存在JLM_EXTRA
	unsigned long long fileBytesLength;                        // 压缩后的总字节长度，包括了头部信息的字节数，加密压缩后的数字证书和目录以及文件数据等（最大2^64-1个字节）
	unsigned char license[JLM_FILE_HASH_BYTE_LIMIT];           // 数字证书的数字指纹，商业授权版：由湖南遥昇通信技术有限公司颁发的SDK授权数字证书哈希值；个人版本由官网申请的个人证书哈希值
}JLM_FILE;

/**************************************************************************************
JLM_CATALOG目录对象，非常重要，是预览目录的重要结构体，仅用于展示JLM压缩包中的目录信息
***************************************************************************************/
typedef struct
{
	int ID;                                  // 当前目录的ID
	int FID;                                 // 上一层目录的ID号，顶层该ID号为0
	char type;                               // 文件类型，是文件夹还是文件类型，0表示文件夹，1表示文件
	char name[NAME_BYTE_LIMIT];              // 当前文件或文件夹的名称，包括了后缀名
	unsigned long long byteslength;          // 原始文件的字节长度，如果是文件夹，则包括了文件夹下的所有文件，如果是文件则是文件的字节数
	unsigned char hv[HASH_BYTE_LIMIT];       // 哈希值（校验或签名），最大支持8个字节（默认是4个字节）
	long long time;                          // 文件创建的时间
}JLM_CATALOG;

/**************************************************************************************
目录列表信息对象，非常重要，是预览目录的重要结构体，也是权限验证的重要结构体
***************************************************************************************/
typedef struct
{
	int ID;                                  // 当前目录的ID
	int FID;                                 // 上一层目录的ID号，方便进行退回
	char name[NAME_BYTE_LIMIT];              // 目录名称
	unsigned long long byteslength;          // 目录下的所有字节数
	long long time;                          // 目录创建的时间
	JLM_CATALOG* CatalogList;                // 子目录队列
	int CatalogListSize;                     // 子目录列表规模
}JLM_CATALOGINFOR;

/**************************************************************************************
JLM_EXTRA对象属于可选项和可自定义结构体，主要用于版权控制，不建议随意修改，只能增加
1、主函数采用sizeof(JLM_EXTRA)计算JLM_EXTRA的实际字节数并写入到文件末尾，所以不能定义指针变量
2、存放在JLM文件的最末尾（或者最后一个包），文件末尾从fileLength - extraLength字节开始属于JLM_EXTRA对象
***************************************************************************************/
typedef struct
{
	char owner[EXTRA_BYTE_LIMIT];                        // 版权拥有人（公司或个人）
	char promulgator[EXTRA_BYTE_LIMIT];                  // 颁布人，或发行人（公司或个人）
	char contact[EXTRA_BYTE_LIMIT];                      // 联系方式
	char versionNumber[EXTRA_BYTE_LIMIT];                // 版本号
	long long datetime;                                  // 发布时的系统时间戳
	unsigned char authorization;                         // 本JLM压缩包对外授权分类，0表示免费，1表示共享授权，2表示商业授权（具体的方式由用户自定义）
	unsigned char signature[JLM_FILE_HASH_BYTE_LIMIT];   // 数字签名，采用了JLMHA算法
}JLM_EXTRA;

/**************************************************************************************
日志结构体，即创建、解包、追加和删除均有日志文件输出，输出的日志文件可以在本地生成也可以存储到网络上
本日志文件只记录时间，动作，至于其他更多信息，可以通过LOGS结构体追加信息
***************************************************************************************/
typedef struct
{
	// 下面这4个对象属于系统对象严禁修改名字和删除，内容由库自动完成填入
	int type;                                                  // 0表示创建，1表示解包，2表示插入，3表示删除，4表示修改（一般指部分文件的重命名），5表示预览目录
	unsigned char consistencyMark[PACK_FILE_HASH_BYTE_LIMIT];  // 用来统一主包和子包以及系统后台信息
	unsigned char fileName[NAME_BYTE_LIMIT];                   // 文件名
	long long datetime;                                        // 发生的系统时间戳
	// 可以在此处追加其他对象，追加的对象和处理请到JLMPackAuthorityControl类中处理
}JLM_LOG;

/**************************************************************************************
VARIABLES变量传递，函数不同则对变量要求不同，请参考各函数的说明
***************************************************************************************/
typedef struct
{
	/**************************************************************************
	* 授权文件YESINE.license的路径，该路径必须正确而且证书也必须是正确有效才能使用本SDK中的所有函数
	**************************************************************************/
	unsigned char* licenseFilePath;
	/**************************************************************************
	* 总字节数totalBytes，和累计字节数Cumulative，主要用于进度条的计算
	**************************************************************************/
	unsigned long long totalBytes;
	unsigned long long cumulativeBytes;
	/**************************************************************************
	* jlmFile为JLM文件格式信息
	* jlmExtra为JLM文件扩展信息
	**************************************************************************/
	JLM_FILE* jlmFile;                              // 输入或输出jlm文件头信息，见结构体JLM_FILE
	JLM_EXTRA* jlmExtra;                            // 输入或输出jlm文件扩展信息，见结构体JLM_EXTRA
	/**************************************************************************
	* RandomBytes为编码前或译码后的随机数数组，随机数是抗量子破解的核心
	* RandomBytes_size为随机数数组中字节个数
	**************************************************************************/
	unsigned char* RandomBytes;                     // 输入或输出随机数，用于生成密钥
	int RandomBytes_size;                           // 输入或输出随机数的字节数
	/**************************************************************************
	* Passwords为编码或译码的密码数组，将根据用户的需要是否
	* Passwords_size为密码数组中字节个数，Passwords_size >= 0且Passwords_size < 64，不建议太长怕忘记后无法找回
	**************************************************************************/
	unsigned char* Passwords;                       // 输入加解密密码，译码时密码是解密的关键
	int Passwords_size;                             // 输入加解密密码长度
	/**************************************************************************
	* jlmFileUrl为编码后或解码前的jlm文件的路径和文件名，可以包括".jlm"
	* 支持XXX.jlm首包路径，也支持XXX_packX.jlm子包路径，任何这样的路径输入给jlmFileUrl均可定位到首包的路径
	**************************************************************************/
	unsigned char* jlmFileUrl;
	/**************************************************************************
	* pathList为编码前或解码后的各类文件和文件夹的路径列表，pathList中允许.jlm文件
	* pathListSize为pathList中路径的个数
	* 注意：编码时pathList为各类文件和文件夹的路径，所以pathListSize大于等于
	* 译码时pathList只能是译码后的文件夹路径，且pathListSize只能等于1
	**************************************************************************/
	unsigned char** pathList;                          // 待编码文件的路径列表或译码后存放文件的文件夹路径，pathList[i]代表一个路径
	int pathListSize;                               // pathList中路径个数
	/**************************************************************************
	* 签名的数字证书哈希值（数据指纹），用于计算JLM_FILE.cer
	**************************************************************************/
	unsigned char* license;                       // 签名的数字证书哈希值（数据指纹），用于计算JLM_FILE.license
	int license_size;                             // 签名的数字证书哈希值字节长度，fatherCer_size为0时说明无签名，此时JLM_FILE.cer为全0序列
	/**************************************************************************
	* rule为目录权限规则，用于管理目录打开条件，请参考SDK说明书
	**************************************************************************/
	short rule;                                     // 权限编号
	/**************************************************************************
	* IdList为待处理的ID列表，比如删除某些ID、追加文件到某个ID下、译码某些ID对应的文件夹或文件等
	* IdList_Size为IdList中ID的个数
	**************************************************************************/
	int* IdList;                                    // ID列表，主要用于译码、删除和增加，创建JLM文件时IdList无效
	int IdList_Size;                                // IdList中ID的个数
	/**************************************************************************
	* 临时运算变量，重复使用
	**************************************************************************/
	unsigned char* InByteArray;                     // 用于压缩和加密编码的临时输入，包括了文件复制等等
	unsigned int InByteArray_Size;
	unsigned char* OutByteArray;                    // 用于解压和解密译码的临时输出，包括了文件复制等等
	unsigned int OutByteArray_Size;
	unsigned char* WriteOrReadBuff;                 // 用于写文件，大小和OutByteArray、InByteArray一致
	unsigned int WriteOrReadBuff_Size;
	/**************************************************************************
	* 日志输入信息
	**************************************************************************/
	JLM_LOG* log;                                   // 日志文件

}VARIABLES;

#ifdef __cplusplus
extern "C" {
#endif
	/*************************************************************************************
	获取授权证书内数字签名的函数
	输入：
	VARIABLES* vars
	返回：
	Errsign:错误标识，0无错，大于1为错误
	*************************************************************************************/
	JLMPACK_API int JlmPack_GetLicense(VARIABLES* vars);

	/*************************************************************************************
	JLMHA哈希算法函数，根据输入的数据获取长度为OutByteArray_size的哈希值，返回1表示计算成功
	1、根据InByteArray生成哈希值，RandomByte是随机生成的字节值，HashByteArray的第0个字节用于存放RandomByte的值
	2、验证的时候，必须将老的哈希值A第一个字节作为RandomByte传递给JLMHA，然后再根据InByteArray计算出新的哈希值B，比较两者是否相同
	输入：
	RandomByte：生成哈希值时RandomByte为0-255的随机数；检验哈希值时RandomByte必须是哈希值的首个字节（前8个比特）
	InByteArray：用于计算哈希值的原始数据
	InByteArray_size：InByteArray的字节数
	HashByteArray：生成哈希值时HashByteArray中HashByteArray_size个字节必须为0x00；迭代计算哈希值时HashByteArray中HashByteArray_size可以为上一次得出的哈希值
	HashByteArray_size：需要生成的哈希值长度，可自定义
	返回：
	Errsign:错误标识，0无错，大于1为错误
	*************************************************************************************/
	JLMPACK_API int JlmPack_JLMHA(unsigned char RandomByte, unsigned char* InByteArray, unsigned int InByteArray_size, unsigned char* HashByteArray, unsigned int HashByteArray_size);

	/*************************************************************************************
	JLMSE对称加密算法函数，注意本函数不做密码校验，无论密码输入是否正确，均会给出一个译码结果
	输入：
	RandomBytes：随机数，主要是抗量子计算破解
	RandomBytes_size：随机数的长度
	Passwords：加密或解码密码
	Passwords_size：密码的长度
	InByteArray：需要编译码的字节缓存
	InByteArray_size：InByteArray的字节数
	OutByteArray：加解密后的字节缓存
	OutByteArray_size：输入需要编译码的字节数，返回实际编译码的字节数
	model：加密模式model = 0，解密模式model = 1
	返回：
	Errsign:错误标识，0无错，大于0出错
	*************************************************************************************/
	JLMPACK_API int JlmPack_JLMSE(unsigned char* RandomBytes, unsigned int RandomBytes_size, unsigned char* Passwords, unsigned int Passwords_size, unsigned char* InByteArray, unsigned int InByteArray_size, unsigned char* OutByteArray, unsigned int* OutByteArray_size, int model);

	/*************************************************************************************
	创建函数，根据参数相关的信息创建一个jlm压缩包
	特别注意：
	1、文件或文件夹通过pathList传递
	2、编码后jlm文件的路径通过jlmFileUrl传递，必须是带文件名（允许不带后缀.jlm）
	3、各权限编号通过rule传递
	4、IdList和IdList_Size在本函数内是无效的
	5、fatherCer为父亲节点的数字证书的数字指纹，可以为空，如果不为空必须与网站后台或license的指纹一致（fatherCer可以查询用户的数字证书），而且整个jlm文件全部采用fatherCer进行签名
	6、密码通过Passwords传递
	7、随机数通过RandomBytes传递，建议采用4-16个随机数，这个是防止量子破解的核心，不建议为空
	输入：
	VARIABLES*：参数变量
	返回：
	Errsign:错误标识
	log中的信息
	*************************************************************************************/
	JLMPACK_API int JlmPack_Create(VARIABLES* vars);

	/*************************************************************************************
	获取权限值的函数，除了JlmPack_Create函数，其他函数在使用前都必须完成权限检验
	特别注意：
	1、编码后jlm文件的路径通过jlmFileUrl传递，必须是带文件名（允许不带后缀.jlm）
	2、权限值通过vars->rule返回
	3、权限获取不记录日志
	输入：
	VARIABLES*：参数变量
	返回：
	Errsign:错误标识
	*************************************************************************************/
	JLMPACK_API int JlmPack_GetRule(VARIABLES* vars);

	/*************************************************************************************
	解包函数：输入IdList列表，解压和解密出IdList列表下面的所有文件或文件夹，解压需要做权限控制
	特别注意：
	1、如果解压整个压缩包，仅需在IdList中包括了0的ID号
	2、译码后的文件存储路径通过pathList[0]传递，而且仅允许pathListSize = 0
	3、待解压的JLM压缩包路径通过jlmFileUrl传递，支持jlmFileUrl = XXX_PackX.jlm（程序会自动找到主文件XXX.jlm）
	4、其他的通过对应的参数传递，注意RandomBytes会被本函数覆盖掉
	安全方案：
	1、用Passwords直接解密jlm文件的目录信息，如果解密错误返回无法解密
	2、如果解密正确，则可获得CatalogInfor对象以及rule的值，如果rule表达了需要强制服务器校验，则进行第3步，否则直接译码
	3、将rule、JLM_FILE.cer、JLM_FILE.consistencyMark和Username、Mac（本地获取）、CPU编号（本地获取）等等信息经过随机加密后推送给后台系统进行验证
	4、验证失败返回NULL，则函数UnpackJlmPackFile将根据rule进行动作
	5、如果验证成功，则返回当前文件在服务器上的全部日志信息
	输入：
	VARIABLES*：参数变量
	返回：
	Errsign:错误标识
	*************************************************************************************/
	JLMPACK_API int JlmPack_Unpack(VARIABLES* vars);

	/*************************************************************************************
	向Jlm文件中追加新待编码文件的函数，追加的过程不会解密任何文件，仅仅重新组织jlm文件包和修改目录信息
	特别注意
	1、只能设置IdList_Size = 1且IdList[0] = 某ID号（可以为0，表示新增信息存放在根目录下），意思是把新增信息存放在某个目录ID下，如果当前的ID为文件将自动存储到
	2、待添加的文件和文件夹路径通过pathList传递
	3、jlm压缩包文件路径通过jlmFileUrl传递，支持jlmFileUrl = XXX_PackX.jlm（程序会自动找到主文件XXX.jlm）
	4、其他的通过对应的参数传递
	输入：
	VARIABLES*：参数变量
	返回：
	Errsign:错误标识
	*************************************************************************************/
	JLMPACK_API int JlmPack_Insert(VARIABLES* vars);

	/*************************************************************************************
	删除Jlm压缩包中某些文件的函数，删除的过程不会解密任何文件，仅仅重新组织jlm文件包和修改目录信息
	特别注意
	1、待删除的目录ID号通过IdList传递，此时IdList_Size>=1
	2、jlm压缩包文件路径通过jlmFileUrl传递，支持jlmFileUrl = XXX_PackX.jlm（程序会自动找到主文件XXX.jlm）
	3、其他的通过对应的参数传递
	输入：
	VARIABLES*：参数变量
	返回：
	Errsign:错误标识
	*************************************************************************************/
	JLMPACK_API int JlmPack_Delete(VARIABLES* vars);

	/*************************************************************************************
	重命名某个ID的名称，要求是密码必须正确且权限足够，重命名过程无法修改目录中的任何缀名，防止文件格式错误
	特别注意：
	1、只能设置IdList_Size = 1且IdList[0] = 某ID号（可以为0，浏览根目录同级的文件和文件夹），每次只能针对一个ID进行重命名
	2、jlm压缩包文件路径通过jlmFileUrl传递，支持jlmFileUrl = XXX_PackX.jlm（程序会自动找到主文件XXX.jlm）
	3、新的名称用pathList[0]传递，所以pathListSize = 1
	输入：
	VARIABLES*：参数变量，重点是jlmFileUrl和IdList[0]
	返回：
	JLM_CATALOGINFOR*：目录信息
	*************************************************************************************/
	JLMPACK_API int JlmPack_Rename(VARIABLES* vars);

	/*************************************************************************************
	预览Jlm压缩包的某个ID下的同级文件和文件夹列表，要求是密码必须正确且权限足够
	特别注意：
	1、只能设置IdList_Size = 1且IdList[0] = 某ID号（可以为0，浏览根目录同级的文件和文件夹）
	如果IdList[0]指向的是文件表示当前文件的同级文件和文件夹
	如果IdList[0]指向的是文件夹表示当前文件夹下的同级的文件和文件夹
	2、jlm压缩包文件路径通过jlmFileUrl传递，支持jlmFileUrl = XXX_PackX.jlm（程序会自动找到主文件XXX.jlm）
	3、其他的通过对应的参数传递
	输入：
	VARIABLES*：参数变量，重点是jlmFileUrl和IdList[0]
	返回：
	JLM_CATALOGINFOR*：目录信息
	*************************************************************************************/
	JLMPACK_API JLM_CATALOGINFOR* JlmPack_BrowseCatalog(VARIABLES* vars, int* errSign);

#ifdef __cplusplus
}
#endif
#endif