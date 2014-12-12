//==============================================================================
// INCLUDES
//==============================================================================
#include "xcore\x_string.h"

#include "xbase\x_debug.h"
#include "xbase\x_memory_std.h"
#include "xbase\x_allocator.h"
#include "xbase\x_string_ascii.h"
#include "xbase\x_string_utf.h"
#include "xbase\x_integer.h"

namespace xcore
{
	namespace utf
	{
		s32		x_vsprintf(ustr8* _dst_str, u32 _dst_str_max_len, const ustr8* _format, x_va_list const& _va_list)
		{
			return 0;
		}

		bool	x_strIsUpper(const ustr8* _str, ulen8 _len_in_bytes)
		{
			return false;
		}

		bool	x_strIsLower(const ustr8* _str, ulen8 _len_in_bytes)
		{
			return false;
		}

		bool	x_strIsCapitalized(const ustr8* _str, ulen8 _len_in_bytes)
		{
			return false;
		}


		bool	x_strStartsWith(const ustr8* _str, ulen8 _len_in_bytes, const ustr8* _start_str)
		{
			return false;
		}

		bool	x_strEndsWith(const ustr8* _str, ulen8 _len_in_bytes, const ustr8* _start_str)
		{
			return false;
		}

		s32		x_strCompare(const ustr8* _str1, ulen8 _len_in_bytes, const ustr8* _str2, u32 _num_chars)
		{
			return 0;
		}

		s32		x_strCompareNoCase(const ustr8* _str1, ulen8 _len_in_bytes, const ustr8* _str2, u32 _num_chars)
		{
			return 0;
		}

		bool	x_strEqual(const ustr8* _str1, ulen8 _len_in_bytes, const ustr8* _str2, u32 _num_chars)
		{
			return x_strCompare(_str1, _len_in_bytes, _str2, _num_chars) == 0;
		}

		bool	x_strEqualNoCase(const ustr8* _str1, ulen8 _len_in_bytes, const ustr8* _str2, u32 _num_chars)
		{
			return x_strCompareNoCase(_str1, _len_in_bytes, _str2, _num_chars) == 0;
		}

		upos8			x_strFindInSubstr(const ustr8* _str, uchar8 inChar, u32 inPosition, s32 inCharNum)
		{
			return upos8();
		}
		upos8			x_strFindInSubstr(const ustr8* _str, const ustr8* _str_to_find, u32 inPosition, s32 inCharNum)
		{
			return upos8();
		}
		upos8			x_strFindNoCaseInSubstr(const ustr8* _str, uchar8 inChar, u32 inPosition, s32 inCharNum)
		{
			return upos8();
		}
		upos8			x_strFindNoCaseInSubstr(const ustr8* _str, const ustr8* _str_to_find, u32 inPosition, s32 inCharNum)
		{
			return upos8();
		}
		upos8			x_strRFind(const ustr8* _str, uchar8 inChar, u32 inPosition, s32 inLen)
		{
			return upos8();
		}
		upos8			x_strRFind(const ustr8* _str, const ustr8* _str_to_find, u32 inPosition, s32 inLen)
		{
			return upos8();
		}
		upos8			x_strFindOneOf(const ustr8* _str, const ustr8* inCharSet, u32 inPosition, ulen8 inLen)
		{
			return upos8();
		}
		upos8			x_strRFindOneOf(const ustr8* _str, const ustr8* inCharSet, u32 inPosition, ulen8 inLen)
		{
			return upos8();
		}
	}
}


namespace xcore
{
	//==============================================================================
	// All buffer data instances
	//==============================================================================
	class xstring_idata
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

		virtual void			copyFrom(ustr8 const* _str, ulen8 _len) = 0;

		virtual void			terminateWithZero() = 0;

		// Reference counting interface
		virtual void			bind() = 0;
		virtual void			unbind() = 0;

		// Dynamic buffer interface
		virtual void			resize(u32) = 0;
		virtual void			unique() = 0;
	};


	class xcstring_data : public xstring_idata
	{
	public:
		inline					xcstring_data() : data_(0), max_(0), len_()		{ }
		inline					xcstring_data(ustr8* _data, u32 _max, ulen8 _len=ulen8()) : data_(_data), max_(_max), len_(_len) { }

		virtual ulen8			getLength() const				{ return len_; }
		virtual u32				getReserved() const				{ return len_.blen(); }
		virtual void			setLength(ulen8 _len)			{ ASSERT(_len.blen() < max_); len_=_len; }

		virtual bool			isEmpty() const					{ return len_.is_empty(); }
		virtual void			setEmpty()						{ len_=ulen8(); terminateWithZero(); }

		virtual uptr8			getPtr()						{ return uptr8(data_); }
		virtual ucptr8			getCPtr() const					{ return ucptr8(data_); }

		virtual bool			isReadonly() const				{ return false; }
		virtual bool			isGrowable() const				{ return false; }

		virtual void			copyFrom(ustr8 const* _str, ulen8 _len);

		virtual void			terminateWithZero()				{ data_[len_.blen()].c='\0'; data_[(u32)len_.blen()+1].c='\0';}

		// Reference counting interface
		virtual void			bind()							{ }
		virtual void			unbind()						{ }

		// Dynamic buffer interface
		virtual void			resize(u32)						{ }
		virtual void			unique()						{ }

		xcstring				get()							{ return xcstring(this); }

		xcstring				construct();
		xcstring				construct(const ustr8* str);
		xcstring				construct(const ustr8* strA, const ustr8* strB);
		xcstring				construct(ulen8 len, const ustr8* str);
		xcstring				construct(ulen8 lenA, const ustr8* strA, ulen8 lenB, const ustr8* strB);

		xcstring				construct(const ustr8* formatString, const x_va_list& args);
		xcstring				construct(const ustr8* formatString, const x_va& v1, const x_va& v2=x_va::sEmpty, const x_va& v3=x_va::sEmpty, const x_va& v4=x_va::sEmpty, const x_va& v5=x_va::sEmpty, const x_va& v6=x_va::sEmpty, const x_va& v7=x_va::sEmpty, const x_va& v8=x_va::sEmpty, const x_va& v9=x_va::sEmpty, const x_va& v10=x_va::sEmpty);

		xcstring				construct(const xstring& str);
		xcstring				construct(const xcstring& str);
		xcstring				construct(const xccstring& str);

	private:
		ustr8*					data_;
		u32						max_;
		ulen8					len_;
	};

	class xstring_data : public xstring_idata
	{
		static const s32 DEFAULT_STRING_LENGTH_IN_BYTES = 64;

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
		inline					xstring_data(x_iallocator* _alloc) : alloc_(_alloc), ref_(0), len_(), max_(0) { init(); }
		inline					xstring_data(x_iallocator* _alloc, s32 _ref, ulen8 _len, u32 _max) : alloc_(_alloc), ref_(_ref), len_(_len), max_(_max) { init(); }

		virtual ulen8			getLength() const				{ return len_; }
		virtual u32				getReserved() const				{ return max_; }
		virtual void			setLength(ulen8 _len);

		virtual bool			isEmpty() const					{ return len_.is_empty(); }
		virtual void			setEmpty();

		virtual uptr8			getPtr()						{ return uptr8(data_); }
		virtual ucptr8			getCPtr() const					{ return ucptr8(data_); }

		virtual bool			isReadonly() const				{ return false; }
		virtual bool			isGrowable() const				{ return true; }

		virtual void			copyFrom(ustr8 const* _str, ulen8 _len);

		virtual void			terminateWithZero()				{ data_[len_.blen()].c='\0'; data_[(u32)len_.blen()+1].c='\0';}

		// Reference counting interface
		virtual void			bind()							{ ++ref_; }
		virtual void			unbind()						{ if (--ref_ <= 0) alloc_->deallocate(this); }

		// Dynamic buffer interface
		virtual void			resize(u32);
		virtual void			unique();

		xstring_data*			clone();

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
		s32						ref_;										///< Reference count (should be atomic in multi-threaded environment)
		ulen8					len_;
		u32						max_;
		ustr8					data_[DEFAULT_STRING_LENGTH_IN_BYTES];		///< String data (allocated with sAlloc so that it is large enough to fit the string)
	};

	class xccstring_data : public xstring_idata
	{
	public:
		inline					xccstring_data() : data_((const ustr8*)""), len_()		{ }
		inline					xccstring_data(const char* _data) : data_((const ustr8*)_data) { len_ = ulen8::strlen(data_); }
		inline					xccstring_data(const ustr8* _data) : data_(_data) { len_ = ulen8::strlen(_data); }
		inline					xccstring_data(const ustr8* _data, ulen8 _len) : data_(_data), len_(_len) { }

		virtual ulen8			getLength() const				{ return len_; }
		virtual u32				getReserved() const				{ return len_.blen(); }
		virtual void			setLength(ulen8)				{ }

		virtual bool			isEmpty() const					{ return len_.is_empty(); }
		virtual void			setEmpty()						{ }

		virtual uptr8			getPtr()						{ return uptr8(NULL); }
		virtual ucptr8			getCPtr() const					{ return ucptr8(data_); }

		virtual bool			isReadonly() const				{ return true; }
		virtual bool			isGrowable() const				{ return false; }

		virtual void			copyFrom(ustr8 const* _str, ulen8 _len) { }

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
}

