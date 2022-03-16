//#pragma once
#ifndef SIMPLESQLDB_CLASS_H
#define SIMPLESQLDB_CLASS_H
#include <tchar.h>
#include "framework.h"

#using <mscorlib.dll>
using namespace System;
using namespace System::Collections;

#using "System.dll"
#using "System.Data.dll"
#using "System.Transactions.dll"
#using "System.EnterpriseServices.dll"
#using "System.Xml.dll"

using namespace System::ComponentModel;
using namespace System::Data;
using namespace System::Data::SqlClient;

#define CONFIG_FILE							L"sqlservertest.ini"
#define NUMELEM(p)                          (sizeof(p)/sizeof((p)[0]))
#define DB_SECTION							L"Database"


public ref class SimpleSQLDb
{
	private:
		String^ m_Driver;
		String^ m_host;	//
		String^ m_port;	//
		String^ m_dbName;	//
		String^ m_user;
		String^ m_pwd;
		String^ m_table;
		SqlConnection^ m_sqlconn;

		

	public:
		inline SimpleSQLDb(void) {

		}
		/*
		* destructor
		*/
		~SimpleSQLDb();

		/*
		* connect to database
		* @param none
		* @return int
		*/
		void connect(void);

		/*
		* 
		*/
		void close(void) {
			m_sqlconn->Close();
		}

		/*
		* init
		* @param none
		* @return int
		*/
		int init(void);

		/*
		*  get all rows stored in the database
		* @param void*
		* @return bool
		*/
		bool getAll(void* data);


		unsigned int getLastID(void);

		
};

#endif