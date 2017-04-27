#include "Matterbot.h"
#include "TraversalCommands.h"
#include <direct.h>
#include <Windows.h>

using namespace std;
using namespace lospi;

TraversalCommands::TraversalCommands(std::shared_ptr<Matterbot> bot) : bot{ bot },
		pwd{ NULL }{
	pwd = make_shared<Pwd>(*this);
	bot->register_command(pwd);
	chd = make_shared<Chd>(*this);
	bot->register_command(chd);
	dir = make_shared<Dir>(*this);
	bot->register_command(dir);
}

TraversalCommands::~TraversalCommands() {
}


std::wstring TraversalCommands::get_name(){
	return L"?"; 
}

std::wstring TraversalCommands::get_help(){ 
	return L"`? [COMMAND]`: `?` will respond with the extended help of traversal commands.";
}

std::wstring TraversalCommands::handle_command(const std::wstring& team, const std::wstring& channel,
		const std::wstring& user, const std::wstring& command_text){
	//list commands and return them.
	wstring ret = L"";
	ret += pwd->get_help() + L"\n";
	ret += chd->get_help() + L"\n";
	ret += dir->get_help();
	return ret;
}

Pwd::Pwd(TraversalCommands& traversal) : main{ traversal } {
}

std::wstring Pwd::get_name() { 
	return L"pwd"; 
}

std::wstring Pwd::get_help() { 
	return L"`pwd` : `pwd` will respond with the present working directory of traversal."; 
}

std::wstring Pwd::handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring & user, const std::wstring& command_text){
	return L"`" + this->get_dir() + L"`";
}

std::wstring Pwd::get_dir() {
	wchar_t* dir = _wgetcwd(NULL, 0);
	if (!dir) {
		return std::wstring(L"ERR-PWD");
	}
	return std::wstring(dir);
}


Chd::Chd(TraversalCommands& traversal) : main{ traversal } {}

std::wstring Chd::get_name() {
	return L"cd";
}

std::wstring Chd::get_help() {
	return L"`cd` : `cd` will change the current directory and print it";
}

std::wstring Chd::handle_command(const std::wstring& team, const std::wstring& channel,
	const std::wstring & user, const std::wstring& command_text) {
	wstring ret =L"";
	if (_wchdir(command_text.c_str())==0){
		ret+= L"Changed dir to ~ `" + 
			this->main.get_pwd()->get_dir() + L"`";
	} else {
		ret+= L"Invalid dir change";
	}
	return ret;
}


Dir::Dir(TraversalCommands& traversal) : main{ traversal } {}

std::wstring Dir::get_name() {
	return L"dir";
}

std::wstring Dir::get_help() {
	return L"`dir` : `dir` will list the files in the current directory";
}

std::wstring Dir::handle_command(const std::wstring& team, const std::wstring& channel,
	const std::wstring & user, const std::wstring& command_text) {
	wstring spath = this->main.get_pwd()->get_dir() + L"/*.*",
		ret = L"";
	WIN32_FIND_DATA fd;
	HANDLE hfind = ::FindFirstFile(spath.c_str(), &fd);
	if (hfind != INVALID_HANDLE_VALUE) {
		ret += L"```\n";
		do {
			ret += fd.cFileName;
			ret += L"\n";
		} while (::FindNextFile(hfind, &fd));
		::FindClose(hfind);
		ret += L"```";
	}
	else {
		ret += L"ERR-DIR";
	}
	return ret;
}

