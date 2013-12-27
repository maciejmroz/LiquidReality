/*******************************************************************************************
  LiquidReality Virtual Filesystem
  Based on VFS 2.0 from my old engine
  (c) 2001-2002 Maciej Mróz
*******************************************************************************************/

#include "StdAfx.h"
#include<stdlib.h>
#include<stdarg.h>
#include"zlib.h"
#include <boost/lexical_cast.hpp>

using namespace LR;

void PrintfBox(const char* format, ...)
{
	char buffer[4096];
	va_list arg_list;
	va_start(arg_list,format);
	vsprintf(buffer,format,arg_list);
	va_end(arg_list);
	MessageBox(NULL,buffer,"LiquidReality filesystem internal error",0);
}

fsImpl::fsImpl()
{
	m_uRefCount=0;
	m_bFilesystemOpened=false;
	m_pszDatafileName=NULL;
	m_nNumOpenFiles=0;
	m_nNumFiles=0;
	m_pfnFileCallback=NULL;
	m_pFileTbl=NULL;
	m_pbDatafileCoverage=NULL;
	m_pDatafileFileStruct=NULL;
	m_pCompressInBuffer=NULL;
	m_pCompressInTempBuffer=NULL;
	m_uBufSize=0;
	m_bUseRootDirectory=false;
	int i;
	for(i=0;i<MAX_LRERR_FS_FILES;i++)
	{
		m_uFileSizes[i]=0;
		m_pFileBuffers[i]=NULL;
		m_pFileTempBuffers[i]=NULL;
		m_bIsOpen[i]=false;
		m_bIsTemp[i]=false;
		m_pFilePositions[i]=NULL;
		m_bIsWriteFileOpen[i]=FALSE;
		m_pWriteFiles[i]=NULL;
		m_bIsTextFile[i]=false;
	}
}

fsImpl::~fsImpl()
{
	if(m_bFilesystemOpened)
	{
		closeFilesystem(NULL);
	}
}

ULONG fsImpl::Release()
{
	m_uRefCount--;
	if( m_uRefCount == 0 )
	{
		delete this;
		return 0;
	}
	return m_uRefCount;
}

ULONG fsImpl::AddRef()
{
	m_uRefCount++;
	return m_uRefCount;
}

void Createfs(LPVOID* ppv)
{
	fs *p_vfs=NULL;
	p_vfs = new fsImpl();
	p_vfs->AddRef();
	*ppv=(LPVOID)p_vfs;
}

/******************************************************
 VFS3 (LiquidReality) datafile format:
 'VFS3' - filetype identifier
 byte[256] - global data (currently unused)
 uint32 numfiles
 fat_entry2 {
  uint32 file_offset
  uint32 file_size
  uint32 flags - currently only lowest bit is used to
                 indicate compression
  uint32 uncompressed_size
  char filename[240] - null terminated
 } [numfiles] - sizeof(fat_entry2)=256
 file_data[numfiles] - data from files
 All numbers in little endian
 EOF
********************************************************/

//Ifs interface methods
void fsImpl::openFilesystem(ULONG uBufSize,const char* pszDatafile,
											PFNFILEOPENCALLBACK pFunc)
{
	if(m_bFilesystemOpened)
	{
		throw IOExceptionImpl("Filesystem already opened");
	}
	m_nNumOpenFiles=0;
	m_nNumFiles=0;
	m_pFileTbl=NULL;
	m_pbDatafileCoverage=NULL;
	m_pCompressInBuffer=NULL;
	m_pCompressInTempBuffer=NULL;
	m_uBufSize=0;
	m_pDatafileFileStruct=NULL;
	m_pszDatafileName=NULL;
	int i;
	for(i=0;i<MAX_LRERR_FS_FILES;i++)
	{
		m_uFileSizes[i]=0;
		m_pFileBuffers[i]=NULL;
		m_pFileTempBuffers[i]=NULL;
		m_bIsOpen[i]=FALSE;
		m_bIsTemp[i]=FALSE;
		m_pFilePositions[i]=NULL;
	}
	if(pszDatafile)
	{
		m_pszDatafileName=new char[strlen(pszDatafile)+1];
		strcpy(m_pszDatafileName,pszDatafile);
		m_pDatafileFileStruct=fopen(m_pszDatafileName,"rb");
		if(!m_pDatafileFileStruct)
		{
			//TODO: define FIleNotFoundException!!!
			throw IOExceptionImpl("File not found");
		}
		char FileID[4];
		::fread(&FileID,1,4,m_pDatafileFileStruct);
		if((FileID[0]!='V') ||
			(FileID[1]!='F') ||
			(FileID[2]!='S') ||
			(FileID[3]!='3'))
		{
			throw IOExceptionImpl("Unknown datafile format");
		}
		::fseek(m_pDatafileFileStruct,256,SEEK_CUR);
		::fread(&m_nNumFiles,1,4,m_pDatafileFileStruct);
		m_pFileTbl=new fsFatEntry[m_nNumFiles];
		::fread(m_pFileTbl,1,256*m_nNumFiles,m_pDatafileFileStruct);
		m_pbDatafileCoverage=new bool[m_nNumFiles];
		ZeroMemory(m_pbDatafileCoverage,m_nNumFiles*sizeof(bool));
	}
	m_pfnFileCallback=pFunc;
	m_uBufSize=uBufSize;
	m_pCompressInBuffer=new unsigned char[m_uBufSize];
	m_bFilesystemOpened=TRUE;
	for(i=0;i<MAX_LRERR_FS_FILES;i++)
	{
		m_pFileBuffers[i]=new unsigned char[m_uBufSize+1];
	}
}