/**
 * xCore namespace
 */
namespace xcore
{
	/**
	* @file
	* PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!!
	* DOM-IGNORE-BEGIN
	* xstring_const_base 
	*/


	//------------------------------------------------------------------------------
	xstring_const_base::xstring_const_base(const xstring_const_base& _i) 
		: mBuffer(_i.mBuffer)
	{
		mBuffer->bind(); 
	}

	//------------------------------------------------------------------------------
	xstring_const_base::~xstring_const_base()
	{
		mBuffer->unbind(); 
	}

	//------------------------------------------------------------------------------

	ulen8 xstring_const_base::len(void) const
	{
		return mBuffer->getLength(); 
	}

	//------------------------------------------------------------------------------

	bool xstring_const_base::isEmpty(void) const
	{
		return mBuffer->isEmpty(); 
	}

	//------------------------------------------------------------------------------

	bool xstring_const_base::isUpper(void) const
	{
		return utf::x_strIsUpper(cbegin().str(), len());
	}

	//------------------------------------------------------------------------------


	bool xstring_const_base::isLower(void) const
	{
		return utf::x_strIsLower(cbegin().str(), len());
	}

	//------------------------------------------------------------------------------


	bool xstring_const_base::isCapitalized(void) const
	{
		return utf::x_strIsCapitalized(cbegin().str(), len());
	}

	//------------------------------------------------------------------------------


	bool xstring_const_base::isQuoted(void) const
	{
		return isQuoted('\"');
	}

	//------------------------------------------------------------------------------


	bool xstring_const_base::isQuoted(uchar8 quote) const
	{
		return firstChar()==quote && lastChar()==quote;
	}

	//------------------------------------------------------------------------------


	bool xstring_const_base::isDelimited(uchar8 left, uchar8 right) const
	{
		return firstChar()==left && lastChar()==right;
	}

	//------------------------------------------------------------------------------

	uchar8 xstring_const_base::getAt(ucpos8 inPosition) const
	{
		ASSERT(inPosition>=0 && inPosition<len().clen());
		ucptr8 p = cbegin();
		ulen8 l = ulen8::at(mBuffer->getPtr().str(), inPosition);
		uchar8 c = *(p + l);
		return c;
	}

	//------------------------------------------------------------------------------

	uchar8 xstring_const_base::firstChar(void) const
	{
		if (isEmpty())
			return uchar8('\0');

		ucptr8 p = cbegin();
		uchar8 c = *p;
		return c;
	}

	//------------------------------------------------------------------------------

	uchar8 xstring_const_base::lastChar(void) const
	{
		uchar8 c('\0');
		if (isEmpty())
			return c;

		ucptr8 p = cend() - 1;
		c = *p;
		return c;
	}

	//------------------------------------------------------------------------------

	bool xstring_const_base::startsWith(uchar8 inChar) const
	{
		const uchar8 c = firstChar();
		return c == uchar8(inChar);
	}

	//------------------------------------------------------------------------------


	bool xstring_const_base::startsWith(const ustr8* inStartStr) const
	{
		return utf::x_strStartsWith(cbegin().str(), len(), (const ustr8*)inStartStr);
	}

	//------------------------------------------------------------------------------

	bool xstring_const_base::endsWith(uchar8 inChar) const
	{
		const uchar8 c = lastChar();
		return c == uchar8(inChar);
	}

	//------------------------------------------------------------------------------


	bool xstring_const_base::endsWith(const ustr8* inEndStr) const
	{
		return utf::x_strEndsWith(cbegin().str(), len(), (const ustr8*)inEndStr);
	}

