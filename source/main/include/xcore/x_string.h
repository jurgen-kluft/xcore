#ifndef __XBASE_XSTRING_H__
#define __XBASE_XSTRING_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_string_utf.h"

/**
 * xCore namespace
 */
namespace xcore
{
	class x_va;
	class x_va_list;
	class x_iallocator;

	class xstring;			///< Heap based string
	class xcstring;			///< C style string (user supplied 'char*')
	class xccstring;		///< C style const string (user supplied 'const char*')
	class xstring_idata;	///< Base class for the string data

	class xstring_const_base
	{
	protected:
		friend class xstring_idata;
		xstring_idata*		mBuffer;

							xstring_const_base		()															{ }
							xstring_const_base		(xstring_idata* _data) : mBuffer(_data)						{ }
							xstring_const_base		(const xstring_const_base& _i);
							~xstring_const_base		();

	public:
		ulen8				len						(void) const;
		u32					size					(void) const;
		bool				isEmpty					(void) const;

		bool				isUpper                 (void) const;
		bool				isLower                 (void) const;
		bool				isCapitalized           (void) const;
		bool				isQuoted                (void) const;
		bool				isQuoted                (uchar8 inQuote) const;
		bool				isDelimited             (uchar8 inLeft, uchar8 inRight) const;

		uchar8				getAt					(ucpos8 inPos) const;
		uchar8				firstChar				(void) const;
		uchar8				lastChar				(void) const;

		bool				startsWith				(uchar8 inChar) const;
		bool				startsWith				(const ustr8* inStartStr) const;
		bool				endsWith				(uchar8 inChar) const;
		bool				endsWith				(const ustr8* inEndStr) const;

		///@name Comparison
		s32					compare					(const ustr8* inRHS, uclen8 inLen=uclen8()) const;							///< Return relationship between strings
		s32					compare					(const xstring& inRHS, uclen8 inLen=uclen8()) const;						///< Return relationship between strings
		s32					compare					(const xcstring& inRHS, uclen8 inLen=uclen8()) const;						///< Return relationship between strings
		s32					compare					(const xccstring& inRHS, uclen8 inLen=uclen8()) const;						///< Return relationship between strings

		s32					compareNoCase			(const ustr8* inRHS, uclen8 inLen=uclen8()) const;							///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xstring& inRHS, uclen8 inLen=uclen8()) const;						///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xcstring& inRHS, uclen8 inLen=uclen8()) const;						///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xccstring& inRHS, uclen8 inLen=uclen8()) const;						///< Return relationship between strings, not taking capitalization into account

		bool				isEqual					(const ustr8* inRHS, uclen8 inLen=uclen8()) const;							///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xstring& inRHS, uclen8 inLen=uclen8()) const;						///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xcstring& inRHS, uclen8 inLen=uclen8()) const;						///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xccstring& inRHS, uclen8 inLen=uclen8()) const;						///< Check if two strings are equal, taking capitalization into account

		bool				isEqualNoCase			(const ustr8* inRHS, uclen8 inLen=uclen8()) const;							///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xstring& inRHS, uclen8 inLen=uclen8()) const;						///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xcstring& inRHS, uclen8 inLen=uclen8()) const;						///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xccstring& inRHS, uclen8 inLen=uclen8()) const;						///< Check if two strings are equal, not taking capitalization into account
			
		///@name Search/replace
		upos8				find					(uchar8 inChar, ucpos8 inPos=ucpos8(), uclen8 inLen=uclen8()) const;		///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
		upos8				find					(const ustr8* inStr, ucpos8 inPos=ucpos8(), uclen8 inLen=uclen8()) const;	///< Return position of first occurrence of <inStr> after <inPos> or -1 if not found
		upos8				findNoCase				(uchar8 inChar, ucpos8 inPos=ucpos8(), uclen8 inLen=uclen8()) const;		///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
		upos8				findNoCase				(const ustr8* inStr, ucpos8 inPos=ucpos8(), uclen8 inLen=uclen8()) const;	///< Return position of first occurrence of <inStr> after <inPos> or -1 if not found

