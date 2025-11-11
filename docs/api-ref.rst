.. _api-ref:


API Reference
=============

The Arduino library provides a unified API for XENSIV™ PAS Gas sensors, supporting multiple sensor types and protocols. The API is structured around a base class and sensor-specific subclasses:

**Class Hierarchy:**

- ``XENSIV_PAS_GASIno``: Base class for all PAS Gas sensors. Provides common methods for initialization, measurement, diagnostics, and register access.
- ``XENSIV_PAS_GASA2LIno``: Subclass for A2L-type PAS Gas sensors. Adds A2L-specific configuration and measurement functions.
- ``XENSIV_PAS_GASR290Ino``: Subclass for R290-type PAS Gas sensors. Adds R290-specific configuration and measurement functions.

Use the appropriate class for your sensor type to access all supported features.

XENSIV™ PAS Gas Sensors Arduino API
---------------------------

.. doxygenclass:: XENSIV_PAS_GASIno
   :members:

.. doxygenclass:: XENSIV_PAS_GASA2LIno
   :members:

.. doxygenclass:: XENSIV_PAS_GASR290Ino
   :members:

Types
""""" 

Return Error Codes
^^^^^^^^^^^^^^^^^^

.. doxygentypedef:: Error_t 

.. doxygendefine:: XENSIV_PAS_GAS_OK
.. doxygendefine:: XENSIV_PAS_GAS_ERR_COMM
.. doxygendefine:: XENSIV_PAS_GAS_ERR_WRITE_TOO_LARGE
.. doxygendefine:: XENSIV_PAS_GAS_ERR_NOT_READY
.. doxygendefine:: XENSIV_PAS_GAS_ICCERR
.. doxygendefine:: XENSIV_PAS_GAS_ORVS
.. doxygendefine:: XENSIV_PAS_GAS_ORTMP
.. doxygendefine:: XENSIV_PAS_GAS_READ_NRDY
.. doxygendefine:: XENSIV_PAS_GAS_INVALID_SENSOR_INTERFACE
.. doxygendefine:: XENSIV_PAS_GAS_INVALID_PARAMETER

Diagnosis 
^^^^^^^^^

.. doxygentypedef:: Diag_t

.. doxygenunion:: xensiv_pas_gas_status_t

Baseline Offset Compensation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygentypedef:: ABOC_t 

.. doxygenenum:: xensiv_pas_gas_boc_cfg_t

XENSIV™ PAS Gas Sensors C Reference API
-------------------------------

The Arduino library is wrapping the platform abstracted C library from `this project <https://github.com/Infineon/xensiv-pas-gas-sensor>`_. 