	///@name Comparison

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compare(const ustr8* inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (utf::strlen(inRHS)) : inCharNum;
		return utf::x_strCompare(cbegin().str(), len(), inRHS, inCharNum);
	}

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compare(const xstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strCompare(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compare(const xcstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strCompare(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compare(const xccstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strCompare(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compareNoCase(const ustr8* inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (utf::strlen(inRHS)) : inCharNum;
		return utf::x_strCompareNoCase(cbegin().str(), len(), inRHS, inCharNum);
	}

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compareNoCase(const xstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strCompareNoCase(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compareNoCase(const xcstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strCompareNoCase(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	s32					xstring_const_base::compareNoCase(const xccstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strCompareNoCase(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqual(const ustr8* inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (utf::strlen(inRHS)) : inCharNum;
		return utf::x_strEqual(cbegin().str(), len(), inRHS, inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqual(const xstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strEqual(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqual(const xcstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strEqual(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqual(const xccstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strEqual(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqualNoCase(const ustr8* inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (utf::strlen(inRHS)) : inCharNum;
		return utf::x_strEqualNoCase(cbegin().str(), len(), inRHS, inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqualNoCase(const xstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strEqualNoCase(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqualNoCase(const xcstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strEqualNoCase(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::isEqualNoCase(const xccstring& inRHS, uclen8 inCharNum) const
	{
		inCharNum = (inCharNum.is_empty()) ? (inRHS.len().clen()) : inCharNum;
		return utf::x_strEqualNoCase(cbegin().str(), len(), inRHS.cbegin().str(), inCharNum);
	}

	///@name Search/replace

	//------------------------------------------------------------------------------


	upos8				xstring_const_base::find(uchar8 inChar, ucpos8 inPosition, uclen8 inCharNum) const
	{
		ASSERT(inPosition >= 0);
		const ulen8 n = len();
		inCharNum = (inCharNum.is_empty()) ? (n.clen()-inPosition) : inCharNum;
		ASSERT((inPosition+inCharNum) <= n.clen());
		upos8 pos = utf::x_strFindInSubstr(cbegin().str(), uchar8(inChar), inPosition, inCharNum);
		return pos;
	}

	//------------------------------------------------------------------------------


	upos8					xstring_const_base::find(const ustr8* inString, ucpos8 inPosition, uclen8 inCharNum) const
	{
		ASSERT(inPosition >= 0);
		const ulen8 n = len();
		inCharNum = (inCharNum.is_empty()) ? (n.clen()-inPosition) : inCharNum;
		ASSERT((inPosition+inCharNum) <= n.clen());
		upos8 pos = utf::x_strFindInSubstr(cbegin().str(), (const ustr8*)inString, inPosition, inCharNum);
		return pos;
	}

	//------------------------------------------------------------------------------


	upos8					xstring_const_base::findNoCase(uchar8 inChar, ucpos8 inPosition, uclen8 inCharNum) const
	{
		ASSERT(inPosition >= 0);
		const ulen8 n = len();
		inCharNum = (inCharNum.is_empty()) ? n.clen() : inCharNum;
		ASSERT((inPosition+inCharNum) <= n.clen());
		const ustr8* str = cbegin().str();
		upos8 find_pos = utf::x_strFindNoCaseInSubstr(str, uchar8(inChar), inPosition, inCharNum);
		return find_pos;
	}

	//------------------------------------------------------------------------------


	upos8					xstring_const_base::findNoCase(const ustr8* inString, ucpos8 inPosition, uclen8 inCharNum) const
	{
		ASSERT(inPosition >= 0);
		const ulen8 n = len();
		inCharNum = (inCharNum.is_empty()) ? n.clen() : inCharNum;
		ASSERT((inPosition+inCharNum) <= n.clen());
		const ustr8* str = cbegin().str();
		upos8 find_pos = utf::x_strFindNoCaseInSubstr(str, (const ustr8*)inString, inPosition, inCharNum);
		return find_pos;
	}

	//------------------------------------------------------------------------------


	upos8					xstring_const_base::rfind(uchar8 inChar, ucpos8 inPosition, uclen8 inLen) const
	{
		inPosition = (inPosition==-1) ? (len().clen()-1) : inPosition;
		inLen = (inLen.is_empty()) ? (inPosition+1) : inLen;
		ASSERT(inPosition>=0 && inPosition<len().clen());
		upos8 pos = utf::x_strRFind(cbegin().str(), uchar8(inChar), inPosition, inLen);
		return pos;
	}

	//------------------------------------------------------------------------------


	upos8					xstring_const_base::rfind(const ustr8* inString, ucpos8 inPosition, uclen8 inLen) const
	{
		inPosition = (inPosition==-1) ? (len().clen()-(u32)1) : inPosition;
		inLen = (inLen.is_empty()) ? (inPosition+1) : inLen;
		ASSERT(inPosition>=0 && inPosition<len().clen());
		upos8 pos = utf::x_strRFind(cbegin().str(), (const ustr8*)inString, inPosition, inLen);
		return pos;
	}

	//------------------------------------------------------------------------------


	upos8					xstring_const_base::findOneOf(const ustr8* inCharSet, ucpos8 inPosition) const
	{
		upos8 pos = utf::x_strFindOneOf(cbegin().str(), (const ustr8*)inCharSet, inPosition, len());
		return pos;
	}

	//------------------------------------------------------------------------------



	//------------------------------------------------------------------------------


	upos8					xstring_const_base::rfindOneOf(const ustr8* inCharSet, ucpos8 inPosition) const
	{
		upos8 pos = utf::x_strRFindOneOf(cbegin().str(), inCharSet, inPosition, len());
		return pos;
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::left(uclen8 inNum, xstring& outLeft) const
	{
		XBOUNDS((u32)inNum, 0, (u32)(len().clen()));
		const ustr8* buffer = cbegin().str();
		outLeft.copy(buffer, x_intu::min((u32)inNum, (u32)len().clen()));
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::left(uclen8 inNum, xcstring& outLeft) const
	{
		XBOUNDS((u32)inNum, 0, (u32)len().clen());
		const ustr8* buffer = cbegin().str();
		outLeft.copy(buffer, x_intu::min((u32)inNum, (u32)len().clen()));
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::right(uclen8 inNum, xstring& outRight) const
	{
		const ulen8 l = len();
		XBOUNDS((u32)inNum, 0, (u32)l.clen());
		ucptr8 buffer = cbegin();
		inNum = x_intu::min((u32)inNum, (u32)l.clen());
		outRight.copy((buffer + l - inNum).str(), inNum);
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::right(uclen8 inNum, xcstring& outRight) const
	{
		const ulen8 l = len();
		XBOUNDS((u32)inNum, 0, (u32)l.clen());
		ucptr8 buffer = cbegin();
		inNum = x_intu::min((u32)inNum, (u32)l.clen());
		outRight.copy((buffer + l - inNum).str(), inNum);
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::mid(ucpos8 inPosition, xstring& outMid, uclen8 inNum) const
	{
		ulen8 l = len();
		ASSERT(l.clen().in_range(inPosition));								// Must start to grab within string
		ASSERT((inNum.is_empty()) || (inPosition+inNum<=l.clen()));			// Can't grab beyond end of string

		inPosition = ucpos8::min(inPosition, l.cpos());
		//inPosition = x_intu::min((s32)inPosition, (s32)(u32)l.clen());
		//inNum = (inNum==-1) ? (l-ulen8(inPosition)).clen() : (x_intu::min(inNum, (s32)(u32)((l-ulen8(inPosition)).clen())));

		inNum = (inNum.is_empty()) ? (l.cpos() - inPosition) : (ucpos8::min(ucpos8(inNum), l.cpos() - inPosition));

		const ustr8* buffer = cbegin().str();
		outMid.copy(buffer+inPosition, inNum);
	}

	//------------------------------------------------------------------------------


	xstring				xstring_const_base::mid(ucpos8 inPosition, uclen8 inNum) const
	{
		const ulen8 l8 = len();
		inPosition = l8.clamp(inPosition);
		inNum = l8.clamp(inPosition, inNum);

		const ustr8* buffer = cbegin().str();
		xstring outMid;
		outMid.copy(buffer + inPosition, inNum);
		return outMid;
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::mid(ucpos8 inPosition, xcstring& outMid, uclen8 inNum) const
	{
		const ulen8 l = len();
		ASSERT(l.clen().in_range(inPosition));								// Must start to grab within string
		ASSERT((inNum.is_empty()) || (inPosition+inNum<=l.clen()));			// Can't grab beyond end of string

		inPosition = x_intu::min((s32)inPosition, (s32)(u32)l.clen());
		inNum = (inNum==-1) ? (l-ulen8(inPosition)).clen() : (x_intu::min(inNum, (s32)(u32)((l-ulen8(inPosition)).clen())));

		const ustr8* buffer =cbegin().str();
		outMid.copy(buffer+inPosition, inNum);
	}

	// xstring version


	void				xstring_const_base::substring(ucpos8 inPosition, xstring& outSubstring, uclen8 inNum) const
	{
		mid(inPosition, outSubstring, inNum);
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::substring(ucpos8 inPosition, xstring& outSubstring) const
	{
		mid(inPosition, outSubstring, (len()-ulen8(inPosition)).clen());
	}


 
	xstring				xstring_const_base::substr(ucpos8 inPosition, uclen8 inNum) const
	{
		 return mid( inPosition, inNum);
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::splitOn(uchar8 inChar, xstring& outLeft, xstring& outRight) const
	{
		// Find the split character
		upos8 split_pos = find(inChar);
		if (split_pos.is_empty()) 
			return xFALSE;

		// Split string
		split(split_pos, true, outLeft, outRight);
		return xTRUE;
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::rsplitOn(uchar8 inChar, xstring& outLeft, xstring& outRight) const
	{
		// Find the split character
		upos8 split_pos = rfind(inChar);
		if (split_pos == -1) 
			return xFALSE;

		// Split string
		split(split_pos, xTRUE, outLeft, outRight);
		return xTRUE;
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::split(ucpos8 inPosition, bool inRemove, xstring& outLeft, xstring& outRight) const
	{
		XBOUNDS(inPosition, 0, len());
		XBOUNDS(inPosition + ((int)inRemove), 0, len());

		left(inPosition, outLeft);
		mid(inPosition + ((int)inRemove), outRight);
	}

	// xcstring version


	void				xstring_const_base::substring(ucpos8 inPosition, xcstring& outSubstring, uclen8 inNum) const
	{
		mid(inPosition, outSubstring, inNum);
	}

	//------------------------------------------------------------------------------


	void				xstring_const_base::substring(ucpos8 inPosition, xcstring& outSubstring) const
	{
		mid(inPosition, outSubstring, len()-inPosition);
	}


	//------------------------------------------------------------------------------


	bool				xstring_const_base::splitOn(uchar8 inChar, xcstring& outLeft, xcstring& outRight) const
	{
		// Find the split character
		upos8 split_pos = find(inChar);
		if (split_pos == -1) 
			return xFALSE;

		// Split string
		split(split_pos, true, outLeft, outRight);
		return xTRUE;
	}

	//------------------------------------------------------------------------------


	bool				xstring_const_base::rsplitOn(uchar8 inChar, xcstring& outLeft, xcstring& outRight) const
	{
		// Find the split character
		upos8 split_pos = rfind(inChar);
		if (split_pos == -1) 
			return xFALSE;

		// Split string
		split(split_pos, xTRUE, outLeft, outRight);
		return xTRUE;
	}

	//------------------------------------------------------------------------------

	void				xstring_const_base::split(ucpos8 inPosition, bool inRemove, xcstring& outLeft, xcstring& outRight) const
	{
		XBOUNDS(inPosition, 0, len());
		XBOUNDS(inPosition + ((int)inRemove), 0, len());

		left(inPosition, outLeft);
		mid(inPosition + (inRemove ? 1 : 0), outRight);
	}

	//------------------------------------------------------------------------------

	ucptr8				xstring_const_base::cbegin() const
	{
		return mBuffer->getCPtr(); 
	}

	//------------------------------------------------------------------------------

	ucptr8				xstring_const_base::cend() const
	{
		return mBuffer->getCPtr() + mBuffer->getLength(); 
	}



	// ---------------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------------------------------------
	// xstring_mutable_base 
	// ---------------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------------------------------------------------------
	s32				xstring_mutable_base::max(void) const
	{
		return mBuffer->getReserved(); 
	}

	void			xstring_mutable_base::clear(void)
	{
		__const_base::mBuffer->unbind();
		__const_base::mBuffer->setEmpty();
	}

	// ---------------------------------------------------------------------------------------------------------------------------

	void			xstring_mutable_base::repeat(const char* inString, s32 inTimes)
	{
		repeat(inString, inTimes, -1);
	}

	void			xstring_mutable_base::repeat(const ustr8* inString, s32 inTimes)
	{
		repeat(inString, inTimes, -1);
	}

	// ---------------------------------------------------------------------------------------------------------------------------

	void			xstring_mutable_base::repeat(const char* inString, s32 inTimes, s32 inStringLength)
	{
		repeat((const ustr8*)inString, inTimes, inStringLength);
	}

	void			xstring_mutable_base::repeat(const ustr8* inString, s32 inTimes, s32 inStringLength)
	{
		ASSERT(inTimes >= 0);

		// Make sure string length is valid
		if (inStringLength < 0)
			inStringLength = utf::strlen(inString);	// If not given, determine string length

		// Construct return string
		__const_base::mBuffer->resize(inStringLength * inTimes);	// Reserve length for string
		while (inTimes--)
			concat(inString, inStringLength);
	}


	void			xstring_mutable_base::repeat	(const xstring& inString, s32 inTimes)
	{
		repeat(inString.c_str(), inTimes, inString.len());
	}


	void			xstring_mutable_base::repeat	(const xcstring& inString, s32 inTimes)
	{
		repeat(inString.c_str(), inTimes, inString.len());
	}


	void			xstring_mutable_base::repeat	(const xccstring& inString, s32 inTimes)
	{
		repeat(inString.c_str(), inTimes, inString.len());
	}


	s32				xstring_mutable_base::format				(const char* formatString, const x_va_list& args)
	{
		ustr8* buffer = __const_base::mBuffer->getPtr();
		s32 len = utf::x_vsprintf(buffer, __const_base::mBuffer->lenInBytesMax(), (const ustr8*)formatString, args);
		__const_base::mBuffer->setLength(len);
		__const_base::mBuffer->terminateWithZero();
		return len;
	}


	s32				xstring_mutable_base::formatAdd			(const char* formatString, const x_va_list& args)
	{
		ustr8* buffer = __const_base::mBuffer->getPtr();
		s32 added = utf::x_vsprintf(buffer + __const_base::mBuffer->lenInBytes(), __const_base::mBuffer->lenInBytesMax(), (const ustr8*)formatString, args);
		__const_base::mBuffer->setLengthInBytes(__const_base::mBuffer->lenInBytes() + added);
		__const_base::mBuffer->terminateWithZero();
		return added;
	}




	void			xstring_mutable_base::setAt	(u32 inPosition, char inChar)
	{
		ASSERT(!__const_base::mBuffer->isEmpty());
		ASSERT(inPosition < __const_base::len());
		__const_base::mBuffer->unique();
		ustr8* buffer = __const_base::mBuffer->getPtr();
		buffer[inPosition].c = inChar;
	}


	void			xstring_mutable_base::setAt	(u32 inPosition, const char* inString, s32 inNum)
	{
		replace(inPosition, inString, inNum);
	}


	void			xstring_mutable_base::setAt	(u32 inPosition, s32 inLen, const char* inString, s32 inNum)
	{
		replace(inPosition, inLen, inString, inNum);
	}


	void			xstring_mutable_base::replace(u32 inPosition, const char* inString, s32 inNum)
	{
		replace(inPosition, 1, inString, inNum);
	}


	void			xstring_mutable_base::replace(u32 inPosition, s32 inLength, const char* inString, s32 inNumChars)
	{
		const ulen8 length = len();
		inNumChars = (inNumChars==-1) ? (x_strlen(inString)) : (inNumChars);

		ASSERT(inLength >= 0);
		XBOUNDS(inPosition, 0, (u32)length.clen() - 1);
		XBOUNDS(inPosition + inLength, 0, (u32)length.clen());
		ASSERT(inNumChars >= 0 && inNumChars <= x_strlen(inString));

		const s32 replaceEndPos = inPosition + inLength;
		const s32 dstEndPos     = inPosition + inNumChars;

		s32 extra = inNumChars - inLength;
		if (inNumChars <= inLength)
		{
			// Remove some data from the string by replacement
			__const_base::mBuffer->unique();
		
			uptr8 replaceStartPtr = mBuffer->getPtr() + inPosition;
			uptr8 replaceEndPtr   = mBuffer->getPtr() + replaceEndPos;
		
			if (inNumChars < inLength)
				x_memcpy(replaceEndPtr + extra, replaceEndPtr, length - replaceEndPos + 1);
			x_memcpy(replaceStartPtr, (const char*)inString, inNumChars);
		}
		else
		{
			if (__const_base::mBuffer->isGrowable())
			{
				// We can grow the buffer, so replacing is quite straightforward
				// Insert zero or more characters into the string
				__const_base::mBuffer->resize(length + extra);
			
				uptr8 replaceStartPtr = mBuffer->getPtr() + inPosition;
				uptr8 replaceEndPtr   = mBuffer->getPtr() + replaceEndPos;
			
				x_memmove(replaceEndPtr + extra, replaceEndPtr, length - replaceEndPos + 1);
				x_memcpy(replaceStartPtr, (const char*)inString, inNumChars);
			}
			else
			{
				// We cannot grow the buffer, so replacing is done by clamping against the maximum length
				const s32 maxLength = max();

				uptr8 replaceStartPtr = mBuffer->getPtr() + inPosition;
				uptr8 replaceEndPtr   = mBuffer->getPtr() + replaceEndPos;

				s32 numCharsToMove = length - replaceEndPos + 1;
				if ((dstEndPos + numCharsToMove) > maxLength)
					numCharsToMove = maxLength - dstEndPos;
				if (numCharsToMove>0)
					x_memmove(replaceEndPtr + extra, replaceEndPtr, numCharsToMove);

				s32 numCharsToCopy = inNumChars;
				if ((inPosition+numCharsToCopy) > maxLength)
					numCharsToCopy = maxLength - inPosition;
				if (numCharsToCopy>0)
					x_memcpy(replaceStartPtr, (const char*)inString, numCharsToCopy);

				if (length+extra > maxLength)
					extra = maxLength - length;
			}
		}
		mBuffer->setLength(length + extra);
		mBuffer->terminateWithZero();
	}


	s32				xstring_mutable_base::replace(const char* inFind, const char* inSubstitute, u32 inPosition)
	{
		s32 num_replacements = 0;
		upos8 str_pos = find(inFind, inPosition);

		if (str_pos != -1)
		{
			s32 findLen = x_strlen(inFind);
			s32 substLen = x_strlen(inSubstitute);
			while (str_pos != -1)
			{
				replace(str_pos, findLen, inSubstitute);
				++num_replacements;
				str_pos = __const_base::find(inFind, str_pos+substLen);
			}
		}

		return num_replacements;
	}


	s32				xstring_mutable_base::replace(char inFind, const char* inSubstitute, u32 inPosition)
	{
		char find[3];
		find[0]=inFind;
		find[1]='\0';
		find[2]='\0';
		return replace(find, inSubstitute, inPosition);
	}


	s32				xstring_mutable_base::replace(const char* inFind, char inSubstitute, u32 inPosition)
	{
		char subst[3];
		subst[0]=inSubstitute;
		subst[1]='\0';
		subst[2]='\0';
		return replace(inFind, subst, inPosition);
	}


	s32				xstring_mutable_base::replace(char inFind, char inSubstitute, u32 inPosition)
	{
		char find[3];
		find[0]=inFind;
		find[1]='\0';
		find[2]='\0';
		return replace(find, inSubstitute, inPosition);
	}


	s32				xstring_mutable_base::replaceAnyWith(const char* inAny, char inWith, u32 inFrom, s32 inNum)
	{
		if (inWith=='\0')
			return 0;

		const ulen8 len = __const_base::len();
		if (inNum==-1)
			inNum = len;

		const s32 _To = (inFrom + inNum) <= len ? (inFrom + inNum) : len;

		uptr8 buffer = __const_base::mBuffer->getPtr();

		s32 count = 0;
		while (*inAny != '\0')
		{
			char any = *inAny++;
			for (s32 i=inFrom; i<_To; ++i)
			{
				if (buffer[i] == any)
				{
					++count;
					buffer[i] = inWith;
				}
			}
		}
		return count;
	}


	void			xstring_mutable_base::insert	(char inChar)
	{
		insert(0, &inChar, 1);
	}


	void			xstring_mutable_base::insert	(const char* inString, s32 inCharNum)
	{
		insert(0, inString, inCharNum);
	}


	void			xstring_mutable_base::insert	(const xstring& inString)
	{
		insert(0, inString.c_str(), inString.len());
	}


	void			xstring_mutable_base::insert	(const xcstring& inString)
	{
		insert(0, inString.c_str(), inString.len());
	}


	void			xstring_mutable_base::insert	(const xccstring& inString)
	{
		insert(0, inString.c_str(), inString.len());
	}


	void			xstring_mutable_base::insert	(u32 inPosition, char inChar)
	{
		insert(inPosition, &inChar, 1);
	}


	void			xstring_mutable_base::insert	(u32 inPosition, const char* inString, s32 inCharNum)
	{
		insert(inPosition, (const ustr8*)inString, inCharNum);
	}

	void			xstring_mutable_base::insert	(u32 inPosition, const ustr8* inString, s32 inCharNum)
	{
		if (inPosition == -1)
			inPosition = 0;

		const s32 l =__const_base::len();
		XBOUNDS(inPosition, 0, l);

		if (inCharNum == -1)
			inCharNum = utf::strlen(inString);

		s32 addedLength = inCharNum;
		if (addedLength<=0)
			return;

		ustr8* str = __const_base::mBuffer->getPtr();
		if (__const_base::mBuffer->isGrowable())
		{
			// Make sure our buffer is unique
			__const_base::mBuffer->resize(l + addedLength);
			str = __const_base::mBuffer->getPtr();

			// Move part of the string to make space to insert the string
			// We need to start at the end and also copy the zero terminator.
			ustr8* pos  = str + l + addedLength;
			const ustr8* read = str + l;
			const ustr8* end  = str + inPosition;
			while (read>=end)
			{
				*pos = *read;
				--pos;
				--read;
			}

			// Copy the string into this string
			for (s32 i=0; i<addedLength; ++i)
				str[inPosition + i] = inString[i];
		}
		else
		{
			const s32 maxLength = getMaxLength();
			const s32 movingLength = l-inPosition;
			if (movingLength>0 && (inPosition+addedLength)<maxLength)
			{
				// Part of our string on the right side of inPosition will need to be moved
				x_memmove(str + inPosition + addedLength, str + inPosition, movingLength);
			}
			// How much of the incoming string can be copied into this string
			if ((inPosition + addedLength) >= maxLength)
				addedLength = maxLength - l;
			if (addedLength>0)
				x_memcpy(str + inPosition, inString, addedLength);

			// How much was actually added
			addedLength = ((l+addedLength)<maxLength) ? (addedLength) : (maxLength-l);
		}
		__const_base::mBuffer->setLengthInBytes(l + addedLength);
		__const_base::mBuffer->terminateWithZero();
	}


	void			xstring_mutable_base::insert	(u32 inPosition, const xstring& inString)
	{
		insert(inPosition, inString.c_str(), inString.len());
	}


	void			xstring_mutable_base::insert	(u32 inPosition, const xcstring& inString)
	{
		insert(inPosition, inString.c_str(), inString.len());
	}


	void			xstring_mutable_base::insert	(u32 inPosition, const xccstring& inString)
	{
		insert(inPosition, inString.c_str(), inString.len());
	}


	void			xstring_mutable_base::remove	(u32 inStart, u32 inLength)
	{
		XBOUNDS(inStart, 0,__const_base::len() - 1);
		XBOUNDS(inLength, 0,__const_base::len());
		XBOUNDS(inStart + inLength, 0,__const_base::len());

		if (inStart == 0 && inLength == __const_base::len())
		{
			__const_base::mBuffer->unbind();
			__const_base::mBuffer->setEmpty();
			return;
		}

		s32 l =__const_base::len();
		__const_base::mBuffer->unique();

		// Check if we're removing stuff at the end or in the middle of the string
		if (inStart + inLength != l)
			x_memcpy(__const_base::mBuffer->getPtr() + inStart, __const_base::mBuffer->getPtr() + inStart + inLength, l - (inStart + inLength));

		// Clean up
		__const_base::mBuffer->setLengthInBytes(l-inLength);
		__const_base::mBuffer->terminateWithZero();
	}

	template <class T>
	void			xstring_mutable_base::erase (s32 inStart, s32 inLength)
	{
		if (inLength == -1)
			inLength = __const_base::len()- inStart;
		remove(inStart, inLength);
	}


	void			xstring_mutable_base::remove	(const char* inCharSet)
	{
		// Find how much initial chars we have to skip
		s32 strPos = __const_base::findOneOf(inCharSet);
		if (strPos!=-1)
		{
			__const_base::mBuffer->unique();
			xccstring::data charSetStrData((const ustr8*)inCharSet);
			xccstring charSetStr = charSetStrData.get();
			while (strPos!=-1)
			{
				const s32 l =__const_base::len();
				s32 nextPos = strPos+1;
				while (nextPos<l && charSetStr.contains(__const_base::mBuffer->getPtr()[nextPos]))
					nextPos++;
				nextPos--;

				// Two cases
				// 1) We have more characters and last found character is at the end of the string
				// 2) We have more characters and next found character is not at the end of the string
				if (nextPos==(l-1))
				{
					// Remove range [strPos, EOS] characters
					__const_base::mBuffer->setLengthInBytes(strPos);
					__const_base::mBuffer->terminateWithZero();
					break;
				}
				else
				{
					// Remove range [strPos, nextPos] characters, nextPos!=EOS
					const s32 afterNextPos = nextPos+1;
					x_memmove(__const_base::mBuffer->getPtr()+strPos, __const_base::mBuffer->getPtr()+afterNextPos, l-afterNextPos);
					__const_base::mBuffer->setLengthInBytes(l - (afterNextPos-strPos));
					__const_base::mBuffer->terminateWithZero();
				}
				strPos = __const_base::findOneOf(inCharSet);
			}
		}
	}


	void			xstring_mutable_base::upper(void)
	{
		__const_base::mBuffer->unique();
		for (s32 i=0; i<__const_base::mBuffer->len(); ++i)
			__const_base::mBuffer->getPtr()[i] = x_toupper(__const_base::mBuffer->getPtr()[i]);
	}


	void			xstring_mutable_base::lower(void)
	{
		__const_base::mBuffer->unique();
		for (s32 i=0; i<__const_base::mBuffer->len(); ++i)
			__const_base::mBuffer->getPtr()[i] = (char)x_tolower(__const_base::mBuffer->getPtr()[i]);
	}


	void			xstring_mutable_base::capitalize	(void)
	{
		capitalize(NULL);
	}


	void			xstring_mutable_base::capitalize	(const char* inSeperators)
	{
		__const_base::mBuffer->unique();

		// Capitalize first letter of every word
		if (__const_base::mBuffer->len() > 0)
		{
			for (int i=0; i<__const_base::mBuffer->len(); ++i)
			{
				__const_base::mBuffer->getPtr()[i] = x_toupper(__const_base::mBuffer->getPtr()[i]);

				// Lower rest of word
				for (s32 j=i+1; j<__const_base::mBuffer->len(); ++j,++i)
				{
					if (inSeperators!=NULL && x_strFindOneOf(&(__const_base::mBuffer->getPtr()[j]), inSeperators, 0, 1)!=NULL)
					{
						i = j;
						break;
					}
					__const_base::mBuffer->getPtr()[j] = x_tolower(__const_base::mBuffer->getPtr()[j]);
				}
			}
		}
	}


	void			xstring_mutable_base::trim(void)
	{
		trimRight();
		trimLeft();
	}


	void			xstring_mutable_base::trimLeft(void)
	{
		// Find how much initial spaces we have to skip
		s32 skip=0;
		for (; skip < __const_base::len(); ++skip)
			if (!x_isspace(__const_base::getAt(skip)))
				break;

		// Move memory down in string and set new string length
		if (skip)
		{
			__const_base::mBuffer->unique();
			x_memmove(__const_base::mBuffer->getPtr(), __const_base::mBuffer->getPtr()+skip,__const_base::len()-skip+1);
			__const_base::mBuffer->setLength(__const_base::len()-skip);
		}
	}


	void			xstring_mutable_base::trimRight(void)
	{
		// Check how many spaces at the end can be cropped
		s32 new_len=__const_base::len();
		for (; new_len>0; new_len--)
			if (!x_isspace(__const_base::getAt(new_len-1)))
				break;

		// Set new length
		if (new_len !=__const_base::len())
		{
			__const_base::mBuffer->unique();
			__const_base::mBuffer->setLength(new_len);
			__const_base::mBuffer->terminateWithZero();
		}
	}


	void			xstring_mutable_base::trim(char inChar)
	{
		trimRight(inChar);
		trimLeft(inChar);
	}


	void			xstring_mutable_base::trimLeft(char inChar)
	{
		// Check how many spaces at the end can be cropped
		s32 skip=0;
		for (; skip<__const_base::len(); ++skip)
			if (__const_base::getAt(skip) != inChar)
				break;

		// Move memory down in string and set new string length
		if (skip)
		{
			__const_base::mBuffer->unique();
			x_memmove(__const_base::mBuffer->getPtr(), __const_base::mBuffer->getPtr()+skip,__const_base::len()-skip+1);
			__const_base::mBuffer->setLength(__const_base::len()-skip);
		}
	}


	void			xstring_mutable_base::trimRight(char inChar)
	{
		// Check how many spaces at the end can be cropped
		s32 new_len=__const_base::len();
		for (; new_len>0; new_len--)
			if (__const_base::getAt(new_len-1) != inChar)
				break;

		// Set new length
		if (new_len !=__const_base::len())
		{
			__const_base::mBuffer->unique();
			__const_base::mBuffer->setLength(new_len);
			__const_base::mBuffer->terminateWithZero();
		}
	}


	void			xstring_mutable_base::trim(const char* inCharSet)
	{
		trimRight(inCharSet);
		trimLeft(inCharSet);
	}


	void			xstring_mutable_base::trimLeft(const char* inCharSet)
	{
		// Check how many spaces at the end can be cropped
		s32 skip=0;
		for (; skip<__const_base::len(); ++skip)
			if (!x_strFind(inCharSet, __const_base::getAt(skip)))
				break;

		// Move memory down in string and set new string length
		if (skip)
		{
			__const_base::mBuffer->unique();
			x_memmove(__const_base::mBuffer->getPtr(), __const_base::mBuffer->getPtr()+skip,__const_base::len()-skip+1);
			__const_base::mBuffer->setLength(__const_base::len()-skip);
		}
	}


	void			xstring_mutable_base::trimRight(const char* inCharSet)
	{
		// Check how many spaces at the end can be cropped
		s32 new_len=__const_base::len();
		for (; new_len>0; new_len--)
			if (x_strFind(inCharSet, __const_base::getAt(new_len-1))==NULL)
				break;

		// Set new length
		if (new_len !=__const_base::len())
		{
			__const_base::mBuffer->unique();
			__const_base::mBuffer->setLength(new_len);
			__const_base::mBuffer->terminateWithZero();
		}
	}


	void			xstring_mutable_base::trimQuotes	(void)
	{
		trimQuotes('\"');
	}


	void			xstring_mutable_base::trimQuotes(char inQuote)
	{
		return trimDelimiters(inQuote, inQuote);
	}


	void			xstring_mutable_base::trimDelimiters(char inLeft, char inRight)
	{
		if (__const_base::isDelimited(inLeft, inRight))
		{
			__const_base::mBuffer->unique();
			x_memmove(__const_base::mBuffer->getPtr(), __const_base::mBuffer->getPtr()+1,__const_base::len()-1);
			__const_base::mBuffer->setLength(__const_base::len()-2);
			__const_base::mBuffer->terminateWithZero();
		}
	}


	void			xstring_mutable_base::reverse(void)
	{
		__const_base::mBuffer->unique();
		for (s32 i1 = 0, i2 =__const_base::len() - 1; i1 < i2; ++i1, --i2)
			x_swap(__const_base::mBuffer->getPtr()[i1], __const_base::mBuffer->getPtr()[i2]);
	}


	void			xstring_mutable_base::reverse(u32 inPosition, s32 inCharNum)
	{
		if (inCharNum == -1)
			inCharNum =__const_base::len() - inPosition;
		else if (inPosition+inCharNum >__const_base::len())
			inCharNum =__const_base::len() - inPosition;

		char* str = __const_base::mBuffer->getPtr();
		for (s32 i1 = inPosition, i2 = inPosition + inCharNum - 1; i1 < i2; ++i1, --i2)
			x_swap(str[i1], str[i2]);

	}



	bool			xstring_mutable_base::splitOn(char inChar, xstring& outRight)
	{
		// Find the split character
		s32 split_pos = __const_base::find(inChar);
		if (split_pos == -1) 
			return xFALSE;

		// Split string
		split(split_pos, xTRUE, outRight);
		return xTRUE;
	}


	bool			xstring_mutable_base::splitOn(char inChar, xstring& outLeft, xstring& outRight) const
	{
		return __const_base::splitOn(inChar, outLeft, outRight);
	}


	bool			xstring_mutable_base::rsplitOn(char inChar, xstring& outRight)
	{
		// Find the split character
		s32 split_pos = __const_base::rfind(inChar);
		if (split_pos == -1) 
			return xFALSE;

		// Split string
		split(split_pos, xTRUE, outRight);
		return xTRUE;
	}


	bool			xstring_mutable_base::rsplitOn(char inChar, xstring& outLeft, xstring& outRight) const
	{
		return __const_base::rsplitOn(inChar, outLeft, outRight);
	}


	void			xstring_mutable_base::split(u32 inPosition, bool inRemove, xstring& outRight)
	{
		XBOUNDS(inPosition, 0,__const_base::len());
		XBOUNDS(inPosition + ((s32)inRemove), 0,__const_base::len());
		ASSERT((s32)&outRight != (s32)this);

		__const_base::mBuffer->unique();
		__const_base::mid(inPosition + ((s32)inRemove), outRight);
		__const_base::mBuffer->setLength(inPosition);
		__const_base::mBuffer->terminateWithZero();
	}


	void			xstring_mutable_base::split(u32 inPosition, bool inRemove, xstring& outLeft, xstring& outRight) const
	{
		__const_base::split(inPosition, inRemove, outLeft, outRight);
	}


	void            xstring_mutable_base::copy(const char* inString, s32 inLength)
	{
		ASSERT(inLength >= 0);
		ASSERT(inString);
		if (inString==NULL || inLength<0)
			return;

		// Check if we're assigning ourselves an empty string, in that case just reuse the global empty string buffer
		if (inLength==0 || *inString=='\0')
		{
			// Reference the one and only empty string buffer
			__const_base::mBuffer->unbind();
			__const_base::mBuffer->setEmpty();
		}
		else
		{
			s32 length = inLength;
			if (__const_base::mBuffer->isGrowable())
			{
				// Copy <inString> to this string
				__const_base::mBuffer->resize(length);
			}
			else
			{
				if (length>getMaxLength())
					length = getMaxLength();
			}
			x_memcpy(__const_base::mBuffer->getPtr(), inString, length);
			__const_base::mBuffer->setLength(length);
			__const_base::mBuffer->terminateWithZero();
		}
	}


	void            xstring_mutable_base::copy(const char* str)
	{
		copy(str, x_strlen(str));
	}


	void            xstring_mutable_base::copy(const xstring& str)
	{
		copy(str.c_str(), str.len());
	}


	void            xstring_mutable_base::copy(const xcstring& str)
	{
		copy(str.c_str(), str.len());
	}


	void            xstring_mutable_base::copy(const xccstring& str)
	{
		copy(str.c_str(), str.len());
	}


	void			xstring_mutable_base::indexToRowCol(s32 index, s32& row, s32& col) const
	{
		ASSERT(index >= 0);

		const char* buffer = __const_base::c_str();
		if (buffer!=NULL && index <__const_base::len())
		{
			s32 scan = 0;
			row = 1;
			col = 1;

			while (scan < index)
			{
				if (buffer[scan] == '\n')
				{
					row++;
					col = 1;
				}
				else
				{
					col++;
				}
				scan++;
			}
		}
		else
		{
			row = -1;
			col = -1;
		}
	}


	void			xstring_mutable_base::concat(const char* inSource)
	{
		concat(inSource, x_strlen(inSource));
	}


	void			xstring_mutable_base::concat(const char* inSource, s32 inLength)
	{
		ASSERT(inSource);
		ASSERT(inLength >= 0 && inLength <= x_strlen(inSource));
		ASSERT(__const_base::len() >= 0);

		s32 length =__const_base::len();
		s32 addedLength = inLength;
		s32 newTotalLen = length+addedLength;
		if (__const_base::mBuffer->isGrowable())
		{
			__const_base::mBuffer->resize(newTotalLen);
		}
		else
		{
			if (newTotalLen >= getMaxLength())
				newTotalLen = getMaxLength() - length;
		}
		if (addedLength > 0)
		{
			x_memcpy(__const_base::mBuffer->getPtr() + length, inSource, addedLength * sizeof(char));
			__const_base::mBuffer->setLength(newTotalLen);
			__const_base::mBuffer->terminateWithZero();
		}
	}

	//------------------------------------------------------------------------------

	uptr8				xstring_mutable_base::begin()
	{
		return mBuffer->getPtr(); 
	}


	//------------------------------------------------------------------------------

	uptr8				xstring_mutable_base::end()
	{
		return mBuffer->getPtr() + mBuffer->getLength(); 
	}


	//------------------------------------------------------------------------------

	inline xcstring::xcstring(void* buffer, s32 bufferSize)
	{
		mBuffer = (xstring_buffer_char(buffer, bufferSize));
	}

	//------------------------------------------------------------------------------

	inline xcstring::xcstring(void* buffer, s32 bufferSize, const char* str)
	{
		mBuffer = (xstring_buffer_char(buffer, bufferSize, str));
	}

	//------------------------------------------------------------------------------

	inline xcstring::xcstring(void* buffer, s32 bufferSize, const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		mBuffer = (xstring_buffer_char(buffer, bufferSize, formatString,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10));
	}

	//------------------------------------------------------------------------------

	inline xcstring::xcstring(void* buffer, s32 bufferSize, const char* formatString, const x_va_list& args)
	{
		mBuffer = (xstring_buffer_char(buffer, bufferSize, formatString,args));
	}

	//------------------------------------------------------------------------------
	inline xcstring::xcstring(void* buffer, s32 bufferSize, const xstring& other)
	{
		mBuffer = (xstring_buffer_char(buffer, bufferSize));
		mBuffer->copyFrom(other.c_str(), other.len());
	}

	//------------------------------------------------------------------------------
	inline xcstring::xcstring(void* buffer, s32 bufferSize, const xcstring& other)
	{
		mBuffer = (xstring_buffer_char(buffer, bufferSize));
		mBuffer->copyFrom(other.c_str(), other.len());
	}

	//------------------------------------------------------------------------------
	inline xcstring::xcstring(void* buffer, s32 bufferSize, const xccstring& other)
	{
		x_iallocator* allocator = x_iallocator::default();
		mBuffer = (xstring_buffer_char(buffer, bufferSize));
		mBuffer->copyFrom(other.c_str(), other.len());
	}

	//------------------------------------------------------------------------------

	inline uchar8			xcstring::operator[] (s32 inIndex) const
	{
		if (inIndex>=0 && inIndex<len().clen())
			return *(mBuffer->getPtr() + (u32)inIndex);
		return uchar8('\0');
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator=(const xstring& inRHS)
	{
		mBuffer->setLength(ulen8(0));
		*this += inRHS;
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator=(const xcstring& inRHS)
	{
		mBuffer->setLength(ulen8(0));
		*this += inRHS;
		return *this;
	}


	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator=(const xccstring& inRHS)
	{
		mBuffer->setLength(ulen8(0));
		*this += inRHS;
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator=(char inRHS)
	{
		mBuffer->copyFrom(&inRHS, 1);
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator=(const char* inRHS)
	{
		mBuffer->copyFrom(inRHS, x_strlen(inRHS));
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator+=(char inRHS)
	{
		concat(&inRHS, 1);
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator+=(const char* inRHS)
	{
		concat(inRHS);
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator+=(const xstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator+=(const xcstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	inline xcstring&		xcstring::operator+=(const xccstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	inline uchar8 xccstring::operator[](s32 inIndex) const
	{
		if (inIndex>= 0 && inIndex < len().clen())
		{
			ucptr8 ptr = mBuffer->getCPtr() + inIndex;
			return *ptr;
		}
		return uchar8('\0');
	}

	//------------------------------------------------------------------------------

	inline xccstring& xccstring::operator = (const char* str)
	{
		data* d = (data*)mBuffer;
		d->set((ustr8*)str, ulen8::strlen((const ustr8*)str));
		return *this;
	}


	/**
	 * Global xstring operators
	 *
	 * operator +
	 */

	inline xstring		operator+ (const xstring&		inLHS, char					inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), &inRHS, 1); }
	inline xstring		operator+ (const xstring&		inLHS, const char*			inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS, x_strlen(inRHS)); }
	inline xstring		operator+ (const xstring&		inLHS, const xstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }
	inline xstring		operator+ (const xstring&		inLHS, const xcstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }
	inline xstring		operator+ (const xstring&		inLHS, const xccstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }

	inline xstring		operator+ (const xcstring&		inLHS, char					inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), &inRHS, 1); }
	inline xstring		operator+ (const xcstring&		inLHS, const char*			inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS, x_strlen(inRHS)); }
	inline xstring		operator+ (const xcstring&		inLHS, const xstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }
	inline xstring		operator+ (const xcstring&		inLHS, const xcstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }
	inline xstring		operator+ (const xcstring&		inLHS, const xccstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }

	inline xstring		operator+ (const xccstring&		inLHS, char					inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), &inRHS, 1); }
	inline xstring		operator+ (const xccstring&		inLHS, const char*			inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS, x_strlen(inRHS)); }
	inline xstring		operator+ (const xccstring&		inLHS, const xstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }
	inline xstring		operator+ (const xccstring&		inLHS, const xcstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }
	inline xstring		operator+ (const xccstring&		inLHS, const xccstring&		inRHS)	{ return xstring(inLHS.c_str(), inLHS.len(), inRHS.c_str(), inRHS.len()); }

	/**
	 * Global xstring operators
	 *
	 * operator <
	 */

	inline bool				operator< (const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
	inline bool				operator< (const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }

	/**
	 * Global xstring operators
	 *
	 * operator >
	 */

	inline bool				operator> (const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
	inline bool				operator> (const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }

	/**
	 * Global xstring operators
	 *
	 * operator <=
	 */

	inline bool				operator<=(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
	inline bool				operator<=(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }

	/**
	 * Global xstring operators
	 *
	 * operator >=
	 */

	inline bool				operator>=(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
	inline bool				operator>=(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }

	/**
	 * Global xstring operators
	 *
	 * operator ==
	 */

	inline bool				operator==(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
	inline bool				operator==(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }

	/**
	 * Global xstring operators
	 *
	 * operator !=
	 */

	inline bool				operator!=(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
	inline bool				operator!=(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }



	//==============================================================================
	// xstring
	//==============================================================================

	//------------------------------------------------------------------------------

	xstring::xstring(void)
	{
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Jurgen Kluft
	 * Arguments:
	 *     str - The string to use to construct this xstring
	 * Returns:
	 *     none
	 * Summary:
	 *     Constructor
	 * Description:
	 *     Construct an xstring from a 'const char*'
	 * See Also:
	 *     Other constructors
	 *------------------------------------------------------------------------------
	 */
	xstring::xstring(const char* str)
	:__base(x_strlen(str), str)
	{
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Jurgen Kluft
	 * Arguments:
	 *     str - The string to use to construct this xstring
	 *     len - The length of 'str' to use
	 * Returns:
	 *     none
	 * Summary:
	 *     Constructor
	 * Description:
	 *     Construct an xstring from 'len' number of characters of 'str' 
	 * See Also:
	 *     Other constructors
	 *------------------------------------------------------------------------------
	 */
	xstring::xstring(const char* str, s32 len)
	:__base(len, str)
	{
	}

	/**
	 *------------------------------------------------------------------------------
	 * Author:
	 *     Jurgen Kluft
	 * Arguments:
	 *     strA - The string to use to construct the first part
	 *     lenA - The length of 'strA' to use
	 *     strB - The string to use concatenate with the first part
	 *     lenB - The length of 'strB' to use
	 * Returns:
	 *     none
	 * Summary:
	 *     Algorithmic Constructor
	 * Description:
	 *     Construct an xstring from 2 strings with a defined length
	 * See Also:
	 *     Other constructors
	 *------------------------------------------------------------------------------
	 */
	xstring::xstring(const char* strA, s32 lenA, const char* strB, s32 lenB)
	:__base(lenA, strA, lenB, strB)
	{
	}

	//------------------------------------------------------------------------------

	xstring::xstring(s32 length)
	:__base(length)
	{
	}

	//------------------------------------------------------------------------------

	xstring::xstring(const xstring& str)
	:__base(str)
	{
	}

	//------------------------------------------------------------------------------

	xstring::xstring(const xcstring& str)
	:__base(str)
	{
	}

	//------------------------------------------------------------------------------
	xstring::xstring(const xccstring& str)
	:__base(str)
	{
	}

	//------------------------------------------------------------------------------

	xstring xstring::sBuildFromFormat(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		xstring outStr;
		outStr.appendFormatV(format, x_va_list(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10));
		return outStr;
	}

	//------------------------------------------------------------------------------

	s32 xstring::format(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return formatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xstring::formatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		xstring_tmp temp(format, args);
		const s32 length = temp.len();

		copy(temp.c_str(), length);

		return length;
	}

	//------------------------------------------------------------------------------

	s32 xstring::appendFormat(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return appendFormatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xstring::appendFormatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		/// Compute the string properties
		const xstring_tmp temp(format, args);
		const s32 length = temp.len();

		concat(temp.c_str(), length);
		return len();
	}

	//------------------------------------------------------------------------------

	char&               xstring::operator [](s32 inIndex)
	{
		ASSERT(inIndex<len()+1);
		return w_str()[inIndex];
	}

	//------------------------------------------------------------------------------

	const char&         xstring::operator [](s32 inIndex) const
	{
		ASSERT(inIndex<len()+1);
		return c_str()[inIndex];
	}


	//------------------------------------------------------------------------------

	xstring&			xstring::operator =(const char* inStr)
	{
		copy(inStr, x_strlen(inStr));
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xstring& inStr)
	{
		copy(inStr.c_str(), inStr.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xstring_tmp& inStr)
	{
		copy(inStr.c_str(), inStr.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xcstring& inStr)
	{
		copy(inStr.c_str(), inStr.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&            xstring::operator =(const xccstring& inStr)
	{
		copy(inStr.c_str(), inStr.len());
		return *this;
	}


	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(char inRHS)
	{
		concat(&inRHS, 1);
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const char* inRHS)
	{
		concat(inRHS);
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xstring_tmp& inRHS)
	{
		concat(inRHS.c_str(), inRHS.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xcstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.len());
		return *this;
	}

	//------------------------------------------------------------------------------

	xstring&			xstring::operator +=(const xccstring& inRHS)
	{
		concat(inRHS.c_str(), inRHS.len());
		return *this;
	}


	//------------------------------------------------------------------------------

	uptr8				xstring::begin()
	{
		return mBuffer->getPtr(); 
	}


	//------------------------------------------------------------------------------

	uptr8				xstring::end()
	{
		return mBuffer->getPtr() + mBuffer->getLength(); 
	}


	//------------------------------------------------------------------------------

	ucptr8				xstring::begin() const
	{
		return mBuffer->getCPtr(); 
	}


	//------------------------------------------------------------------------------

	ucptr8				xstring::end() const
	{
		return mBuffer->getCPtr() + mBuffer->getLength(); 
	}


	//------------------------------------------------------------------------------

	void				xstring::terminateWithZero(void)
	{
		mBuffer->terminateWithZero(); 
	}



	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------

	s32 xcstring::format(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return formatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xcstring::formatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		xstring_tmp temp(format, args);
		const s32 length = temp.len();

		copy(temp.c_str(), length);

		return length;
	}

	//------------------------------------------------------------------------------

	s32 xcstring::appendFormat(const char* format, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
	{
		ASSERT(format);

		x_va_list args(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10);
		return appendFormatV(format, args);
	}

	//------------------------------------------------------------------------------

	s32 xcstring::appendFormatV(const char* format, const x_va_list& args)
	{
		ASSERT(format);

		/// Compute the string properties
		const xstring_tmp temp(format, args);
		const s32 length = temp.len();

		concat(temp.c_str(), length);
		return len();
	}
};
/**
 *  END xCore namespace
 */

#endif // ifndef SPU