void fsImpl::setFilesystemRootDirectory(const char *pszRootDirectory)
{
	if(pszRootDirectory)
	{
		m_bUseRootDirectory=TRUE;
		m_lstrRootDirectory=pszRootDirectory;
	}
	else
	{
		m_bUseRootDirectory=FALSE;
		m_lstrRootDirectory="";
	}
}

void	fsImpl::closeFilesystem(PFNFILEUNUSEDCALLBACK pFunc)
{
	if(!m_bFilesystemOpened)
	{
		throw IOExceptionImpl("Filesystem not opened");
	}
	ULONG i;
	if(m_pbDatafileCoverage)
	{
		for(i=0;i<m_nNumFiles;i++)
		{
			if((!m_pbDatafileCoverage[i]) &&
				(pFunc))
			{
				pFunc(m_pFileTbl[i].filename);
			}
		}
		delete m_pbDatafileCoverage;
		m_pbDatafileCoverage=NULL;
	}
	m_bFilesystemOpened=FALSE;
	if(m_pszDatafileName)
	{
		delete m_pszDatafileName;
		m_pszDatafileName=NULL;
	}
	m_nNumOpenFiles=0;
	m_nNumFiles=0;
	m_pfnFileCallback=NULL;
	if(m_pFileTbl)
	{
		delete m_pFileTbl;
		m_pFileTbl=NULL;
	}
	if(m_pDatafileFileStruct)
	{
		fclose(m_pDatafileFileStruct);
		m_pDatafileFileStruct=NULL;
	}
	if(m_pCompressInBuffer)
	{
		delete m_pCompressInBuffer;
		m_pCompressInBuffer=NULL;
	}
	if(m_pCompressInTempBuffer)
	{
		delete m_pCompressInTempBuffer;
		m_pCompressInTempBuffer=NULL;
	}
	m_uBufSize=0;
	for(i=0;i<MAX_LRERR_FS_FILES;i++)
	{
		m_uFileSizes[i]=0;
		if(m_pFileBuffers[i])
		{
			delete m_pFileBuffers[i];
			m_pFileBuffers[i]=NULL;
		}
		if(m_pFileTempBuffers[i])
		{
			delete m_pFileTempBuffers[i];
			m_pFileTempBuffers[i]=NULL;
		}
		m_bIsOpen[i]=FALSE;
		m_bIsTemp[i]=FALSE;
		m_pFilePositions[i]=NULL;
		if(m_bIsWriteFileOpen[i])
		{
			fclose(m_pWriteFiles[i]);
		}
		m_pWriteFiles[i]=NULL;
		m_bIsTextFile[i]=NULL;
	}
}

void fsImpl::createFlatDatafile(const char ** ppszFiles,ULONG* pFlags,
											ULONG nFiles, const char* szDatafile,
											PFNFILEPROCESSCALLBACK pFunc)
{
	WIN32_FILE_ATTRIBUTE_DATA	FileAttribs;
	fsFatEntry*				pFatEntries=new fsFatEntry[nFiles];
	ZeroMemory(pFatEntries,nFiles*sizeof(fsFatEntry));
	ULONG i;
	if(pFunc) pFunc("Gathering file information ...");
	for(i=0;i<nFiles;i++)
	{
		if(!GetFileAttributesEx(ppszFiles[i],GetFileExInfoStandard,&FileAttribs))
		{
			throw IOExceptionImpl("File not found!");		//TODO: be more informative
		}
		pFatEntries[i].dwUncompressedSize=FileAttribs.nFileSizeLow;
		pFatEntries[i].dwFlags=pFlags[i];
		if(pFlags[i] & 1)
		{
			unsigned char	*pTempCompressOutBuf=NULL;
			unsigned char	*pTempCompressInBuf=NULL;
			FILE*			f=NULL;
			pFatEntries[i].dwSize=pFatEntries[i].dwUncompressedSize*11/10+12;
			pTempCompressOutBuf=new unsigned char[pFatEntries[i].dwSize];
			pTempCompressInBuf=new unsigned char[pFatEntries[i].dwUncompressedSize];
			f=fopen(ppszFiles[i],"rb");
			::fread(pTempCompressInBuf,1,pFatEntries[i].dwUncompressedSize,f);
			fclose(f);
			if(compress(pTempCompressOutBuf,&pFatEntries[i].dwSize,pTempCompressInBuf,
				pFatEntries[i].dwUncompressedSize)!=Z_OK)
			{
				throw IOExceptionImpl("Compress error");
			}
			delete [] pTempCompressInBuf;
			delete [] pTempCompressOutBuf;
		}
		else
		{
			pFatEntries[i].dwSize=pFatEntries[i].dwUncompressedSize;
		}
		if(i==0)
		{
			pFatEntries[i].dwOffset=264+256*nFiles;
		}
		else
		{
			pFatEntries[i].dwOffset=pFatEntries[i-1].dwOffset+
				pFatEntries[i-1].dwSize;
		}
		char *tmp=const_cast<char*>(ppszFiles[i])+strlen(ppszFiles[i]);  //ending '\0'
		while((tmp!=ppszFiles[i]) &&
			(*tmp!='\\') &&
			(*tmp!='/'))
		{
			tmp--;
		}
		if((*tmp=='\\') || (*tmp=='/'))
		{
			tmp++;
		}
		strcpy(pFatEntries[i].filename,tmp);
	}
	FILE			*datafile=NULL;
	FILE			*f=NULL;
	unsigned char	*pTempInBuf;
	char			FileID[4]={'V','F','S','3'};
	char			FilePrivateData[256];
	datafile=fopen(szDatafile,"wb");
	ZeroMemory(FilePrivateData,256);
	if(pFunc)
	{
		pFunc("Writing file allocation table ...");
	}
	::fwrite(FileID,1,4,datafile);
	::fwrite(FilePrivateData,1,256,datafile);
	::fwrite(&nFiles,1,4,datafile);
	::fwrite(pFatEntries,1,nFiles*sizeof(fsFatEntry),datafile);
	for(i=0;i<nFiles;i++)
	{
		if(pFunc)
		{
			pFunc(pFatEntries[i].filename);
		}
		f=fopen(ppszFiles[i],"rb");
		pTempInBuf=new unsigned char[pFatEntries[i].dwUncompressedSize];
		::fread(pTempInBuf,1,pFatEntries[i].dwUncompressedSize,f);
		fclose(f);
		if(pFatEntries[i].dwFlags & 1)
		{
			unsigned char*	pTempCompressOutBuf=NULL;
			unsigned long lTmpLen=pFatEntries[i].dwUncompressedSize*11/10+12;
			pTempCompressOutBuf=new unsigned char[lTmpLen];
			compress(pTempCompressOutBuf,&lTmpLen,pTempInBuf,
				pFatEntries[i].dwUncompressedSize);
			if(lTmpLen!=pFatEntries[i].dwSize)
			{
				throw IOExceptionImpl("Unknown error");
			}
			::fwrite(pTempCompressOutBuf,1,pFatEntries[i].dwSize,datafile);
		}
		else
		{
			::fwrite(pTempInBuf,1,pFatEntries[i].dwSize,datafile);
		}
		delete [] pTempInBuf;
		pTempInBuf=NULL;
	}
	fclose(datafile);
}

