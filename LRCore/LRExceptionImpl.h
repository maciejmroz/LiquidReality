
#ifndef _LR_EXCEPTION_IMPL_H_
#define _LR_EXCEPTION_IMPL_H_

#include "LRException.h"
#include "LRString.h"

namespace LR
{

	class ExceptionImpl : public Exception
	{
		LRString		m_Description;
	public:
		//constructors
		ExceptionImpl()
		{
			m_Description="Exception: (no info)";
		}

		ExceptionImpl(const char *pszMessage)
		{
			m_Description="Exception:";
			m_Description+=pszMessage;
		}

		//copying constructor
		ExceptionImpl(const ExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		ExceptionImpl& operator=(const ExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class InvalidOperationExceptionImpl : public InvalidOperationException
	{
		LRString		m_Description;
	public:
		//constructors
		InvalidOperationExceptionImpl()
		{
			m_Description="InvalidOperationException: (no info)";
		}

		InvalidOperationExceptionImpl(const char *pszMessage) {
			m_Description="InvalidOperationException:";
			m_Description+=pszMessage;
		}

		//copying constructor
		InvalidOperationExceptionImpl(const InvalidOperationExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		InvalidOperationExceptionImpl& operator=(const InvalidOperationExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class IndexOutOfBoundsExceptionImpl : public IndexOutOfBoundsException
	{
		LRString		m_Description;
		int				m_iMin;
		int				m_iMax;
		int				m_iUsed;
	public:
		//constructors
		IndexOutOfBoundsExceptionImpl(int iMin,int iMax,int iUsed) :
			m_iMin(iMin),
			m_iMax(iMax),
			m_iUsed(iUsed)
		{
			m_Description.format("IndexOutOfBoundException: minimum %d, maximum %d, used %d",
				m_iMin,m_iMax,m_iUsed);
		}

		IndexOutOfBoundsExceptionImpl(const char *pszMessage,int iMin,int iMax,int iUsed) :
			m_iMin(iMin),
			m_iMax(iMax),
			m_iUsed(iUsed)
		{
			m_Description.format("IndexOutOfBoundException: %s ;minimum %d, maximum %d, used %d",
				pszMessage,m_iMin,m_iMax,m_iUsed);
		}

		//copying constructor
		IndexOutOfBoundsExceptionImpl(const IndexOutOfBoundsExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			m_iMax=rfException.m_iMax;
			m_iMin=rfException.m_iMin;
			m_iUsed=rfException.m_iUsed;
		}

		//assignment
		IndexOutOfBoundsExceptionImpl& operator=(const IndexOutOfBoundsExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			m_iMax=rfException.m_iMax;
			m_iMin=rfException.m_iMin;
			m_iUsed=rfException.m_iUsed;
			return *this;
		}
		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}

		int getMinimumIndex()
		{
			return m_iMin;
		}

		int getMaximumIndex()
		{
			return m_iMax;
		}

		int getUsedIndex()
		{
			return m_iUsed;
		}
	};

	class InternalObjectCorruptionExceptionImpl : public InternalObjectCorruptionException
	{
		LRString		m_Description;
	public:
		//constructors
		InternalObjectCorruptionExceptionImpl()
		{
			m_Description="InternalObjectCorruptionException: (no info)";
		}

		InternalObjectCorruptionExceptionImpl(const char *pszMessage)
		{
			m_Description="InternalObjectCorruptionException:";
			m_Description+=pszMessage;
		}

		//copying constructor
		InternalObjectCorruptionExceptionImpl(const InternalObjectCorruptionExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		InternalObjectCorruptionExceptionImpl& operator=(const InternalObjectCorruptionExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class InvalidParameterExceptionImpl : public InvalidParameterException
	{
		LRString		m_Description;
	public:
		//constructors
		InvalidParameterExceptionImpl()
		{
			m_Description="InvalidParameterException: (no info)";
		}

		InvalidParameterExceptionImpl(const char *pszMessage)
		{
			m_Description="InvalidParameterException:";
			m_Description+=pszMessage;
		}

		//copying constructor
		InvalidParameterExceptionImpl(const InvalidParameterExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		InvalidParameterExceptionImpl& operator=(const InvalidParameterExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class ParseExceptionImpl : public ParseException
	{
		LRString		m_Description;
	public:
		//constructors
		ParseExceptionImpl()
		{
			m_Description="ParseException: (no info)";
		}

		ParseExceptionImpl(const char *pszMessage)
		{
			m_Description="ParseException:";
			m_Description+=pszMessage;
		}

		//copying constructor
		ParseExceptionImpl(const ParseExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		ParseExceptionImpl& operator=(const ParseExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class IOExceptionImpl : public IOException
	{
		LRString		m_Description;
	public:
		//constructors
		IOExceptionImpl()
		{
			m_Description="IOException: (no info)";
		}

		IOExceptionImpl(const char *pszMessage)
		{
			m_Description="IOException:";
			m_Description+=pszMessage;
		}

		//copying constructor
		IOExceptionImpl(const IOExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		IOExceptionImpl& operator=(const IOExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class ClassNotFoundExceptionImpl : public ClassNotFoundException
	{
		LRString		m_Description;
	public:
		//constructors
		ClassNotFoundExceptionImpl()
		{
			m_Description="ClassNotFoundException: (no info)";
		}

		ClassNotFoundExceptionImpl(const char *pszClass,const char *pszSuperclass)
		{
			m_Description="ClassNotFoundException: class: ";
			m_Description+=pszClass;
			m_Description+=", superclass: ";
			m_Description+=pszSuperclass;
		}

		//copying constructor
		ClassNotFoundExceptionImpl(const ClassNotFoundExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		ClassNotFoundExceptionImpl& operator=(const ClassNotFoundExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class FileFormatExceptionImpl : public FileFormatException
	{
		LRString		m_Description;
	public:
		//constructors
		FileFormatExceptionImpl()
		{
			m_Description="FileFormatException: (no info)";
		}

		FileFormatExceptionImpl(const char *pszMessage)
		{
			m_Description="FileFormatException:";
			m_Description+=pszMessage;
		}

		//copying constructor
		FileFormatExceptionImpl(const FileFormatExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		FileFormatExceptionImpl& operator=(const FileFormatExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

	class ExpressionParsingExceptionImpl : public ExpressionParsingException
	{
		LRString		m_Description;
	public:
		//constructors
		ExpressionParsingExceptionImpl()
		{
			m_Description="ExpressionParsingException: (no info)";
		}

		ExpressionParsingExceptionImpl(const char *pszMessage)
		{
			m_Description="ExpressionParsingException:";
			m_Description+=pszMessage;
		}

		//copying constructor
		ExpressionParsingExceptionImpl(const ExpressionParsingExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
		}

		//assignment
		ExpressionParsingExceptionImpl& operator=(const ExpressionParsingExceptionImpl &rfException)
		{
			m_Description=rfException.m_Description;
			return *this;
		}

		//methods
		const char *getDescription()
		{
			return m_Description.getPtr();
		}
	};

}

#endif