#pragma once
#include <array>
#include <unordered_map>

namespace lospi
{
	typedef std::array<unsigned char, 16> Md5Digest;
	typedef std::array<std::uint64_t, 2> uint128_t;
	Md5Digest compute_md5(const char *data, unsigned long size);
	Md5Digest get_md5_from_str(const std::string &md5_str);
	Md5Digest get_md5_from_wstr(const std::wstring &md5_str);
	uint128_t minify_md5(const Md5Digest &md5);
	bool operator==(uint128_t one, uint128_t two);
	std::wstring get_wstr_from_md5(const Md5Digest &md5_str);
	std::string get_str_from_md5(const Md5Digest &md5_str);
	std::string wstring_to_string(const std::wstring &other);
	std::wstring string_to_wstring(const std::string &s);

	struct Combinator {
		Combinator(std::string alphabet, size_t length);
		bool has_next() const;
		std::vector<char> next();
	private:
		bool could_increment(int index = 0);
		const std::string alphabet;
		const size_t length;
		std::vector<uint8_t> combination_indices;
		std::vector<char> combination;
		bool has_next_flag;
	};

}