#pragma once

namespace lospi {
	struct Rivestment;
	struct Rivtoggle : ICommand {
		Rivtoggle(Rivestment & riv) : riv{ riv } {};
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring &team, const std::wstring &channel,
			const std::wstring &user, const std::wstring &command_text) override;
	private:
		Rivestment & riv;
	};

	struct Rivpass :ICommand {
		Rivpass(Rivestment & riv) : riv{ riv } {};
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring &team, const std::wstring &channel,
			const std::wstring &user, const std::wstring &command_text) override;
	private:
		Rivestment & riv;
	};
	struct Rivscraps :ICommand {
		Rivscraps(Rivestment & riv) : riv{ riv } {};
		std::wstring get_name() override;
		std::wstring get_help() override;
		std::wstring handle_command(const std::wstring &team, const std::wstring &channel,
			const std::wstring &user, const std::wstring &command_text) override;
	private:
		Rivestment & riv;
	};
}