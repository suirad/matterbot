#include "Md5Utilities.h"
#include "Md5.h"
#include <locale>
#include <vector>

using namespace std;
using namespace lospi;

wstring lospi::string_to_wstring(const string &s)
{
	return wstring(s.begin(), s.end());
}

string lospi::wstring_to_string(const wstring &ws)
{
	return string(ws.begin(), ws.end());
}

Md5Digest lospi::compute_md5(const char* data, unsigned long size)
{
	MD5_CTX context;
	MD5_Init(&context);
	MD5_Update(&context, data, size);
	Md5Digest result;
	MD5_Final(result.data(), &context);
	return result;
}

Md5Digest lospi::get_md5_from_wstr(const wstring &md5_str)
{
	Md5Digest result;
	wchar_t temp[3] = { 0 };
	for (auto i = 0; i<32; i += 2)
	{
		temp[0] = md5_str[i];
		temp[1] = md5_str[i + 1];
		auto as_ulong = wcstoul(temp, nullptr, 16);
		result[i / 2] = static_cast<unsigned char>(as_ulong);
	}
	return result;
}

Md5Digest lospi::get_md5_from_str(const string &md5_str)
{
	Md5Digest result;
	wchar_t temp[3] = { 0 };
	for (auto i = 0; i<32; i += 2)
	{
		temp[0] = md5_str[i];
		temp[1] = md5_str[i + 1];
		auto as_ulong = wcstoul(temp, nullptr, 16);
		result[i / 2] = static_cast<unsigned char>(as_ulong);
	}
	return result;
}


wstring lospi::get_wstr_from_md5(const Md5Digest &md5_str)
{
	wstring result(34, '0');
	for (auto i = 0; i<md5_str.size(); i++)
	{
		auto md5_val = md5_str[i];
		if (md5_val < 16)
		{
			wchar_t temp[2];
			_itow_s(md5_str[i], temp, 2, 16);
			result[2 * i] = '0';
			result[2 * i + 1] = temp[0];
		}
		else
		{
			_itow_s(md5_str[i], &result[2 * i], 3, 16);
		}
	}
	result.resize(32);
	return result;
}

string lospi::get_str_from_md5(const Md5Digest &md5_str)
{
	string result(34, '0');
	for (auto i = 0; i<md5_str.size(); i++)
	{
		auto md5_val = md5_str[i];
		if (md5_val < 16)
		{
			wchar_t temp[2];
			_itow_s(md5_str[i], temp, 2, 16);
			result[2 * i] = '0';
			result[2 * i + 1] = (char)temp[0];
		}
		else
		{
			_itoa_s(md5_str[i], &result[2 * i], 3, 16);
		}
	}
	result.resize(32);
	return result;
}

uint128_t lospi::minify_md5(const Md5Digest &md5) {
	uint128_t ret = { 0,0 };
	for (int i = 0; i < 8; i++) {
		ret[0] <<= 8;
		ret[0] |= md5[i];
	}
	for (int i = 8; i < 15; i++) {
		ret[1] <<= 8;
		ret[1] |= md5[i];
	}
	return ret;
}


bool lospi::operator==(uint128_t one, uint128_t two) {
	return (one[0] == two[0] && one[1] == two[1]);
}

Combinator::Combinator(std::string alphabet, size_t length)
	: alphabet{ alphabet },
	length{ length },
	combination_indices(length, 0),
	combination(length, alphabet[0]),
	has_next_flag{ true } {
}

bool Combinator::has_next() const {
	return has_next_flag;
}

vector<char> Combinator::next() {
	auto result{ combination };
	has_next_flag = could_increment();
	return result;
}

bool Combinator::could_increment(int index) {
	if (index == length) return false;
	combination_indices[index]++;
	if (combination_indices[index] == alphabet.size())
	{
		index++;
		fill_n(combination_indices.begin(), index, 0);
		fill_n(combination.begin(), index, alphabet[0]);
		return could_increment(index);
	}
	combination[index] = alphabet[combination_indices[index]];
	return true;
}