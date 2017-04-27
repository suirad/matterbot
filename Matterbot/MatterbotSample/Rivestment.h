#pragma once
#include "Matterbot.h"
#include "Rivestment_helper.h"
#include "MD5Utilities.h"
#include "MapStorage.h"
#include "SqlStorage.h"
#include <map>
#include <thread>


namespace lospi {
	struct Rivtoggle;
	struct Rivestment : ICommand {
		Rivestment(std::shared_ptr<Matterbot> bot);
		~Rivestment();
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring &team, const std::wstring &channel,
			const std::wstring &user, const std::wstring &command_text) override;
		
		bool toggle() {return this->started = this->started ? false : true; }
		void password(const std::wstring &str);
		std::shared_ptr<Matterbot>& get_bot() { return bot; }

	private:
		bool started = false;
		int activelv = 1, maxlv = 3, scrap = 0;
		std::string passtext;
		std::shared_ptr<Matterbot> bot;
		std::shared_ptr<Rivtoggle> tog;
		std::shared_ptr<Rivpass> pass;
		std::shared_ptr<Rivscraps> scrps;
		darius::MapStorage* stor = NULL;
		std::thread gen;


		std::vector<std::string> parse_hashes(const std::wstring& str);
		std::vector<std::wstring>* fetch_passes(std::vector<std::string> &hashes);

	};
	
}