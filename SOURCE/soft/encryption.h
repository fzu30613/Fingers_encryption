#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <cstdlib>

class EncryptionAlgorithm
{
    private:
           char *Password;
           char *Sourcefile;
           char *Objectfile;
           char *Scurecode;
           char *Message;
           int Changesize;
    public:
		   EncryptionAlgorithm();
           void setChangesize(int num);
		   void setMessage(char *msg);
           void setPassword(char *psw);
           void setSourcefile(char *path);
           void setObjectfile(char *path);
           void setScurecode(char *scode);
           int getFilesize(char *path);
           int encryptFile();
           int decryptFile(int way);
		   int deleteFile();
};

#endif