void fsImpl::unpackDatafile(const char* pszDatafile,const char* pszDir,
												PFNFILEPROCESSCALLBACK pFunc)
{
	if(m_bFilesystemOpened)
	{
		throw IOExceptionImpl("Filesystem already opened");
	}
	ULONG	i=0;
	ULONG	uFileHandle=0;
	FILE*	f=NULL;
	openFilesystem(500000,pszDatafile,NULL);
	HRESULT hr=S_OK;
	for(i=0;i<m_nNumFiles;i++)
	{
		openFile(m_pFileTbl[i].filename,uFileHandle);
		char szOutName[1024];
		strcpy(szOutName,pszDir);
		if(szOutName[strlen(pszDir)]!='\\') strcat(szOutName,"\\");
		strcat(szOutName,m_pFileTbl[i].filename);
		f=fopen(szOutName,"wb");
		if(m_bIsTemp[uFileHandle])
		{
			::fwrite(m_pFileTempBuffers[uFileHandle],1,
				m_pFileTbl[i].dwUncompressedSize,f);
		}
		else
		{
			::fwrite(m_pFileBuffers[uFileHandle],1,
				m_pFileTbl[i].dwUncompressedSize,f);
		}
		fclose(f);
		if(pFunc)
		{
			pFunc(szOutName);
		}
		closeFile(uFileHandle);
	}
	closeFilesystem(NULL);
}


