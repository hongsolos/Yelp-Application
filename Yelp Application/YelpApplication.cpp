// YelpApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "database.h"
#include <fstream>
#include <sstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

struct myclass {
	bool operator() (Record i, Record j) { return stoi(i[0]) > stoi(j[0]); }
} myobject;

void print_attr(Table target) {
	vector <string> att = target.getAttributes();
	for (int i = 0; i < att.size(); i++) {
		cout << left << setw(16) << att[i] << setw(16);
	}
	cout << "\n----------------------------------------------------------------" << endl;
}

string concat(vector<string> input) {
	for (int i = 0; i < input.size(); i++) {
		if (input[i].find("Restaurants") != string::npos)
			return "RESTAURANTS";
		else if (input[i].find("Shopping") != string::npos)
			return "SHOPPING";
		else if (input[i].find("Gyms") != string::npos)
			return "GYM";
		else if (input[i].find("Education") != string::npos)
			return "EDUCATION";
		else if (input[i].find("Health & Medical") != string::npos)
			return "HEALTH";
		else if (input[i].find("Beauty & Spas") != string::npos)
			return "BEAUTY";
	}
	return "MISC";
}

Database json_parser(string business, string user, string reviews) {
	Database master;
	cout << "LOADING DATA BASE CAN TAKE A FEW SECONDS, PLEASE BE PATIENT" << endl;
	/*-----------------------------------//
	//PARSE BUSINESS INTO BUSINESS TABLE"//
	//-----------------------------------*/
	ifstream biz(business);
	string biz_line;

	//TABLE CREATION
	//ID | NAME | CITY | STATE | STARS | REVIEW COUNT | IS OPEN | BIKE PARKING | CAR PARKING | ROMANTIC | INTIMATE | CLASSY | CATEGORY | GROUP | KIDS FRIENDLY | DELIVERY
	vector<string> attributes = { "BUSINESS_ID", "BUSINESS_NAME", "CITY", "STATE", "STARS", "REVIEW_COUNT", "IS_OPEN", "BIKE_PARKING", "CAR_PARKING", "ROMANTIC", "INTIMATE", "CLASSY", "CATEGORY", "GROUP", "KIDS_FRIENDLY", "DELIVERY" };
	Table bus_table(attributes);

	//PARSE JSON LINES INTO RECORDS
	while (getline(biz, biz_line))
	{
		ifstream iss(biz_line);
		json biz_j = json::parse(biz_line);

		//LOADING PROGRESS
		cout << biz_j.at("name").get<string>() << endl;
		//

		//RECORD CREATION
		Record new_record(16);
		new_record[0] = biz_j.at("business_id").get<string>();
		new_record[1] = biz_j.at("name").get<string>();
		//remove ' from business names
		if (new_record[1]->find("'") != string::npos) {
			new_record[1]->erase(remove(new_record[1]->begin(), new_record[1]->end(), '\''), new_record[1]->end());
		}
		new_record[2] = biz_j.at("city").get<string>();
		new_record[3] = biz_j.at("state").get<string>();
		new_record[4] = to_string(biz_j.at("stars").get<double>());
		new_record[5] = to_string(biz_j.at("review_count").get<int>());
		new_record[6] = to_string(biz_j.at("is_open").get<int>());
		if (biz_line.find("BikeParking") != string::npos) {
			new_record[7] = "False";
			if (biz_j.at("attributes").at("BikeParking").get<bool>())
				new_record[7] = "True";
		}
		else {
			new_record[7] = "N/A";
		}
		//check for garage, street, lot, or valet parking
		if (biz_line.find("BusinessParking") != string::npos) {
			json car_park = biz_j.at("attributes").at("BusinessParking");
			int parking = car_park.at("garage").get<bool>() + car_park.at("street").get<bool>() +
				car_park.at("lot").get<bool>() + car_park.at("valet").get<bool>();
			new_record[8] = "False";
			if (parking != 0)
				new_record[8] = "True";
		}
		else {
			new_record[8] = "N/A";
		}
		if (biz_line.find("Ambience") != string::npos) {
			bool rom = biz_j.at("attributes").at("Ambience").at("romantic").get<bool>();
			bool inm = biz_j.at("attributes").at("Ambience").at("intimate").get<bool>();
			bool cla = biz_j.at("attributes").at("Ambience").at("classy").get<bool>();
			new_record[9] = "False";
			new_record[10] = "False";
			new_record[11] = "False";
			if (rom)
				new_record[9] = "True";
			if (inm)
				new_record[10] = "True";
			if (cla)
				new_record[11] = "True";
		}
		else {
			new_record[9] = "N/A";
			new_record[10] = "N/A";
			new_record[11] = "N/A";
		}
		new_record[12] = concat(biz_j.at("categories").get<vector<string>>());
		if (biz_line.find("RestaurantsGoodForGroups") != string::npos) {
			bool gro = biz_j.at("attributes").at("RestaurantsGoodForGroups").get<bool>();
			new_record[13] = "False";
			if (gro)
				new_record[13] = "True";
		}
		else {
			new_record[13] = "N/A";
		}
		if (biz_line.find("GoodForKids") != string::npos) {
			bool kid = biz_j.at("attributes").at("GoodForKids").get<bool>();
			new_record[14] = "False";
			if (kid)
				new_record[14] = "True";
		}
		else {
			new_record[14] = "N/A";
		}
		if (biz_line.find("RestaurantsDelivery") != string::npos) {
			bool res = biz_j.at("attributes").at("RestaurantsDelivery").get<bool>();
			new_record[15] = "False";
			if (res)
				new_record[15] = "True";
		}
		else {
			new_record[15] = "N/A";
		}

		//ADD RECORD TO TABLE
		bus_table.add(new_record);
	}

	/*-----------------------------------//
	//PARSE USER INTO USER TABLE"        //
	//-----------------------------------*/
	ifstream usr(user);
	string usr_line;

	//USER ID | NAME | REVIEW COUNT | DATE JOINED
	vector<string> usr_attributes = { "USER_ID", "USER_NAME", "REVIEW_COUNT", "DATE_JOINED" , "AVERAGE_STARS" };
	Table usr_table(usr_attributes);

	while (getline(usr, usr_line))
	{
		ifstream iss(usr_line);
		json usr_j = json::parse(usr_line);

		//LOADING PROGRESS
		cout << usr_j.at("name").get<string>() << endl;
		//

		//RECORD CREATION
		Record new_record(5);
		new_record[0] = usr_j.at("user_id").get<string>();
		new_record[1] = usr_j.at("name").get<string>();
		new_record[2] = to_string(usr_j.at("review_count").get<int>());
		new_record[3] = (usr_j.at("yelping_since").get<string>());
		new_record[4] = to_string(usr_j.at("average_stars").get<double>());

		//ADD RECORD TO TABLE
		usr_table.add(new_record);
	}
	/*-----------------------------------//
	//PARSE REVIEW INTO REVIEW TABLE"    //
	//-----------------------------------*/
	ifstream rev(reviews);
	string rev_line;

	//REVIEW ID | USER ID | BUSINESS ID | STARS | DATE | REVIEW TXT
	vector<string> rev_attributes = { "REVIEW ID", "USER_ID", "BUSINESS_ID", "STARS", "DATE", "REVIEW" };
	Table rev_table(rev_attributes);

	while (getline(rev, rev_line))
	{
		ifstream iss(rev_line);
		json rev_j = json::parse(rev_line);

		//LOADING PROGRESS
		cout << rev_j.at("review_id").get<string>() << endl;
		//-----------------------------------------------

		//RECORD CREATION
		Record new_record(6);
		new_record[0] = rev_j.at("review_id").get<string>();
		new_record[1] = rev_j.at("user_id").get<string>();
		new_record[2] = rev_j.at("business_id").get<string>();
		new_record[3] = to_string(rev_j.at("stars").get<double>());
		new_record[4] = rev_j.at("date").get<string>();
		new_record[5] = rev_j.at("text").get<string>();

		//ADD RECORD TO TABLE
		rev_table.add(new_record);

	}
	//ADD TABLES INTO THE DATABASE -- USERS, BUSINESS, REVIEWS
	master.addTable(bus_table, "BUSINESS");
	master.addTable(usr_table, "USERS");
	master.addTable(rev_table, "REVIEWS");
	//---------------------------------------

	system("CLS");
	return master;
}

