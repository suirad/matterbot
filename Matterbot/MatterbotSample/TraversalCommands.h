#pragma once
#include "Matterbot.h"

namespace lospi {
	struct Pwd;
	struct Chd;
	struct Dir;
	struct TraversalCommands : ICommand {
		TraversalCommands(std::shared_ptr<Matterbot> bot);
		~TraversalCommands();
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring& user, const std::wstring& command_text)override;
		std::shared_ptr<Pwd>& get_pwd() { return pwd; }
		std::shared_ptr<Chd>& get_chd() { return chd; }
		std::shared_ptr<Dir>& get_dir() { return dir; }

	private:
		std::shared_ptr<Matterbot> bot;
		std::shared_ptr<Pwd> pwd;
		std::shared_ptr<Chd> chd;
		std::shared_ptr<Dir> dir;
	};

	struct Pwd : ICommand {
		explicit Pwd(TraversalCommands& traversal);
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring & user, const std::wstring& command_text) override;
		std::wstring get_dir();

	private:
		TraversalCommands& main;
	};

	struct Chd : ICommand{
		explicit Chd(TraversalCommands& traversal);
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring & user, const std::wstring& command_text) override;
	private:
		TraversalCommands& main;
	};

	struct Dir : ICommand {
		explicit Dir(TraversalCommands& traversal);
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring& team, const std::wstring& channel,
			const std::wstring & user, const std::wstring& command_text) override;
		std::wstring dir_list();

	private:
		TraversalCommands& main;
	};
}
