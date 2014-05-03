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