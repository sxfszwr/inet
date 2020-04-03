:orphan:

Analog Model
============

Goals
-----

.. so

    this showcase describes the available analog models, their degrees of freedom, limitations
    advantages, and the typical use cases

    benefit/advantage

    when to use which one

    -what are analog models

    In INET, radio signal analog models

    The analog representation of radio signals

    so

    - analog signal representation models how radio signals are
    - signal analog models model radio signals are they are transmitted, received and propagate
    - how the signals are represented
    - there are multiple models, with different complexity, depending on which is needed

    - tradeoff between accuracy and performance

    Radio signal analog models model signals as they are transmitted, received and propagate in the wireless medium.

    Radio signal analog models represent signals as they are transmitted, received and propagate in the wireless medium. Models of different complexity are available, suitable for different purposes.
    Ranging from less accurate models when simulating the details of radio signals is not important, to very accurate but slower models for accurate signal simulation.

    Some example...

    the unit disk just has a range (two ranges)
    the scalar has frequency data but nothing else
    the dimensional models spectrums accurately

In INET, radio signal analog models represent signals as they are transmitted, received and propagate in the wireless medium. Models of different complexity are available, suitable for different purposes.

This showcase describes the avaialbe analog signal representation models, their advantages and drawbacks, and typical use cases. There are three example simulations for typical use cases.

.. Through three example simulations.

   - there are examples for typical use cases

| INET version: ``4.2``
| Source files location: `inet/showcases/wireless/analogmodel <https://github.com/inet-framework/inet-showcases/tree/master/wireless/analogmodel>`__

About Analog Models
-------------------

more details about analog models, performance/accuracy
what kinds are available
its part of the radio medium module

    so

    the analog model

    - it implements the analog signal representation, i.e. how signals as a physical phenomena are represented/at what detail level they are represented/what features of them are represented/modeled.
    - this is a simple module, a submodule of the radio medium module
    - several types exist, ranging from simple to complex, suitable for different purposes
    - when the details are not important, a simpler model is sufficient
    - such as the unit disk is sufficient for testing routing protocols, because it just has a communication range, and that level of abstration is enough for testing routing protocols
    - but when its important that reception depends on distance/power level and snir, the scalar model is sufficient. there is just one wifi channel, the interesting part is the snir
    - then the emphasis might be on multiple wifi channels and interference. for that the dimensional analog model is needed

The analog signal representation is implemented as a submodule of the radio medium module.
INET contains the following analog model types, presented in the order of increasing complexity:

- **Unit disk**: Simple model featuring only communication range and interference range as parameters. Reception in communication range is always successful, and outside of it always unsuccessful. Signals can be detected in the interference range, which might cause network nodes to defer from transmitting. Suitable for simulations where the emphasis is not on the details of radio transmissions, but something else, e.g. testing routing protocols.
- **Scalar**: Signal power is represented by a scalar value. Transmissions have a center frequency and bandwidth. **TODO** attentuation due to distance or other stuff Signal interfere when their center frequency and bandwidth is exactly the same (and the signal power is high enough TODO). When there is no overlap in the frequency spectrum, they don't interfere. Overlapping signals can't be simulated with this model, it throws and error. **TODO** suitable for what
- **Dimensional**: Signal power is represented as a multidimensional function of frequency and time; arbitrary signal shapes can be defined in time and the frequency spectrum. Attenuation and interference is modeled accurately. Suitable for simulating cross-technology interference (**TODO** see also Coexistence) **TODO** suitable for accurate interference and CTI simulation

More complex models are more accurate but require more computing performance.
INET contains a version of radio and radio medium module for each type and technology, e.g. :ned:`Ieee80211UnitDiskRadio`/:ned:`UnitDiskRadioMedium`, :ned:`ApskScalarRadio`/:ned:`ApskScalarRadioMedium`,
:ned:`Ieee802154NarrowbandDimensionalRadio`/:ned:`Ieee802154NarrowbandDimensionalRadioMedium`, etc.
**TODO** these models use the appropriate analog signal representation (i.e. the receiver, the transmitter, and the radio medium)

    so

    - signal power is represented as a function of frequency and time
    - arbitrary signal shape can be defined in frequency and time
    - its a multi dimensional function
    - attenuation, signal spectrum and interference is modeled accurately
    - can model CTI -> can the scalar model cti ?

    so

    there are types as in unit disk, scalar and dimensional
    and also types as in ieee80211, apsk, ieee802154 etc

    There are several

