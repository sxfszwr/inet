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

#include "inet/common/checksum/EthernetCRC.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/protocol/ethernet/EthernetFcsVerification2.h"

namespace inet {

Define_Module(EthernetFcsVerification2);

void EthernetFcsVerification2::initialize(int stage)
{
    PacketFilterBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
//        registerService(IProtocol::fcs, nullptr, inputGate);
//        registerProtocol(IProtocol::fcs, nullptr, outputGate);
    }
}

bool EthernetFcsVerification2::matchesPacket(Packet *packet)
{
    const auto& ethTrailer = packet->peekAtBack<EthernetFcs>(B(4));
    switch (ethTrailer->getFcsMode()) {
        case FCS_DECLARED_CORRECT: {
            // if the FCS mode is declared to be correct, then the check passes if and only if the chunks are correct
            const auto& data = packet->peekData();
            return ethTrailer->isCorrect() && data->isCorrect();
        }
        case FCS_DECLARED_INCORRECT:
            // if the FCS mode is declared to be incorrect, then the check fails
            return false;
        case FCS_COMPUTED: {
            // otherwise compute the FCS, the check passes if the result is 0xFFFF (includes the received FCS) and the chunks are correct
            const auto& data = packet->peekDataAt<BytesChunk>(B(0), packet->getDataLength() - ethTrailer->getChunkLength());
            auto dataLength = B(data->getChunkLength()).get();
            uint32_t computedFcs = ethernetCRC(data->getBytes().data(), dataLength);
            auto receivedFcs = ethTrailer->getFcs();
            return receivedFcs == computedFcs;
        }
        default:
            throw cRuntimeError("Unknown FCS mode");
    }
}

} // namespace inet

