//
// Copyright (C) 2019 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "SimpleModule.h"

namespace inet {

void SimpleModule::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    if (stage == 0)
        clock = resolveClockModule();

    #ifndef NDEBUG
    clazz = getClassName();
#endif
}

SimpleModule::~SimpleModule()
{
#ifndef NDEBUG
    bool supportsClock = hasPar("clockModule");
    if (supportsClock && !usesClockApi)
        std::cerr << "** Warning: Class '" << clazz << "' has a 'clockModule' parameter but does not use the clock API (at least in this simulation)\n";
    if (!supportsClock && usesClockApi)
        std::cerr << "** Warning: Class '" << clazz << "' uses the clock API but does not have a 'clockModule' parameter\n";
#endif
}

IClock *SimpleModule::resolveClockModule()
{
    if (hasPar("clockModule")) {
        const char *clockModulePath = par("clockModule");
        if (*clockModulePath) {
            cModule *clockModule = getModuleByPath(clockModulePath);
            if (clockModule == nullptr)
                throw cRuntimeError("Clock module '%s' not found", clockModulePath);
            IClock *clock = check_and_cast<IClock*>(clockModule);
            return clock;
        }
    }
    return nullptr;
}

void SimpleModule::scheduleClockEvent(simclocktime_t t, cMessage *msg)
{
#ifndef NDEBUG
    usesClockApi = true;
#endif
    if (clock)
        clock->scheduleClockEvent(t, msg);
    else
        cSimpleModule::scheduleAt(t.asSimTime(), msg);
}

cMessage *SimpleModule::cancelClockEvent(cMessage *msg)
{
#ifndef NDEBUG
    usesClockApi = true;
#endif
    if (clock)
        return clock->cancelClockEvent(msg);
    else
        return cSimpleModule::cancelEvent(msg);
}

void SimpleModule::cancelAndDeleteClockEvent(cMessage *msg)
{
#ifndef NDEBUG
    usesClockApi = true;
#endif
    if (clock)
        delete clock->cancelClockEvent(msg);
    else
        cSimpleModule::cancelAndDelete(msg);
}

simclocktime_t SimpleModule::getClockTime() const
{
#ifndef NDEBUG
    usesClockApi = true;
#endif
    if (clock)
        return clock->getClockTime();
    else
        return SimClockTime::from(simTime());
}

simclocktime_t SimpleModule::getArrivalClockTime(cMessage *msg) const
{
#ifndef NDEBUG
    usesClockApi = true;
#endif
    if (clock)
        return clock->getArrivalClockTime(msg);
    else
        return SimClockTime::from(msg->getArrivalTime());
}

} // namespace inet


