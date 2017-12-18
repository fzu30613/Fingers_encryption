#define _CRT_SECURE_NO_WARNINGS
#include "encryption.h"
#include <cstdio>
#include <string.h>

//密钥传入
void EncryptionAlgorithm::setMessage(char *msg)
{
	Message = (char*)malloc(strlen(msg) * sizeof(char));
	strcpy(Message,msg);
}
EncryptionAlgorithm::EncryptionAlgorithm()
{
}
//给可变长编码密钥定长 
void EncryptionAlgorithm::setChangesize(int num)
{
	Changesize = num;
}
// 给PASSWORD赋值 
void EncryptionAlgorithm::setPassword(char *psw)
{
	Password = (char*)malloc(strlen(psw) * sizeof(char));
	strcpy(Password,psw);
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
void EncryptionAlgorithm::encryptFile()
{
    //打开二进制文件
	FILE *fw, *fr;
	fr = fopen(Sourcefile,"rb"); 
	fw = fopen(Objectfile,"wb"); 
	if(fr == NULL || fw == NULL)
	{
		fclose(fw);
		fclose(fr);
		return;
	}
	
	//加密 
	else
	{
		int length = getFilesize(Sourcefile) + Changesize;
		char *p = (char*)malloc(length*sizeof(char)); //分配内存，读取文件
		fread(p, sizeof(char), length, fr); //读二进制到内存
		
		//对内容进行增添 
		for (int i = 0; i < length-Changesize; i++)
		{
			p[i] ^= 'enc';
		 } 
		for (int i = 0; i < Changesize/2; i++)
		{
			p[i+length-Changesize] = Scurecode[i];
		}
		for (int i = 0; i < Changesize/2; i++)
		{
			p[i+length-Changesize+Changesize/2] = Password[i];
		}
		//将文件内容写入文件
		fwrite(p, sizeof(char), length, fw);
		fclose(fw);
		fclose(fr); 
	}
}
//解密文件
void EncryptionAlgorithm::decryptFile()
{
	//读取文件
	FILE *fr, *fw;
	fr = fopen (Sourcefile,"rb");
	if (fr == NULL)
	{
		fclose(fr);
		return;
	} 
	
	//解密条件 
	bool p1,p2;
	p1 = true;
	p2= true;
	//判定是否符合解密条件 
	int length = getFilesize(Sourcefile);
	char *p = (char*)malloc(length*sizeof(char));
	fread(p, sizeof(char), length, fr);
	for (int i = 0; i < Changesize/2; i++)
	{
		if (p[i+length-Changesize] != Message[i])
		{
			p1 = false;break;
		}
	}
	for (int i = 0; i < Changesize/2; i++)
	{
		if (p[i+length-Changesize+Changesize/2] != Message[i])
		{
			p2 = false;break;
		}
	}
	//解密文件 
	if(p1 || p2)
	{
		fw = fopen(Objectfile,"wb");
		if (fw == NULL)
		{
			fclose(fw);
			return;
		}
		//对文件内容反编译 
		for (int i = 0; i < length-Changesize; i++)
		{
			p[i] ^= 'enc';
		}
		
		//将文件内容写入解密文件
		fwrite(p, sizeof(char), length-Changesize, fw);
		fclose(fw);
		fclose(fr);
		return; 
	}
	else
	{	
	}
}
