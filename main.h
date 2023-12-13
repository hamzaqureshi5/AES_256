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

#pragma once
// .h - Contains declarations of math functions

#ifdef STC_EXPORTS
#define STC_API __declspec(dllexport)
#else
#define STC_API __declspec(dllimport)
#endif

#include <string>


#define API_ENDPOINT "http://127.0.0.1:5000/encryption_key"

extern "C" STC_API const char* log_data_parser();

extern "C" STC_API std::string decrypt_each_record(const char* table_name, const char* record);

extern "C" STC_API const char * test_decrypt_each_record(const char* table_name, const char* record);