int main() {
	/*----------------------------------//
	//MAIN CONSOLE LOOP - EXIT ON "EXIT"//
	//----------------------------------*/
	Database master = json_parser("business.json", "user.json", "review.json");

	//COMBINE SPECIAL TABLES
	cout << "PREPARING .... PLEASE WAIT JUST A LITTLE LONGER" << endl;

	Table firstUSR = master[0];
	Table secondREV = master[2];
	firstUSR.specifyKey("USER_ID");
	Table user_and_review = secondREV.naturalJoin(firstUSR);
	master.addTable(user_and_review, "USER_REVIEWS");

	bool EXIT = false;
	while (!EXIT) {
		system("CLS");
		/*----------------------------------//
		//WELCOME SCREEN                    //
		//----------------------------------*/
		printf("********************************************************************************************\n");
		printf("*___________                     __________                   __               __     ____ *\n");
		printf("*\\__    ___/___ _____    _____   \\______   \\_______  ____    |__| ____   _____/  |_  /_   |*\n");
		printf("*   |   |_/ __ \\\\__  \\  /     \\   |     ___/\\_  __ \\/  _ \\   |  |/ __ \\_/ ___\\   __\\  |   |*\n");
		printf("*   |   |\\  ___/ / __ \\|  Y Y  \\  |    |     |  | \\(  <_> )  |  \\  ___/\\  \\___|  |    |   |*\n");
		printf("*   |___| \\___  >____  /__|_|  /  |____|     |__|   \\____/\\__|  |\\___  >\\___  >__|    |___|*\n");
		printf("*                                                        \\______|                          *\n");
		printf("********************************************************************************************\n");
		/*----------------------------------//
		//MAIN MENU                         //
		//----------------------------------*/
		printf("********************BUSINESS FEATURES**********************\n");
		printf("ENTER  [1]: LIST ALL BUSINESSES - NAME ONLY   \n");
		printf("ENTER  [2]: LIST ALL BUSINESSES - BASED ON SELECT OPTIONS   \n");
		printf("ENTER  [3]: FIND A BUSINESS     - BY NAME   \n");
		printf("\n");

		printf("**********************USER FEATURES************************\n");
		printf("ENTER  [4]: LIST ALL USERS      - NAME ONLY\n");
		printf("ENTER  [5]: USER PROFILE        - SELECT BY NAME\n");
		printf("\n");

		printf("*********************RANKING FEATURES**********************\n");
		printf("ENTER  [6]: RANKING BUSINESS\n");
		printf("ENTER  [7]: RANKING USER\n");
		printf("\n");

		printf("*********************PREMIUM FEATURE***********************\n");
		printf("ENTER  [8]: *** WEEK-NIGHT AT BERNIE'S ***\n");
		printf("\n");

		printf("XXXXXXXXXXXXX  ENTER 101 TO EXIT THE PROGRAM XXXXXXXXXXXXXX\n");
		/*------------------------------------------------------------------//
		//                         EXCUTING OPTIONS                         //
		//------------------------------------------------------------------*/
		int INPUT_OPTION = 0;
		cin >> INPUT_OPTION;
		system("CLS");
		switch (INPUT_OPTION) {

		/*----------------------------------//
		//BUSINESS FEATURES                 //
		//----------------------------------*/
		//PRINT ALL BUSINESSES - NAME ONLY ====================================================================
		case 1: {
			Table result = master[1];
			for (int i = 0; i < result.size(); i++) {
				cout << result[i][1] << endl;
			}
			cout << "END OF PRINT ----> EXITING       \n" << endl;

			system("pause");
			break;
		}

		//PRINT ALL BY OPTIONS ==============================================================================
		case 2: {
			printf("******************SELECT YOUR OPTIONS**********************\n");
			printf("ENTER  [1]: BY STATE   \n");
			printf("ENTER  [2]: BY CATEGORIES   \n");
			printf("ENTER  [3]: BY STARS \n");
			int inputs;
			cin >> inputs;
			system("CLS");
			cin.clear();
			cin.ignore(100, '\n');
			switch (inputs) {
			//PRINT ALL BUSINESSES BY STATE -----------------------------------------------------
			case 1: {
				cout << "PLEASE ENTER YOUR STATE (UPPERCASE ABBREVIATION ONLY, EX: OH): ";
				string state;
				cin >> state;
				string field = "STATE = " + state;
				try {
					Table result = master.query("CITY STARS REVIEW_COUNT CATEGORY BUSINESS_NAME", "BUSINESS", field);
					if (result.size() != 0) {
						cout << left << result << endl;
					}
					else {
						cout << "NO BUSINESSES IN SPECIFIED FIELD\n" << endl;
					}
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
				break;
			}
			//PRINT ALL BUSINESSES BY CATEGORIES ----------------------------------------------------
			case 2: {
				cout << "PLEASE ENTER A CATEGORY (RESTAURANTS / SHOPPING / GYM / EDUCATION / HEALTH / BEAUTY / MISC): ";
				string cat;
				cin >> cat;
				string field = "CATEGORY = " + cat;
				try {
					Table result = master.query("CITY STATE STARS REVIEW_COUNT BUSINESS_NAME", "BUSINESS", field);
					if (result.size() != 0) {
						cout << left << result << endl;
					}
					else {
						cout << "NO BUSINESSES IN SPECIFIED FIELD\n" << endl;
					}
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
				break;
			}
			//PRINT ALL BUSINESS BY STARS ----------------------------------------------------	
			case 3: {
				cout << "PLEASE ENTER 1 / 1.5 / 2 / 2.5 / 3 / 3.5 / 4 / 4.5 OR 5: ";
				double stars = 0;
				cin >> stars;
				string field = "STARS = " + to_string(stars);
				try {
					Table result = master.query("CITY STATE REVIEW_COUNT CATEGORY BUSINESS_NAME", "BUSINESS", field);
					if (result.size() != 0) {
						cout << left << result << endl;
					}
					else {
						cin.clear();
						cin.ignore(100, '\n');
						cout << "NO BUSINESSES IN SPECIFIED FIELD\n" << endl;
					}
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
				break;
			}
			default: {
				printf("INCORRECT ARGUMENT ENTERED\n\n");
			}
			}

			system("pause");
			break;
		}

		//FIND BUSINESS BY OPTIONS =========================================================================
		case 3: {
			cout << "ENTER THE BUSINESS NAME (no apostrophes): ";

			string target;
			cin.ignore();
			getline(cin, target);

			string field = "BUSINESS_NAME = '" + target + "'";
			try {
				Table result = master.query("CITY STATE STARS REVIEW_COUNT IS_OPEN CATEGORY", "BUSINESS", field);
				if (result.size() != 0) {
					cout << left << result << endl;
				}
				else {
					cout << "NO BUSINESS FOUND\n" << endl;
				}
			}
			catch (const DatabaseError& e) {
				cerr << "INVALID INPUT" << endl;
			}

			system("pause");
			break;
		}

		/*----------------------------------//
		//USER FEATURES                     //
		//----------------------------------*/
		//PRINT ALL USERS  -------------------------------------------------------------------------------------------
		case 4: {
			Table result = master[0];
			for (int i = 0; i < result.size(); i++) {
				cout << result[i][1] << endl;
			}
			cout << "END OF PRINT ----> EXITING       \n" << endl;

			system("pause");
			break;
		}

		//FIND USER --------------------------------------------------------------------------------------------------
		case 5: {
			cout << "PLEASE ENTER THE USER NAME (MIGHT BE MULTIPLE USERS): ";
			try {
				string usrname;
				cin >> usrname;
				string field = "USER_NAME = '" + usrname + "'";
				//get table of all users of inputted name
				Table result = master.query("REVIEW_COUNT DATE_JOINED AVERAGE_STARS USER_ID", "USERS", field);
				if (result.size() != 0) {
					cout << "ID              ";
					print_attr(result);
					for (int i = 0; i < result.size(); i++) {
						cout << left << setw(16) << i + 1 << result[i] << endl;
					}

					//select a user and print reviews
					cout << "\nPLEASE ENTER THE ID OF DESIRED USER: ";
					int usrid;
					cin >> usrid;
					if (usrid > 0 && usrid <= result.size()) {
						field = "USER_ID = " + *(result[usrid - 1][3]);
						Table result1 = master.query("STARS DATE REVIEW", "USER_REVIEWS", field);
						if (result1.size() != 0) {
							cout << result1 << endl;
						}
						else {
							cout << "NO REVIEWS FOR USER IN DATABASE\n" << endl;
						}
					}
					else {
						cout << "INVALID ID ENTERED\n" << endl;
					}

				}
				else {
					cin.clear();
					cin.ignore(100, '\n');
					cout << "USER NAME NOT FOUND\n" << endl;
				}
			}
			catch (const DatabaseError& e) {
				cin.clear();
				cin.ignore(100, '\n');
				cerr << "INVALID INPUT" << endl;
			}

			system("pause");
			break;
		}

		/*----------------------------------//
		//RANKING FEATURES                  //
		//----------------------------------*/
		case 6: {
			//RANKING BUSINESS
			printf("ENTER [1]: RAMSAY MAY APPROVE - TOP 10 RESTAURANTS BY STARS AND REVIEW COUNTS\n");
			printf("ENTER [2]: THE ABS STORE      - TOP 5 GYMS BY REVIEW COUNT\n");
			int rank_biz;
			cin >> rank_biz;

			switch (rank_biz) {
			//RAMSAY ----------------------------------------
			case 1: {
				system("CLS");
				try {
					string field = "STARS = " + to_string(5.0) + " AND CATEGORY = RESTAURANTS";

					Table result = master.query("REVIEW_COUNT STARS BUSINESS_NAME", "BUSINESS", field);
					vector<Record> resultT;

					for (int i = 0; i < result.size(); i++) {
						resultT.push_back(result[i]);
					}
					std::sort(resultT.begin(), resultT.end(), myobject);
					print_attr(result);
					for (int i = 0; i < 10; i++) {
						cout << left << resultT[i] << endl;
					}
					cout << endl;
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
				break;
			}
			//ABS -------------------------------------------
			case 2: {
				system("CLS");
				try {
					Table result = master.query("REVIEW_COUNT STARS BUSINESS_NAME", "BUSINESS", "CATEGORY = GYM");
					vector<Record> resultT;

					for (int i = 0; i < result.size(); i++) {
						resultT.push_back(result[i]);
					}
					sort(resultT.begin(), resultT.end(), myobject);
					print_attr(result);
					for (int i = 0; i < 5; i++) {
						cout << left << resultT[i] << endl;
					}
					cout << endl;
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
				break;
			}
			default: {
				cin.clear();
				cin.ignore(100, '\n');
				printf("INCORRECT ARGUMENT ENTERED\n\n");
			}
			}

			system("pause");
			break;
		}

		case 7: {
			//RANKING USERS
			printf("ENTER [1]: THE TRAILBLAZERS  - TOP 10 USERS WITH MOST REVIEW COUNTS\n");
			printf("ENTER [2]: THE OPTIMUS PRIME - TOP 10 USERS WITH HIGHEST AVG REVIEWS\n");

			int rank_usr;
			cin >> rank_usr;
			switch (rank_usr) {
			//TRAILBLAZERS ------------------------------------------------------------------------------
			case 1: {
				system("CLS");
				try {
					Table result = master.query("REVIEW_COUNT USER_NAME", "USERS", "REVIEW_COUNT > '0'");
					vector<Record> resultT;

					for (int i = 0; i < result.size(); i++) {
						resultT.push_back(result[i]);
					}
					std::sort(resultT.begin(), resultT.end(), myobject);
					print_attr(result);
					for (int i = 0; i < 10; i++) {
						cout << left << resultT[i] << endl;
					}
					cout << endl;
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
				break;
			}
			//OPTIMUS PRIME ------------------------------------------------------------------------------
			case 2: {
				system("CLS");
				try {
					string field = "AVERAGE_STARS = " + to_string(5.0);
					Table result = master.query("REVIEW_COUNT USER_NAME AVERAGE_STARS", "USERS", field);
					vector<Record> resultT;

					for (int i = 0; i < result.size(); i++) {
						resultT.push_back(result[i]);
					}
					sort(resultT.begin(), resultT.end(), myobject);
					print_attr(result);
					for (int i = 0; i < 10; i++) {
						cout << left << resultT[i] << endl;
					}
					cout << endl;
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
				break;
			}
			default: {
				cin.clear();
				cin.ignore(100, '\n');
				printf("INCORRECT ARGUMENT ENTERED\n\n");
			}
			}

			system("pause");
			break;
		}
		// PREMIUM FEATURE -----------------------------------------------------------------------------------------
		case 8: {
			cout << "WELCOME TO THE PREMIUM FEATURE" << endl;
			cout << "This feature will help you plan you dinner night! Just answer" << endl;
			cout << "a few questions, and we will give you a list of the best dining place" << endl;
			cout << "to choose from! Are you ready?\n" << endl;
			system("pause");
			system("CLS");

			string result_table;
			string trans;
			string singroup;
			string kidz;

			int choice;
			bool valid = true;

			cout << "Will you be traveling by automobile [1], bike [2], or N/A [3]? ";
			cin >> choice;
			switch (choice) {
			case 1: {
				trans = " AND CAR_PARKING = 'True'";
				break;
			}
			case 2: {
				trans = " AND BIKE_PARKING = 'True'";
				break;
			}
			case 3: {
				trans = "";
				break;
			}
			default: {
				cin.clear();
				cin.ignore(100, '\n');
				printf("INCORRECT ARGUMENT ENTERED\n");
				valid = false;
			}
			}

			cout << "Will you be in group [1], single [2] , or couple [3]? ";
			cin >> choice;
			switch (choice) {
			case 1: {
				singroup = " AND GROUP = 'True'";
				break;
			}
			case 2: {
				singroup = " AND DELIVERY = 'True'";
				break;
			}
			case 3: {
				singroup = " AND (ROMANTIC = 'True' OR INTIMATE = 'True')";
				break;
			}
			default: {
				cin.clear();
				cin.ignore(100, '\n');
				printf("INCORRECT ARGUMENT ENTERED\n");
				valid = false;
			}
			}

			cout << "Do you have any kids going with you Yes [1] No [2]? ";
			cin >> choice;
			switch (choice) {
			case 1: {
				kidz = " AND KIDS_FRIENDLY = 'True'";
				break;
			}
			case 2: {
				kidz = "";
				break;
			}
			default: {
				cin.clear();
				cin.ignore(100, '\n');
				printf("INCORRECT ARGUMENT ENTERED\n");
				valid = false;
			}
			}

			//prints table according to choices picked
			if (valid == true) {
				cout << "\nOkay! We will have your list shortly ...\n" << endl;
				try {
					result_table = "CATEGORY = RESTAURANTS AND IS_OPEN = '1'" + trans + singroup + kidz;
					Table result_choice = master.query("STARS REVIEW_COUNT BUSINESS_NAME ", "BUSINESS", result_table);

					if (result_choice.size() != 0) {
						vector<Record> resultT;
						for (int i = 0; i < result_choice.size(); i++) {
							resultT.push_back(result_choice[i]);
						}
						sort(resultT.begin(), resultT.end(), myobject);
						print_attr(result_choice);
						for (int i = 0; i < resultT.size(); i++) {
							cout << left << resultT[i] << endl;
						}
						cout << endl;
					}
					else {
						cout << "NO RESTAURANTS FOUND\n" << endl;
					}
				}
				catch (const DatabaseError& e) {
					cerr << "INVALID INPUT" << endl;
				}
			}
			else {
				printf("INCORRECT ARGUMENT WAS ENTERED\n\n");
			}

			system("pause");
			break;
		}
		case 101: {
			printf("TERMINATING THE PROGRAM\n");
			EXIT = true;
			break;
		}
		default: {
			cin.clear();
			cin.ignore(100, '\n');
			printf("INCORRECT ARGUMENT ENTERED, PLEASE TRY AGAIN\n");
			system("pause");
		}
		}
	}
	return 0;
}
