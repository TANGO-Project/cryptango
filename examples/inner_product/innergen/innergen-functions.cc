#include <chrono>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <NTL/ZZ.h>
#include "innergen.h"
#include "InnergenMapTask.h"

void run_map_task(int rho, int d, int numLines, file secrets, file parameters, file outputPath, file resultsPath){
	auto start = std::chrono::high_resolution_clock::now();

	//Read secrets into string
	std::ifstream secrets_ifs(secrets);
	std::string secrets_str,secrets_line;
	while(getline(secrets_ifs,secrets_line)) secrets_str+=secrets_line;
	secrets_ifs.close();

	//Read parameters into string
	std::ifstream params_ifs(parameters);
	std::string params_str,params_line;
	while(getline(params_ifs,params_line)) params_str+=params_line;
	params_ifs.close();

	InnergenMapTask mapTask(rho, d, numLines, secrets_str, params_str, outputPath);
	NTL::ZZ result = mapTask.run();
	auto finish = std::chrono::high_resolution_clock::now();
	auto totalTaskTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
	Json::Value root;
	std::stringstream ss;
	ss << result;
	root["result"] = ss.str();
	root["task_execution_time"] = Json::UInt64(totalTaskTime);
	root["number_of_inputs"] = Json::UInt64(mapTask.numberEncrypted);
	root["total_encryption_time"] = Json::UInt64(mapTask.totalEncryptionTime);
	std::ofstream results_ofs(resultsPath);
	if(results_ofs.is_open()){
		results_ofs << root;
		results_ofs.close();
	}
}

