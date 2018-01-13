#define _CRT_SECURE_NO_WARNINGS
#include "encryption.h"
#include <cstdio>
#include <string.h>
#include <QDebug>

//密钥传入
void EncryptionAlgorithm::setMessage(char *msg)
{
	Message = (char*)malloc(strlen(msg) * sizeof(char));
	strcpy(Message,msg);
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		Message[i] ^= Message[Changesize / 2 - 1];
	}
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		Message[Changesize / 2 - 1] ^= Message[i];
	}
}

EncryptionAlgorithm::EncryptionAlgorithm()
{
}

//给可变长编码密钥(Password+Scurecode)定长
void EncryptionAlgorithm::setChangesize(int num)
{
	Changesize = num;
}

// 给PASSWORD赋值
void EncryptionAlgorithm::setPassword(char *psw)
{
	Password = (char*)malloc(strlen(psw) * sizeof(char));
	strcpy(Password,psw);
	for (int i = 0; i < Changesize/2-1; i++)
	{
		Password[i] ^= Password[Changesize/2-1];
	}
	for (int i = 0; i < Changesize/2-1; i++)
	{
		Password[Changesize/2-1] ^= Password[i];
	}
}

//给SOURCEFILE赋值
void EncryptionAlgorithm::setSourcefile(char *path)
{
	Sourcefile = (char*)malloc(strlen(path) * sizeof(char));
	strcpy(Sourcefile,path);
}

//给OBJECTFILE赋值
void EncryptionAlgorithm::setObjectfile(char *path)
{
	Objectfile = (char*)malloc(strlen(path) * sizeof(char));
	strcpy(Objectfile,path);
}

//给SCURECODE赋值
void EncryptionAlgorithm::setScurecode(char *scode)
{
	Scurecode = (char*)malloc(strlen(scode) * sizeof(char));
	strcpy(Scurecode,scode);
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		Scurecode[i] ^= Scurecode[Changesize / 2-1];
	}
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		Scurecode[Changesize / 2-1] ^= Scurecode[i];
	}
}

//计算文件大小
int EncryptionAlgorithm::getFilesize(char *path)
{
	FILE *fp = fopen(path,"r");
	if (fp == NULL)
	{
		return -1;
	}
	else
	{
		fseek(fp,0,SEEK_END);   //fseek用于重定位流上的文件指针 SEEK_END 文件尾  SEEK_SET 文件头
		int length = ftell(fp); //ftell用于得到文件位置指针当前位置相对于文件首的偏移字节数
		return length;
	}
}

//加密文件
/*************************************
0   正确
-1 打开文件错误
/*************************************/
int EncryptionAlgorithm::encryptFile()
{
	//打开二进制文件
	FILE *fw, *fr;
    fr = fopen(Sourcefile,"a+b");
	fw = fopen(Objectfile,"w+b");

	int offset = 0;//文件偏移量
	int numRead; //文件指针指向

	if(fr == NULL && fw != NULL)
	{
		fclose(fw);
		return -1;
	}
	if (fr != NULL && fw == NULL)
	{
		fclose(fr);
		return -1;
	}
	else if (fr == NULL && fw != NULL)
	{
		fclose(fw);
		return -1;
	}
	else if (fr == NULL && fw == NULL)
	{
		return -1;
	}
	//加密
	else
	{
		int length = getFilesize(Sourcefile) + Changesize;

		//psw 反编译
		char *psw = (char*)malloc(Changesize/2 * sizeof(char));
		for (int i = 0; i < Changesize / 2; i++)
		{
			psw[i] = Password[i];
		}
		for (int i = Changesize / 2 - 2; i >= 0; i--)
		{
			psw[Changesize / 2 - 1] ^= psw[i];
		}
		for (int i = 0; i < Changesize / 2 - 1; i++)
		{
			psw[i] ^= psw[Changesize / 2 - 1];
		}


		char *p = (char*)malloc(1024*1024*10*sizeof(char)+sizeof(char)*Changesize); //分配内存，读取文件
		while(!feof(fr))
		{
			numRead = fread(p, sizeof(char), 1024*1024*10, fr); //读二进制到内存

			//对内容进行增添
			if (1024*1024*10+offset < length-Changesize)
			{
				for (int i = 0; i < 1024*1024*10; i++)
				{
					p[i] ^= psw[i%(Changesize/2-1)];
				}
				fwrite(p, sizeof(char),1024*1024*10 , fw);//将文件内容写入文件
				/***************************************************/
				//干扰项
				for (int i = 0; i < 8; i++)
				{
					p[i] ^= psw[i%(Changesize/2)];
				}
				fwrite(p, sizeof(char), 8, fw);//写入干扰项
				/***************************************************/
			}
			else
			{
				for (int i = 0; i+offset < length-Changesize; i++)
				{
					p[i] ^=psw[i%(Changesize/2-1)];
				}

				for (int i = 0; i < Changesize/2; i++)
				{
					p[i+length-Changesize-offset] = Scurecode[i];
				}
				for (int i = 0; i < Changesize/2; i++)
				{
					p[i+length-Changesize+Changesize/2-offset] = Password[i];
				}
				fwrite(p, sizeof(char), length-offset, fw);//将文件内容写入文件
			}
			offset += numRead;//指针偏移量记录
		}
		fclose(fw);
		fclose(fr);
		return 0;
	}
}

