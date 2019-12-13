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

#ifndef __INET_SIMPLEMODULE_H
#define __INET_SIMPLEMODULE_H

#include "inet/common/clock/IClock.h"

namespace inet {

/**
 * Base class for most INET simple modules.
 */
class INET_API SimpleModule : public cSimpleModule
{
  protected:
    IClock *clock = nullptr;
#ifndef NDEBUG
    mutable bool usesClockApi = false;
    const char *clazz = nullptr; // saved class name for use in destructor
#endif
  protected:
    IClock *resolveClockModule();

  public:
    SimpleModule(unsigned stacksize = 0) : cSimpleModule(stacksize) {}
    ~SimpleModule();
    virtual void initialize(int stage);

    virtual void scheduleClockEvent(simclocktime_t t, cMessage *msg);
    virtual cMessage *cancelClockEvent(cMessage *msg);
    virtual void cancelAndDeleteClockEvent(cMessage *msg);
    virtual simclocktime_t getClockTime() const;
    virtual simclocktime_t getArrivalClockTime(cMessage *msg) const;

    using cSimpleModule::uniform;
    using cSimpleModule::exponential;
    using cSimpleModule::normal;
    using cSimpleModule::truncnormal;
    virtual SimClockTime uniform(SimClockTime a, SimClockTime b, int rng=0) const  {return uniform(a.dbl(), b.dbl(), rng);}
    virtual SimClockTime exponential(SimClockTime mean, int rng=0) const  {return exponential(mean.dbl(), rng);}
    virtual SimClockTime normal(SimClockTime mean, SimClockTime stddev, int rng=0) const  {return normal(mean.dbl(), stddev.dbl(), rng);}
    virtual SimClockTime truncnormal(SimClockTime mean, SimClockTime stddev, int rng=0) const  {return truncnormal(mean.dbl(), stddev.dbl(), rng);}

};


} // namespace inet

#endif // ifndef __INET_SIMPLEMODULE_H

