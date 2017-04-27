#include "Matterbot.h"
#include "Rivestment.h"
#include "MD5.h"
#include "MD5Utilities.h"
#include <vector>
#include "sqlite_modern_cpp\sqlite_modern_cpp.h"
#include "MapStorage.h"

using namespace std;
using namespace lospi;
using namespace darius;

Rivestment::Rivestment(std::shared_ptr<Matterbot> bot) : bot{ bot }, 
tog{ NULL }, pass{ NULL }, passtext{ NULL } {
	tog = make_shared<Rivtoggle>(*this);
	bot->register_command(tog);
	pass = make_shared<Rivpass>(*this);
	bot->register_command(pass);
	scrps = make_shared<Rivscraps>(*this);
	bot->register_command(scrps);
	//password(L"prx4yaaaZZ");
}

Rivestment::~Rivestment() {
}

std::wstring Rivestment::get_name() {
	return L"challenges";
}

std::wstring Rivestment::get_help() {
	return L"`challenges` : Solves challenges provided by rivestment bot";
}

std::wstring Rivestment::handle_command(const std::wstring &team, const std::wstring &channel,
	const std::wstring &user, const std::wstring &command_text){
	if (!started) { return L"Not started"; }
	if (command_text.empty()) { return L"No hashes found."; }
	wstring res = L"";
	vector<string> hashes = parse_hashes(command_text);
	if (hashes.empty()) {
		return L"No hashes found.";
	} else {
		auto passes = fetch_passes(hashes);
		if (!passes) { return L"Error generating passes"; }
		// build response
		res += L"rivestment try ";
		for (auto &it : *passes) {
			res += it + L" ";
		}
		res.pop_back();
		bot->post_message(res);
		delete passes;
		this_thread::sleep_for(1s);
		if (activelv < maxlv && activelv + 10 < stor->getCompleted()) {
			activelv++;
			bot->post_message(L"rivestment level " + to_wstring(activelv));
			this_thread::sleep_for(1s);
		}
		scrap++;
		if (scrap > 10) {
			bot->post_message(L"rivestment scraps");
			this_thread::sleep_for(1s);
			scrap = 0;
		}
	}
	this_thread::sleep_for(1s);
	return L"rivestment challenge 100";
}


vector<wstring>* Rivestment::fetch_passes(std::vector<std::string> &hashes) {
	while (stor->getCompleted() < activelv + 10) {
		this_thread::sleep_for(1s);
	 }
	return stor->getPassword(hashes);
}


void Rivestment::password(const std::wstring &str) {
	if (str == string_to_wstring(passtext)) { return; }
	this->passtext = wstring_to_string(str);
	if (stor != NULL) {
		stor->die();
		gen.detach();
	}
	activelv = 1;
	stor = new MapStorage(str);
	stor->setMax(maxlv);
	gen = thread(&MapStorage::generate, stor);
	bot->post_message(L"rivestment level 1");
	this_thread::sleep_for(1s);
	bot->post_message(L"rivestment challenge 1");
}

std::vector<std::string> Rivestment::parse_hashes(const std::wstring &str) {
	vector<string> hashes;
	if (str.find(L" ") == wstring::npos) {
		hashes.push_back(wstring_to_string(str));
	}
	else {
		string current = wstring_to_string(str.substr(0, str.find(L" ")));
		string rest = wstring_to_string(str.substr(str.find(L" ")+1, str.length() - current.length()+1));
		while (!current.empty()) {
			hashes.push_back(current);

			if (rest.empty()) {
				current = "";
			}
			else if (rest.find(" ") == string::npos) {
				current = rest;
				rest = "";
			}
			else {
				current = rest.substr(0, rest.find(" "));
				rest.erase(0, current.length()+1);
			}
		}
	}
	return hashes;
}

