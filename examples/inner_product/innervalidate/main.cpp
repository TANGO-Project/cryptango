/*
 * main.cpp
 *
 *  Created on: 20 Aug 2018
 *      Author: scsjd
 */
#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <HE1NDecrypter.h>

void usage() {
    std::cerr << "[ERROR] Bad number of parameters" << std::endl;
    std::cout << "    Usage: innervalidate <cipher secrets file> <cipher parameters file> <result file> <output file>" << std::endl;
}

int main(int argc, char **argv) {
	const char* secretsPath;
	const char* parametersPath;
	const char* resultPath;
	const char* outputPath;
	if (argc < 5){
		usage();
		return -1;
	}
	else{
		secretsPath = argv[1];
		parametersPath = argv[2];
		resultPath = argv[3];
		outputPath = argv[4];
	}

	std::cout << "Starting innervalidate with the following configuration:" << std::endl;
	std::cout << "\tcipher secrets file: " << secretsPath << std::endl;
	std::cout << "\tcipher parameters file: " << parametersPath << std::endl;
	std::cout << "\tplaintext result file: " << resultPath << std::endl;
	std::cout << "\tciphertext result file: " << outputPath << std::endl;

	//read in value from result file
	NTL::ZZ result;
	std::ifstream ifs_res(resultPath);
	if (ifs_res.is_open()){
		ifs_res >> result;
		ifs_res.close();
	}

	std::cout << "Result from computing on plaintexts: " << result << std::endl;

	//read in modulus from parameters file
	std::ifstream ifs_params(parametersPath);
	if (!ifs_params.is_open()) throw std::ios_base::failure("Could not open cipher parameters file.");
	std::string parameters;
	getline(ifs_params,parameters);
	ifs_params.close();
	Json::Value root;
	Json::Reader reader;
	NTL::ZZ modulus;
	bool parsedOK = reader.parse(parameters, root);
	if (parsedOK){
		modulus = NTL::conv<NTL::ZZ>(root["modulus"].asCString());
	}
	std::cout << "Modulus: " << modulus << std::endl;

	//read in value from output file
	NTL::ZZ_p output;
	std::ifstream ifs_out(outputPath);
	if (ifs_out.is_open()){
		NTL::ZZ_p::init(modulus);
		ifs_out >> output;
		ifs_out.close();
	}
	std::cout << "Result from computing on ciphertexts: " << output << std::endl;

	//read in secrets and parameters
	std::ifstream ifs_secrets(secretsPath);
	if (!ifs_secrets.is_open()) throw std::ios_base::failure("Could not open cipher secrets file.");
	std::string secrets;
	getline(ifs_secrets,secrets);
	ifs_secrets.close();

	//decrypt
	HE1NDecrypter d;
	d.readSecretsFromJSON(secrets);
	NTL::ZZ decrypted = d.decrypt(output);
	std::cout << "Decrypted result from computing on ciphertexts: " << decrypted << std::endl;
	//compare with value from result file
	if(decrypted==result){
		std::cout << "Result computed correctly" << std::endl;
	}
	else{
		std::cout << "Something went wrong" << std::endl;
		std::cout << "\tdecrypted=" << decrypted << std::endl;
		std::cout << "\tresult=" << result << std::endl;
	}
}