void fsImpl::openFile(const char* pszFilename,ULONG &uVfileHandle)
{
	if(!m_bFilesystemOpened)
	{
		throw IOExceptionImpl("Filesystem closed");
	}
	const char	*pszTmp=NULL;
	DWORD		dwFileIndex;
	UINT		i;
	if(m_pszDatafileName)
	{
		pszTmp=pszFilename;
		while((pszTmp!=pszFilename) &&
			(*pszTmp!='\\') &&
			(*pszTmp!='/'))
		{
			pszTmp--;
		}
		if((*pszTmp=='\\') || (*pszTmp=='/'))
		{
			pszTmp++;
		}
		dwFileIndex=0xffff;
		for(i=0;i<m_nNumFiles;i++)
		{
			if(!_stricmp(pszTmp,m_pFileTbl[i].filename))
			{
				dwFileIndex=i;
			}
		}
		if(dwFileIndex==0xffff)
		{
			PrintfBox("File %s not found",pszFilename);
			throw IOExceptionImpl("File not found");
		}
		for(i=0;i<MAX_LRERR_FS_FILES;i++)
		{
			if(!m_bIsOpen[i])
			{
				m_bIsOpen[i]=true;
				::fseek(m_pDatafileFileStruct,
					m_pFileTbl[dwFileIndex].dwOffset,SEEK_SET);
				if(m_pFileTbl[dwFileIndex].dwFlags & 1)
				{
					if(m_pFileTbl[dwFileIndex].dwSize>m_uBufSize)
					{
						m_pCompressInTempBuffer=new unsigned char[m_pFileTbl[dwFileIndex].dwSize];
						::fread(m_pCompressInTempBuffer,1,m_pFileTbl[dwFileIndex].dwSize,
							m_pDatafileFileStruct);
						if(m_pFileTbl[dwFileIndex].dwUncompressedSize>m_uBufSize)
						{
							m_bIsTemp[i]=TRUE;
							m_pFileTempBuffers[i]=
								new unsigned char[m_pFileTbl[dwFileIndex].dwUncompressedSize+1];
							ULONG	uTmp=m_pFileTbl[dwFileIndex].dwUncompressedSize;
							if(Z_OK!=uncompress(m_pFileTempBuffers[i],
								&uTmp,
								m_pCompressInTempBuffer,
								m_pFileTbl[dwFileIndex].dwSize))
							{
								throw IOExceptionImpl("Uncompress error");
							}
							m_pFilePositions[i]=m_pFileTempBuffers[i];
							m_pFileTempBuffers[i][m_pFileTbl[dwFileIndex].dwUncompressedSize]='\0';
						}
						else
						{	//this is VERY unlikely :)))
							m_bIsTemp[i]=FALSE;
							ULONG	uTmp=m_pFileTbl[dwFileIndex].dwUncompressedSize;
							if(Z_OK!=uncompress(m_pFileBuffers[i],
								&uTmp,
								m_pCompressInTempBuffer,
								m_pFileTbl[dwFileIndex].dwSize))
							{
								throw IOExceptionImpl("Uncompress error");
							}
							m_pFilePositions[i]=m_pFileBuffers[i];
							m_pFileBuffers[i][m_pFileTbl[dwFileIndex].dwUncompressedSize]='\0';
						}
						delete [] m_pCompressInTempBuffer;
						m_pCompressInTempBuffer=NULL;
					}
					else
					{
						::fread(m_pCompressInBuffer,1,m_pFileTbl[dwFileIndex].dwSize,
							m_pDatafileFileStruct);
						if(m_pFileTbl[dwFileIndex].dwUncompressedSize>m_uBufSize)
						{
							m_bIsTemp[i]=TRUE;
							m_pFileTempBuffers[i]=
								new unsigned char[m_pFileTbl[dwFileIndex].dwUncompressedSize+1];
							ULONG	uTmp=m_pFileTbl[dwFileIndex].dwUncompressedSize;
							if(Z_OK!=uncompress(m_pFileTempBuffers[i],
								&uTmp,
								m_pCompressInBuffer,
								m_pFileTbl[dwFileIndex].dwSize))
							{
								throw IOExceptionImpl("Uncompress error");
							}
							m_pFilePositions[i]=m_pFileTempBuffers[i];
							m_pFileTempBuffers[i][m_pFileTbl[dwFileIndex].dwUncompressedSize]='\0';
						}
						else
						{
							m_bIsTemp[i]=FALSE;
							ULONG	uTmp=m_pFileTbl[dwFileIndex].dwUncompressedSize;
							if(Z_OK!=uncompress(m_pFileBuffers[i],
								&uTmp,
								m_pCompressInBuffer,
								m_pFileTbl[dwFileIndex].dwSize))
							{
								throw IOExceptionImpl("Uncompress error");
							}
							m_pFilePositions[i]=m_pFileBuffers[i];
							m_pFileBuffers[i][m_pFileTbl[dwFileIndex].dwUncompressedSize]='\0';
						}
					}
					m_nNumOpenFiles++;
					m_pbDatafileCoverage[dwFileIndex]=TRUE;
					uVfileHandle=i;
					m_uFileSizes[i]=m_pFileTbl[dwFileIndex].dwUncompressedSize;
					if(m_pfnFileCallback)
					{
						m_pfnFileCallback(pszFilename);
					}
				}
				else
				{
					if(m_pFileTbl[dwFileIndex].dwUncompressedSize>m_uBufSize)
					{
						m_bIsTemp[i]=TRUE;
						m_pFileTempBuffers[i]=
							new unsigned char[m_pFileTbl[dwFileIndex].dwUncompressedSize+1];
						::fread(m_pFileTempBuffers[i],1,m_pFileTbl[dwFileIndex].dwUncompressedSize,
							m_pDatafileFileStruct);
						m_pFilePositions[i]=m_pFileTempBuffers[i];
						m_pFileTempBuffers[i][m_pFileTbl[dwFileIndex].dwUncompressedSize]='\0';
					}
					else
					{
						m_bIsTemp[i]=FALSE;
						::fread(m_pFileBuffers[i],1,m_pFileTbl[dwFileIndex].dwUncompressedSize,
							m_pDatafileFileStruct);
						m_pFilePositions[i]=m_pFileBuffers[i];
						m_pFileBuffers[i][m_pFileTbl[dwFileIndex].dwUncompressedSize]='\0';
					}
					m_nNumOpenFiles++;
					m_pbDatafileCoverage[dwFileIndex]=TRUE;
					uVfileHandle=i;
					m_uFileSizes[i]=m_pFileTbl[dwFileIndex].dwUncompressedSize;
					if(m_pfnFileCallback)
					{
						m_pfnFileCallback(pszFilename);
					}
				}
				return;
			}
		}
		throw IOExceptionImpl("Too many open files");
	}
	else
	{		//direct file mapping
		if(m_bUseRootDirectory)
		{
			WIN32_FILE_ATTRIBUTE_DATA	FileAttribs;
			LRString lstrFilename=m_lstrRootDirectory;
			lstrFilename+=pszFilename;
			if(!GetFileAttributesEx(lstrFilename.getPtr(),GetFileExInfoStandard,&FileAttribs))
			{
				PrintfBox("File %s not found",lstrFilename.getPtr());
				throw IOExceptionImpl("File not found");
			}
			for(i=0;i<MAX_LRERR_FS_FILES;i++)
			{
				if(!m_bIsOpen[i])
				{
					m_bIsOpen[i]=TRUE;
					FILE* f;
					f=fopen(lstrFilename.getPtr(),"rb");
					if(!f)
					{
						throw IOExceptionImpl("Unknown error");
					}
					if(FileAttribs.nFileSizeLow>m_uBufSize)
					{
						m_bIsTemp[i]=TRUE;
						m_pFileTempBuffers[i]=new unsigned char[FileAttribs.nFileSizeLow+1];
						::fread(m_pFileTempBuffers[i],1,FileAttribs.nFileSizeLow,f);
						m_pFilePositions[i]=m_pFileTempBuffers[i];
						m_pFileTempBuffers[i][FileAttribs.nFileSizeLow]='\0';
					}
					else
					{
						m_bIsTemp[i]=FALSE;
						::fread(m_pFileBuffers[i],1,FileAttribs.nFileSizeLow,f);
						m_pFilePositions[i]=m_pFileBuffers[i];
						m_pFileBuffers[i][FileAttribs.nFileSizeLow]='\0';
					}
					m_nNumOpenFiles++;
					uVfileHandle=i;
					m_uFileSizes[i]=FileAttribs.nFileSizeLow;
					if(m_pfnFileCallback)
					{
						m_pfnFileCallback(lstrFilename.getPtr());
					}
					fclose(f);
					return;
				}
			}
			throw IOExceptionImpl("Too many open files");
		}
		else
		{
			WIN32_FILE_ATTRIBUTE_DATA	FileAttribs;
			if(!GetFileAttributesEx(pszFilename,GetFileExInfoStandard,&FileAttribs))
			{
				PrintfBox("File %s not found",pszFilename);
				throw IOExceptionImpl("File not found");
			}
			for(i=0;i<MAX_LRERR_FS_FILES;i++)
			{
				if(!m_bIsOpen[i])
				{
					m_bIsOpen[i]=TRUE;
					FILE* f;
					f=fopen(pszFilename,"rb");
					if(!f)
					{
						throw IOExceptionImpl("Unknown error");
					}
					if(FileAttribs.nFileSizeLow>m_uBufSize)
					{
						m_bIsTemp[i]=TRUE;
						m_pFileTempBuffers[i]=new unsigned char[FileAttribs.nFileSizeLow+1];
						::fread(m_pFileTempBuffers[i],1,FileAttribs.nFileSizeLow,f);
						m_pFilePositions[i]=m_pFileTempBuffers[i];
						m_pFileTempBuffers[i][FileAttribs.nFileSizeLow]='\0';
					}
					else
					{
						m_bIsTemp[i]=FALSE;
						::fread(m_pFileBuffers[i],1,FileAttribs.nFileSizeLow,f);
						m_pFilePositions[i]=m_pFileBuffers[i];
						m_pFileBuffers[i][FileAttribs.nFileSizeLow]='\0';
					}
					m_nNumOpenFiles++;
					uVfileHandle=i;
					m_uFileSizes[i]=FileAttribs.nFileSizeLow;
					if(m_pfnFileCallback)
					{
						m_pfnFileCallback(pszFilename);
					}
					fclose(f);
					return;
				}
			}
			throw IOExceptionImpl("Too many open files");
		}
	}
}

