#include<iostream>
#include <pqxx/pqxx>
#include<windows.h>
#pragma execution_character_set( utf-8 )

#include<tuple>
#include<string>
#include<vector>
using namespace std;

class DatabaseWorker
{
public:
	DatabaseWorker()
		:conn(
			"host=localhost "
			"port=5432 "
			"dbname=homework5 "
			"user=postgres "
			"password=12345 ")
	{	
		creteTable();
		conn.prepare("insert_client", "insert into client (first_name, last_name ,email ) values ($1,$2,$3)");
		conn.prepare("insert_phone", "insert into phone (client_id, phone)values($1,$2)");
		conn.prepare("chang_first_name", "update  client  set first_name=$1 where id =$2");
		conn.prepare("chandg_last_name", "update  client  set last_name=$1 where id =$2");
		//conn.prepare("chandg_email", "update  client  set email=$1 where id =$2");
		conn.prepare("delete_phone", "delete  from phone where client_id=$1 and phone = $2");
		conn.prepare("delete_client", "delete  from client where id = $1");
		
	}

	void creteTable()
	{
		static const string createClientTable = "create table if not exists client(id serial not null primary key, first_name text not null, last_name text not null,email text UNIQUE)";
		static const string createPhoneTable = "create table if not exists phone (id serial not null primary key, client_id int not null references  client(id) ON DELETE CASCADE, phone text not null)";

		pqxx::transaction t(conn);
		t.exec(createClientTable);
		t.exec(createPhoneTable);

		t.commit();
	}
	void addClient(const string& first_name,const string last_name,const string email)
	{
		pqxx::transaction t(conn);
		t.exec_prepared("insert_client", first_name, last_name, email);
		t.commit();
	}
	void addPhone(int  client_id, const string phone)
 	{
		pqxx::transaction t(conn);
		t.exec_prepared("insert_phone", client_id, phone);
		t.commit();
	}
	void changFirstName(int  client_id, const string first_name)
	{
		pqxx::transaction t(conn);
		t.exec_prepared("chang_first_name", first_name, client_id);
		t.commit();
	}
	void changLastName(int  client_id, const string  last_name)
	{
		pqxx::transaction t(conn);
		t.exec_prepared("chang_email",  client_id, last_name);
		t.commit();
	}
	/*void changEmail(int  client_id, const string email)
	{
		pqxx::transaction t(conn);
		t.exec_prepared("chang_email", email ,  client_id);
		t.commit();
	}*/
	void deletPhone(int  client_id, const string phone)
	{
		pqxx::transaction t(conn);
		t.exec_prepared("delete_phone", client_id, phone);
		t.commit();
	}

	void deletClent(int  client_id)
	{
		pqxx::transaction t(conn);
		t.exec_prepared("delete_client", client_id);
		t.commit();
	}

	vector<	tuple<int, string, string, string>>findClient( string value)
	{
		vector<	tuple<int, string, string, string>>result;
		pqxx::transaction t(conn);
		
		value = t.esc(value);



		auto clients1 = t.query<int, string, string, string>("select id, first_name, last_name, email from client where first_name like '" + value + "' or last_name like '" + value + "' or email like '" + value + "'");
     for (auto& client : clients1)
     { 
		 result.push_back(client);
      }
	 auto clients2 = t.query<int, string, string, string>("select client.id, client.first_name, client.last_name ,client.email from phone join client on client.id=phone.client_id where phone  like '" + value + "'");
		 for (auto& client : clients2)
		 {
			 result.push_back(client);
		 }
		 return result;


     }



protected:
	pqxx::connection conn;
};



void main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	//setlocale(LC_ALL, "Russian");
	try 
	{
		DatabaseWorker databaseWorkeer;
		databaseWorkeer.addClient("ivan", "ivanov", "ghj@jj");
		databaseWorkeer.addClient("petr", "peptikov" ," jkhj@yy");
		databaseWorkeer.addClient("pjuytr", "peptikov", " qwe@yy");
		databaseWorkeer.addPhone(1, "12354");
		databaseWorkeer.addPhone(1, "56789");
		databaseWorkeer.addPhone(2, "7953856");
		databaseWorkeer.changFirstName(1, "vitya");
		databaseWorkeer.changLastName(2, "viladg");
		databaseWorkeer.deletPhone(1, "12354");
		//databaseWorkeer.changEmail(1, "emely");
		auto clients = databaseWorkeer.findClient("vitya");

		for(auto & client: clients)
		{
			cout << "client id:" << get<0>(client) << " ";
			cout << "first name:" << get<1>(client) << " ";
			cout << "last name:" << get<2>(client) << " ";
			cout << "email:" << get<3>(client) << " ";


		}


     }
	catch (const exception& e)
	{
		cout << "Exeption is:" << e.what() << endl;

	}
	cout << "HELLO" << endl;
}