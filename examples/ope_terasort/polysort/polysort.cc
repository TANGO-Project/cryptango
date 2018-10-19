#include <chrono>
#include <dirent.h>
#include <sys/stat.h>
#include "polysort.h"
#include "CollatedPartitions.h"
#include "PartitionList.h"
#include "PolyCiphertext.h"
#include "PartitionBins.h"
#include "PartitionSampler.h"

inline bool file_exists (const char* name) {
  struct stat buffer;
  return (stat(name, &buffer) == 0);
}

void usage() {
    std::cerr << "[ERROR] Bad number of parameters" << std::endl;
    std::cout << "    Usage: polysort <number of mappers> <number of reducers> <percent of inputs to sample> <input directory> <map output prefix> <reduce output prefix>" << std::endl;
}

Json::Value parse_json(const char* filename){
	std::ifstream ifs(filename, std::ios::in);
	Json::Value root;
	ifs >> root;
	ifs.close();
	return root;
}

void write_json(Json::Value json, const char* filename){
	std::ofstream ofs(filename, std::ios::out);
	ofs << json << endl;
	ofs.close();
}

unsigned long get_task_time(const char* filename){
	std::ifstream ifs(filename);
	unsigned long timing;
	ifs >> timing;
	return timing;
}

int main(int argc, char **argv) {
	std::size_t numMappers, numReducers;
	double percentSamples;
	char* inputDir;
	char* partitionListFile;
	char* mapOutputPrefix;
	char* mapTimingsPrefix;
	char* partitionsPrefix;
	char* outputPrefix;
	char* reduceTimingsPrefix;
	char* timingsPath;
	std::vector<std::string> inputFileNames;

	auto start = std::chrono::high_resolution_clock::now();
	if (argc < 12){
		usage();
		return -1;
	}
	else{
		numMappers = strtoul(argv[1],nullptr,0);
		numReducers = strtoul(argv[2],nullptr,0);
		percentSamples = strtod(argv[3],nullptr);
		inputDir = argv[4];
		partitionListFile = argv[5];
		mapOutputPrefix = argv[6];
		mapTimingsPrefix = argv[7];
		partitionsPrefix = argv[8];
		outputPrefix = argv[9];
		reduceTimingsPrefix = argv[10];
		timingsPath = argv[11];

		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(inputDir))!=nullptr) {
			while ((ent = readdir(dir)) != nullptr) {
				std::stringstream filename;
				filename << inputDir << '/' << ent->d_name;
				if (strcmp(ent->d_name,".") != 0 && strcmp(ent->d_name,"..") != 0)
					inputFileNames.push_back(filename.str());
			}
			std::stable_sort(inputFileNames.begin(),inputFileNames.end());
		}

		std::cout << "Starting polygen with the following configuration:" << std::endl;
		std::cout << "\tNumber of mappers: " << numMappers << std::endl;
		std::cout << "\tNumber of reducers: "<< numReducers << std::endl;
		std::cout << "\tPercentage of samples: "<< percentSamples << std::endl;
		std::cout << "\tInput directory: " << inputDir << std::endl;
		std::cout << "\tPartition list file: " << partitionListFile << std::endl;
		std::cout << "\tMap output prefix: " << mapOutputPrefix << std::endl;
		std::cout << "\tMap timings prefix: " << mapTimingsPrefix << std::endl;
		std::cout << "\tPartitions file prefix: " << partitionsPrefix << std::endl;
		std::cout << "\tReduce output prefix: " << outputPrefix << std::endl;
		std::cout << "\tReduce timings prefix: " << reduceTimingsPrefix << std::endl;
		std::cout << "\tTimings file: " << timingsPath << std::endl;
		std::cout.flush();
	}

	//sample for partitions
	std::cout << "Sampling input files. Approximately " << percentSamples
			<< "% of inputs sampled per file." << std::endl;
	std::cout.flush();
	PartitionSampler sampler(numReducers, percentSamples, &inputFileNames);
	PartitionList list = sampler.createPartitions();

	std::cout << "Partition list: " << list.to_json() << std::endl;

	write_json(list.to_json(),partitionListFile);

	compss_on();

	file listFile = strdup(partitionListFile);
	//invoke map tasks
	std::cout << "Invoking map tasks." << std::endl;
	for (unsigned int i = 0; i < numMappers; i++) {
		std::cout << "Map task " << i << ":" << std::endl;
		std::string filename = inputFileNames[i];
		file splitFile = strdup(filename.c_str());
		std::cout << "\tSplit file: " << splitFile << std::endl;
		std::cout << "\tPartition list file: " << listFile << std::endl;
		std::stringstream mapOutputFilename;
		mapOutputFilename << mapOutputPrefix << '_' << i;
		file mapOutputFile = strdup(mapOutputFilename.str().c_str());
		std::stringstream mapTimingsFilename;
		mapTimingsFilename << mapTimingsPrefix << '_' << i;
		file mapTimingsFile = strdup(mapTimingsFilename.str().c_str());
		run_map_task(splitFile, listFile, mapOutputFile, mapTimingsFile);
	}

	compss_barrier();

	//read map output into objects from JSON
	PartitionBins mapperBins[numMappers];
	unsigned long totalMapTaskTime = 0;
	for (std::size_t i = 0; i < numMappers; i++){
		std::stringstream mapOutputFilename;
		mapOutputFilename << mapOutputPrefix << '_' << i;
		mapperBins[i].from_json(parse_json(mapOutputFilename.str().c_str()));
		std::stringstream mapTimingsFilename;
		mapTimingsFilename << mapTimingsPrefix << '_' << i;
		totalMapTaskTime += get_task_time(mapTimingsFilename.str().c_str());
		std::cout << "\tMap task time: " << totalMapTaskTime << std::endl;
	}

	//invoke reduce tasks
	//collate partitions from each mapper and pass to reducer
	for (std::size_t i = 0; i < list.size()+1; i++) {
		CollatedPartitions unsortedPartition;
		for (std::size_t j = 0; j < numMappers; j++) {
			unsortedPartition.add_partition(mapperBins[j].get_bin(i));
		}
		std::stringstream partitionsFilename;
		partitionsFilename << partitionsPrefix << '_' << i;
		file partitionsFile = strdup(partitionsFilename.str().c_str());
		write_json(unsortedPartition.to_json(), partitionsFile);
		std::stringstream outFile;
		outFile << outputPrefix << '_' << i;
		file output = strdup(outFile.str().c_str());
		std::stringstream reduceTimingsFilename;
		reduceTimingsFilename << reduceTimingsPrefix << '_' << i;
		file reduceTimingsFile = strdup(reduceTimingsFilename.str().c_str());
		run_reduce_task(partitionsFile,output,reduceTimingsFile);
	}

	compss_barrier();

	unsigned long totalReduceTaskTime = 0;
	for (std::size_t i = 0; i < numMappers; i++){
		std::stringstream reduceTimingsFilename;
		reduceTimingsFilename << reduceTimingsPrefix << '_' << i;
		totalReduceTaskTime += get_task_time(reduceTimingsFilename.str().c_str());
	}
	compss_off();

	//write out job statistics
	auto finish = std::chrono::high_resolution_clock::now();
	auto jobExecutionTime = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();

	bool fileExists = file_exists(timingsPath);
	std::ofstream timings(timingsPath,std::ios::out | std::ios::app);
	if (!fileExists) timings << "jobExecTime,avgMapTaskTime,avgReduceTaskTime" << std::endl;
	double avgMapTaskTime = (double)totalMapTaskTime/numMappers;
	double avgReduceTaskTime = (double)totalReduceTaskTime/numReducers;
	timings << jobExecutionTime << "," << avgMapTaskTime << "," << avgReduceTaskTime << std::endl;


	return 0;
}