void fsImpl::openFileAsPtr(const char* pszFilename,ULONG &uVfileHandle,
		unsigned char **ppMemBuf)
{
	openFile(pszFilename,uVfileHandle);
	*ppMemBuf=m_pFilePositions[uVfileHandle];
}


void fsImpl::closeFile(ULONG uVfileHandle)
{
	if(uVfileHandle>=MAX_LRERR_FS_FILES)
	{
		if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) && 
			(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)))
		{
			throw IOExceptionImpl("Invalid file handle");
		}
		if(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
		{
			throw IOExceptionImpl("Invalid file handle");
		}
		fclose(m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]);
		m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]=NULL;
		m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]=FALSE;
		m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]=NULL;
		return;
	}
	if(!m_bIsOpen[uVfileHandle])
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(m_bIsTemp[uVfileHandle])
	{
		delete m_pFileTempBuffers[uVfileHandle];
		m_pFileTempBuffers[uVfileHandle]=NULL;
	}
	m_pFilePositions[uVfileHandle]=NULL;
	m_bIsOpen[uVfileHandle]=NULL;
	m_nNumOpenFiles--;
	m_uFileSizes[uVfileHandle]=0;
}

ULONG fsImpl::fread(ULONG uVfileHandle,ULONG uNumBytes,void *pBuffer)
{
	if(uVfileHandle>=MAX_LRERR_FS_FILES)
	{
		return 0;		//TODO: should we return -1
	}
	if(!m_bIsOpen[uVfileHandle])
	{
		return 0;
	}
	if((uNumBytes==0) ||
		(pBuffer==NULL))
	{
		return 0;
	}

	if(m_bIsTemp[uVfileHandle])
	{
		if((m_pFilePositions[uVfileHandle]+uNumBytes)<=
			(m_pFileTempBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle]))
		{
			memcpy(pBuffer,m_pFilePositions[uVfileHandle],uNumBytes);
			m_pFilePositions[uVfileHandle]+=uNumBytes;
			return uNumBytes;
		}
		else
		{
			ULONG nRead;
			nRead=m_pFileTempBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
			if(nRead==0) return nRead;
			memcpy(pBuffer,m_pFilePositions[uVfileHandle],nRead);
			m_pFilePositions[uVfileHandle]+=nRead;
			return nRead;
		}
	}
	else
	{
		if((m_pFilePositions[uVfileHandle]+uNumBytes)<=
			(m_pFileBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle]))
		{
			memcpy(pBuffer,m_pFilePositions[uVfileHandle],uNumBytes);
			m_pFilePositions[uVfileHandle]+=uNumBytes;
			return uNumBytes;
		}
		else
		{
			ULONG nRead;
			nRead=m_pFileBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
			if(nRead==0) return nRead;
			memcpy(pBuffer,m_pFilePositions[uVfileHandle],nRead);
			m_pFilePositions[uVfileHandle]+=nRead;
			return nRead;
		}
	}
	return 0;
}

