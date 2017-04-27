#include "Matterbot.h"
#include <iostream>
#include <sstream>
#include "ConsoleLogger.h"
#include "EchoCommand.h"
#include "ReverseCommand.h"
#include "TraversalCommands.h"
#include "Rivestment.h"

#define ERROR_SUCCESS 0
#define ERROR_FAILURE -1

using namespace std;
using namespace lospi;

int main() {
	wstring mattermost_url = L"https://hooks.slack.com",
		incoming_hook_route = L"",
		outgoing_hook_route = L"http://127.0.0.1:5000/",
		outgoing_hook_token = L"",
		welcome_message = L"sup.";

	try {
		auto bot = make_shared<Matterbot>(mattermost_url, incoming_hook_route, outgoing_hook_route, outgoing_hook_token);
		bot->set_logger(make_unique<ConsoleLogger>());
		bot->register_command(make_shared<EchoCommand>());
		bot->register_command(make_shared<ReverseCommand>(bot));
		bot->register_command(make_shared<TraversalCommands>(bot));
		auto riv = make_shared<Rivestment>(bot);
		bot->register_command(riv);
		bot->post_message(welcome_message);


		wstring console;
		wclog << ">> Type \'quit\' to quit. Any other input will be sent to the bot." << endl;
		while (getline(wcin, console)) {
			if (L"quit" == console) {
				wclog << ">> Quitting." << endl;
				return ERROR_SUCCESS;
			}
			else if (L"hash " == console.substr(0, 5)) {
				auto msg = riv->handle_command(L"", L"", L"", console.substr(5, console.length()));
				wcout << msg << endl;
			}
			else if (L"" != console) {
				bot->post_message(console);
			}
		}
	}
	catch (exception e) {
			wcerr << "[-] Unhandled exception:" << e.what() << endl;
			return ERROR_FAILURE;
		}
	}
