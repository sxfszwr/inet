//
// Copyright (C) OpenSim Ltd.
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
// along with this program; if not, see http://www.gnu.org/licenses/.
//

#ifndef __INET_PACKETMARKERBASE_H
#define __INET_PACKETMARKERBASE_H

#include "inet/queueing/base/PacketFlowBase.h"
#include "inet/queueing/contract/IPacketMarker.h"

namespace inet {
namespace queueing {

class INET_API PacketMarkerBase : public PacketFlowBase, public virtual IPacketMarker
{
  protected:
    virtual void processPacket(Packet *packet) override;
    virtual void markPacket(Packet *packet) = 0;
};

} // namespace queueing
} // namespace inet

#endif // ifndef __INET_PACKETMARKERBASE_H