//解密文件
/*************************************
0  正确
-1 打开文件错误
-2 救援码错误
-3 解密密码错误
/*************************************/
int EncryptionAlgorithm::decryptFile(int way)
{
	//读取文件
	FILE *fr, *fw;
    fr = fopen (Sourcefile,"a+b");

	int offset = 0;//文件偏移量
	int numRead; //文件指针指向
	char *psw = (char*)malloc(Changesize / 2 * sizeof(char));

	if (fr == NULL)
	{
		fclose(fr);
		return -1;
	}

	//解密条件
	bool p1,p2;
	p1 = true;//securecode
	p2= true;//password
	if (way == 1)
	{
		p1 = false;
	}
	else if (way == 2)
	{
		p2 = false;
	}
	//判定是否符合解密条件
	int length = getFilesize(Sourcefile);
	char *p = (char*)malloc(1024*1024*10*sizeof(char)+Changesize*sizeof(char)+8);

	fseek(fr,-Changesize*sizeof(char),SEEK_END);//文件指针置尾部验证代码开始处

	fread(p, sizeof(char), Changesize*sizeof(char), fr);
	for (int i = 0; i < Changesize/2; i++)
	{
		if (p[i] != Message[i])
		{
			p1 = false;
			break;
		}
	}	
	for (int i = 0; i < Changesize/2; i++)
	{
		psw[i] = p[i + Changesize / 2];
		if (p[i+Changesize/2] != Message[i])
		{
			p2 = false;
		}
	}
	fseek(fr,0,SEEK_SET);

	//psw反编译
	for (int i = Changesize / 2 - 2; i >= 0; i--)
	{
		psw[Changesize / 2 - 1] ^= psw[i];
	}
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		psw[i] ^= psw[Changesize / 2 - 1];
	}

	//解密文件
	if(p1 || p2)
	{
		fw = fopen(Objectfile,"w+b");
		if (fw == NULL)
		{
			fclose(fr);
			fclose(fw);
			return -1;
		}
		//对文件内容反编译
		while (!feof(fr))
		{

			numRead = fread(p, sizeof(char), 1024*1024*10+8, fr);
			if (1024*1024*10+offset < length-Changesize)
			{
				for (int i = 0; i < 1024*1024*10; i++)
				{
					p[i] ^= psw[i%(Changesize/2-1)];
				}
				//将文件内容写入解密文件
			    fwrite(p, sizeof(char), 1024*1024*10, fw);
			}
			else
			{
				for (int i = 0; i+offset < length-Changesize; i++)
				{
					p[i] ^= psw[i%(Changesize/2-1)];
				}
				//将文件内容写入解密文件
			    fwrite(p, sizeof(char), length-Changesize-offset, fw);
			}
			offset += numRead;
		}
		free(p);
		fclose(fw);
		fclose(fr);
		return 0;
	}
	else
	{
        if (!p1)
		{
			fclose(fr);
			return -2;
		}
		else
		{
			fclose(fr);
			return -3;
		}
	}
}

int EncryptionAlgorithm::deleteFile()
{
	//return remove(Sourcefile);
	char* command;
	command = (char*)malloc(100 * sizeof(char));
	sprintf(command, "del/f/s/q %s", Sourcefile); 

	FILE  *fp;
	fp = fopen("./error.txt", "w");
	fwrite(Sourcefile, sizeof(char), sizeof(Sourcefile) / sizeof(char), fp);
	fclose(fp);
	return system(command);
}