void fsImpl::fseek(ULONG uVfileHandle,LONG lOffset,ULONG uMode)
{
	if(uVfileHandle>=MAX_LRERR_FS_FILES)
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(!m_bIsOpen[uVfileHandle])
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(m_bIsTemp[uVfileHandle])
	{
		switch(uMode)
		{
			case SEEK_CUR:
				m_pFilePositions[uVfileHandle]+=lOffset;
				break;
			case SEEK_END:
				m_pFilePositions[uVfileHandle]=
					m_pFileTempBuffers[uVfileHandle]
					+m_uFileSizes[uVfileHandle]+lOffset;
				break;
			case SEEK_SET:
				m_pFilePositions[uVfileHandle]=
					m_pFileTempBuffers[uVfileHandle]+lOffset;
				break;
			default:
				throw IOExceptionImpl("Invalid call");
		}
		if(m_pFilePositions[uVfileHandle]<m_pFileTempBuffers[uVfileHandle])
		{
			m_pFilePositions[uVfileHandle]=m_pFileTempBuffers[uVfileHandle];
		}
		if(m_pFilePositions[uVfileHandle]>
			(m_pFileTempBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle]))
		{
			m_pFilePositions[uVfileHandle]=
				m_pFileTempBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle];
		}
	}
	else
	{
		switch(uMode)
		{
			case SEEK_CUR:
				m_pFilePositions[uVfileHandle]+=lOffset;
				break;
			case SEEK_END:
				m_pFilePositions[uVfileHandle]=
					m_pFileBuffers[uVfileHandle]
					+m_uFileSizes[uVfileHandle]+lOffset;
				break;
			case SEEK_SET:
				m_pFilePositions[uVfileHandle]=m_pFileBuffers[uVfileHandle]+lOffset;
				break;
			default:
				throw IOExceptionImpl("Invalid call");
		}
		if(m_pFilePositions[uVfileHandle]<m_pFileBuffers[uVfileHandle])
		{
			m_pFilePositions[uVfileHandle]=m_pFileBuffers[uVfileHandle];
		}
		if(m_pFilePositions[uVfileHandle]>
			(m_pFileBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle]))
		{
			m_pFilePositions[uVfileHandle]=
				m_pFileBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle];
		}
	}
}

static inline bool lr_isspace(int c)
{
	switch(c)
	{
		case ' ':
		case '\t':
		case '\n':
		case 13:
			return true;
			break;
		case 0:
			return true;
			break;
		default:
			return false;
	}
}

