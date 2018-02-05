========================================================================
    CONSOLE APPLICATION : Yelp Application Project Overview
========================================================================

To run this application, open .sln file in Visual Studio 2017 and run the program.

The application will begin parsing json files and creating the tables. After a
few seconds, the welcome screen will appear and the user may enter a number to 
select their choice. 

    Options 1-3 will display information about businesses, 4-5
    will display information about users, 6-7 will display a few different 
    rankings for businesses or users, and option 8 will allow the user to search 
    for businesses according to various filters.

/////////////////////////////////////////////////////////////////////////////
Database functionality:

Record:
    All functionality was used except size() because this value was equal
    to the size of the attribute record for the table. 

Table:
    We did not have to use addAttribute, deleteAttribute, or getAttributes 
    because we created the table with the necessary attributes and used query to 
    get what is needed from the table. naturalJoin was used to combine the users 
    and reviews but crossJoin was not needed. count, min, and max were not used 
    in this application because it was easier to use sort().

Database:
    dropTable, listTables, and getTables were not used because we performed 
    query on the tables parsed from the json files. Aside from these, all other
    functions were used.
    
*Note: We were able to use all functions that we needed.*

/////////////////////////////////////////////////////////////////////////////
Important files:

database-api.lib
    Contains Team 8's database implemention

json.hpp
    This file is used to parse json files. 

/////////////////////////////////////////////////////////////////////////////
Other notes:

Must run program in Visual Studio 2017

/////////////////////////////////////////////////////////////////////////////
