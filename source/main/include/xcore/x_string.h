#ifndef __XBASE_XSTRING_H__
#define __XBASE_XSTRING_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_string_utf.h"

#include "xcore\private\x_string_buffers.h"

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

	class xstring_base
	{
	protected:
		class idata
		{
		public:
			virtual ulen8			getLength() const = 0;
			virtual u32				getReserved() const = 0;
			virtual void			setLength(ulen8) = 0;

			virtual bool			isEmpty() const = 0;
			virtual void			setEmpty() = 0;

			virtual uptr8			getPtr() = 0;
			virtual ucptr8			getCPtr() const = 0;

			virtual bool			isReadonly() const = 0;
			virtual bool			isGrowable() const = 0;

			virtual void			copyFrom(char const* _str, u32 _len) = 0;
			virtual void			copyFrom(ustr8 const* _str, ulen8 _len) = 0;

			virtual void			terminateWithZero() = 0;

			// Reference counting interface
			virtual void			bind() = 0;
			virtual void			unbind() = 0;

			// Dynamic buffer interface
			virtual void			resize(u32) = 0;
			virtual void			unique() = 0;
		};
	};

	class xstring_const_base : public xstring_base
	{
	protected:
		idata*				mBuffer;

							xstring_const_base		()															{ }
							xstring_const_base		(idata* _data) : mBuffer(_data)								{ }
							xstring_const_base		(const xstring_const_base& _i) : mBuffer(_i.mBuffer)		{ mBuffer->bind(); }
							~xstring_const_base		()															{ mBuffer->unbind(); }

	public:
		ulen8				len						(void) const;
		u32					size					(void) const;
		bool				isEmpty					(void) const;

		bool				isUpper                 (void) const;
		bool				isLower                 (void) const;
		bool				isCapitalized           (void) const;
		bool				isQuoted                (void) const;
		bool				isQuoted                (char inQuote) const;
		bool				isDelimited             (char inLeft, char inRight) const;
		bool				isQuoted                (uchar8 inQuote) const;
		bool				isDelimited             (uchar8 inLeft, uchar8 inRight) const;

		uchar8				getAt					(s32 inPosition) const;
		uchar8				firstChar				(void) const;
		uchar8				lastChar				(void) const;

		bool				startsWith				(char inChar) const;
		bool				startsWith				(uchar8 inChar) const;
		bool				startsWith				(const char* inStartStr) const;
		bool				startsWith				(const ustr8* inStartStr) const;
		bool				endsWith				(char inChar) const;
		bool				endsWith				(uchar8 inChar) const;
		bool				endsWith				(const char* inEndStr) const;
		bool				endsWith				(const ustr8* inEndStr) const;

		///@name Comparison
		s32					compare					(const char* inRHS, s32 inCharNum = -1) const;				///< Return relationship between strings
		s32					compare					(const ustr8* inRHS, s32 inCharNum = -1) const;				///< Return relationship between strings
		s32					compare					(const xstring& inRHS, s32 inCharNum = -1) const;			///< Return relationship between strings
		s32					compare					(const xcstring& inRHS, s32 inCharNum = -1) const;			///< Return relationship between strings
		s32					compare					(const xccstring& inRHS, s32 inCharNum = -1) const;			///< Return relationship between strings

		s32					compareNoCase			(const char* inRHS, s32 inCharNum = -1) const;				///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const ustr8* inRHS, s32 inCharNum = -1) const;				///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xstring& inRHS, s32 inCharNum = -1) const;			///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xcstring& inRHS, s32 inCharNum = -1) const;			///< Return relationship between strings, not taking capitalization into account
		s32					compareNoCase			(const xccstring& inRHS, s32 inCharNum = -1) const;			///< Return relationship between strings, not taking capitalization into account

		bool				isEqual					(const char* inRHS, s32 inCharNum = -1) const;				///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const ustr8* inRHS, s32 inCharNum = -1) const;				///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xstring& inRHS, s32 inCharNum = -1) const;			///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xcstring& inRHS, s32 inCharNum = -1) const;			///< Check if two strings are equal, taking capitalization into account
		bool				isEqual					(const xccstring& inRHS, s32 inCharNum = -1) const;			///< Check if two strings are equal, taking capitalization into account
		
		bool				isEqualNoCase			(const char* inRHS, s32 inCharNum = -1) const;				///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const uchar8* inRHS, s32 inCharNum = -1) const;			///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xstring& inRHS, s32 inCharNum = -1) const;			///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xcstring& inRHS, s32 inCharNum = -1) const;			///< Check if two strings are equal, not taking capitalization into account
		bool				isEqualNoCase			(const xccstring& inRHS, s32 inCharNum = -1) const;			///< Check if two strings are equal, not taking capitalization into account
			
		///@name Search/replace
		upos8				find					(uchar8 inChar, s32 inPos=0, s32 inLen=-1) const;			///< Return position of first occurrence of <inChar> after <inPosition> or -1 if not found
		upos8				find					(const ustr8* inStr, s32 inPos=0, s32 inLen=-1) const;		///< Return position of first occurrence of <inStr> after <inPosition> or -1 if not found
		upos8				findNoCase				(uchar8 inChar, s32 inPos=0, s32 inLen=-1) const;			///< Return position of first occurrence of <inChar> after <inPosition> or -1 if not found
		upos8				findNoCase				(const ustr8* inStr, s32 inPos=0, s32 inLen=-1) const;		///< Return position of first occurrence of <inStr> after <inPosition> or -1 if not found

		upos8				find					(char inChar, s32 inPos=0, s32 inLen=-1) const				{ return find(uchar8(inChar), inPos, inLen); }
		upos8				find					(const char* inStr, s32 inPos=0, s32 inLen=-1) const		{ return find((const ustr8*)inStr, inPos, inLen); }
		upos8				findNoCase				(char inChar, s32 inPos=0, s32 inLen=-1) const				{ return findNoCase(uchar8(inChar), inPos, inLen); }
		upos8				findNoCase				(const char* inStr, s32 inPos=0, s32 inLen=-1) const		{ return findNoCase((const ustr8*)inStr, inPos, inLen); }

		upos8				rfind					(uchar8 inChar, s32 inPos=-1, s32 inLen=-1) const;			///< Return position of last occurrence of <inChar> on or before <inPosition> or -1 if not found
		upos8				rfind					(const ustr8* inStr, s32 inPos=-1, s32 inLen=-1) const;		///< Return position of last occurrence of <inChar> on or before <inPosition> or -1 if not found
		upos8				findOneOf				(const ustr8* inCharSet, s32 inPos = 0) const;				///< Return position of first occurrence of a character in <inCharSet> after <inPosition> or -1 if not found
		upos8				rfindOneOf				(const ustr8* inCharSet, s32 inPos = -1) const;				///< Return position of last occurrence of a character in <inCharSet> after <inPosition> or -1 if not found

		upos8				rfind					(char inChar, s32 inPos=-1, s32 inLen=-1) const				{ return rfind(uchar8(inChar), inPos, inLen); }
		upos8				rfind					(const char* inStr, s32 inPos=-1, s32 inLen=-1) const		{ return rfind((const ustr8*)inStr, inPos, inLen); }
		upos8				findOneOf				(const char* inCharSet, s32 inPos = 0) const				{ return findOneOf((const ustr8*)inCharSet, inPos); }
		upos8				rfindOneOf				(const char* inCharSet, s32 inPos = -1) const				{ return rfindOneOf((const ustr8*)inCharSet, inPos); }

		upos8				indexOf					(char inChar, s32 inPosition = 0) const						{ return find(inChar, inPosition); }
		upos8				indexOf					(const char* inStr, s32 inPosition = 0) const				{ return find(inStr, inPosition); }
		upos8				lastIndexOf				(char inChar, s32 inPosition = -1) const					{ return rfind(inChar, inPosition); }
		upos8				lastIndexOf				(const char* inStr, s32 inPosition = -1) const				{ return rfind(inStr, inPosition); }

		upos8				indexOf					(uchar8 inChar, s32 inPosition = 0) const					{ return find(inChar, inPosition); }
		upos8				indexOf					(const ustr8* inStr, s32 inPosition = 0) const				{ return find(inStr, inPosition); }
		upos8				lastIndexOf				(uchar8 inChar, s32 inPosition = -1) const					{ return rfind(inChar, inPosition); }
		upos8				lastIndexOf				(const ustr8* inStr, s32 inPosition = -1) const				{ return rfind(inStr, inPosition); }

		bool				contains				(char inChar) const											{ return !find(inChar).is_empty(); }					///< Check if this string contains character <inChar>
		bool				contains				(char inChar, s32 inPos, s32 inLen=-1) const				{ return !find(inChar, 0, inLen).is_empty(); }			///< Check if this string contains character <inChar>
		bool				contains				(const char* inStr) const									{ return !find(inStr).is_empty(); }						///< Check if this string contains string <inString>
		bool				contains				(const char* inStr, s32 inPos, s32 inLen=-1) const			{ return !find(inStr, 0, inLen).is_empty(); }			///< Check if this string contains string <inString>
		bool				containsNoCase			(const char* inStr) const									{ return !findNoCase(inStr).is_empty(); }				///< Check if this string contains <inString> without paying attention to case
		bool				containsNoCase			(const char* inStr, s32 inPos, s32 inLen=-1) const			{ return !findNoCase(inStr, inPos, inLen).is_empty(); } ///< Check if this string contains <inString> without paying attention to case

		bool				contains				(uchar8 inChar) const										{ return find(inChar).is_empty(); }						///< Check if this string contains character <inChar>
		bool				contains				(uchar8 inChar, s32 inPos, s32 inLen=-1) const				{ return find(inChar, 0, inLen).is_empty(); }			///< Check if this string contains character <inChar>
		bool				contains				(const ustr8* inStr) const									{ return find(inStr).is_empty(); }						///< Check if this string contains string <inString>
		bool				contains				(const ustr8* inStr, s32 inPos, s32 inLen=-1) const			{ return find(inStr, 0, inLen).is_empty(); }			///< Check if this string contains string <inString>
		bool				containsNoCase			(const ustr8* inStr) const									{ return findNoCase(inStr).is_empty(); }				///< Check if this string contains <inString> without paying attention to case
		bool				containsNoCase			(const ustr8* inStr, s32 inPos, s32 inLen=-1) const			{ return findNoCase(inStr, inPos, inLen).is_empty(); }	///< Check if this string contains <inString> without paying attention to case

		/// xstring versions
		void				left					(s32 inNum, xstring& outLeft) const;						///< Return the leftmost <inNum> characters of this string
		void				right					(s32 inNum, xstring& outRight) const;						///< Return the rightmost <inNum> characters of this string
		void				mid						(u32 inPosition, xstring& outMid, s32 inNum = -1) const;	///< Return a string containing <inNum> characters from this string, starting at <inPosition>
		xstring				mid						(u32 inPosition, s32 inNum = -1) const;						///< Return a string containing <inNum> characters from this string, starting at <inPosition>
		void				substring				(u32 inPosition, xstring& outSubstring, s32 inNum) const;
		void				substring				(u32 inPosition, xstring& outSubstring) const;
		xstring				substr					(u32 inPosition=0, s32 inNum=-1) const;

		bool				splitOn					(const char inChar, xstring& outLeft, xstring& outRight) const;	///< Split string on first of occurrence of <ch>, returns result in <outLeft> and <outRight>
		bool				rsplitOn				(const char inChar, xstring& outLeft, xstring& outRight) const;	///< Split string on last of occurrence of <ch>, returns result in <outLeft> and <outRight>
		void				split					(u32 inPosition, bool inRemove, xstring& outLeft, xstring& outRight) const;	///< Split string at <inPosition>, return results in <outLeft> and <outRight>, if <inRemove> is true result doesn't include tchar at <inPosition>

		/// xcstring versions
		void				left					(s32 inNum, xcstring& outLeft) const;						///< Return the leftmost <inNum> characters of this string
		void				right					(s32 inNum, xcstring& outRight) const;						///< Return the rightmost <inNum> characters of this string
		void				mid						(u32 inPosition, xcstring& outMid, s32 inNum = -1) const;	///< Return a string containing <inNum> characters from this string, starting at <inPosition>
		void				substring				(u32 inPosition, xcstring& outSubstring, s32 inNum) const;
		void				substring				(u32 inPosition, xcstring& outSubstring) const;

		bool				splitOn					(const char inChar, xcstring& outLeft, xcstring& outRight) const;		///< Split string on first of occurrence of <ch>, returns result in <outLeft> and <outRight>
		bool				rsplitOn				(const char inChar, xcstring& outLeft, xcstring& outRight) const;		///< Split string on last of occurrence of <ch>, returns result in <outLeft> and <outRight>
		void				split					(u32 inPosition, bool inRemove, xcstring& outLeft, xcstring& outRight) const;	///< Split string at <inPosition>, return results in <outLeft> and <outRight>, if <inRemove> is true result doesn't include tchar at <inPosition>

		inline const ustr8*	c_str					(void) const 															{ return mBuffer->getPtr().str(); }
	};

	class xstring_mutable_base : public xstring_const_base
	{
	public:
		typedef				xstring_const_base		__const_base;

	protected:
							xstring_mutable_base	(void);
							xstring_mutable_base	(idata* _data) : __const_base(_data)									{ }
							xstring_mutable_base	(u32 len);
							xstring_mutable_base	(const char* str);
							xstring_mutable_base	(u32 len, const char* str);
							xstring_mutable_base	(u32 lenA, const char* strA, u32 lenB, const char* strB);
							xstring_mutable_base	(const char* formatString, const x_va_list& args);
							xstring_mutable_base	(const char* formatString, const x_va& v1, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);

							xstring_mutable_base	(const xstring& str);
							xstring_mutable_base	(const xcstring& str);
							xstring_mutable_base	(const xccstring& str);

	public:

		s32					max						(void) const															{ return mBuffer->getReserved(); }

		void				clear					(void);

		void				repeat					(const char* inString, s32 inTimes);
		void				repeat					(const char* inString, s32 inTimes, s32 inStringLength);
		void				repeat					(const ustr8* inString, s32 inTimes);
		void				repeat					(const ustr8* inString, s32 inTimes, s32 inStringLength);
		void				repeat					(const xstring& inString, s32 inTimes);
		void				repeat					(const xcstring& inString, s32 inTimes);
		void				repeat					(const xccstring& inString, s32 inTimes);

		s32					format					(const char* formatString, const x_va_list& args);
		s32					formatAdd				(const char* formatString, const x_va_list& args);

		void				setAt					(u32 inPosition, char inChar);											///< Set character <inChar> at <inPosition>
		void				setAt					(u32 inPosition, const char* inString, s32 inNum);						///< Replace character at <inPosition> with <inString>
		void				setAt					(u32 inPosition, s32 inLen, const char* inString, s32 inNum=-1);		///< Replace <inNum> characters at <inPosition> with <inString>

		void				replace					(u32 inPos, const char* inString, s32 inLen=-1);						///< Replace character at <inPosition> with <inString>
		void				replace					(u32 inPos, s32 inLen, const char* inString, s32 inNum=-1);				///< Replace <inLength> characters at <inPosition> with inNumChars characters of <inString>
		s32					replace					(const char* inFind, const char* inSubstitute, u32 inPos=0);			///< Replace all occurrences of string <inFind> after <inPosition> with string <inSubstitute>, returns amount of replacements made
		s32					replace					(char inFind, const char* inSubstitute, u32 inPos=0);					///< Replace all occurrences of character <inFind> after <inPosition> with string <inSubstitute>, returns amount of replacements made
		s32					replace					(const char* inFind, char inSubstitute, u32 inPos=0);					///< Replace all occurrences of string <inFind> after <inPosition> with char <inSubstitute>, returns amount of replacements made
		s32					replace					(char inFind, char inSubstitute, u32 inPos=0);							///< Replace all occurrences of char <inFind> after <inPosition> with char <inSubstitute>, returns amount of replacements made

		s32					replaceAnyWith			(const char* inAny, char inWith, u32 inFrom=0, s32 inNum=-1);			///< Replace any character from <inAny> in the string with the <inWith> character

		void				insert					(char inChar);															///< Insert inChar at current position
		void				insert					(uchar8 inChar);														///< Insert inChar at current position
		void				insert					(const char* inString, s32 inNum=-1);									///< Insert inString starting at current position
		void				insert					(const ustr8* inString, s32 inNum=-1);									///< Insert inString starting at current position
		void				insert					(const xstring& inString);												///< Insert inString starting at current position
		void				insert					(const xcstring& inString);												///< Insert inString starting at current position
		void				insert					(const xccstring& inString);											///< Insert inString starting at current position

		void				insert					(u32 inPos, char inChar);												///< Insert inChar at <inPosition>
		void				insert					(u32 inPos, uchar8 inChar);												///< Insert inChar at <inPosition>
		void				insert					(u32 inPos, const char* inString, s32 inNum=-1);						///< Insert inString starting at <inPosition>
		void				insert					(u32 inPos, const ustr8* inString, s32 inNum=-1);						///< Insert inString starting at <inPosition>
		void				insert					(u32 inPos, const xstring& inString);									///< Insert inString starting at <inPosition>
		void				insert					(u32 inPos, const xcstring& inString);									///< Insert inString starting at <inPosition>
		void				insert					(u32 inPos, const xccstring& inString);									///< Insert inString starting at <inPosition>

		void				remove					(u32 inStart, u32 inLength);											///< Remove <inLength> characters starting at <inStart>
		void				erase					(u32 inStart=0, s32 inLength = -1);										///< Erase <inLength> characters starting at <inStart>>
		void				remove					(const char* inCharSet);												///< Remove characters in <inCharSet> from string
		void				remove					(const ustr8* inCharSet);												///< Remove characters in <inCharSet> from string

		void				upper					(void);																	///< Uppercase all chars in string (e.g. "myWord" -> "MYWORD")
		void				lower					(void);																	///< Lowercase all chars in string (e.g. "myWord" -> "myword")
		void				capitalize				(void);																	///< Capitalize first char in string (e.g. "myWord" -> "Myword")
		void				capitalize				(const char* inSeperators);												///< Capitalize first char in words (e.g. "my1stWord my2ndWord" -> "My1stword My2ndword")
		void				trim					(void);																	///< Trim whitespace from left and right side of string
		void				trimLeft				(void);																	///< Trim whitespace from left side of string
		void				trimRight				(void);																	///< Trim whitespace from right side of string
		void				trim					(char inChar);															///< Trim characters in <inCharSet> from left and right side of string
		void				trimLeft				(char inChar);															///< Trim character <inChar> from left side of string
		void				trimRight				(char inChar);															///< Trim character <inChar> from right side of string
		void				trim					(const char* inCharSet);												///< Trim characters in <inCharSet> from left and right side of string
		void				trimLeft				(const char* inCharSet);												///< Trim characters in <inCharSet> from left side of string
		void				trimRight				(const char* inCharSet);												///< Trim characters in <inCharSet> from right side of string
		void				trimQuotes				(void);																	///< Trim double quotes from left and right side of string
		void				trimQuotes				(char quote);															///< Trim double quotes from left and right side of string
		void				trimDelimiters			(char inLeft, char inRight);											///< Trim delimiters from left and right side of string
		void				reverse					(void);																	///< Reverse characters in string
		void				reverse					(s32 inPosition, s32 inNum=-1);											///< Reverse characters in a part of this string

		bool				splitOn					(char inChar, xstring& outRight);										///< Split string on first occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				splitOn					(char inChar, xstring& outLeft, xstring& outRight) const;				///< Split string on first occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				rsplitOn				(char inChar, xstring& outRight);										///< Split string on last occurrence of <ch>, moves all text after <ch> into <outRight>
		bool				rsplitOn				(char inChar, xstring& outLeft, xstring& outRight) const;				///< Split string on last occurrence of <ch>, moves all text after <ch> into <outRight>
		void				split					(u32 inPosition, bool inRemove, xstring& outRight);						///< Split string at <inPosition>, move text at the right of inPosition into <outRight>
		void				split					(u32 inPosition, bool inRemove, xstring& outLeft, xstring& outRight) const;	///< Split string at <inPosition>, move text at the right of inPosition into <outRight>

		void				copy					(const char* str);
		void				copy					(const char* str, s32 length);
		void				copy					(const ustr8* str);
		void				copy					(const ustr8* str, s32 length);
		void				copy					(const xstring& str);
		void				copy					(const xcstring& str);
		void				copy					(const xccstring& str);

		void				indexToRowCol			(s32 index, s32& row, s32& col) const;

	protected:
		void				concat					(const char* inSource);													///< Concatenate <inSource> to this string
		void				concat					(const char* inSource, u32 inLength);									///< Concatenate string with <inLength> characters from <inSource>
		void				concat					(const ustr8* inSource);												///< Concatenate <inSource> to this string
		void				concat					(const ustr8* inSource, ulen8 inLength);								///< Concatenate string with <inLength> characters from <inSource>
	};

	/// Mutable, Heap
	class xstring : public xstring_mutable_base
	{
	public:
		typedef				xstring_mutable_base	__base;

		class data : public idata
		{
			inline	void init() 
			{
				data_[0].c='\0';
				data_[1].c='\0';
				data_[2].c='h'; 
				data_[3].c='e'; 
				data_[4].c='a'; 
				data_[5].c='p'; 
				data_[6].c='\0';
				data_[7].c='\0';
			}

		public:
			inline					data(x_iallocator* _alloc) : alloc_(_alloc), ref_(0), len_(), max_(0) { init(); }
			inline					data(x_iallocator* _alloc, s32 _ref, ulen8 _len, u32 _max) : alloc_(_alloc), ref_(_ref), len_(_len), max_(_max) { init(); }

			virtual ulen8			getLength() const				{ return len_; }
			virtual u32				getReserved() const				{ return max_; }
			virtual void			setLength(ulen8 _len);

			virtual bool			isEmpty() const					{ return len_.clen() == 0; }
			virtual void			setEmpty();

			virtual uptr8			getPtr()						{ return uptr8(data_); }
			virtual ucptr8			getCPtr() const					{ return ucptr8(data_); }

			virtual bool			isReadonly() const				{ return false; }
			virtual bool			isGrowable() const				{ return true; }
			
			virtual void			terminateWithZero()				{ data_[len_.blen()].c='\0'; data_[(u32)len_.blen()+1].c='\0';}

			// Reference counting interface
			virtual void			bind()							{ ++ref_; }
			virtual void			unbind()						{ if (--ref_ <= 0) alloc_->deallocate(this); }

			// Dynamic buffer interface
			virtual void			resize(u32);
			virtual void			unique();

			data*					clone();

			xstring					construct();
			xstring					construct(u32 inLength);
			xstring					construct(const char* inStr);
			xstring					construct(const char* inStr, u32 length);
			xstring					construct(const char* inStrA, u32 inStrLengthA, const char* inStrB, u32 inStrLengthB);
			xstring					construct(const ustr8* inStr);
			xstring					construct(const ustr8* inStr, u32 length);
			xstring					construct(const ustr8* inStrA, u32 inStrLengthA, const ustr8* inStrB, u32 inStrLengthB);
			xstring					construct(const xstring& inStr);
			xstring					construct(const xcstring& inStr);
			xstring					construct(const xccstring& inStr);
			xstring					construct(const char* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
			xstring					construct(const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);

		private:
			x_iallocator*			alloc_;
			s32						ref_;								///< Reference count
			ulen8					len_;
			u32						max_;
			ustr8					data_[8];							///< String data (allocated with sAlloc so that it is large enough to fit the string)
		};

							xstring					(void);
							xstring					(u32 inLength);
							xstring					(const char* inStr);
							xstring					(const char* inStr, u32 length);
							xstring					(const char* inStrA, u32 inStrLengthA, const char* inStrB, u32 inStrLengthB);
							xstring					(const ustr8* inStr);
							xstring					(const ustr8* inStr, u32 length);
							xstring					(const ustr8* inStrA, u32 inStrLengthA, const ustr8* inStrB, u32 inStrLengthB);
							xstring					(const xstring& inStr);
		explicit			xstring					(const xcstring& inStr);
		explicit            xstring					(const xccstring& inStr);

		s32                 format					(const char* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 formatV					(const char* inFormat, const x_va_list& Args);
		s32                 appendFormat			(const char* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 appendFormatV			(const char* inFormat, const x_va_list& Args);

		s32                 format					(const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 formatV					(const ustr8* inFormat, const x_va_list& Args);
		s32                 appendFormat			(const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32                 appendFormatV			(const ustr8* inFormat, const x_va_list& Args);

		uchar8		        operator []				(u32 inIndex) const;

		xstring&			operator =				(const char* inStr);										///< Copy a 'C' string to this string
		xstring&			operator =				(const ustr8* inStr);										///< Copy a UTF-8 string to this string
		xstring&            operator =				(const xstring& inStr);
		xstring&            operator =				(const xcstring& inStr);
		xstring&            operator =				(const xccstring& inStr);

		xstring&			operator +=				(char inRHS);												///< Add a character to this string
		xstring&			operator +=				(uchar8 inRHS);												///< Add a character to this string
		xstring&			operator +=				(const char* inRHS);										///< Add a C-String to this string
		xstring&			operator +=				(const ustr8* inRHS);										///< Add a C-String to this string
		xstring&			operator +=				(const xstring& inRHS);										///< Add a String to this string
		xstring&			operator +=				(const xcstring& inRHS);
		xstring&			operator +=				(const xccstring& inRHS);
	};

	xstring					construct(xstring::data*);
	xstring					construct(xstring::data*, u32 inLength);
	xstring					construct(xstring::data*, const char* inStr);
	xstring					construct(xstring::data*, const char* inStr, u32 length);
	xstring					construct(xstring::data*, const char* inStrA, u32 inStrLengthA, const char* inStrB, u32 inStrLengthB);
	xstring					construct(xstring::data*, const ustr8* inStr);
	xstring					construct(xstring::data*, const ustr8* inStr, u32 length);
	xstring					construct(xstring::data*, const ustr8* inStrA, u32 inStrLengthA, const ustr8* inStrB, u32 inStrLengthB);
	xstring					construct(xstring::data*, const xstring& inStr);
	xstring					construct(xstring::data*, const xcstring& inStr);
	xstring					construct(xstring::data*, const xccstring& inStr);
	xstring					construct(xstring::data*, const char* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
	xstring					construct(xstring::data*, const ustr8* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);

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
	//      xstring mystring("This is a string which will be copied in the dynamic string!");
	//
	// 

	/// Mutable, Memory given by user
	class xcstring : public xstring_mutable_base
	{
	public:
		typedef				xstring_mutable_base	__base;

		class data : public idata
		{
		public:
			inline					data() : data_(0), max_(0), len_()		{ }
			inline					data(ustr8* _data, u32 _max, ulen8 _len=ulen8()) : data_(_data), max_(_max), len_(_len) { }

			virtual ulen8			getLength() const				{ return len_; }
			virtual u32				getReserved() const				{ return len_.blen(); }
			virtual void			setLength(ulen8 _len)			{ ASSERT(_len.blen() < max_); len_=_len; }

			virtual bool			isEmpty() const					{ return len_.clen() == 0; }
			virtual void			setEmpty()						{ len_=ulen8(); terminateWithZero(); }

			virtual uptr8			getPtr()						{ return uptr8(data_); }
			virtual ucptr8			getCPtr() const					{ return ucptr8(data_); }

			virtual bool			isReadonly() const				{ return false; }
			virtual bool			isGrowable() const				{ return false; }

			virtual void			terminateWithZero()				{ data_[len_.blen()].c='\0'; data_[(u32)len_.blen()+1].c='\0';}

			// Reference counting interface
			virtual void			bind()							{ }
			virtual void			unbind()						{ }

			// Dynamic buffer interface
			virtual void			resize(u32)						{ }
			virtual void			unique()						{ }

			xcstring				get()							{ return xcstring(this); }

		private:
			ustr8*					data_;
			u32						max_;
			ulen8					len_;
		};

		inline         		xcstring				(void)															{ }
		inline         		xcstring				(data* _data) : __base(_data)									{ }
							xcstring				(void* buffer, s32 bufferSize);
							xcstring				(void* buffer, s32 bufferSize, const char* str);
							xcstring				(void* buffer, s32 bufferSize, const char* formatString, const x_va_list& args);
							xcstring				(void* buffer, s32 bufferSize, const char* formatString, const x_va& v1, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		explicit			xcstring				(void* buffer, s32 bufferSize, const xstring& str);
		explicit			xcstring				(void* buffer, s32 bufferSize, const xcstring& str);
		explicit			xcstring				(void* buffer, s32 bufferSize, const xccstring& str);

		s32					format					(const char* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32					formatV					(const char* inFormat, const x_va_list& Args);
		s32					appendFormat			(const char* inFormat, const x_va& v1=x_va::sEmpty, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);
		s32					appendFormatV			(const char* inFormat, const x_va_list& Args);

		uchar8				operator []				(s32 inIndex) const;

		xcstring&			operator =				(char inRHS);													///< Copy a char to this string
		xcstring&			operator =				(const char* inStr);											///< Copy a C-String to this string
		xcstring&			operator =				(const xstring& inStr);
		xcstring&			operator =				(const xcstring& inStr);
		xcstring&			operator =				(const xccstring& inStr);

		xcstring&			operator +=				(char inRHS);													///< Add a character to this string
		xcstring&			operator +=				(const char* inRHS);											///< Add a C-String to this string
		xcstring&			operator +=				(const xstring& inStr);
		xcstring&			operator +=				(const xcstring& inRHS);										///< Add a C-String to this string
		xcstring&			operator +=				(const xccstring& inRHS);										///< Add a Const C-String to this string

	protected:        
		inline void			terminateWithZero		(void)															{ mBuffer->terminateWithZero(); }
	};

	/// Const
	class xccstring : public xstring_const_base
	{
	public:
		typedef				xstring_const_base		__base;

		class data : public idata
		{
		public:
			inline					data() : data_((const ustr8*)""), len_()		{ }
			inline					data(const ustr8* _data) : data_(_data) { len_ = ulen8::strlen(_data); }
			inline					data(const ustr8* _data, ulen8 _len) : data_(_data), len_(_len) { }

			virtual ulen8			getLength() const				{ return len_; }
			virtual u32				getReserved() const				{ return len_.blen(); }
			virtual void			setLength(ulen8)				{ }

			virtual bool			isEmpty() const					{ return len_.blen() == 0; }
			virtual void			setEmpty()						{ }

			virtual uptr8			getPtr()						{ return uptr8(NULL); }
			virtual ucptr8			getCPtr() const					{ return ucptr8(data_); }

			virtual bool			isReadonly() const				{ return true; }
			virtual bool			isGrowable() const				{ return false; }

			virtual void			terminateWithZero()				{ }

			// Reference counting interface
			virtual void			bind()							{ }
			virtual void			unbind()						{ }

			// Dynamic buffer interface
			virtual void			resize(u32)						{ }
			virtual void			unique()						{ }

			void					set(ustr8* _str, ulen8 _len)	{ data_ = _str; len_ = _len; }
			xccstring				get()							{ return xccstring(this); }

		private:
			const ustr8*			data_;
			ulen8					len_;
		};

							xccstring				(void);
		explicit			xccstring				(data* _data) : xstring_const_base(_data)			{ }

		uchar8				operator[]				(s32 inIndex) const;
		xccstring&			operator =				(const char* str);										///< Assign a 'C' string to this string
		xccstring&			operator =				(const ustr8* str);										///< Assign a UTF-8 string to this string

		data*				get()					{ return (data*)mBuffer; }
		data const*			get() const				{ return (data const*)mBuffer; }
	};

	xcstring		construct(xcstring::data* _data);
	xcstring		construct(xcstring::data* _data, const ustr8* str);
	xcstring		construct(xcstring::data* _data, const ustr8* strA, const ustr8* strB);
	xcstring		construct(xcstring::data* _data, ulen8 len, const ustr8* str);
	xcstring		construct(xcstring::data* _data, ulen8 lenA, const ustr8* strA, ulen8 lenB, const ustr8* strB);

	xcstring		construct(xcstring::data* _data, const ustr8* formatString, const x_va_list& args);
	xcstring		construct(xcstring::data* _data, const ustr8* formatString, const x_va& v1, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);

	xcstring		construct(xcstring::data* _data, const xstring& str);
	xcstring		construct(xcstring::data* _data, const xcstring& str);
	xcstring		construct(xcstring::data* _data, const xccstring& str);

	// example
	class test_object
	{
	public:
		inline			test_object() 
		: mycstringdata((ustr8*)mychararray, 256)
		, myconststring((const ustr8*)"This is just # a test!")
		, myccstringdata(myconststring) {}

		void			example_func()
		{
			// this string is const, we can only find etc..
			xccstring ccstring = myccstringdata.get();
			// .. do some stuff with ccstring, like:
			upos8 pos = ccstring.find('#');

			// this string is mutable, we can assign strings to it etc..
			xcstring cstring = mycstringdata.get();
			cstring = ccstring;
			cstring.remove("#");
		}

	private:
		ustr8			mychararray[256];
		xcstring::data	mycstringdata;		// construct with mycstringdata(mychararray, 256)

		const ustr8*	myconststring;
		xccstring::data	myccstringdata;		// construct with mycstringdata(myconststring)
	};


	#include "xcore\private\x_string_inline.h"






//==============================================================================
// END
//==============================================================================
//DOM-IGNORE-END

}

#endif	///< __XBASE_XSTRING_H__
