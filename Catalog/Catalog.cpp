//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<iostream>
#include	<string>

#include	"Catalog.h"

using namespace std;

Product::Product(uint64_t number, string name) { //Constructor with parameters for product class
	name_ = name;
	number_ = number;
}

Category::Category(uint64_t number, string name) { //Constructor with parameters for category class
	name_ = name;
	number_ = number;
}

bool	Catalog::AddCategory(uint64_t categoryNumber, const string& name) 
{
	if (log_.count(categoryNumber)) //Check if category is already in catalog map
		return false;

	Category newCat(categoryNumber, name); //Make a new category object and insert it into the map
	log_.insert(std::pair<uint64_t, Category>(categoryNumber, newCat));
	return true;
}

bool	Catalog::AddProduct(uint64_t categoryNumber, uint64_t productNumber, const string& name)
{
	if (!log_.count(categoryNumber)) //Check if category number exists
		return false;

	map<uint64_t, Category>::iterator it; //Make an iterator pointing to the correct category node
	it = log_.find(categoryNumber);
	Product newProd(productNumber, name); //Make a new product

	if (it->second.cat_.count(productNumber))  //Check if the product number already exists
		return false;

	it->second.cat_.insert(std::pair<uint64_t, Product>(productNumber, newProd)); //Insert the product in the category's map
	return true;
}

uint64_t	Catalog::GetCategoryCount()
{
	return log_.size(); //Return the size of the catalog map
}

int64_t	Catalog::GetProductCount(uint64_t categoryNumber)
{
	map<uint64_t, Category>::iterator it = log_.find(categoryNumber); //Make an iterator pointing to category node
	if (!log_.count(categoryNumber)) //Check if the category exists
		return -1;
	
	return it->second.cat_.size(); //Return the size of the product map
}

bool	Catalog::Load(const string& fileName)
{
	ifstream myfile(fileName);
	string line;

	if (myfile.is_open()) {
		uint64_t curcat; //Used to track proper category
		string catnum, catname; //Used to insert new categories
		string prodnum, prodname; //Used to insert new products

		while (!myfile.eof()) { //Loop until the end of the file
			getline(myfile, line, '\t');
			if (line == "Category") { //If category, make new category and insert into category map
				getline(myfile, line, '\t');
				catnum = line;
				getline(myfile, line, '\n');
				catname = line;
				AddCategory(stoi(catnum), catname);
				curcat = stoi(catnum);
			}
			else if (line != "\n" && line != "") { //If product, make new product and insert it into product map
				prodnum = line;
				getline(myfile, line, '\n');
				prodname = line;
				AddProduct(curcat, stoi(prodnum), prodname);
			}
		}
	}
	else
		return false;
	return true;
}

//****************************************************************************************
//
//	Show... methods
//
//	The stream argument allows thess functions to write its output to any output stream,
//	such as cout or a file, that the caller passes. The argument's name "stream" should
//	be used just as you would use "cout":
//
//		stream << "X = " << x << endl;
//
//	The output will go where the caller intended without special action by this function.
//
//****************************************************************************************

bool	Catalog::ShowProduct(ostream& stream, uint64_t categoryNumber, uint64_t productNumber)
{
	if (!log_.count(categoryNumber)) //Check if the category exists
		return false;
	map<uint64_t, Category>::iterator it = log_.find(categoryNumber); //Make iterator and point it to category

	if (!it->second.cat_.count(productNumber)) //Check if the product exists
		return false;

	map<uint64_t, Product>::iterator p = it->second.cat_.find(productNumber); //Make iterator and point it to product 
	stream << p->second.number_ << "\t" << p->second.name_; //Cout product number	product name

	return true;
}

bool	Catalog::ShowCategory(ostream& stream, uint64_t categoryNumber) {
	if (log_.size() == 0) { //Check if catalog is empty
		return false;
	}
	
	if (!log_.count(categoryNumber)) //Check if category exists
		return false;

	map<uint64_t, Category>::iterator it = log_.find(categoryNumber); //Make an iterator and point it to category
	map<uint64_t, Product>::iterator p = it->second.cat_.begin(); //Make an iterator and point it to product

	stream << "Category\t" << it->second.number_ << '\t' << it->second.name_ << endl; //Cout category	category number		category name

	while (p != it->second.cat_.end()) { //Print each product in category
		stream << p->second.number_ << "\t" << p->second.name_ << endl;
		p++;
	}
	return true;
}

bool	Catalog::ShowAll(ostream& stream) 
{
	if (log_.size() == 0) { //Check if catalog is empty
		return false;
	}

	map<uint64_t, Category>::iterator it = log_.begin(); //Make an iterator pointing to beginning of map

	while (it != log_.end()) { //Print every category with product in the map
		ShowCategory(stream, it->second.number_); 
		it++;
	}

	return true;
}

