#define _CRT_SECURE_NO_WARNINGS
#include "encryption.h"
#include <cstdio>
#include <string.h>
#include <QDebug>

//��Կ����
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

//���ɱ䳤������Կ(Password+Scurecode)����
void EncryptionAlgorithm::setChangesize(int num)
{
	Changesize = num;
}

// ��PASSWORD��ֵ
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
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		Scurecode[i] ^= Scurecode[Changesize / 2-1];
	}
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		Scurecode[Changesize / 2-1] ^= Scurecode[i];
	}
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
/*************************************
0   ��ȷ
-1 ���ļ�����
/*************************************/
int EncryptionAlgorithm::encryptFile()
{
	//�򿪶������ļ�
	FILE *fw, *fr;
    fr = fopen(Sourcefile,"a+b");
	fw = fopen(Objectfile,"w+b");

	int offset = 0;//�ļ�ƫ����
	int numRead; //�ļ�ָ��ָ��

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
	//����
	else
	{
		int length = getFilesize(Sourcefile) + Changesize;

		//psw ������
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


		char *p = (char*)malloc(1024*1024*10*sizeof(char)+sizeof(char)*Changesize); //�����ڴ棬��ȡ�ļ�
		while(!feof(fr))
		{
			numRead = fread(p, sizeof(char), 1024*1024*10, fr); //�������Ƶ��ڴ�

			//�����ݽ�������
			if (1024*1024*10+offset < length-Changesize)
			{
				for (int i = 0; i < 1024*1024*10; i++)
				{
					p[i] ^= psw[i%(Changesize/2-1)];
				}
				fwrite(p, sizeof(char),1024*1024*10 , fw);//���ļ�����д���ļ�
				/***************************************************/
				//������
				for (int i = 0; i < 8; i++)
				{
					p[i] ^= psw[i%(Changesize/2)];
				}
				fwrite(p, sizeof(char), 8, fw);//д�������
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
				fwrite(p, sizeof(char), length-offset, fw);//���ļ�����д���ļ�
			}
			offset += numRead;//ָ��ƫ������¼
		}
		fclose(fw);
		fclose(fr);
		return 0;
	}
}

//�����ļ�
/*************************************
0  ��ȷ
-1 ���ļ�����
-2 ��Ԯ�����
-3 �����������
/*************************************/
int EncryptionAlgorithm::decryptFile(int way)
{
	//��ȡ�ļ�
	FILE *fr, *fw;
    fr = fopen (Sourcefile,"a+b");

	int offset = 0;//�ļ�ƫ����
	int numRead; //�ļ�ָ��ָ��
	char *psw = (char*)malloc(Changesize / 2 * sizeof(char));

	if (fr == NULL)
	{
		fclose(fr);
		return -1;
	}

	//��������
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
	//�ж��Ƿ���Ͻ�������
	int length = getFilesize(Sourcefile);
	char *p = (char*)malloc(1024*1024*10*sizeof(char)+Changesize*sizeof(char)+8);

	fseek(fr,-Changesize*sizeof(char),SEEK_END);//�ļ�ָ����β����֤���뿪ʼ��

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

	//psw������
	for (int i = Changesize / 2 - 2; i >= 0; i--)
	{
		psw[Changesize / 2 - 1] ^= psw[i];
	}
	for (int i = 0; i < Changesize / 2 - 1; i++)
	{
		psw[i] ^= psw[Changesize / 2 - 1];
	}

	//�����ļ�
	if(p1 || p2)
	{
		fw = fopen(Objectfile,"w+b");
		if (fw == NULL)
		{
			fclose(fr);
			fclose(fw);
			return -1;
		}
		//���ļ����ݷ�����
		while (!feof(fr))
		{

			numRead = fread(p, sizeof(char), 1024*1024*10+8, fr);
			if (1024*1024*10+offset < length-Changesize)
			{
				for (int i = 0; i < 1024*1024*10; i++)
				{
					p[i] ^= psw[i%(Changesize/2-1)];
				}
				//���ļ�����д������ļ�
			    fwrite(p, sizeof(char), 1024*1024*10, fw);
			}
			else
			{
				for (int i = 0; i+offset < length-Changesize; i++)
				{
					p[i] ^= psw[i%(Changesize/2-1)];
				}
				//���ļ�����д������ļ�
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
