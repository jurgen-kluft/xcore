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
	class xcstring;			///< const string
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
		u32					len						(void) const;
		u32					size					(void) const;
		bool				isEmpty					(void) const;

		bool				isUpper                 (void) const;
		bool				isLower                 (void) const;
		bool				isCapitalized           (void) const;
		bool				isQuoted                (void) const;
		bool				isQuoted                (uchar inQuote) const;
		bool				isDelimited             (uchar inLeft, uchar inRight) const;

		uchar				getAt					(u32 inPos) const;
		uchar				firstChar				(void) const;
		uchar				lastChar				(void) const;

		bool				startsWith				(uchar inChar) const;
		bool				startsWith				(const ustr* inStartStr) const;
		bool				endsWith				(uchar inChar) const;
		bool				endsWith				(const ustr* inEndStr) const;

		///@name Comparison
		s32					compare					(const ustr* inRHS, u32 inLen) const;					///< Return relationship between strings
		s32					compare					(const xstring& inRHS, u32 inLen) const;				///< Return relationship between strings
		s32					compare					(const xcstring& inRHS, u32 inLen) const;				///< Return relationship between strings
		s32					compare					(const xccstring& inRHS, u32 inLen) const;				///< Return relationship between strings

		s32					compareNoCase			(const ustr* inRHS, u32 inLen) const;					///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xstring& inRHS, u32 inLen) const;				///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xcstring& inRHS, u32 inLen) const;				///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xccstring& inRHS, u32 inLen) const;				///< Return relationship between strings, not taking capitalization into account

		bool				isEqual					(const ustr* inRHS, u32 inLen) const;					///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xstring& inRHS, u32 inLen) const;				///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xcstring& inRHS, u32 inLen) const;				///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xccstring& inRHS, u32 inLen) const;				///< Check if two strings are equal, taking capitalization into account

		bool				isEqualNoCase			(const ustr* inRHS, u32 inLen) const;					///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xstring& inRHS, u32 inLen) const;				///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xcstring& inRHS, u32 inLen) const;				///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xccstring& inRHS, u32 inLen) const;				///< Check if two strings are equal, not taking capitalization into account

		///@name Search/replace
		u32					find					(uchar inChar, u32 inPos, u32 inLen) const;				///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
		u32					find					(const ustr* inStr, u32 inPos, u32 inLen) const;		///< Return position of first occurrence of <inStr> after <inPos> or -1 if not found
		u32					findNoCase				(uchar inChar, u32 inPos, u32 inLen) const;				///< Return position of first occurrence of <inChar> after <inPos> or -1 if not found
		u32					findNoCase				(const ustr* inStr, u32 inPos, u32 inLen) const;		///< Return position of first occurrence of <inStr> after <inPos> or -1 if not found

		u32					rfind					(uchar inChar, u32 inPos, u32 inLen) const;				///< Return position of last occurrence of <inChar> on or before <inPos> or -1 if not found
		u32					rfind					(const ustr* inStr, u32 inPos, u32 inLen) const;		///< Return position of last occurrence of <inChar> on or before <inPos> or -1 if not found
		u32					findOneOf				(const ustr* inCharSet, u32 inPos) const;				///< Return position of first occurrence of a character in <inCharSet> after <inPos> or -1 if not found
		u32					rfindOneOf				(const ustr* inCharSet, u32 inPos) const;				///< Return position of last occurrence of a character in <inCharSet> after <inPos> or -1 if not found

		u32					indexOf					(uchar inChar, u32 inPos) const							{ return find(inChar, inPos); }
		u32					indexOf					(const ustr* inStr, u32 inPos) const					{ return find(inStr, inPos); }
		u32					lastIndexOf				(uchar inChar, u32 inPos) const							{ return rfind(inChar, inPos); }
		u32					lastIndexOf				(const ustr* inStr, u32 inPos) const					{ return rfind(inStr, inPos); }

		bool				contains				(uchar inChar) const									{ return find(inChar).is_empty(); }								///< Check if this string contains character <inChar>
		bool				contains				(uchar inChar, u32 inPos, u32 inLen) const				{ return find(inChar, inPos, inLen).is_empty(); }			///< Check if this string contains character <inChar>
		bool				contains				(const ustr* inStr) const								{ return find(inStr).is_empty(); }								///< Check if this string contains string <inString>
		bool				contains				(const ustr* inStr, u32 inPos, u32 inLen) const			{ return find(inStr, inPos, inLen).is_empty(); }				///< Check if this string contains string <inString>
		bool				containsNoCase			(uchar inChar) const									{ return findNoCase(inChar).is_empty(); }						///< Check if this string contains character <inChar>
		bool				containsNoCase			(uchar inChar, u32 inPos, u32 inLen) const				{ return findNoCase(inChar, inPos, inLen).is_empty(); }		///< Check if this string contains character <inChar>
		bool				containsNoCase			(const ustr* inStr) const								{ return findNoCase(inStr).is_empty(); }						///< Check if this string contains <inString> without paying attention to case
		bool				containsNoCase			(const ustr* inStr, u32 inPos, u32 inLen) const			{ return findNoCase(inStr, inPos, inLen).is_empty(); }			///< Check if this string contains <inString> without paying attention to case

		/// xstring versions
		void				left					(u32 inNum, xstring& outLeft) const;					///< Return the leftmost <inNum> characters of this string
		void				right					(u32 inNum, xstring& outRight) const;					///< Return the rightmost <inNum> characters of this string
		void				mid						(u32 inPos, xstring& outMid, u32 inNum) const;			///< Return a string containing <inNum> characters from this string, starting at <inPos>
		xstring				mid						(u32 inPos, u32 inNum) const;							///< Return a string containing <inNum> characters from this string, starting at <inPos>
		void				substring				(u32 inPos, xstring& outSubstring, u32 inNum) const;
		void				substring				(u32 inPos, xstring& outSubstring) const;
		xstring				substr					(u32 inPos, u32 inNum) const;

		bool				splitOn					(uchar inChar, xstring& outLeft, xstring& outRight) const;	///< Split string on first of occurrence of <ch>, returns result in <outLeft> and <outRight>
		bool				rsplitOn				(uchar inChar, xstring& outLeft, xstring& outRight) const;	///< Split string on last of occurrence of <ch>, returns result in <outLeft> and <outRight>
		void				split					(u32 inPos, xstring& outLeft, xstring& outRight) const;		///< Split string at <inPos>, return results in <outLeft> and <outRight>, if <inRemove> is true result doesn't include char at <inPos>

		/// xcstring versions
		void				left					(u32 inNum, xcstring& outLeft) const;					///< Return the leftmost <inNum> characters of this string
		void				right					(u32 inNum, xcstring& outRight) const;					///< Return the rightmost <inNum> characters of this string
		void				mid						(u32 inPos, xcstring& outMid, u32 inNum) const;			///< Return a string containing <inNum> characters from this string, starting at <inPos>
		void				substring				(u32 inPos, xcstring& outSubstring, u32 inNum) const;
		void				substring				(u32 inPos, xcstring& outSubstring) const;

		bool				splitOn					(uchar inChar, xcstring& outLeft, xcstring& outRight) const;	///< Split string on first of occurrence of <ch>, returns result in <outLeft> and <outRight>
		bool				rsplitOn				(uchar inChar, xcstring& outLeft, xcstring& outRight) const;	///< Split string on last of occurrence of <ch>, returns result in <outLeft> and <outRight>
		void				split					(u32 inPos, xcstring& outLeft, xcstring& outRight) const;		///< Split string at <inPos>, return results in <outLeft> and <outRight>, if <inRemove> is true result doesn't include char at <inPos>

		ustr const*			cbegin					() const;
		ustr const*			cend					() const;
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

		void				repeat					(const ustr* inString, s32 inTimes);
		void				repeat					(const ustr* inString, s32 inTimes, u32 inStringLength);
		void				repeat					(const xstring& inString, s32 inTimes);
		void				repeat					(const xcstring& inString, s32 inTimes);
		void				repeat					(const xccstring& inString, s32 inTimes);

		u32					format					(const ustr* formatString, const x_va_list& args);
		u32					formatAdd				(const ustr* formatString, const x_va_list& args);

		void				setAt					(u32 inPos, uchar inChar);										///< Set character <inChar> at <inPos>
		void				setAt					(u32 inPos, const ustr* inString, u32 inNum);					///< Replace character at <inPos> with <inString>
		void				setAt					(u32 inPos, u32 inLen, const ustr* inString, u32 inNum);		///< Replace <inNum> characters at <inPos> with <inString>

		void				replace					(u32 inPos, const ustr* inString, u32 inLen);					///< Replace character at <inPos> with <inString>
		void				replace					(u32 inPos, u32 inLen, const ustr* inString, u32 inNum);		///< Replace <inLength> characters at <inPos> with inNumChars characters of <inString>
		s32					replace					(const ustr* inFind, const ustr* inSubstitute, u32 inPos);	///< Replace all occurrences of string <inFind> after <inPos> with string <inSubstitute>, returns amount of replacements made
		s32					replace					(uchar inFind, const ustr* inSubstitute, u32 inPos);			///< Replace all occurrences of character <inFind> after <inPos> with string <inSubstitute>, returns amount of replacements made
		s32					replace					(const ustr* inFind, uchar inSubstitute, u32 inPos);			///< Replace all occurrences of string <inFind> after <inPos> with char <inSubstitute>, returns amount of replacements made
		s32					replace					(uchar inFind, uchar inSubstitute, u32 inPos);				///< Replace all occurrences of char <inFind> after <inPos> with char <inSubstitute>, returns amount of replacements made

		s32					replaceAnyWith			(const ustr* inAny, uchar inWith, u32 inFrom(), u32 inNum);	///< Replace any character from <inAny> in the string with the <inWith> character

		void				insert					(uchar inChar);													///< Insert inChar at current position
		void				insert					(const ustr* inString, u32 inNum);							///< Insert inString starting at current position
		void				insert					(const xstring& inString);										///< Insert inString starting at current position
		void				insert					(const xcstring& inString);										///< Insert inString starting at current position
		void				insert					(const xccstring& inString);									///< Insert inString starting at current position

		void				insert					(u32 inPos, uchar inChar);										///< Insert inChar at <inPos>
		void				insert					(u32 inPos, const ustr* inString, u32 inNum);					///< Insert inString starting at <inPos>
		void				insert					(u32 inPos, const xstring& inString);							///< Insert inString starting at <inPos>
		void				insert					(u32 inPos, const xcstring& inString);							///< Insert inString starting at <inPos>
		void				insert					(u32 inPos, const xccstring& inString);							///< Insert inString starting at <inPos>

		void				remove					(u32 inStart, u32 inLength);									///< Remove <inLength> characters starting at <inStart>
		void				remove					(const ustr* inCharSet);										///< Remove characters in <inCharSet> from string

		void				upper					(void);															///< Uppercase all chars in string (e.g. "myWord" -> "MYWORD")
		void				lower					(void);															///< Lowercase all chars in string (e.g. "myWord" -> "myword")
		void				capitalize				(void);															///< Capitalize first char in string (e.g. "myWord" -> "Myword")
		void				capitalize				(const ustr* inSeperators);										///< Capitalize first char in words (e.g. "my1stWord my2ndWord" -> "My1stword My2ndword")
		void				trim					(void);															///< Trim whitespace from left and right side of string
		void				trimLeft				(void);															///< Trim whitespace from left side of string
		void				trimRight				(void);															///< Trim whitespace from right side of string
		void				trim					(uchar inChar);													///< Trim characters in <inCharSet> from left and right side of string
		void				trimLeft				(uchar inChar);													///< Trim character <inChar> from left side of string
		void				trimRight				(uchar inChar);													///< Trim character <inChar> from right side of string
		void				trim					(const ustr* inCharSet);										///< Trim characters in <inCharSet> from left and right side of string
		void				trimLeft				(const ustr* inCharSet);										///< Trim characters in <inCharSet> from left side of string
		void				trimRight				(const ustr* inCharSet);										///< Trim characters in <inCharSet> from right side of string
		void				trimQuotes				(void);															///< Trim double quotes from left and right side of string
		void				trimQuotes				(uchar quote);													///< Trim double quotes from left and right side of string
		void				trimDelimiters			(uchar inLeft, uchar inRight);									///< Trim delimiters from left and right side of string
		void				reverse					(void);															///< Reverse characters in string
		void				reverse					(u32 inPos, s32 inNum=-1);										///< Reverse characters in a part of this string

		bool				splitOn					(uchar inChar, xstring& outRight);										///< Split string on first occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				splitOn					(uchar inChar, xstring& outLeft, xstring& outRight) const;				///< Split string on first occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				rsplitOn				(uchar inChar, xstring& outRight);										///< Split string on last occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				rsplitOn				(uchar inChar, xstring& outLeft, xstring& outRight) const;				///< Split string on last occurrence of <ch>, moves all text after <ch> into <outRight>
		void				split					(u32 inPos, bool inRemove, xstring& outRight);							///< Split string at <inPos>, move text at the right of inPos into <outRight>
		void				split					(u32 inPos, bool inRemove, xstring& outLeft, xstring& outRight) const;	///< Split string at <inPos>, move text at the right of inPos into <outRight>

		void				copy					(const ustr* str);
		void				copy					(const ustr* str, u32 length);
		void				copy					(const xstring& str);
		void				copy					(const xcstring& str);
		void				copy					(const xccstring& str);

		void				indexToRowCol			(s32 index, s32& row, s32& col) const;

		uptr8				begin					();
		uptr8				end						();

	protected:
		void				concat					(const ustr* inSource);												///< Concatenate <inSource> to this string
		void				concat					(const ustr* inSource, u32 inLength);								///< Concatenate string with <inLength> characters from <inSource>
	};


	class xstring : public xstring_mutable_base
	{


	public:
		typedef				xstring_mutable_base	__base;


							xstring			(void);
							xstring			(u32 inLength);
							xstring			(const ustr* inStr);
							xstring			(const ustr* inStr, u32 length);
							xstring			(const ustr* inStrA, u32 inStrLengthA, const ustr* inStrB, u32 inStrLengthB);
							xstring			(const xstring& inStr);
		explicit			xstring			(const xcstring& inStr);

		s32                 format			(const ustr* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 formatV			(const ustr* inFormat, const x_va_list& Args);
		s32                 appendFormat	(const ustr* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 appendFormatV	(const ustr* inFormat, const x_va_list& Args);

		xstring&			operator =		(const ustr* inStr);								///< Copy a UTF-8 string to this string
		xstring&            operator =		(const xstring& inStr);
		xstring&            operator =		(const xcstring& inStr);

		xstring&			operator +=		(uchar inRHS);										///< Add a character to this string
		xstring&			operator +=		(const ustr* inRHS);								///< Add a C-String to this string
		xstring&			operator +=		(const xstring& inRHS);								///< Add a String to this string
		xstring&			operator +=		(const xcstring& inRHS);
	};


	// When you want to control the memory that is used by instances of xstring you can do the following:
	// In your library (e.g. xfilesystem) declare a 'xstring::data strings' somewhere, now you can:
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

	/// Const
	class xcstring : public xstring_const_base
	{
	public:
							xcstring		(void);
		explicit			xcstring		(xstring_idata* _data) : xstring_const_base(_data)	{ }

		xccstring&			operator =		(const ustr* str);					///< Assign a UTF-8 string to this string
	};

	#include "xcore\private\x_string_inline.h"

//==============================================================================
// END
//==============================================================================
//DOM-IGNORE-END

}

#endif	///< __XBASE_XSTRING_H__
