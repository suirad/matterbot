#include "Matterbot.h"
#include "Rivestment.h"

using namespace std;
using namespace lospi;

std::wstring Rivtoggle::get_name() {
	return L"toggle";
}

std::wstring Rivtoggle::get_help() {
	return L"`toggle` : `toggle` Starts/Stops Rivestment automation";
}

std::wstring Rivtoggle::handle_command(const std::wstring &team, const std::wstring &channel,
	const std::wstring &user, const std::wstring &command_text) {
	if (riv.toggle()) {
		riv.get_bot()->post_message(L"Turned on Botting");
		riv.get_bot()->post_message(L"rivestment register notbot");
		return L"rivestment password";
	}
	else {
		return L"Turned off botting";
	}
}


std::wstring Rivpass::get_name() {
	return L"password";
}

std::wstring Rivpass::get_help() {
	return L"`password` : `password` Sets the active password for rivestment";
}

std::wstring Rivpass::handle_command(const std::wstring &team, const std::wstring &channel,
	const std::wstring &user, const std::wstring &command_text) {
	riv.password(command_text);
	return L"Changed pass to: " + command_text;
}

std::wstring Rivscraps::get_name() {
	return L"scraps";
}

std::wstring Rivscraps::get_help() {
	return L"`scraps` : `scraps` Attempts to consume scraps";
}

std::wstring Rivscraps::handle_command(const std::wstring &team, const std::wstring &channel,
	const std::wstring &user, const std::wstring &command_text) {
	if (!command_text.empty()) {
		riv.handle_command(L"", L"", L"", command_text);
	}
	return L"Consumed scraps";
}
