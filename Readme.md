# Yelp Application  

CSCE-315 Project 2: Data extraction and Database Management 

## Authors

* **Han Hong** - *Contributor* - 
* **Amy Li** - *Contributor* - 
* **Claire Duong** - *Contributor* - [hongsolos](https://github.com/hongsolos)


## Background

This project's goal is to build a database using Yelp data saved in jason format.

### Installation Instructions

What things you need to install the software and how to install them

```
To run this application, open .sln file in Visual Studio 2017 and run the program.

The application will begin parsing json files and creating the tables. After a
few seconds, the welcome screen will appear and the user may enter a number to 
select their choice. 

    Options 1-3 will display information about businesses, 4-5
    will display information about users, 6-7 will display a few different 
    rankings for businesses or users, and option 8 will allow the user to search 
    for businesses according to various filters.

```
### Issues Notes - Testing Notes
```
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
```

### Future Improvement
Project completed

## Timeline

10/2017 - Project Initiated
11/2017 - Project Completed

## Built With

* [Visual Studio](https://www.eclipse.org/) - Code IDE

## Acknowledgments

* Steam
* Ben Hu Phd 


