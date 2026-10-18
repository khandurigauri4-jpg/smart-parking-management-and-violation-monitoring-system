#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include "include/Vehicle.h"
#include "include/User.h"
#include "include/ParkingSlot.h"
#include "include/Ticket.h"
#include "include/Violation.h"

static bool validTime(const std::string& t) {
    if (t.size()!=5 || t[2]!=':') return false;
    if (!isdigit(t[0])||!isdigit(t[1])||!isdigit(t[3])||!isdigit(t[4])) return false;
    return std::stoi(t.substr(0,2))<=23 && std::stoi(t.substr(3,2))<=59;
}
int main(int argc,char**argv){
    // quote VEHICLE OWNER CATEGORY(2|4) VEHICLE_NAME SLOT ENTRY(HH:MM) EXIT(HH:MM) ALLOWED_HOURS
    if(argc!=9){std::cerr<<"Expected 8 arguments: vehicle owner category name slot entry exit allowedHours\n";return 2;}
    try {
        std::string plate=argv[1], owner=argv[2], category=argv[3], name=argv[4], slotId=argv[5], entry=argv[6], exit=argv[7];
        int hours=std::stoi(argv[8]);
        if(plate.empty()||owner.empty()||(category!="2"&&category!="4")||!validTime(entry)||!validTime(exit)||hours<1||hours>24){std::cerr<<"Invalid ticket input\n";return 2;}
        Vehicle* vehicle = category=="2" ? static_cast<Vehicle*>(new TwoWheeler(plate,name)) : static_cast<Vehicle*>(new FourWheeler(plate,name));
        User user(owner,vehicle); ParkingSlot slot(slotId); Ticket ticket("API",&user,&slot,entry,hours); ticket.exitParking(exit);
        if (ticket.hasOverstay()) { Violation violation(vehicle, "Overstay", ticket.getPenalty()); (void)violation; }
        std::cout<<std::fixed<<std::setprecision(2)<<"{\"parkingFee\":"<<ticket.getParkingFee()<<",\"penalty\":"<<ticket.getPenalty()<<",\"total\":"<<(ticket.getParkingFee()+ticket.getPenalty())<<",\"durationMinutes\":"<<ticket.getActualMinutes()<<",\"overstayMinutes\":"<<ticket.getOverstayMinutes()<<",\"hasOverstay\":"<<(ticket.hasOverstay()?"true":"false")<<",\"ratePer30Minutes\":"<<vehicle->getRatePerBlock()<<"}\n";
        delete vehicle; return 0;
    } catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 2;}
}
