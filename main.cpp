//#pragma warning(suppress : 4996)
/*
--   ----------------------------------------------------------------------
--   Project:          DLL creation
--
--   Author:           Hamza Qureshi
--
--   File name:        STC.h
--
--   Rev.:             1.0
--   Creation date:    DEC 2023
--
--   ---------------------------------------------------------------------
Purpose of this module: MAIN MODULE

----------Main body--------------

Comments:

WARININGS

---------------------------------------------------------------------------
*/
// RDU_1_0Dlg.cpp : implementation file
//

//#define DEBUG
#undef DEBUG

//#define API_ENDPOINT "http://127.0.0.1:5000/encryption_key"


#include "pch.h"
#include <limits.h>
#include "main.h"
#include "string"
#include "framework.h"
#include <iostream>
#include <curl/curl.h>
#include <conio.h>
#include <nlohmann/json.hpp>
#include <utility>
#include <fstream>
#include "aes_enc_dec.h"
#include "ini_parser.h"


using json = nlohmann::json;



// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

static std::string buffer_table_name_;
//static std::string previous_table_name_;
static std::string encrption_key_;


// Function declarations 
std::string read_api_endpoint();
std::string api_call_post_method(const char* url, const char* table_name, const char* iccid_name);
const char* call_encryption_api(const char* table_name, const char* iccid_name);
std::string extract_from_json(std::string parse_string_json);
std::string read_api_active();
//std::string  log_data_parser();


// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void fibonacci_init(
	const unsigned long long a,
	const unsigned long long b)
{
	index_ = 0;
	current_ = a;
	previous_ = b; // see special case when initialized
}

// Callback function to handle the response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
	size_t totalSize = size * nmemb;
	output->append(static_cast<char*>(contents), totalSize);
	return totalSize;
}

//int api_call_post_method(const char* url,const char* table_name,const char* record_iccid)
std::string api_call_post_method(const char* url, const char* table_name, const char* iccid_name)
{
	nlohmann::json jsonData;
	jsonData["table_name"] = table_name;
	jsonData["iccids"] = iccid_name;

	// Convert the JSON object to a string
	std::string jsonString = jsonData.dump();

	// Initialize cURL
	CURL* curl = curl_easy_init();
	if (!curl)
	{
		std::cerr << "Error initializing cURL." << std::endl;
		return "Error";
	}

	json m_handle;
	// Set the API endpoint URL
	curl_easy_setopt(curl, CURLOPT_URL, url);
	// Set the HTTP method to POST
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

	// Set the callback function to handle the response
	std::string responseData;
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

	// Perform the HTTP request
	CURLcode res = curl_easy_perform(curl);

	// Check for errors
	if (res != CURLE_OK)
	{
		std::cerr << "cURL request failed: " << curl_easy_strerror(res) << std::endl;
	}
	else
	{
		// Get HTTP response code
		long response_code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

		// Check if the response code is in the 2xx range (indicating success)
		if (response_code >= 200 && response_code < 300) {
#ifdef DEBUG
			std::cout << "Data send is " << jsonData << "\n";
			std::cout << "Response: " << responseData << std::endl;
#endif
			curl_easy_cleanup(curl);
			return responseData;
		}
		else
		{
			std::cerr << "HTTP request failed with response code: " << response_code << std::endl;
			curl_easy_cleanup(curl);
			return "ERROR";

		}
		return "ERROR";


	}
	// Cleanup cURL
	return "ERROR";

}








//
const char* call_encryption_api(const char* table_name, const char* iccid_name)
{
	//	const char* url = std::string(temp.begin(), temp.end()).c_str();
	//	std::wstring temp = read_api_end_point_from_ini();
	//  Convert wstring to const char*
	//	const char* url = std::string(temp.begin(), temp.end()).c_str();

	const char* url = API_ENDPOINT;
	try
	{
		std::string temp_str = api_call_post_method(url, table_name, iccid_name);
		const char* t = temp_str.c_str();
		return  t;


	}
	catch (const std::exception&)
	{
		std::cout << "Error";

	}

}


std::string extract_from_json(std::string parse_string_json) 
{

	//std::string response = R"(
	//       {
    //         "Description": "Post request received.",
	//         "key": "ABCD2222222222222222222222222222"
	//       }
	//   )";

	std::string key;
	std::string response;

	response = parse_string_json;
	try
	{
		// Parse the JSON string
		json responseData = json::parse(response);

		// Extract values
//		std::string description = responseData["Description"];
		key = responseData["key"];

#ifdef DEBUG
		std::cout << "Description: " << description << std::endl;
		std::cout << "Key: " << key << std::endl;
#endif // DEBUG


		return key;
	}
	catch (const json::parse_error& e)
	{
		std::cerr << "JSON parsing error: " << e.what() << std::endl;
	}

	return "";

}