int fsImpl::scanInt(ULONG uVfileHandle)
{
	if((uVfileHandle>=MAX_LRERR_FS_FILES) || (!m_bIsOpen[uVfileHandle]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	ULONG nFree=0;
	if(m_bIsTemp[uVfileHandle])
	{
		nFree=m_pFileTempBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	else
	{
		nFree=m_pFileBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	if(nFree==0)
	{
		throw IOExceptionImpl("End of file reached");
	}
	int iOut=0;
	if(sscanf((const char*)m_pFilePositions[uVfileHandle]," %d ",&iOut)!=1)
	{
		throw IOExceptionImpl("sscanf failed");
	}
	char cTmp=0;
	bool bEndSeek=false;
	while(nFree && (!bEndSeek))
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		m_pFilePositions[uVfileHandle]++;
		nFree--;
		if(!lr_isspace(cTmp))
		{
			bEndSeek=true;
		}
	}
	bEndSeek=false;
	while(nFree && (!bEndSeek))
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		m_pFilePositions[uVfileHandle]++;
		nFree--;
		if(lr_isspace(cTmp))
		{
			bEndSeek=true;
		}
	}
	return iOut;
}

unsigned int fsImpl::scanUInt(ULONG uVfileHandle)
{
	if((uVfileHandle>=MAX_LRERR_FS_FILES) || (!m_bIsOpen[uVfileHandle]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	ULONG nFree=0;
	if(m_bIsTemp[uVfileHandle])
	{
		nFree=m_pFileTempBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	else
	{
		nFree=m_pFileBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	if(nFree==0)
	{
		throw IOExceptionImpl("End of file reached");
	}
	unsigned int uiOut=0;
	char cTmp=0;
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(!lr_isspace(cTmp))
		{
			break;
		}
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(lr_isspace(cTmp))
		{
			break;
		}
		if((cTmp<'0') || (cTmp>'9'))
		{
			uiOut=10*uiOut+(cTmp-'0');
		}
		else
		{
			uiOut=10*uiOut+(cTmp-'0');
		}
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	return uiOut;
}

long fsImpl::scanLong(ULONG uVfileHandle)
{
	if((uVfileHandle>=MAX_LRERR_FS_FILES) || (!m_bIsOpen[uVfileHandle]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	ULONG nFree=0;
	if(m_bIsTemp[uVfileHandle])
	{
		nFree=m_pFileTempBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	else
	{
		nFree=m_pFileBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	if(nFree==0)
	{
		throw IOExceptionImpl("End of file reached");
	}
	long lOut=0;
	char cTmp=0;
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(!lr_isspace(cTmp))
		{
			break;
		}
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	cTmp=*m_pFilePositions[uVfileHandle];
	bool bNegative=false;
	if(cTmp=='-')
	{
		bNegative=true;
		m_pFilePositions[uVfileHandle]++;
	}
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(lr_isspace(cTmp))
		{
			break;
		}
		if((cTmp<'0') || (cTmp>'9'))
		{
			lOut=10*lOut+(cTmp-'0');
		}
		else
		{
			lOut=10*lOut+(cTmp-'0');
		}
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	if(bNegative)
	{
		lOut=-lOut;
	}
	return lOut;
}

float fsImpl::scanFloat(ULONG uVfileHandle)
{
	if((uVfileHandle>=MAX_LRERR_FS_FILES) || (!m_bIsOpen[uVfileHandle]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	ULONG nFree=0;
	if(m_bIsTemp[uVfileHandle])
	{
		nFree=m_pFileTempBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	else
	{
		nFree=m_pFileBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	if(nFree==0)
	{
		throw IOExceptionImpl("End of file reached");
	}
	char szBuf[32];
	char *pOut=szBuf;
	char cTmp=0;
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(!lr_isspace(cTmp))
		{
			break;
		}
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(lr_isspace(cTmp))
		{
			break;
		}
		*pOut++=cTmp;
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	*pOut++=0;
	return boost::lexical_cast<float>(szBuf);
}

double fsImpl::scanDouble(ULONG uVfileHandle)
{
	if((uVfileHandle>=MAX_LRERR_FS_FILES) || (!m_bIsOpen[uVfileHandle]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	ULONG nFree=0;
	if(m_bIsTemp[uVfileHandle])
	{
		nFree=m_pFileTempBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	else
	{
		nFree=m_pFileBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	if(nFree==0)
	{
		throw IOExceptionImpl("End of file reached");
	}
	char szBuf[32];
	char *pOut=szBuf;
	char cTmp=0;
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(!lr_isspace(cTmp))
		{
			break;
		}
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(lr_isspace(cTmp))
		{
			break;
		}
		*pOut++=cTmp;
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	*pOut++=0;
	return boost::lexical_cast<double>(szBuf);
}

void fsImpl::scanString(ULONG uVfileHandle,char *pOut)
{
	if((uVfileHandle>=MAX_LRERR_FS_FILES) || (!m_bIsOpen[uVfileHandle]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	ULONG nFree=0;
	if(m_bIsTemp[uVfileHandle])
	{
		nFree=m_pFileTempBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	else
	{
		nFree=m_pFileBuffers[uVfileHandle]
				+m_uFileSizes[uVfileHandle]
				-m_pFilePositions[uVfileHandle];
	}
	if(nFree==0)
	{
		throw IOExceptionImpl("End of file reached");
	}
	char cTmp=0;
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(!lr_isspace(cTmp))
		{
			break;
		}
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	while(nFree)
	{
		cTmp=*m_pFilePositions[uVfileHandle];
		if(lr_isspace(cTmp))
		{
			break;
		}
		*pOut++=cTmp;
		m_pFilePositions[uVfileHandle]++;
		nFree--;
	}
	*pOut++=0;
}


bool fsImpl::eof(ULONG uVfileHandle)
{
	if(uVfileHandle>=MAX_LRERR_FS_FILES)
	{
		return true;
	}
	if(!m_bIsOpen[uVfileHandle])
	{
		return true;
	}
	if(m_bIsTemp[uVfileHandle])
	{
		if(m_pFilePositions[uVfileHandle]>=
			(m_pFileTempBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle]))
		{
			return true;
		}
	}
	else
	{
		if(m_pFilePositions[uVfileHandle]>=
			(m_pFileBuffers[uVfileHandle]+m_uFileSizes[uVfileHandle]))
		{
			return true;
		}
	}
	return false;
}

ULONG fsImpl::getFileSize(ULONG uVfileHandle)
{
	if(uVfileHandle>=MAX_LRERR_FS_FILES)
	{
		return 0;
	}
	if(!m_bIsOpen[uVfileHandle])
	{
		return 0;
	}
	return m_uFileSizes[uVfileHandle];
}

void fsImpl::readFileHeader(const char *pszFileName,DWORD dwHeaderSize,BYTE *pOutBuf)
{
	if(!m_bFilesystemOpened)
	{
		throw IOExceptionImpl("Filesystem is closed");
	}
	if((dwHeaderSize==0) || (!pszFileName))
	{
		throw IOExceptionImpl("Invalid call");
	}
	if(m_pszDatafileName)
	{
		const char	*pszTmp=NULL;
		pszTmp=pszFileName;
		while((pszTmp!=pszFileName) &&
			(*pszTmp!='\\') &&
			(*pszTmp!='/'))
		{
			pszTmp--;
		}
		if((*pszTmp=='\\') || (*pszTmp=='/'))
		{
			pszTmp++;
		}
		DWORD dwFileIndex=0xffff;
		ULONG i=0;
		for(i=0;i<m_nNumFiles;i++)
		{
			if(!_stricmp(pszTmp,m_pFileTbl[i].filename))
			{
				dwFileIndex=(DWORD)i;
			}
		}
		if(dwFileIndex==0xffff)
		{
			PrintfBox("File %s not found",pszFileName);
			throw IOExceptionImpl("File not found");
		}
		if(m_pFileTbl[dwFileIndex].dwUncompressedSize<dwHeaderSize)
		{
			throw IOExceptionImpl("End of file reached");
		}
		::fseek(m_pDatafileFileStruct,
			m_pFileTbl[dwFileIndex].dwOffset,SEEK_SET);
		if(m_pFileTbl[dwFileIndex].dwFlags & 1)
		{	//compressed - oops, our worst case
			DEBUG_STRING("LRCORE (WARNING): IfsImpl::readFileHeader - forced decompression of file!!!\n");
			m_pCompressInTempBuffer=new unsigned char[m_pFileTbl[dwFileIndex].dwSize];
			::fread(m_pCompressInTempBuffer,1,m_pFileTbl[dwFileIndex].dwSize,
			m_pDatafileFileStruct);
			unsigned char *pFileTempBuffer=
				new unsigned char[m_pFileTbl[dwFileIndex].dwUncompressedSize+1];
			ULONG	uTmp=m_pFileTbl[dwFileIndex].dwUncompressedSize;
			if(Z_OK!=uncompress(pFileTempBuffer,
				&uTmp,
				m_pCompressInTempBuffer,
				m_pFileTbl[dwFileIndex].dwSize))
			{
				throw IOExceptionImpl("Uncompress error");
			}
			memcpy(pOutBuf,pFileTempBuffer,dwHeaderSize);
			delete [] pFileTempBuffer;
			pFileTempBuffer=NULL;
			delete [] m_pCompressInTempBuffer;
			m_pCompressInTempBuffer=NULL;
		}
		else
		{
			::fread(pOutBuf,1,dwHeaderSize,m_pDatafileFileStruct);
		}
		return;
	}
	else
	{	//direct file mapping, piece of cake
		if(m_bUseRootDirectory)
		{
			WIN32_FILE_ATTRIBUTE_DATA	FileAttribs;
			LRString lstrFilename=m_lstrRootDirectory;
			lstrFilename+=pszFileName;
			if(!GetFileAttributesEx(lstrFilename.getPtr(),GetFileExInfoStandard,&FileAttribs))
			{
				PrintfBox("File %s not found",lstrFilename.getPtr());
				throw IOExceptionImpl("File not found");
			}
			if(dwHeaderSize>FileAttribs.nFileSizeLow)
			{
				throw IOExceptionImpl("End of file reached");
			}
			FILE *f=NULL;
			f=fopen(lstrFilename.getPtr(),"rb");
			if(f)
			{
				//read file
				::fread(pOutBuf,1,dwHeaderSize,f);
				fclose(f);
				f=NULL;
			}
			else
			{
				throw IOExceptionImpl("Unknown error");
			}
		}
		else
		{
			WIN32_FILE_ATTRIBUTE_DATA	FileAttribs;
			if(!GetFileAttributesEx(pszFileName,GetFileExInfoStandard,&FileAttribs))
			{
				PrintfBox("File %s not found",pszFileName);
				throw IOExceptionImpl("File not found");
			}
			if(dwHeaderSize>FileAttribs.nFileSizeLow)
			{
				throw IOExceptionImpl("End of file reached");
			}
			FILE *f=NULL;
			f=fopen(pszFileName,"rb");
			if(f)
			{
				//read file
				::fread(pOutBuf,1,dwHeaderSize,f);
				fclose(f);
				f=NULL;
			}
			else
			{
				throw IOExceptionImpl("Unknown error");
			}
		}
	}
}

void fsImpl::createTextFile(const char *pszFilename,ULONG &uVfileHandle)
{
	UINT i;
	for(i=0;i<MAX_LRERR_FS_FILES;i++)
	{
		if(!m_bIsWriteFileOpen[i]) break;
	}
	if(i==MAX_LRERR_FS_FILES)
	{
		throw IOExceptionImpl("Too many open files");
	}
	if(m_bUseRootDirectory)
	{
		LRString lstrFilename=m_lstrRootDirectory;
		lstrFilename+=pszFilename;
		m_pWriteFiles[i]=fopen(lstrFilename.getPtr(),"w");
	}
	else
	{
		m_pWriteFiles[i]=fopen(pszFilename,"w");
	}
	if(!m_pWriteFiles[i])
	{
		throw IOExceptionImpl("File not found");
	}
	m_bIsTextFile[i]=TRUE;
	m_bIsWriteFileOpen[i]=TRUE;
	uVfileHandle=BASE_LRERR_FS_WRITE_FILE + i;
}

void fsImpl::createBinaryFile(const char* pszFilename,ULONG &uVfileHandle)
{
	UINT i;
	for(i=0;i<MAX_LRERR_FS_FILES;i++)
	{
		if(!m_bIsWriteFileOpen[i])
		{
			break;
		}
	}
	if(i==MAX_LRERR_FS_FILES)
	{
		throw IOExceptionImpl("Too many open files");
	}
	if(m_bUseRootDirectory)
	{
		LRString lstrFilename=m_lstrRootDirectory;
		lstrFilename+=pszFilename;
		m_pWriteFiles[i]=fopen(lstrFilename.getPtr(),"wb");
	}
	else
	{
		m_pWriteFiles[i]=fopen(pszFilename,"wb");
	}
	if(!m_pWriteFiles[i])
	{
		throw IOExceptionImpl("File not found");
	}
	m_bIsTextFile[i]=FALSE;
	m_bIsWriteFileOpen[i]=TRUE;
	uVfileHandle=BASE_LRERR_FS_WRITE_FILE + i;
}

void fsImpl::writeInt(ULONG uVfileHandle,int iVal)
{
	if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) ||
		(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)) ||
		(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(!m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
	{
		throw IOExceptionImpl("Invalid call");
	}
	fprintf(m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE],"%d",iVal);
}

void fsImpl::writeUInt(ULONG uVfileHandle,unsigned int uiVal)
{
	if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) ||
		(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)) ||
		(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(!m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
	{
		throw IOExceptionImpl("Invalid call");
	}
	fprintf(m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE],"%d",uiVal);
}

void fsImpl::writeLong(ULONG uVfileHandle,long lVal)
{
	if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) ||
		(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)) ||
		(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(!m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
	{
		throw IOExceptionImpl("Invalid call");
	}
	fprintf(m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE],"%d",lVal);
}

void fsImpl::writeFloat(ULONG uVfileHandle,float fVal)
{
	if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) ||
		(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)) ||
		(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(!m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
	{
		throw IOExceptionImpl("Invalid call");
	}
	fprintf(m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE],"%f",fVal);
}

void fsImpl::writeDouble(ULONG uVfileHandle,double dVal)
{
	if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) ||
		(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)) ||
		(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(!m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
	{
		throw IOExceptionImpl("Invalid call");
	}
	fprintf(m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE],"%lf",dVal);
}

void fsImpl::writeString(ULONG uVfileHandle,const char *pszString)
{
	if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) ||
		(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)) ||
		(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(!m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
	{
		throw IOExceptionImpl("Invalid call");
	}
	fprintf(m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE],"%s",pszString);
}

void fsImpl::fwrite(ULONG uVfileHandle,const void *pszData,unsigned int uiByteCount)
{
	if((uVfileHandle<BASE_LRERR_FS_WRITE_FILE) ||
		(uVfileHandle>=(BASE_LRERR_FS_WRITE_FILE+MAX_LRERR_FS_FILES)) ||
		(!m_bIsWriteFileOpen[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]))
	{
		throw IOExceptionImpl("Invalid file handle");
	}
	if(m_bIsTextFile[uVfileHandle-BASE_LRERR_FS_WRITE_FILE])
	{
		throw IOExceptionImpl("Invalid call");
	}
	::fwrite(pszData,1,uiByteCount,m_pWriteFiles[uVfileHandle-BASE_LRERR_FS_WRITE_FILE]);
}
