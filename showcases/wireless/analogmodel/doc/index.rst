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