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

package inet.protocol.ethernet;

import inet.linklayer.contract.IEthernetInterface;
import inet.networklayer.common.InterfaceEntry;
import inet.protocol.common.InterPacketGap;
import inet.protocol.common.PacketEmitter;
import inet.protocol.common.PaddingInserter;
import inet.protocol.fragmentation.FragmentTagBasedDefragmenter;
import inet.protocol.fragmentation.FragmentTagInserter;
import inet.protocol.scheduler.PreemptingScheduler;
import inet.protocol.server.PreemptingServer;
import inet.protocol.transceiver.PreemptibleTransmitter;
import inet.protocol.transceiver.StreamingReceiver;
import inet.queueing.classifier.PacketClassifier;
import inet.queueing.common.PacketMultiplexer;
import inet.queueing.queue.PacketQueue;
import inet.queueing.scheduler.PriorityScheduler;
import inet.queueing.server.InstantServer;

module EthernetInterfaceS extends InterfaceEntry like IEthernetInterface
{
    parameters:
        bool broadcast = default(true);
        bool multicast = default(true);
        string address = default("auto");
        double bitrate @unit(bps);
        string interfaceTableModule;
        @class(InterfaceEntry);
        @display("i=block/ifcard");
    gates:
        input upperLayerIn;
        output upperLayerOut;
        inout phys;
    submodules:
        typeOrLengthInserter: EthernetTypeOrLengthInserter {
            @display("p=300,100");
        }
        addressInserter: EthernetAddressInserter {
            @display("p=300,200");
        }
        paddingInserter: PaddingInserter {
            minLength = default(60B);
            @display("p=300,300");
        }
        priorityClassifier1: PacketClassifier {
            classifierClass = default("inet::EthernetExpressOverNormalClassifier");
            @display("p=300,400");
        }
        expressQueue: PacketQueue {
            @display("p=188,526");
        }
        normalQueue: PacketQueue {
            @display("p=438,500");
        }
        expressFcsInserter: EthernetFcsInserter {
            @display("p=188,659");
        }
        expressPreambleInserter: EthernetPreambleInserter {
            @display("p=188,791");
        }
        fragmentFcsInserter: EthernetFragmentFcsInserter {
            @display("p=438,681");
        }
        fragmentPreambleInserter: EthernetFragmentPreambleInserter {
            @display("p=438,791");
        }
        outboundEmitter: PacketEmitter {
            direction = "outbound";
            protocolName = "ethernetphy";
            @display("p=300,1200");
        }
        interFrameGapInserter: InterPacketGap {
            duration = default(96s / dropUnit(bitrate));
            @display("p=300,1300");
        }
        transmitter: PreemptibleTransmitter {
            datarate = default(bitrate);
            @display("p=300,1400");
        }
        typeOrLengthChecker: EthernetTypeOrLengthChecker {
            @display("p=950,100");
        }
        addressChecker: EthernetAddressChecker {
            @display("p=950,200");
        }
        m2: PacketMultiplexer {
            @display("p=950,800");
        }
        expressFcsChecker: EthernetFcsChecker {
            @display("p=800,900");
        }
        expressPreambleChecker: EthernetPreambleChecker {
            @display("p=800,1100");
        }
        defragmenter: FragmentTagBasedDefragmenter {
            @display("p=1100,900");
        }
        fragmentFcsChecker: EthernetFragmentFcsChecker {
            @display("p=1100,1000");
        }
        fragmentPreambleChecker: EthernetFragmentPreambleChecker {
            @display("p=1100,1100");
        }
        preambleClassifier: PacketClassifier {
            classifierClass = default("inet::EthernetPreambleClassifier");
            @display("p=950,1200");
        }
        inboundEmitter: PacketEmitter {
            direction = "inbound";
            protocolName = "ethernetphy";
            @display("p=950,1300");
        }
        receiver: StreamingReceiver {
            datarate = default(bitrate);
            @display("p=950,1400");
        }
        preemptingScheduler: PreemptingScheduler {
            @display("p=300,907");
        }
        instantServer: InstantServer {
            @display("p=300,1046");
        }
        fragmentTagInserter: FragmentTagInserter {
            @display("p=438,588");
        }
    connections:
        upperLayerIn --> typeOrLengthInserter.in;
        typeOrLengthInserter.out --> addressInserter.in;
        addressInserter.out --> paddingInserter.in;
        paddingInserter.out --> priorityClassifier1.in;
        priorityClassifier1.out++ --> normalQueue.in;
        priorityClassifier1.out++ --> expressQueue.in;
        expressFcsInserter.out --> expressPreambleInserter.in;
        fragmentFcsInserter.out --> fragmentPreambleInserter.in;
        outboundEmitter.out --> interFrameGapInserter.in;
        interFrameGapInserter.out --> transmitter.in;
        transmitter.out --> phys$o;

        phys$i --> receiver.in;
        receiver.out --> inboundEmitter.in;
        inboundEmitter.out --> preambleClassifier.in;
        preambleClassifier.out++ --> expressPreambleChecker.in;
        preambleClassifier.out++ --> fragmentPreambleChecker.in;
        expressPreambleChecker.out --> expressFcsChecker.in;
        expressFcsChecker.out --> m2.in++;
        fragmentPreambleChecker.out --> fragmentFcsChecker.in;
        fragmentFcsChecker.out --> defragmenter.in;
        defragmenter.out --> m2.in++;
        m2.out --> addressChecker.in;
        addressChecker.out --> typeOrLengthChecker.in;
        typeOrLengthChecker.out --> upperLayerOut;
        expressPreambleInserter.out --> preemptingScheduler.in++;
        fragmentPreambleInserter.out --> preemptingScheduler.in++;
        expressQueue.out --> expressFcsInserter.in;
        preemptingScheduler.out --> instantServer.in;
        instantServer.out --> outboundEmitter.in;
        normalQueue.out --> fragmentTagInserter.in;
        fragmentTagInserter.out --> fragmentFcsInserter.in;
}
