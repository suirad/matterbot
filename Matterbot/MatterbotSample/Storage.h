#pragma once
#include <string>
#include <vector>

namespace darius {
	class IStorage {
	public:
		virtual ~IStorage() {};
		virtual void die() = 0;
		virtual int getCompleted() = 0;
		virtual void setMax(int max) = 0;
		virtual std::vector<std::wstring>* getPassword(std::vector<std::string> &&strs) = 0;
		//virtual void generate(std::shared_ptr<void *> ptr) = 0;
	};
}