		upos8				rfind					(uchar8 inChar, ucpos8 inPos=ucpos8(), uclen8 inLen=uclen8()) const;		///< Return position of last occurrence of <inChar> on or before <inPos> or -1 if not found
		upos8				rfind					(const ustr8* inStr, ucpos8 inPos=ucpos8(), uclen8 inLen=uclen8()) const;	///< Return position of last occurrence of <inChar> on or before <inPos> or -1 if not found
		upos8				findOneOf				(const ustr8* inCharSet, ucpos8 inPos=ucpos8()) const;						///< Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found
		upos8				rfindOneOf				(const ustr8* inCharSet, ucpos8 inPos=ucpos8()) const;						///< Return position of last occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		upos8				indexOf					(uchar8 inChar, ucpos8 inPos=ucpos8()) const								{ return find(inChar, inPos); }
		upos8				indexOf					(const ustr8* inStr, ucpos8 inPos=ucpos8()) const							{ return find(inStr, inPos); }
		upos8				lastIndexOf				(uchar8 inChar, ucpos8 inPos=ucpos8()) const								{ return rfind(inChar, inPos); }
		upos8				lastIndexOf				(const ustr8* inStr, ucpos8 inPos=ucpos8()) const							{ return rfind(inStr, inPos); }

		bool				contains				(uchar8 inChar) const														{ return find(inChar).is_empty(); }								///< Check if this string contains character <inChar>
		bool				contains				(uchar8 inChar, ucpos8 inPos, uclen8 inLen=uclen8()) const					{ return find(inChar, ucpos8(), inLen).is_empty(); }			///< Check if this string contains character <inChar>
		bool				contains				(const ustr8* inStr) const													{ return find(inStr).is_empty(); }								///< Check if this string contains string <inString>
		bool				contains				(const ustr8* inStr, ucpos8 inPos, uclen8 inLen=uclen8()) const				{ return find(inStr, ucpos8(), inLen).is_empty(); }				///< Check if this string contains string <inString>
		bool				containsNoCase			(uchar8 inChar) const														{ return findNoCase(inChar).is_empty(); }						///< Check if this string contains character <inChar>
		bool				containsNoCase			(uchar8 inChar, ucpos8 inPos, uclen8 inLen = uclen8()) const				{ return findNoCase(inChar, ucpos8(), inLen).is_empty(); }		///< Check if this string contains character <inChar>
		bool				containsNoCase			(const ustr8* inStr) const													{ return findNoCase(inStr).is_empty(); }						///< Check if this string contains <inString> without paying attention to case
		bool				containsNoCase			(const ustr8* inStr, ucpos8 inPos, uclen8 inLen=uclen8()) const				{ return findNoCase(inStr, inPos, inLen).is_empty(); }			///< Check if this string contains <inString> without paying attention to case

		/// xstring versions
		void				left					(uclen8 inNum, xstring& outLeft) const;										///< Return the leftmost <inNum> characters of this string
		void				right					(uclen8 inNum, xstring& outRight) const;									///< Return the rightmost <inNum> characters of this string
		void				mid						(ucpos8 inPos, xstring& outMid, uclen8 inNum = uclen8()) const;				///< Return a string containing <inNum> characters from this string, starting at <inPos>
		xstring				mid						(ucpos8 inPos, uclen8 inNum = uclen8()) const;								///< Return a string containing <inNum> characters from this string, starting at <inPos>
		void				substring				(ucpos8 inPos, xstring& outSubstring, uclen8 inNum) const;
		void				substring				(ucpos8 inPos, xstring& outSubstring) const;
		xstring				substr					(ucpos8 inPos=ucpos8(), uclen8 inNum=uclen8()) const;

		bool				splitOn					(uchar8 inChar, xstring& outLeft, xstring& outRight) const;					///< Split string on first of occurrence of <ch>, returns result in <outLeft> and <outRight>
		bool				rsplitOn				(uchar8 inChar, xstring& outLeft, xstring& outRight) const;					///< Split string on last of occurrence of <ch>, returns result in <outLeft> and <outRight>
		void				split					(ucpos8 inPos, bool inRemove, xstring& outLeft, xstring& outRight) const;	///< Split string at <inPos>, return results in <outLeft> and <outRight>, if <inRemove> is true result doesn't include char at <inPos>

