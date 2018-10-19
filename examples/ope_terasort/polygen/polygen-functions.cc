#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "polygen.h"
#include "PolygenMapTask.h"

void run_map_task(int mu, int n, int numOutputs, file secrets, file output, file timings){
	auto start = std::chrono::high_resolution_clock::now();
	std::ifstream ifs(secrets);
	std::string str,line;
	while(getline(ifs,line)) str+=line;
	PolygenMapTask mapTask(mu, n, numOutputs, str, output);
	mapTask.run();
	auto finish = std::chrono::high_resolution_clock::now();
	auto totalTaskTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	Json::Value root;
	root["task_execution_time"] = Json::UInt64(totalTaskTime);
	root["number_encrypted"] = Json::UInt64(mapTask.numberEncrypted);
	root["total_encryption_time"] = Json::UInt64(mapTask.totalEncryptionTime);
	Json::FastWriter writer;
	std::string json = writer.write(root);
	std::ofstream ofs(timings);
	if (ofs.is_open()){
		ofs << json;
		ofs.close();
	}
}
