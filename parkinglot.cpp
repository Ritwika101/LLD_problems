#include<iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <set>
#include <climits>
#include <cstdlib>
#include <queue>
#include <ctime>
#include <exception>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace std;
#define ll long long
class Rates;
class TypeMatch;
class Address;
class Person;
class ParkingTicket;
class Spot;
class Vehicle;
class DisplayBoard;
class Floor;
class Payment;
class Credit;
class Cash;
class ParkingLot;
class ParkingAttendant;
class Admin;

enum ParkingSpotType {
    MOTORCYLE, COMPACT, LARGE
};
enum VehicleType {
    BIKE, CAR, TRUCK
};
enum TicketStatus {
    PAID, ACTIVE
};
enum PaymentType {
    CASH, CREDIT
};
string getCurrentTime() {
    auto currentTimePoint = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(currentTimePoint);
    string timeStr;
    {
        stringstream ss;
        ss << put_time(localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
        timeStr = ss.str();
    }
    return timeStr;
}

int getHourDifference(const string& timeStr1, const string& timeStr2) {
    tm tm1 = {};
    tm tm2 = {};
    stringstream ss1(timeStr1);
    stringstream ss2(timeStr2);
    ss1 >> get_time(&tm1, "%Y-%m-%d %H:%M:%S");
    ss2 >> get_time(&tm2, "%Y-%m-%d %H:%M:%S");
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    int secondsDifference = abs(int(difftime(time1, time2)));
    int hoursDifference = secondsDifference / 3600;
    return hoursDifference;
}

std::string addFiveHours(const std::string& timeStr) {
    // Convert time string to tm structure
    struct tm tmStruct = {};
    std::istringstream ss(timeStr);
    ss >> std::get_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
    
    if (ss.fail()) {
        std::cerr << "Failed to parse the input time string: " << timeStr << std::endl;
        return "";  // Handle the error appropriately
    }

    time_t oldTime = std::mktime(&tmStruct);
    time_t newTime = oldTime + 5 * 3600;
    struct tm* newTm = std::localtime(&newTime);
    
    std::ostringstream newTimeStr;
    newTimeStr << std::put_time(newTm, "%Y-%m-%d %H:%M:%S");
    return newTimeStr.str();
}

class Rates {
    unordered_map<int, int> rateMap;
    static Rates* rates;
    Rates() {}
    public:
        static unordered_map<int, int> getMap() {
            if(rates == nullptr) {
                unordered_map<int, int>temp;
                ParkingSpotType s1 = MOTORCYLE, s2 = COMPACT, s3 = LARGE;
                temp[s1] = 10, temp[s2] = 30, temp[s3] = 70;
                rates = new Rates();
                rates->rateMap = temp;
            }
            return rates->rateMap;
        }
};


class TypeMatch {
    unordered_map<int, int> VehicleSpotMap;
    static TypeMatch* typeMatchMap;
    TypeMatch() {}
    public :
        static TypeMatch* getMap() {
            if(typeMatchMap == nullptr) {
                ParkingSpotType s1 = MOTORCYLE, s2 = COMPACT, s3 = LARGE;
                VehicleType v1 = BIKE, v2 = CAR, v3 = TRUCK;
                typeMatchMap = new TypeMatch();
                unordered_map<int, int>temp;
                temp[v1] = s1, temp[v2] = s2, temp[v3] = s3;
                typeMatchMap->VehicleSpotMap = temp;
            }
            return typeMatchMap;
        }
        int getSpotType(VehicleType vehicleType) {
            return this->VehicleSpotMap[vehicleType];
        }
};

class Address {
    string city;
    string state;
    public:
    Address(string city, string state) {
        this->city = city;
        this->state = state;
    }
};

class Person {
    protected: 
        string name;
        int id;
        Address* address;
    public:
    //add a person to db
        Person* addPerson(string name, int id, Address* address) {
            Person* person = new Person();
            this->name = name;
            this->id = id;
            this->address = address;
            return person;
        }
};

class ParkingTicket { //vehicle
    string id;
    int floorId;
    int spotId;
    string vehicleNo;
    string starttime;
    string endtime;
    ParkingSpotType spottype;
    int amount;
    TicketStatus status;
    public:
    ParkingTicket* createTicket(string id, int floorId, int spotId, string vehicleNo, string starttime, ParkingSpotType spottype) {
        this->id = id;
        this->floorId = floorId;
        this->spotId = spotId;
        this->vehicleNo = vehicleNo;
        this->starttime = starttime;
        this->spottype = spottype;
        this->status = ACTIVE;
        return this;
    }
    TicketStatus getStatus() {
        return this->status;
    }
    void updateTicketStatus(ParkingTicket* ticket, TicketStatus status) {
        ticket->status = status;
        cout<<"Updated ticket status to : "<<status<<endl;
    }
    int getFloorId() {
        return this->floorId;
    }
    int getSpotId() {
        return this->spotId;
    }
    void display() {
        cout<<"Ticket details : "<<endl;
        cout<<"Vehicle no: "<<this->vehicleNo<<" floorId: "<<this->floorId<<" spotId: "<<this->spotId<<endl;
    }
    int calculate() {
        int hourlyRate = Rates :: getMap()[spottype];
        cout<<"Spot type : "<<spottype<<" for which hourly rate is : "<<hourlyRate<<endl;
        // this->endtime = getCurrentTime(); // this should be done
        this->endtime = addFiveHours(this->starttime);
        cout<<"Initiating calculation of amount"<<endl;
        int hours = getHourDifference(this->endtime, this->starttime);
        cout<<"Hour difference = "<<hours<<endl;
        int amount = hourlyRate * hours;
        this->amount = amount;
        return amount;
    }

};

class Spot;

class Vehicle {
    string vehicleNo;
    int id;
    VehicleType vehicleType;
    Person* person;
    int spotId;
    ParkingTicket* ticket;
    public:
    Vehicle(string vehicleNo, int id, VehicleType vehicleType, Person* person) {
        this->vehicleNo = vehicleNo;
        this->id = id;
        this->vehicleType = vehicleType;
        this->person = person;
        this->ticket = NULL;
    }
    VehicleType getVehicleType() {
        return this->vehicleType;
    }
    string getVehicleNo() {
        return this->vehicleNo;
    }
    ParkingTicket* addTicket(ParkingTicket* ticket) {
        return this->ticket = ticket;
    }
    ParkingTicket* getTicket() {
        return this->ticket;
    }
    int getSpotId() {
        return this->spotId;
    }
    int getId() {
        return this->id;
    }
    string getTicketId() {
        string ticketId = this->vehicleNo + "_1";
        return ticketId;
    }
    void addSpotToVehicle(int spotId) {
        this->spotId = spotId;
    }
    void removeSpotFromVehicle() {
        this->spotId = -1;
    }
    void display() {
        cout<<"Vehicle details"<<endl;
        cout<<"id : "<<this->id<<" vehicle_type : "<<this->vehicleType<<endl;
    }

};


class Spot {
    int id;
    int floorId;
    ParkingSpotType type;
    Vehicle* vehicle; 
    bool isEmpty;
    public:
    Spot* createSpot(int id, int floorId, ParkingSpotType type) {
        this->id = id;
        this->floorId = floorId;
        this->type = type;
        this->isEmpty = true;
        this->display();
        return this;
    }
    ParkingSpotType getSpotType() {
        return this->type;
    }
    int getId() {
        return this->id;
    }
    int getFloorId() {
        return this->floorId;
    }
    bool isSpotEmpty() {
        return this->isEmpty;
    }
    bool assignVehicle(Vehicle* vehicle) {
        this->vehicle = vehicle;
        this->vehicle->display();
        this->isEmpty = false;
        vehicle->addSpotToVehicle(this->id);
        return true;
    }
    bool removeVehicle(Spot* spot) {
        //Spot* spot = vehicle->getSpotOfVehicle();
        Vehicle* vehicle = spot->vehicle;
        vehicle->removeSpotFromVehicle();
        spot->vehicle = NULL;
        spot->isEmpty = true;
        return true;
    }
    void display() {
        cout<<"Spot Details"<<endl;
        cout<<this->type<<" "<<this->id<<" "<<this->floorId<<endl;
    }
};

class DisplayBoard {
    int id;
    unordered_map< int, vector<Spot*> >availableSpotsMap;
    unordered_map< int, int> count;
    public:
    DisplayBoard* createDisplayBoard(int id, unordered_map< int, vector<Spot*> >availableSpotsMap, unordered_map< int, int >count) {
        this->id = id;
        this->availableSpotsMap = availableSpotsMap;
        this->count = count;
        return this;
    }
    bool isAvailable(VehicleType type) {
        TypeMatch* typeMatchMap = TypeMatch :: getMap();
        int spotType = typeMatchMap->getSpotType(type);
        if(count[spotType] > 0) return true;
        else return false;
    }
    Spot* getAvailableSpot(VehicleType type) {
        TypeMatch* typeMatchMap = TypeMatch :: getMap();
        int spotType = typeMatchMap->getSpotType(type);
        return this->availableSpotsMap[spotType][0];
    } 
    void removeSpotFromMap(Spot* spot) {
        ParkingSpotType spottype = spot->getSpotType();
        if(this->availableSpotsMap[spottype].size() > 0) {
            this->availableSpotsMap[spottype].erase(availableSpotsMap[spottype].begin());
            this->count[spottype]--;
        }
    }
    void addSpotToMap(Spot* spot) {
        ParkingSpotType spottype = spot->getSpotType();
        this->availableSpotsMap[spottype].push_back(spot);
        this->count[spottype]++;
    }
    void display() {
        vector<ParkingSpotType> types;
        types.push_back(MOTORCYLE);
        types.push_back(COMPACT);
        types.push_back(LARGE);
        cout<<"---------------------------------SPOTS--------------------------------"<<endl;
        for(auto t : types) {
            cout<<"Spot type : "<<t<<" Available spots : "<<this->count[t]<<endl;
        }
        cout<<"----------------------------------------------------------------------"<<endl;
    }

};

class Floor {
    int id;
    vector<Spot*>spots;
    DisplayBoard* board;
    public:
    //
    int getId() {
        return this->id;
    }
    //add spot api
    void addSpot(Spot* spot) {
        this->spots.push_back(spot);
    }
    //create floor
    Floor* createFloor(int id) {
        this->id = id;
        this->display();
        return this;
    }
    void display() {
        cout<<"Floor details : "<<endl;
        cout<<this->id<<" "<<endl;
    }
    Spot* getSpotById(int id) {
        for(auto s : this->spots) {
            if(s->getId() == id) return s;
        }
        return NULL;
    }
    vector<Spot*> getSpots() {
        return this->spots;
    }
    
    //add board api
    DisplayBoard* addDisplayBoard() {
        unordered_map<int, vector<Spot*> >availableSpotsMap; 
        unordered_map<int, int>count;
        vector<ParkingSpotType> types;
        types.push_back(MOTORCYLE);
        types.push_back(COMPACT);
        types.push_back(LARGE);
        for(auto spot : spots) {
            if(spot->isSpotEmpty()) 
                availableSpotsMap[spot->getSpotType()].push_back(spot);
        }
        for(auto t : types) {
            count[t] = availableSpotsMap[t].size();
        }
        DisplayBoard* board = new DisplayBoard();
        board->createDisplayBoard(this->id, availableSpotsMap, count);
        this->board = board;
        return this->board;
    }
    DisplayBoard* getDisplayBoard() {
        return this->board;
    }
};

class Payment {
    public:
        virtual void processPayment(ParkingTicket* ticket, PaymentType paymentType) = 0;
};

class Credit : public Payment {
    public:
        //do payment by credit card
        void processPayment(ParkingTicket* ticket, PaymentType paymentType) {
            int amount  = ticket->calculate();
            cout<<"Payment done by credit card for an amount : "<<amount<<endl;
        }
};

class Cash : public Payment {
    public:
        //do payment by cash
        void processPayment(ParkingTicket* ticket, PaymentType paymentType) {
            int amount  = ticket->calculate();
            cout<<"Payment done by cash for an amount : "<<amount<<endl;
        }
};

class Entry {
    int id;
    public:
        Entry* createEntryGate(int id) {
            Entry* gate = new Entry();
            gate->id = id;
            return gate;
        } 
        int getId() {
            return this->id;
        }
        ParkingTicket* addTicket(string ticketId, int floorId, int spotId, string vehicleNo, ParkingSpotType spottype) {
            ParkingTicket* ticket = new ParkingTicket();
            ticket->createTicket(ticketId, floorId, spotId, vehicleNo, getCurrentTime(), spottype);
            ticket->display();
            return ticket;
        }
};

class Exit {
    int id;
    public:
        Exit* createExitGate(int id) {
            Exit* gate = new Exit();
            gate->id = id;
            return gate;
        } 
        int getId() {
            return this->id;
        }
        void expireTicket(ParkingTicket* ticket) {
            ticket->updateTicketStatus(ticket, PAID);
        }

};

class ParkingLot {
    vector<Floor*>floors;
    vector<Entry*>entryGates;
    vector<Exit*>exitGates;
    Address* address;
    string name;
    static ParkingLot* parkingLot;
    ParkingLot() {}
    ParkingLot(Address* address, string name) {
        this->address = address;
        this->name = name;
    }
    ParkingLot operator=(const ParkingLot &);
    public:
        static ParkingLot* addParkingLot(Address* address, string name);
        Floor* addFloor(int id) {
            if(!parkingLot) {
                cout<<"No parking lot available"<<endl;
                return NULL;
            }
            else {
                Floor* floor = new Floor();
                floor->createFloor(id);
                parkingLot->floors.push_back(floor);
                return floor;
            }
        }
        Entry* addEntry(int id) {
            if(!parkingLot) {
                cout<<"No parking lot available"<<endl;
                return NULL;
            }
            else {
                Entry* gate = new Entry();
                gate->createEntryGate(id);
                parkingLot->entryGates.push_back(gate);
                return gate;
            }
        }
        Exit* addExit(int id) {
            if(!parkingLot) {
                cout<<"No parking lot available"<<endl;
                return NULL;
            }
            else {
                Exit* gate = new Exit();
                gate->createExitGate(id);
                parkingLot->exitGates.push_back(gate);
                return gate;
            }
        }
        Floor* getFloorById(int id) {
            for(auto f : floors) {
                if(f->getId() == id) return f;
            }
            return NULL;
        }
        Entry* getEntryById(int id) {
            for(auto en : entryGates) {
                if(en->getId() == id) return en;
            }
            return NULL;
        }
        Exit* getExitById(int id) {
            for(auto ex : exitGates) {
                if(ex->getId() == id) return ex;
            }
            return NULL;
        }
        static ParkingLot* getParkingLot() {
            return parkingLot;
        }
        vector<Floor*> getFloors() {
            return parkingLot->floors;
        }
};

ParkingLot* ParkingLot :: addParkingLot(Address* address, string name){
    if(parkingLot == nullptr) {
        parkingLot = new ParkingLot();
        parkingLot->address = address;
        parkingLot->name = name;
    }
    return parkingLot;
}


class ParkingAttendant : public Person {
    Payment* paymentService;
    Entry* entry;
    Exit* exit;
    public:
        void assignToEntry(Entry* gate) {
            this->entry = gate;
        }
        void assignToExit(Exit* gate) {
            this->exit = gate;
        }
        void exitVehicle(Vehicle* vehicle, ParkingTicket* ticket, PaymentType paymentType) {
            if(paymentType == CASH) paymentService = new Cash();
            else paymentService = new Credit();
            if(ticket->getStatus() == PAID) cout<<"Cannot take payment from a vehicle that has already cleared payment"<<endl;
            paymentService->processPayment(ticket, paymentType);
            //update ticket status
            exit->expireTicket(ticket);
            //empty parking spot
            int spotId = ticket->getSpotId();
            int floorId = ticket->getFloorId();
            ParkingLot* parkingLot = ParkingLot :: getParkingLot();
            Floor* floor = parkingLot->getFloorById(floorId);
            Spot* vehicleSpot = floor->getSpotById(spotId);
            DisplayBoard* board = floor->getDisplayBoard();
            vehicleSpot->display();
            vehicleSpot->removeVehicle(vehicleSpot);
            //update board
            board->addSpotToMap(vehicleSpot);
            cout<<"Removed vehicle from floor : "<<floor->getId()<<" on spot : "<<vehicleSpot->getId()<<endl;
            board->display();
        }

        void enterVehicle(Vehicle* vehicle) {
            ParkingLot* parkingLot = ParkingLot :: getParkingLot();
            vector<Floor*>floors = parkingLot->getFloors();
            VehicleType vehicleType = vehicle->getVehicleType();
            bool assigned = false;
            for(int i=0; i<floors.size(); i++) {
                DisplayBoard* board = floors[i]->getDisplayBoard();
                if(board->isAvailable(vehicleType)) {
                    //get available spot
                    Spot* availableSpot = board->getAvailableSpot(vehicleType);
                    cout<<"Available spot details"<<endl;
                    availableSpot->display();
                    //assign vehicle to the spot and update display board
                    availableSpot->assignVehicle(vehicle);
                    board->removeSpotFromMap(availableSpot);
                    //create parking ticket and assign it to the vehicle
                    ParkingTicket* ticket = entry->addTicket(vehicle->getTicketId(), floors[i]->getId(), availableSpot->getId(), vehicle->getVehicleNo(), availableSpot->getSpotType());
                    ticket->display();
                    vehicle->addTicket(ticket);
                    //display
                    assigned = true;
                    cout<<"Assigned vehicle : "<<vehicle->getId()<<" to floor : "<<floors[i]->getId()<<" of type : "<<vehicle->getVehicleType()<<" on spot : "<<availableSpot->getId()<<endl;
                    break;
                    //board->display();
                }
            }
            if(!assigned) cout<<"No spaces available for vehicle : "<<vehicle->getId()<<" of type : "<<vehicle->getVehicleType()<<endl;
        }
};



class Admin : public Person {
    public:
       //add parking spot
        Spot* addParkingSpot(Floor* floor, int id, ParkingSpotType type) {
            Spot* spot = new Spot();
            spot->createSpot(id, floor->getId(), type);
            floor->addSpot(spot); 
            return spot;
        }
        //add floor
        Floor* addParkingFloor(ParkingLot* lot, int id) {
            return lot->addFloor(id);
        }
        //add entry gate
        Entry* addEntryGate(ParkingLot* lot, int id) {
            return lot->addEntry(id);
        }
        //add exit gate
        Exit* addExitGate(ParkingLot* lot, int id) {
            return lot->addExit(id);
        }
        //add display board
        DisplayBoard* addDisplayBoard(Floor* floor) {
            return floor->addDisplayBoard();
        }

};



ParkingLot* ParkingLot :: parkingLot = nullptr;
Rates* Rates::rates = nullptr;
TypeMatch* TypeMatch::typeMatchMap = nullptr;

int main() {
    cout<<"************************************************************************"<<endl;
    Address* add1 = new Address("bangalore", "karnataka");
    Address* add2 = new Address("kolkata", "west bengal");
    Address* add3 = new Address("hyderabad", "telengana");
    Address* add4 = new Address("mumbai", "maharashtra");
 
    ParkingLot* parkingLot = ParkingLot :: addParkingLot(add3, "Ritwika's Lot");

    Admin* Ritwika = new Admin();
    Ritwika->addPerson("Ritwika", 0, add1);

    int numberOfFloors = 2;
    int numberOfSpotsOnEachFloor = 6; //2 motorcyle, 3 compact, 1 large
    for(int i=0; i<numberOfFloors; i++) {
        //add floor
        Floor* floor = Ritwika->addParkingFloor(parkingLot, i);
        //add spots for each floor
        ParkingSpotType type = MOTORCYLE;
        int spotCount = 0;
        for(int j=0; j<2; j++) {
            Spot* spot = Ritwika->addParkingSpot(floor, spotCount, type);
            spotCount++;
        }
        type = COMPACT;
        for(int j=0; j<3; j++) {
            Spot* spot = Ritwika->addParkingSpot(floor, spotCount, type);
            spotCount++;
        }        
        type = LARGE;
        for(int j=0; j<1; j++) {
            Spot* spot = Ritwika->addParkingSpot(floor, spotCount, type);
            spotCount++;
        }
        int entryCount = 0, exitCount = 0;
        //add display board for each floor
        DisplayBoard* board = Ritwika->addDisplayBoard(floor);
        board->display();
    }

    Entry* entry1 = parkingLot->addEntry(0);
    Entry* entry2 = parkingLot->addEntry(1);
    Exit* exit1 = parkingLot->addExit(2);
    Exit* exit2 = parkingLot->addExit(3);

    ParkingAttendant* Rina = new ParkingAttendant();
    Rina->addPerson("Rina", 1, add2); //entry
    Rina->assignToEntry(entry1);

    ParkingAttendant* Roshmi = new ParkingAttendant();
    Roshmi->addPerson("Roshmi", 2, add1); //exit
    Roshmi->assignToEntry(entry2);

    ParkingAttendant* Avishek = new ParkingAttendant();
    Avishek->addPerson("Avishek", 3, add2); //entry
    Avishek->assignToExit(exit1);

    ParkingAttendant* Subarna = new ParkingAttendant();
    Subarna->addPerson("Subarna", 4, add2); //exit
    Subarna->assignToExit(exit2);

    Person* Sagnik = new Person();
    Sagnik->addPerson("Sagnik", 5, add2);
    Person* Swatantra = new Person();
    //Swatantra->addPerson("Swatantra", 6, add2);

    //Sagnik's Bike and Car
    Vehicle* bike1 = new Vehicle("WB009321", 0, BIKE, Sagnik);
    Vehicle* car1 = new Vehicle("WB654470", 1, CAR, Sagnik);
    Vehicle* car2 = new Vehicle("WB654471", 2, CAR, Sagnik);
    Vehicle* car3 = new Vehicle("WB654472", 3, CAR, Sagnik);
    Vehicle* car4 = new Vehicle("WB654473", 4, CAR, Sagnik);
    Vehicle* car5 = new Vehicle("WB654474", 5, CAR, Sagnik);

    //Swatantra's Bike, Car
    Vehicle* truck1 = new Vehicle("WB009300", 6, TRUCK, Swatantra);
    Vehicle* truck2 = new Vehicle("WB009301", 7, TRUCK, Swatantra);
    Vehicle* truck3 = new Vehicle("WB989800", 8, TRUCK, Swatantra);
    Vehicle* truck4 = new Vehicle("WB989801", 9, TRUCK, Swatantra);
    Vehicle* car6 = new Vehicle("WB654475", 10, CAR, Swatantra);
    Vehicle* car7 = new Vehicle("WB654476", 11, CAR, Swatantra);

    Rina->enterVehicle(bike1); 
    Roshmi->enterVehicle(car1);
    Roshmi->enterVehicle(car2);
    Avishek->enterVehicle(car3);
    Avishek->enterVehicle(car4);
    Roshmi->enterVehicle(car5);
    Roshmi->enterVehicle(car6);
    Avishek->enterVehicle(car7);
    Avishek->enterVehicle(truck1);
    Avishek->enterVehicle(truck2);

    Subarna->exitVehicle(bike1, bike1->getTicket(), CASH);
    Subarna->exitVehicle(car4, car4->getTicket(), CREDIT);
    Rina->exitVehicle(car5, car5->getTicket(), CREDIT);
    Rina->exitVehicle(car6, car6->getTicket(), CREDIT);
    Subarna->exitVehicle(truck1, truck1->getTicket(), CREDIT);

    return 0;

}