		/// xcstring versions
		void				left					(uclen8 inNum, xcstring& outLeft) const;									///< Return the leftmost <inNum> characters of this string
		void				right					(uclen8 inNum, xcstring& outRight) const;									///< Return the rightmost <inNum> characters of this string
		void				mid						(ucpos8 inPos, xcstring& outMid, uclen8 inNum = uclen8()) const;			///< Return a string containing <inNum> characters from this string, starting at <inPos>
		void				substring				(ucpos8 inPos, xcstring& outSubstring, uclen8 inNum) const;
		void				substring				(ucpos8 inPos, xcstring& outSubstring) const;

		bool				splitOn					(uchar8 inChar, xcstring& outLeft, xcstring& outRight) const;				///< Split string on first of occurrence of <ch>, returns result in <outLeft> and <outRight>
		bool				rsplitOn				(uchar8 inChar, xcstring& outLeft, xcstring& outRight) const;				///< Split string on last of occurrence of <ch>, returns result in <outLeft> and <outRight>
		void				split					(ucpos8 inPos, bool inRemove, xcstring& outLeft, xcstring& outRight) const;	///< Split string at <inPos>, return results in <outLeft> and <outRight>, if <inRemove> is true result doesn't include char at <inPos>

		ucptr8				cbegin					() const;
		ucptr8				cend					() const;
	};

	class xstring_mutable_base : public xstring_const_base
	{
	public:
		typedef				xstring_const_base		__const_base;

	protected:
		friend class xstring_idata;
		inline				xstring_mutable_base	(xstring_idata* _data) : __const_base(_data)							{ }
		inline				xstring_mutable_base	(const xstring_mutable_base& _r) : __const_base(_r)						{ }

	public:

		s32					max						(void) const;

		void				clear					(void);

		void				repeat					(const ustr8* inString, s32 inTimes);
		void				repeat					(const ustr8* inString, s32 inTimes, uclen8 inStringLength);
		void				repeat					(const xstring& inString, s32 inTimes);
		void				repeat					(const xcstring& inString, s32 inTimes);
		void				repeat					(const xccstring& inString, s32 inTimes);

		s32					format					(const ustr8* formatString, const x_va_list& args);
		s32					formatAdd				(const ustr8* formatString, const x_va_list& args);

		void				setAt					(ucpos8 inPos, uchar8 inChar);													///< Set character <inChar> at <inPos>
		void				setAt					(ucpos8 inPos, const ustr8* inString, uclen8 inNum);							///< Replace character at <inPos> with <inString>
		void				setAt					(ucpos8 inPos, uclen8 inLen, const ustr8* inString, uclen8 inNum=uclen8());		///< Replace <inNum> characters at <inPos> with <inString>

		void				replace					(ucpos8 inPos, const ustr8* inString, uclen8 inLen=uclen8());					///< Replace character at <inPos> with <inString>
		void				replace					(ucpos8 inPos, uclen8 inLen, const ustr8* inString, s32 inNum=-1);				///< Replace <inLength> characters at <inPos> with inNumChars characters of <inString>
		s32					replace					(const ustr8* inFind, const ustr8* inSubstitute, ucpos8 inPos=ucpos8());		///< Replace all occurrences of string <inFind> after <inPos> with string <inSubstitute>, returns amount of replacements made
		s32					replace					(uchar8 inFind, const ustr8* inSubstitute, ucpos8 inPos=ucpos8());				///< Replace all occurrences of character <inFind> after <inPos> with string <inSubstitute>, returns amount of replacements made
		s32					replace					(const ustr8* inFind, uchar8 inSubstitute, ucpos8 inPos=ucpos8());				///< Replace all occurrences of string <inFind> after <inPos> with char <inSubstitute>, returns amount of replacements made
		s32					replace					(uchar8 inFind, uchar8 inSubstitute, ucpos8 inPos=ucpos8());					///< Replace all occurrences of char <inFind> after <inPos> with char <inSubstitute>, returns amount of replacements made

		s32					replaceAnyWith			(const ustr8* inAny, uchar8 inWith, ucpos8 inFrom=ucpos8(), uclen8 inNum=uclen8());	///< Replace any character from <inAny> in the string with the <inWith> character

		void				insert					(uchar8 inChar);															///< Insert inChar at current position
		void				insert					(const ustr8* inString, uclen8 inNum=uclen8());								///< Insert inString starting at current position
		void				insert					(const xstring& inString);													///< Insert inString starting at current position
		void				insert					(const xcstring& inString);													///< Insert inString starting at current position
		void				insert					(const xccstring& inString);												///< Insert inString starting at current position

