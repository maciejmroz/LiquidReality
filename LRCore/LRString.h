/*******************************************************************************
 LiquidReality engine utility string class
 (c) 2001 Maciej Mróz
 *******************************************************************************/

#ifndef _LIQUID_REALITY_STRING_H_
#define _LIQUID_REALITY_STRING_H_

class __declspec(dllexport) LRString {
	char	*m_pszString;
public:
	LRString();
	LRString(const char *pszString);
	LRString(const LRString &rfString);
	~LRString();
	void	operator=(const LRString &rfString);
	void	operator=(const char *pszString);
	void	operator+=(const LRString &rfString);
	void	operator+=(const char *pszString);
	int		operator<(const LRString &rfString);
	int		operator==(const LRString &rfString);
	void	format(const char* pszFormatString, ...);
	//use with care
	const char*	getPtr() const
	{
		return m_pszString;
	}

	int size() const;
};

#endif