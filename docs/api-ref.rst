.. _api-ref:

API Reference
=============

The Arduino library API is implemented via the PASGASIno class.

XENSIV™ PAS GAS Arduino API
---------------------------

.. doxygenclass::  gasinoapi
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

Diagnosis 
^^^^^^^^^

.. doxygentypedef:: Diag_t

.. doxygenunion:: xensiv_pas_gas_status_t

Baseline Offset Compensation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygentypedef:: ABOC_t 

.. doxygenenum:: xensiv_pas_gas_boc_cfg_t

XENSIV™ PAS GAS C Reference API
-------------------------------

The Arduino library is wrapping the platform abstracted C library from `this project <https://github.com/Infineon/xensiv-pas-gas-sensor>`_. 