		void				insert					(ucpos8 inPos, uchar8 inChar);												///< Insert inChar at <inPos>
		void				insert					(ucpos8 inPos, const ustr8* inString, uclen8 inNum=uclen8());				///< Insert inString starting at <inPos>
		void				insert					(ucpos8 inPos, const xstring& inString);									///< Insert inString starting at <inPos>
		void				insert					(ucpos8 inPos, const xcstring& inString);									///< Insert inString starting at <inPos>
		void				insert					(ucpos8 inPos, const xccstring& inString);									///< Insert inString starting at <inPos>

		void				remove					(ucpos8 inStart, uclen8 inLength);											///< Remove <inLength> characters starting at <inStart>
		void				remove					(const ustr8* inCharSet);													///< Remove characters in <inCharSet> from string

		void				upper					(void);																		///< Uppercase all chars in string (e.g. "myWord" -> "MYWORD")
		void				lower					(void);																		///< Lowercase all chars in string (e.g. "myWord" -> "myword")
		void				capitalize				(void);																		///< Capitalize first char in string (e.g. "myWord" -> "Myword")
		void				capitalize				(const ustr8* inSeperators);												///< Capitalize first char in words (e.g. "my1stWord my2ndWord" -> "My1stword My2ndword")
		void				trim					(void);																		///< Trim whitespace from left and right side of string
		void				trimLeft				(void);																		///< Trim whitespace from left side of string
		void				trimRight				(void);																		///< Trim whitespace from right side of string
		void				trim					(uchar8 inChar);															///< Trim characters in <inCharSet> from left and right side of string
		void				trimLeft				(uchar8 inChar);															///< Trim character <inChar> from left side of string
		void				trimRight				(uchar8 inChar);															///< Trim character <inChar> from right side of string
		void				trim					(const ustr8* inCharSet);													///< Trim characters in <inCharSet> from left and right side of string
		void				trimLeft				(const ustr8* inCharSet);													///< Trim characters in <inCharSet> from left side of string
		void				trimRight				(const ustr8* inCharSet);													///< Trim characters in <inCharSet> from right side of string
		void				trimQuotes				(void);																		///< Trim double quotes from left and right side of string
		void				trimQuotes				(uchar8 quote);																///< Trim double quotes from left and right side of string
		void				trimDelimiters			(uchar8 inLeft, uchar8 inRight);											///< Trim delimiters from left and right side of string
		void				reverse					(void);																		///< Reverse characters in string
		void				reverse					(ucpos8 inPos, s32 inNum=-1);												///< Reverse characters in a part of this string

		bool				splitOn					(uchar8 inChar, xstring& outRight);											///< Split string on first occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				splitOn					(uchar8 inChar, xstring& outLeft, xstring& outRight) const;					///< Split string on first occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				rsplitOn				(uchar8 inChar, xstring& outRight);											///< Split string on last occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				rsplitOn				(uchar8 inChar, xstring& outLeft, xstring& outRight) const;					///< Split string on last occurrence of <ch>, moves all text after <ch> into <outRight>
		void				split					(ucpos8 inPos, bool inRemove, xstring& outRight);							///< Split string at <inPos>, move text at the right of inPos into <outRight>
		void				split					(ucpos8 inPos, bool inRemove, xstring& outLeft, xstring& outRight) const;	///< Split string at <inPos>, move text at the right of inPos into <outRight>

		void				copy					(const ustr8* str);
		void				copy					(const ustr8* str, uclen8 length);
		void				copy					(const xstring& str);
		void				copy					(const xcstring& str);
		void				copy					(const xccstring& str);

		void				indexToRowCol			(s32 index, s32& row, s32& col) const;

		uptr8				begin					();
		uptr8				end						();

	protected:
		void				concat					(const ustr8* inSource);												///< Concatenate <inSource> to this string
		void				concat					(const ustr8* inSource, ulen8 inLength);								///< Concatenate string with <inLength> characters from <inSource>
	};


	class xstring : public xstring_mutable_base
	{


	public:
		typedef				xstring_mutable_base	__base;


