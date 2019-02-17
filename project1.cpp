#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

/*
 * TITLE: project1.cpp
 * ABSTRACT: This program creates vendingmachine-objects and allows the user to select and buy different items, depending on stock and if
 *           payment method is valid. 
 * AUTHOR: Ralf Leistad
 * DATE: 10/08/2018
 */

class VendingMachine
{
public:
    VendingMachine();
    VendingMachine(int id, string name);
    void setNumber(int id);
    void setName(string name);
    string display();
    void status();
    void refill(int water, int coffee, int chip, int chocolate);
    void displayMenu();
    void selectItem();
    bool payment();
    void deselect();
    void displayReceipt();
    void reset();
    
    // HELPER FUNCTIONS
    void fillStructArray();
    double getTotalFromReceipt();
    void paymentFailed();
    void cancelSelection();
    string beautifyDouble(double uglyDouble);
    
private:
    int ID;
    string machineName;
    struct Items
    {
        string itemName;
        int quantity;
        int sold;
        double price;
        int selected;
    } water, coffee, chip, chocolate;
    
    double totalEarning;
    Items items[4];
    
};

int main()
{

    cout << "===== WELCOME TO CSUMB VENDING MACHINE =====\n";
    VendingMachine machine1;
    cout << machine1.display();
    
    VendingMachine machine2 (200, "Library");
    cout << machine2.display();
    
    cout << "\n===== UPDATED MACHINE INFO =====\n";
    machine1.setNumber(100);
    machine1.setName("BIT 117");
    cout << machine1.display();
    
    cout << "\n===== MACHINE 1 STATUS (1) =====\n";
    machine1.status();
    
    machine1.refill(5, 7, 3, 10);
    
    cout << "\n===== MACHINE 1 STATUS (2) =====\n";
    machine1.status();
    
    machine1.displayMenu();
    
    machine1.selectItem();
    
    cout << "\n===== machine1 Payment (1) =====\n";
    if(machine1.payment() == true)   // Pay with a debit card or cash.
	{
        cout << "\n===== Receipt (1) =====\n";
        machine1.displayReceipt();
    }
    else 
    {
        // Note that if a user entered incorrect payment information,
        // all selections should be deselected.
        cout << "Invalid payment. All selections are cancelled.\n";
    }

    cout << "\n===== MACHINE 1 STATUS (3) =====\n";
    machine1.status();
    
    cout << endl;
    machine1.selectItem();

    cout << endl;
    machine1.deselect( );  // A user can de-select items.

    cout << endl;
    machine1.selectItem();

    cout << "\n===== machine1 Payment (2) =====\n";
    if(machine1.payment())
    {
        cout << "\n===== Receipt (2) =====\n";
        machine1.displayReceipt();
    }

    cout << "\n===== machine1 Status (4) =====\n";
    machine1.status();

    machine1.reset();  // An admin resets the machine to the initial 
    // status, except the ID and name of the machine.
    cout << "\n===== machine1 Status (5) =====\n";
    machine1.status();
	
    cout << "\n===== Thank you! =====\n";

    return 0;

}

// FUNCTIONS

VendingMachine::VendingMachine()
: ID(0), machineName("UNKNOWN")
{
    fillStructArray();
}

VendingMachine::VendingMachine(int setId, string setName)
: ID(setId), machineName(setName)
{
    fillStructArray();
}

void VendingMachine::setNumber(int setId)
{
    ID = setId;
}

void VendingMachine::setName(string setName)
{
    machineName = setName;
}

string VendingMachine::display()
{
    return "Number: " + to_string(ID) + ", Name: " + machineName + "\n";
}

void VendingMachine::status()
{
    string sold = "Sold: ";
    string remaining = "Remaining: ";
    
    for(int i = 0; i < 4; i++)
    {
        sold += items[i].itemName + ": " + to_string(items[i].sold) + " / ";
        remaining += items[i].itemName + ": " + to_string(items[i].quantity) + " / ";
    }
    
    // REMOVE LAST "/" AT THE END OF STRING
    sold = sold.substr(0, sold.size()-2);
    remaining = remaining.substr(0, remaining.size()-2);
    
    cout << display() + sold + "\n" + remaining + "\n" + "Total Earning: $" + beautifyDouble(totalEarning) + "\n";
    
}

void VendingMachine::refill(int reWater, int reCoffee, int reChip, int reChocolate)
{
    items[0].quantity += reWater;
    items[1].quantity += reCoffee;
    items[2].quantity += reChip;
    items[3].quantity += reChocolate;
}

void VendingMachine::displayMenu()
{
    string menu;
    menu += "\n===== VENDING MACHINE MENU =====\n";
    menu += "  1.   Water............$1.50\n";
    menu += "  2.   Regular Coffee...$2.00\n";
    menu += "  3.   Sun Chip.........$1.00\n";
    menu += "  4.   Chocolate Bar....$2.50\n";
    cout << menu;
}

// SELECT WHAT ITEM AND QUANTITY
void VendingMachine::selectItem()
{
    int whatItem = 0;
    int selectedAmount = 0;
    char proceed = 'y';
    
    while((proceed == 'y' || proceed == 'Y'))
    {
        cout << "\nSelect Item: ";
        cin >> whatItem;
        
        if(whatItem < 0 || whatItem > 4)
        {
            cout << "Sorry, you selected an invalid item...\n";
            break;
        }
        
        cout << "How many do you want to buy? ";
        cin >> selectedAmount;
        
        if(items[whatItem-1].quantity < selectedAmount)
        {
            cout << "You selected " << items[whatItem-1].itemName << " (" << selectedAmount << ") - Sorry. We don't have enough " << items[whatItem-1].itemName;
            items[whatItem-1].selected = 0;
        }
        else if(selectedAmount < 0)
        {
            cout << "Negative selection amount not allowed...\n";
        }
        else
        {
            cout << "You selected " << items[whatItem-1].itemName << " (" << selectedAmount << ")";
            items[whatItem-1].selected += selectedAmount;
            items[whatItem-1].quantity -= items[whatItem-1].selected;
        }
        
        cout << "\nContinue? (Y/N) ";
        cin >> proceed;

    }
}

