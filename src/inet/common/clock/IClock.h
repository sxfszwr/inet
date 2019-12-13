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

#ifndef __INET_ICLOCK_H
#define __INET_ICLOCK_H

#include "SimClockTime.h"

namespace inet {

#ifdef WITH_CLOCK_SUPPORT

/**
 * Abstract base class for modeling a (hardware) clock. Having a clock in the
 * model of a network device and using it for all timing (instead of the
 * standard OMNeT++ simulation time) allows for modeling clock skew
 * and clock drift.
 *
 * Clocks are typically simple modules, and are used by other modules via
 * methods of the SimpleModule base class (scheduleClockEvent(), cancelClockEvent(),
 * etc.)
 */
class INET_API IClock
{
  protected:
    cSimpleModule *getTargetModule() const;

  public:
    virtual ~IClock() {}

    /**
     * Return the current time.
     */
    virtual simclocktime_t getClockTime() const = 0;

    /**
     * Schedule an event to be delivered to the context module at the given time.
     */
    virtual void scheduleClockEvent(simclocktime_t t, cMessage *msg) = 0;

    /**
     * Cancels an event.
     */
    virtual cMessage *cancelClockEvent(cMessage *msg) = 0;

    /**
     * Returns the arrival time of a message scheduled via scheduleClockEvent().
     */
    virtual simclocktime_t getArrivalClockTime(cMessage *msg) const = 0;

};

inline cSimpleModule *IClock::getTargetModule() const
{
    cSimpleModule *target = getSimulation()->getContextSimpleModule();
    if (target == nullptr)
        throw cRuntimeError("IClock::scheduleAt()/cancelEvent() must be called with a simple module in context");
    std::cout << "targetPtr=" << target << std::endl;
    std::cout << "targetFullPath=" << target->getFullPath() << std::endl;
    return target;
}


#endif // ifdef WITH_CLOCK_SUPPORT

} // namespace inet

#endif // ifndef __INET_ICLOCK_H