							xstring			(void);
							xstring			(ulen8 inLength);
							xstring			(const ustr8* inStr);
							xstring			(const ustr8* inStr, ulen8 length);
							xstring			(const ustr8* inStrA, ulen8 inStrLengthA, const ustr8* inStrB, ulen8 inStrLengthB);
							xstring			(const xstring& inStr);
		explicit			xstring			(const xcstring& inStr);
		explicit            xstring			(const xccstring& inStr);

		s32                 format			(const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 formatV			(const ustr8* inFormat, const x_va_list& Args);
		s32                 appendFormat	(const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 appendFormatV	(const ustr8* inFormat, const x_va_list& Args);

		xstring&			operator =		(const ustr8* inStr);								///< Copy a UTF-8 string to this string
		xstring&            operator =		(const xstring& inStr);
		xstring&            operator =		(const xcstring& inStr);
		xstring&            operator =		(const xccstring& inStr);

		xstring&			operator +=		(uchar8 inRHS);										///< Add a character to this string
		xstring&			operator +=		(const ustr8* inRHS);								///< Add a C-String to this string
		xstring&			operator +=		(const xstring& inRHS);								///< Add a String to this string
		xstring&			operator +=		(const xcstring& inRHS);
		xstring&			operator +=		(const xccstring& inRHS);
	};

	// When you want to control the memory that is used by instances of xstring you can do the following:
	// In your library declare a 'xstring::data {name of your instance}' somewhere, now you can:
	// 
	//      xstring mystring = [xfilesystem::]strings.construct("This is a string which will be copied in the dynamic string!");
	//
	// The memory however is now controlled by xfilesystem since it can initialize the xstring::data object with an allocator
	// that is managed by xfilesystem.
	// 
	// Of course you can still use an xstring that uses the system allocator directly:
	// 
	//      xstring my_string("This is a string which will be copied in the dynamic string!");
	//
	// 

	/// Mutable, Memory given by user
	class xcstring : public xstring_mutable_base
	{
	public:
		typedef				xstring_mutable_base	__base;



			         		xcstring		(void);
		inline         		xcstring		(xstring_idata* _data) : __base(_data)				{ }
							xcstring		(void* buffer, s32 bufferSize);
							xcstring		(void* buffer, s32 bufferSize, const ustr8* str);
							xcstring		(void* buffer, s32 bufferSize, const ustr8* formatString, const x_va_list& args);
							xcstring		(void* buffer, s32 bufferSize, const ustr8* formatString, const x_va& v1, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		explicit			xcstring		(void* buffer, s32 bufferSize, const xstring& str);
		explicit			xcstring		(void* buffer, s32 bufferSize, const xcstring& str);
		explicit			xcstring		(void* buffer, s32 bufferSize, const xccstring& str);

		s32					format			(const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32					formatV			(const ustr8* inFormat, const x_va_list& Args);
		s32					appendFormat	(const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32					appendFormatV	(const ustr8* inFormat, const x_va_list& Args);

		xcstring&			operator =		(uchar8 inRHS);										///< Copy a char to this string
		xcstring&			operator =		(const ustr8* inStr);								///< Copy a string to this string
		xcstring&			operator =		(const xstring& inStr);
		xcstring&			operator =		(const xcstring& inStr);
		xcstring&			operator =		(const xccstring& inStr);

		xcstring&			operator +=		(uchar8 inRHS);										///< Add a character to this string
		xcstring&			operator +=		(const ustr8* inRHS);								///< Add a string to this string
		xcstring&			operator +=		(const xstring& inStr);
		xcstring&			operator +=		(const xcstring& inRHS);							///< Add a C-String to this string
		xcstring&			operator +=		(const xccstring& inRHS);							///< Add a Const C-String to this string

		uptr8				begin			();
		uptr8				end				();
		ucptr8				begin			() const;
		ucptr8				end				() const;

	protected:        
		void				terminateWithZero (void);
	};

	/// Const
	class xccstring : public xstring_const_base
	{
	public:
							xccstring		(void);
		explicit			xccstring		(xstring_idata* _data) : xstring_const_base(_data)	{ }

		xccstring&			operator =		(const ustr8* str);									///< Assign a UTF-8 string to this string
	};

	#include "xcore\private\x_string_inline.h"

//==============================================================================
// END
//==============================================================================
//DOM-IGNORE-END

}

#endif	///< __XBASE_XSTRING_H__
