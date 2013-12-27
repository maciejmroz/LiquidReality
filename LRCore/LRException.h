/*******************************************************************************
 LiquidReality engine exception handling
 (c) 2001 Maciej Mróz
 *******************************************************************************/

#ifndef _LIQUID_REALITY_EXCEPTIONS_H_
#define _LIQUID_REALITY_EXCEPTIONS_H_

#include<windows.h>

//define all exception classes used together with base exception class
namespace LR {

	class LRCLASS Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS InvalidOperationException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS IndexOutOfBoundsException : public Exception {
	public:
		virtual const char* getDescription()=0;
		virtual int getMinimumIndex()=0;
		virtual int getMaximumIndex()=0;
		virtual int getUsedIndex()=0;
	};

	class LRCLASS InternalObjectCorruptionException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS InvalidParameterException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS ParseException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS IOException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS ClassNotFoundException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS FileFormatException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

	class LRCLASS ExpressionParsingException : public Exception {
	public:
		virtual const char* getDescription()=0;
	};

}

#endif