std::string decrypt_each_record(const char* table_name, const char* record)
{
	// Input to this function is table name and encrypted record
	// From Piotec 


	std::string key;
	std::string temp_str;

	std::string enc_enable = read_api_active();
//#if (enc_enable==True)
	std::string temp = read_api_endpoint();
//#endif // 


#ifdef DEBUG
	std::cout << "API :" << temp << std::endl;
	std::cout << "Encrypted record is : " << record << std::endl;
#endif // DEBUG

//#if (enc_enable==True)


	if (enc_enable == "True")
	{
		const char* url = temp.c_str();

		//=====================================================================// ////////
		//==== Hit api only if table name changes otherwise use previous ======// //Done//
		//=====================================================================// ////////

		std::string new_table_name_ = std::string(table_name);
		if (buffer_table_name_ != new_table_name_)
		{
			std::cout << "==================> Table name changed" << std::endl;

			std::cout << "==================> Previous: " << buffer_table_name_ << " New: " << new_table_name_ << std::endl;


			try
			{
				temp_str = api_call_post_method(url, table_name, "");
#ifdef DEBUG
				std::cout << "==================> api return " << temp_str << std::endl;

#endif // DEBUG

				if (temp_str != "ERROR")
				{

					key = extract_from_json(temp_str);

					encrption_key_ = key;
					buffer_table_name_ = table_name;
					std::cout << "==================> API requested for key " << "key recieved is :" << key << std::endl;
				}

			}
			catch (...)
				//			catch (const std::exception&)
			{
				std::cout << "==================> Error calling API " << std::endl;
			}

		}
//#else

		else
		{
			key = encrption_key_;

		}
//#endif
		//============================================// ////////
		// Perform AES decrytion here using above key // //Done//
		//============================================// ////////

		if (temp_str != "ERROR")
		{
			unsigned char* temp_key_var = (unsigned char*)key.c_str();
			AESEncDec m_AES(temp_key_var);
			//	std::string decrypted_record = record;

			std::string decrypted_record = m_AES.decrypt_string(record);
			//	decrypted_record = record;

#ifdef DEBUG
			std::cout << "Decrypted record is : " << decrypted_record << std::endl;

#endif // DEBUG

			//=================END=======================//

			return decrypted_record;
		}

	}
	else
	{
		return record;
	}
}

#define INI_FILE_NAME "config.ini"

std::string read_api_endpoint()
{
	std::string api_end_point;
	INIParser iniParser;
	if (iniParser.load(INI_FILE_NAME))
	{
		api_end_point = iniParser.getValue("API_PATH", "API_ENDPOINT");
		return api_end_point;

	}

	else
	{
		return API_ENDPOINT;
	}

}

std::string read_api_active()
{
	std::string control_variable;
	INIParser iniParser;
	if (iniParser.load(INI_FILE_NAME))
	{
		control_variable = iniParser.getValue("API_ACTIVE", "ENCRYPTION");
		return control_variable;
	}
	else
	{
		return "False";
	}

}

//std::string test_api_call_encryption_api()
//{
//	const char* url = API_ENDPOINT;
//	std::string temp_str = api_call_post_method(url, "Test_1234", "1");
//	//	std::string temp_str = "1234111111111111111111111111";
//	return temp_str;
//
//}

const char *  log_data_parser()
{
	std::string temp = "TEST STRING";
	const char* ext = "TEST STRING";
	std::cout << ext << std::endl;
	return ext;

}



const char * test_decrypt_each_record(const char* table_name, const char* record)
{
	// Input to this function is table name and encrypted record
	// From Piotec 


	std::string key;
	std::string temp_str;

	std::string enc_enable = read_api_active();
	//#if (enc_enable==True)
	std::string temp = read_api_endpoint();
	//#endif // 


#ifdef DEBUG
	std::cout << "API :" << temp << std::endl;
	std::cout << "Encrypted record is : " << record << std::endl;
#endif // DEBUG

	//#if (enc_enable==True)


	if (enc_enable == "True")
	{
		const char* url = temp.c_str();

		//=====================================================================// ////////
		//==== Hit api only if table name changes otherwise use previous ======// //Done//
		//=====================================================================// ////////

		std::string new_table_name_ = std::string(table_name);
		if (buffer_table_name_ != new_table_name_)
		{
			std::cout << "==================> Table name changed" << std::endl;

			std::cout << "==================> Previous: " << buffer_table_name_ << " New: " << new_table_name_ << std::endl;


			try
			{
				temp_str = api_call_post_method(url, table_name, "");
#ifdef DEBUG
				std::cout << "==================> api return " << temp_str << std::endl;

#endif // DEBUG

				if (temp_str != "ERROR")
				{

					key = extract_from_json(temp_str);

					encrption_key_ = key;
					buffer_table_name_ = table_name;
					std::cout << "==================> API requested for key " << "key recieved is :" << key << std::endl;
				}

			}
			catch (...)
				//			catch (const std::exception&)
			{
				std::cout << "==================> Error calling API " << std::endl;
			}

		}
		//#else

		else
		{
			key = encrption_key_;

		}
		//#endif
				//============================================// ////////
				// Perform AES decrytion here using above key // //Done//
				//============================================// ////////

		if (temp_str != "ERROR")
		{
			unsigned char* temp_key_var = (unsigned char*)key.c_str();
			AESEncDec m_AES(temp_key_var);
			//	std::string decrypted_record = record;

			std::string decrypted_record = m_AES.decrypt_string(record);
			//	decrypted_record = record;

#ifdef DEBUG
			std::cout << "Decrypted record is : " << decrypted_record << std::endl;

#endif // DEBUG

			//=================END=======================//
			const char* result = decrypted_record.c_str();
			return result;
		}

	}
	else
	{
		return record;
	}
}