Testing routing protocols (Unit disk)
-------------------------------------

    - this example is about the unit disk
    - why is this good for routing?
    - what are the drawbacks/benefits
    - what are the parameters and the available models
    - the config/network
    - the results (show the ranges, something about the transmissions in the inspector

..  so the parameters are set in the transmitter and the receiver

  there are some degrees of freedom; the ranges, and some features of protocols are configurable, such as headerlength, bitrate, preambleduration. These are configured in the transmitter.
  The receiver has the ignoreInterference parameter.

  This example simulation demonstrates the unit disk analog model. The model is the simplest available in INET; it only models a communication range and an interference range. Tranmissions are always successfully received in the communication range, and never outside of it. Transmissions in the interference range can cause network nodes to back off from transmitting **TODO**. Also can set preamble duration, headerlength, and bitrate (used for calculating transmission duration). Set these in the transmitter. Set the ignoreInterference in the receiver.

The unit disk analog model is the simplest available in INET. It models three ranges, configurable with parameters:

..  of unit disk transmitters

- **Communication range**: tranmissions are always successful within this range (unless ruined by another interfering signal)
- **Interference range**: transmissions can't be received successfully in this range, but they can ruin other receptions
- **Detection range**: transmission can't interfere with other transmissions in this range, but they can cause network nodes detecting them to defer from transmitting (when using a suitable MAC module, such as :ned:`CsmaCaMac` or :ned:`Ieee80211Mac`)

These parameters can be set in the unit disk transmitters (:par:`communicationRange`, :par:`interferenceRange`, :par:`detectionRange` parameters).
Furthermore, the unit disk transmitters model some protocol features, configurable by parameters, such as :par:`headerLength`, :par:`preambleDuration`, and :par:`bitrate`; the bitrate is used to calculate transmission duration.

.. **TODO**: the actual parameters

The unit disk receiver's :par:`ignoreInterference` parameter configures whether interfering signals ruin receptions (``false`` by default).

**TODO**

- why is it good? for what purpose?
- what modules are available? and how to use it
- ideally, the ranges should be incrementally larger

- its simple and runs fast; suitable for scenarios where the details of tranmissisons don't matter/the emphasis is not on that, for example, routing
- there are actually, there are radios, which contain the transmitters and receivers
and there is the radio medium...need to use compatible...some examples
and there are some MAC's which can be used...not all features of the unit disk can be used with all MACs...the ackingmac doesnt do defer...need csmaca for that
there is apsk and wifi...there are interfaces...

.. It has three ranges configurable by parameters:

  - Communication range: tranmissions are always successful (unless ruined by an interfering signal)
  - Interference range: transmissions can't be received successfully, but they can ruin other receptions
  - Detection range: transmission can't interfere with other transmissions, but can cause network nodes detecting them to defer from transmitting (when using a suitable MAC module, such as CsmaCaMac or Ieee80211Mac)

**TODO** need to use the appropriate radio medium, and radio modules (which have the correct transmitter and receiver types) -> not here

Number of received packets vs distance (Scalar)
-----------------------------------------------

- this example is about the scalar
- why is t his good for this purpose? cos we need the attenuation/the reception needs to depend on the power levels
- drawbacks/benefits/degrees of freedom
- parameters, available models
- config/network
- results (show the BER/PER/number of packets)(should a study?)

Interference from a periodic noise source (Dimensional)
-------------------------------------------------------

- this example is about dimensional
- why is it good for this purpose? cos need to model the spectrums accurately. there are two channels which overlap, the noise generator has a different spectrum, and short transmissions which might not be enough in time to corrupt the wifi nodes' transmissions
- parameters, available models
- drawbacks/benefits/degrees of freedom
- config/network
- results (spectrum figure, spectrogram, inspector)
