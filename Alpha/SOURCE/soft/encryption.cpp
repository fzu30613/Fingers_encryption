#define _CRT_SECURE_NO_WARNINGS
#include "encryption.h"
#include <cstdio>
#include <string.h>

//��Կ����
void EncryptionAlgorithm::setMessage(char *msg)
{
	Message = (char*)malloc(strlen(msg) * sizeof(char));
	strcpy(Message,msg);
}
EncryptionAlgorithm::EncryptionAlgorithm()
{
}
//���ɱ䳤������Կ���� 
void EncryptionAlgorithm::setChangesize(int num)
{
	Changesize = num;
}
// ��PASSWORD��ֵ 
void EncryptionAlgorithm::setPassword(char *psw)
{
	Password = (char*)malloc(strlen(psw) * sizeof(char));
	strcpy(Password,psw);
}
//��SOURCEFILE��ֵ 
void EncryptionAlgorithm::setSourcefile(char *path)
{
	Sourcefile = (char*)malloc(strlen(path) * sizeof(char));
	strcpy(Sourcefile,path);
}
//��OBJECTFILE��ֵ 
void EncryptionAlgorithm::setObjectfile(char *path)
{
	Objectfile = (char*)malloc(strlen(path) * sizeof(char));
	strcpy(Objectfile,path);
}
//��SCURECODE��ֵ 
void EncryptionAlgorithm::setScurecode(char *scode)
{
	Scurecode = (char*)malloc(strlen(scode) * sizeof(char));
	strcpy(Scurecode,scode);
}
//�����ļ���С 
int EncryptionAlgorithm::getFilesize(char *path)
{
	FILE *fp = fopen(path,"r");
	if (fp == NULL)
	{
		return -1;
	}
	else
	{
		fseek(fp,0,SEEK_END);   //fseek�����ض�λ���ϵ��ļ�ָ�� SEEK_END �ļ�β  SEEK_SET �ļ�ͷ 
		int length = ftell(fp); //ftell���ڵõ��ļ�λ��ָ�뵱ǰλ��������ļ��׵�ƫ���ֽ���
		return length;
	}
}
//�����ļ� 
void EncryptionAlgorithm::encryptFile()
{
    //�򿪶������ļ�
	FILE *fw, *fr;
	fr = fopen(Sourcefile,"rb"); 
	fw = fopen(Objectfile,"wb"); 
	if(fr == NULL || fw == NULL)
	{
		fclose(fw);
		fclose(fr);
		return;
	}
	
	//���� 
	else
	{
		int length = getFilesize(Sourcefile) + Changesize;
		char *p = (char*)malloc(length*sizeof(char)); //�����ڴ棬��ȡ�ļ�
		fread(p, sizeof(char), length, fr); //�������Ƶ��ڴ�
		
		//�����ݽ������� 
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
		//���ļ�����д���ļ�
		fwrite(p, sizeof(char), length, fw);
		fclose(fw);
		fclose(fr); 
	}
}
//�����ļ�
void EncryptionAlgorithm::decryptFile()
{
	//��ȡ�ļ�
	FILE *fr, *fw;
	fr = fopen (Sourcefile,"rb");
	if (fr == NULL)
	{
		fclose(fr);
		return;
	} 
	
	//�������� 
	bool p1,p2;
	p1 = true;
	p2= true;
	//�ж��Ƿ���Ͻ������� 
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
	//�����ļ� 
	if(p1 || p2)
	{
		fw = fopen(Objectfile,"wb");
		if (fw == NULL)
		{
			fclose(fw);
			return;
		}
		//���ļ����ݷ����� 
		for (int i = 0; i < length-Changesize; i++)
		{
			p[i] ^= 'enc';
		}
		
		//���ļ�����д������ļ�
		fwrite(p, sizeof(char), length-Changesize, fw);
		fclose(fw);
		fclose(fr);
		return; 
	}
	else
	{	
	}
}
