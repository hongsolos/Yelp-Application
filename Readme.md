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

### License
```
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
## Timeline

10/2017 - Project Initiated
11/2017 - Project Completed

## Built With

* [Visual Studio](https://www.eclipse.org/) - Code IDE

## Acknowledgments
* [JSON Parser] https://github.com/nlohmann/json - Big thanks to nlohmnn
* Yelp - Big thanks for providing us the open data
* CSCE 315 Staffs