// HANDLES PAYMENT
bool VendingMachine::payment()
{
    int option = 1;
    int pin = 0;
    double cash = 0;
    double change = 0;
    bool validPayment = false;

    while(option != 1 || option != 2)
    {
        cout << "\nPayment Option - Debit (1) / Cash (2): ";
        cin >> option;
        if(option == 1)
        {
            cout << "Enter PIN: ";
            cin >> pin;
            
            if(pin != 7777)
            {
                paymentFailed();
                return false;
            }
            validPayment = true;
            break;
        }
        
        else if(option == 2)
        {
            cout << "Enter Money Amount: ";
            char dollarSign;
            cin >> dollarSign;
            
            if(dollarSign != '$')
            {
                cout << "Uknown currency, please specify with a dollarsign.\n";
                exit(1);
            }
            
            if(dollarSign == '$')
            {
                cin >> cash;
                
                if(cash < getTotalFromReceipt())
                {
                    paymentFailed();
                    return false;
                }
                validPayment = true;
                break;
            }
        }
        
        else
        {
            cout << "Payment method not recognized...";
        }
    }
    
    if(validPayment){
        if(option == 1)
        {
            for(int i = 0; i < 4; i++)
            {
                items[i].sold += items[i].selected;
            }
            cout << "Valid\n";
        }
        else if(option == 2)
        {
            for(int i = 0; i < 4; i++)
            {
                items[i].sold += items[i].selected;
            }
            
            change = (cash - getTotalFromReceipt());
            cout << "Change: $" << beautifyDouble(change) << endl;
        }
        else
        {
            cout << "Something went very wrong.....\n";
        }
    }
    
    return true;
}

// SET ALL SELECTED ITEMS TO 0
void VendingMachine::deselect()
{
    string displayDeselected = "You de-selected\n";
    for(int i = 0; i < 4; i++)
    {
        if(items[i].selected > 0)
        {
            displayDeselected += "\t" + items[i].itemName + " (" + to_string(items[i].selected) + ")\n";
        }
        items[i].quantity += items[i].selected;
    }
    cancelSelection();
    
    cout << displayDeselected;
}

// PRINTS AND DOES CALCULATIONS FOR THE RECEIPT
void VendingMachine::displayReceipt()
{
    double subtotal;
    string receipt;
    double tax;
    
    for(int i = 0; i < 4; i++)
    {
        if(items[i].selected > 0)
        {
            double priceOfItems;
            priceOfItems = (items[i].price * items[i].selected);
            receipt += items[i].itemName + ": $" + beautifyDouble(items[i].price) + " X " + to_string(items[i].selected) + " = $" + beautifyDouble(priceOfItems) + "\n";
            subtotal += (items[i].price * items[i].selected);
        }
    }
    
    tax = (subtotal / 10);
    
    receipt += "Subtotal: $" + beautifyDouble(subtotal) + "\n";
    receipt += "Tax (10.0%): $" + beautifyDouble(tax) + "\n";
    receipt += "Total: $" + beautifyDouble(subtotal + tax);
    
    totalEarning += (subtotal + tax);
    
    cout << receipt + "\n";
    cancelSelection();
}

// RESETS MACHINE TO ORIGINAL STATE, EXCLUDING ID AND NAME
void VendingMachine::reset()
{
    for(int i = 0; i < 4; i++)
    {
        items[i].sold = 0;
        items[i].quantity = 0;
        items[i].selected = 0;
    }
    totalEarning = 0.0;
}



// FUNCTION TO REVERSE QUANTITY IF PAYMENT FAILS
void VendingMachine::paymentFailed()
{
    for(int i = 0; i < 4; i++)
    {
        items[i].quantity += items[i].selected;
    }
    cancelSelection();
}

// FUNCTION TO ADD THE ITEMS TO ARRAY
void VendingMachine::fillStructArray()
{
    Items water = {"Water", 0, 0, 1.50, 0};
    Items coffee = {"Regular Coffee", 0, 0, 2.00, 0};
    Items chip = {"Sun Chip", 0, 0, 1.00, 0};
    Items chocolate = {"Chocolate bar", 0, 0, 2.50, 0};
    items[0] = water;
    items[1] = coffee;
    items[2] = chip;
    items[3] = chocolate;
}

// GET TOTAL AMOUNT 
double VendingMachine::getTotalFromReceipt()
{
    double subtotal = 0;
    double total = 0;
    
    for(int i = 0; i < 4; i++)
    {
        subtotal += items[i].selected * items[i].price;
    }
    
    total = (subtotal/10) + subtotal;
    return total;
}

// CANCEL SELECTION
void VendingMachine::cancelSelection()
{
    for(int i = 0; i < 4; i++)
    {
        items[i].selected = 0;
    }
}

// SET PRECISION OF DOUBLE TO 2
string VendingMachine::beautifyDouble(double uglyDouble)
{
    stringstream prettyDouble;
    prettyDouble << fixed << setprecision(2) << uglyDouble;
    return prettyDouble.str();
}